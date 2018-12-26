/*
*\file systemRT.c
*\brief   Logic driver,main loop for task polling.
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
#include"systemRT.h"
#include"delay.h"


pan_t panAddr;
nwkStat_t nwkStatus;
uint8 rx_cnt;

/*
 *\brief If the router not receive data request,restart network
 */
time_t restart;	 /*< Detect Rx data time-out >*/
time_t flStart;


/*
 *\brief System timer initialization
 */
void timer_init(void)
{
	TMOD |= 0x01;
	TH0 = 0xEE;	//5ms@11.0592MHz
	TL0 = 0x00;
	TR0 = 1;
	ET0 = 1;
}

/*
 *\brief Enable interrupt
 */
void intr_init(void)
{
	INT_CLKO |= 0x20;
	EX0 = 1;
//	IT0 = 1; //Falling edge trigger
}

/*
 *\brief System initialized,call this function first when system power on
 */
void SystemInit(void)
{
	//Initialize GPIO
	Port_Init();
	//Initailize LEDs
	halLedInit();
	//Enable UART communication
	UART_Init();
	//Network initialization
	nwkNetworkInit(&nwkStatus,&panAddr);
	//Open interrupt
	intr_init();
	//Board initializeation
	tx_buf[0] = ROUTER_ADDR; /*< Indicator where the data come from>*/
	tx_buf[1] = CONNECTED;	 /*< Data request command >*/
	PrintString("Router!");	 /*< Print board imformation >*/
	//Start system clock
	timer_init();

	EA = 1;
}

/*
 *\brief Process a key event
 */
void KeyProcess(void)
{
	if(GetKeyStatus()==KEY_PUSHED)
	{
		//TODO:
		/*\comment: Reserve */
		halVolLedBlink(ON);
	}
}

void FlushRestartTicks(void)
{
	restart.start = 0;
	restart.ticks = 0;
}

/*
 *\brief Network management
 */
void NetworkManagement(void)
{
	uint8 nwk_status;
	uint8 sver_status;

	nwk_status = nwkNetworkMode(&panAddr,&nwkStatus);
	#if	(HAL_LED == TRUE)
		halModeLedToggle(nwk_status);
	#endif
	sver_status = nwkNetworkService(&nwkStatus);
	if(sver_status == TX_OK)
	{
		//Communication with end device,disconnect from main channel utill receive from
		//end device
		if((rx_buf[0] == COORDINATOR_ADDR) && (rx_buf[1] == CONNECTED))
		{
			nwkStatus.channel = ENDEVICE_CHANNEL;
			nwkStatus.mode = ENTER_TX;
			nwkStatus.lock = OFF;
			rx_buf[1] = DISCONNECT;	//Disconnect from main channel
			panAddr.dstAddr[2] = ENDEVICE_ADDR;
			#ifdef DEBUG
				PrintString("End-Device\r\n");
			#endif
		}
		else //Enter RX mode to receive coordiantor requset or status from end-device
		{
			nwkStatus.mode = ENTER_RX;
			nwkStatus.lock = OFF;
		}

		FlushRestartTicks();
	}
	else if(sver_status == RX_OK) //Receive completed
	{
        PrintString("Received!\r\n");
		if(rx_buf[0] == ENDEVICE_ADDR) //Status from end-device
		{
			if((tx_buf[1] == KILLED) && (rx_buf[1] == HANDSUP))
			{
				if(flStart.flag == 1)
				{
					tx_buf[1] = FOULED;
					flStart.flag = 0;
				}
				else
				{
					tx_buf[1] = KILLED;
				}
			}
			else if((tx_buf[1] != FOULED) && (rx_buf[1] == SURRENDER))
			{
				tx_buf[1] = KILLED;
				flStart.start = ON;
			}
			//Enter RX mode to receive coordiantor requset
			nwkStatus.mode = ENTER_RX;
		}
		else if((rx_buf[1] == RESET_NWK) && (rx_buf[0] == COORDINATOR_ADDR))
		{
			tx_buf[1] = CONNECTED;
		}
		else //Request from coordiantor
		{
			//Send status to coordinator
			//nwkStatus.mode = ENTER_TX;
		}
		//Network toggle
		panAddr.dstAddr[2] = COORDINATOR_ADDR;
		nwkStatus.channel = MAIN_CHANNEL;
		nwkStatus.lock = OFF;
		#ifdef DEBUG
			PrintString("Coordinator\r\n");
		#endif

		FlushRestartTicks();

	}
	else if(sver_status == MAX_RT) //Maximun retries counter overflow,enter RX mode wait next communication request from coordinator
	{
		nwkStatus.channel = MAIN_CHANNEL;
		nwkStatus.mode = ENTER_RX;
		panAddr.dstAddr[2] = COORDINATOR_ADDR;
		nwkStatus.lock = OFF;
		#ifdef DEBUG
			//PrintString("Coordinator RT\r\n");
		#endif

		FlushRestartTicks();
	}
	else
	{
		restart.start = ON;
		if(restart.flag == 1)
		{
			restart.flag = 0;
			restart.start = OFF;
			nwkStatus.channel = MAIN_CHANNEL;
			nwkStatus.mode = ENTER_RX;
			panAddr.dstAddr[2] = COORDINATOR_ADDR;
			nwkStatus.lock = OFF;
			#ifdef DEBUG
				PrintString("Receive Failed!\r\n");
			#endif
		}
	}


}

/*
 *\brief System task loop
 */
void RunSystem(void)
{
	KeyProcess();
	NetworkManagement();
}

/*
 *\brief CS
 */
void EXINT0_ISR(void) interrupt 0
{
	if(tx_buf[1] == CONNECTED)
	{
		tx_buf[1] = KILLED;
		flStart.start = ON;
	}
}

/*
 *\brief  System clock
 */
void Timer0_ISR(void) interrupt 1
{
	TH0 = 0xEE;	//5ms@11.0592MHz
	TL0 = 0x00;

	if(flStart.start == ON)
	{
		flStart.ticks++;
		if(flStart.ticks >= 1000)
		{
			flStart.flag = 1;
			flStart.ticks = 0;
			flStart.start = OFF;
		}
	}
	if(restart.start == ON)
	{
		restart.ticks++;
		if(restart.ticks >= 500)
		{
			restart.flag = 1;
			restart.ticks = 0;
			restart.start = OFF;
		}
	}

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
