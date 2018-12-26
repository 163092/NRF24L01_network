/*
*\file systemED.c
*\brief   Logic drivers.
Author : <HuJun>
Copyright : ChengDu Zhi Nang Tech Co., Ltd.
Revision : <1.0.0>
Design Created : 2014.5.12
Design Last Modified : 2014.5.22

E-mail : venthujun@hotmail.com
*
*\license_start
Redistribution and use source code must retain the above copyright notice, with or without
modification are permitted,please specify in which data and procedures from Zhi Nang Tech
*\license_end

*\! Remark : Contact  if further requirements
*/

#include"hal_led.h"
#include"nwk_service.h"
#include"hal_key.h"
#include"hal_uart.h"
#include"phy_port.h"
#include"systemED.h"
#include"delay.h"
#include"hal_adc.h"
#include"hal_powerMgr.h"


pan_t panAddr; 		/*< Network address >*/
nwkStat_t nwkStatus; /*< Network status >*/
time_t rxDetect,flDetect,volBlink;
uint8 rx_cnt; //Resreved
uint8 taskEvents[MAX_TASK_NUM]; /*< Task events list>*/



/*
 *\brief Timer 0 for nRF24L01 receive timeout detect
 */
void timer0_init(void)
{
	TMOD |= 0x01;
	TH0 = 0xEE;	//5ms@11.0592MHz
	TL0 = 0x00;
	TR0 = 1;
	ET0 = 1;
}

/*
 *\brief Interrupt initialized
 */
void inter_init(void)
{
	EX0 = 1;  //External interrupt 0 for key event
	EX1 = 1;  //External interrupt 1 for power manager(falling edge and the rising edge)
	IT0 = 1;  //Falling edge trigger interrupt
	INT_CLKO |= 0x30; //External interrupt 2,3
	PX1	= 1;  //Charging interrupt has the highest priority
	timer0_init();
}

/*
 *\brief Initialized variables
 */
void DataInit(void)
{
	tx_buf[0] = ENDEVICE_ADDR;
	tx_buf[1] = CONNECTED;	  /*< CONNECTED massage sned(default) >*/
	PrintString("End device!\r\n");
	taskEvents[MASSAGE_MGR] = 1;
	taskEvents[POWER_MGR] = 1;
	flDetect.start = ON;
	volBlink.flag = ON;
}

/*
 *\brief System initial,call this function first when system power on
 */
void SystemInit(void)
{
	//Initialized GPIO
	Port_Init();
	//LED mode
	halLedInit();
	//Enable uart
	UART_Init();
	//Enable ADC
	adc_init();
	//Network initialized
	nwkNetworkInit(&nwkStatus,&panAddr);
	//Interrupt enable
	inter_init();
	//Data initialized
	DataInit();
	enable_wdt(); /*< enable watch-dog timer>*/
	EA = 1;
}

/*
 *\brief Power monitoring task
 */
void PowerMonitoring(void)
{
	if(VoltageDetect() == VOL_LOW)
	{
		volBlink.start = ON;
		taskEvents[POWER_MGR] = 1;	//Active power manage
	}
	else
	{
		volBlink.sec++;
		if(volBlink.sec >= 25) //Wait the voltage stability
		{
			taskEvents[POWER_MGR] = 0;
			volBlink.start = OFF;
			volBlink.flag = ON;
			volBlink.ticks = 0;
			volBlink.sec = 0;
		}
	}
	halVolLedBlink(volBlink.flag);
	PowerManagement(volBlink.start);
	//clear watch dog
	clr_wdt();
}

/*
 *\brief Process a key event
 */
void KeyProcess(void)
{
	taskEvents[KEY_PROCESS] = 0;
	tx_buf[1] = SURRENDER;	 /*< SURRENDER massage sned >*/
	FlushRxTicks();
	//Shut RX time out detect,Waiting untill communicate completed
	rxDetect.flag = 0;
	taskEvents[MASSAGE_MGR] = 1;
	//clear watch dog
	clr_wdt();
}

/*
 *\brief Network management
 */
void NetworkManagement(void)
{
	uint8 nwk_status;
	uint8 sver_status;

	nwk_status = nwkNetworkMode(&panAddr,&nwkStatus);
	#if (HAL_LED == TRUE)
		halModeLedToggle(nwk_status);
	#endif
	sver_status = nwkNetworkService(&nwkStatus);

	//The massage task can only patrol within a time window,if the maximun retries
	//number overflow,the end device enter RX mode wait for next communication
	if((sver_status == TX_OK)||(sver_status == MAX_RT))
	{
		 //Massage task completed or no acknowledge
		 rxDetect.flag = 1; //Enter RX mode before enter sleeping mode
		 //Other task will wake up target,then the end device will receive request of
		//communication,so flush TX buffer is necessary
		if((rx_buf[1] == KILLED) || (rx_buf[1] == FOULED))
		{tx_buf[1] = CONNECTED;}	/*< Silence warning >*/
	}
	if(nwk_status == ENTER_RX)
	{

		rxDetect.start = ON;
		if(rxDetect.flag == 1)
		{
			if(tx_buf[1] != SURRENDER) //if someone surrender,communication task active till transimit completed
			{taskEvents[MASSAGE_MGR] = 0;} //Receive data time out
			rxDetect.flag = 0;
		}
	}

	//clear watch dog
	clr_wdt();

}

/*
 *\brief Flush RX ticks for next detect
 */
void FlushRxTicks(void)
{
	rxDetect.start = OFF;
	rxDetect.ticks = 0;
	rxDetect.sec = 0;
}

/*
 *\brief All task polling
 */
void RunSystem(void)
{
	uint8 idx = 0;

	do{
		if(taskEvents[idx]) // Task is highest priority that is ready.
			break;

	}while(++idx < MAX_TASK_NUM);

	if(idx < MAX_TASK_NUM)
	{
		#ifdef POWER_DEBUG
			Tx1Send(GetAdcValue(ADC_CHS0));
		#endif
		switch(idx)
		{
			case MASSAGE_MGR:
							NetworkManagement();
				    	break;
			case KEY_PROCESS:
							KeyProcess();
						break;
			case POWER_MGR:
							PowerMonitoring();
						break;
			default :
					break;
		}
	}
	else // Complete pass through all task events with no activity?
	{
		FlushRxTicks();
		//clear watch dog
		clr_wdt();
		#ifdef	POWER_SAVING
			EnterPowerConserve(); //Put system into sleeping mode
			nwkStatus.lock = OFF; //Unlock network when MCU wake up,Is mainly used for wake-up nRF24L01+
		#endif
	}
}

/*
 *\brief Key task events
 */
void EXINT0_ISR(void) interrupt 0
{
	if(GetKeyStatus()==KEY_PUSHED)
	{
		taskEvents[KEY_PROCESS] = 1;
		taskEvents[POWER_MGR] = 1;
		volBlink.sec = 0;
	}
}

/*
 *\brief System timer
 */
void Timer0_ISR(void) interrupt 1
{
	TH0 = 0xEE;
	TL0 = 0x00;

	if(rxDetect.start == ON)
	{
		rxDetect.ticks++;
		if(rxDetect.ticks >= MAX_RX_RETRY) //MAX_RX_RETRY * 5ms
		{
			rxDetect.ticks = 0;
			rxDetect.sec++;
			if(rxDetect.sec >= 100)
			{
				rxDetect.sec = 0;
				rxDetect.start = OFF;
				rxDetect.flag = 1;
			}
		}
	}
	if(volBlink.start == ON)
	{
		volBlink.ticks++;
		if(volBlink.ticks < 200)
		{
			volBlink.flag = 1;
		}
		else
		{
			volBlink.flag = 0;
		}
		if(volBlink.ticks >= 400)
		{
			volBlink.ticks = 0;
		}
	}
	if(flDetect.start == ON)
	{
		flDetect.ticks++;
		if(flDetect.ticks >= 200)
		{
			flDetect.ticks = 0;
			flDetect.start = OFF;
			flDetect.flag = 1;
		}
	}
}


void EXINT1_ISR(void) interrupt 2
{
	taskEvents[POWER_MGR] = 1;
}

/*
 *\brief UART0 interrupt service routine
 */
void UART0_ISR(void) interrupt 4
{
	if(RI)
	{
		RI = 0;
	}
}

void EXINT2_ISR(void) interrupt 10
{
	//Start detect someone fould
	flDetect.start = ON;
//	//Active hands up detect task
//	taskEvents[FOUL_DETECT] = 1;
	//Wait voltage stability
	volBlink.sec = 0;
	//Start RX time ticks for detect communication,if time out means communication failed
	FlushRxTicks();
	//Active the message management task
	taskEvents[MASSAGE_MGR] = 1;
	//Active power management
	taskEvents[POWER_MGR] = 1;
	if(tx_buf[1] != SURRENDER) /*< SURRENDER transmit completed >*/
	{tx_buf[1] = HANDSUP; }/*< HANDSUP message send >*/
}
