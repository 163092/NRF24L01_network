/*
*\file systemCR.c
*\brief All logic drivers.
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


#include"systemCR.h"
#include"hal_uart.h"
#include"hal_radio.h"
#include"hal_led.h"
#include"nwk_addrMgr.h"
#include"delay.h"



uint8 rx_cnt;

/*
 *\brief Timer 0 for nRF24L01 receive timeout detect
 */
void timer0_init(void)
{
	TMOD = 0x01;
	TH0 = 0x00;	//5ms@11.0592MHz
	TL0 = 0xEE;
	TR0 = 1;
	ET0 = 1;
}


/*
 *\brief Interrupt initialized
 */
void Inter_Init(void)
{
	INT_CLKO |= 0x20; //External interrupt 3
	timer0_init();
}


/*
 *\brief System initialized,run system with call this function,
 */
void SystemInit(void)
{
	Port_Init();
	UART_Init();
	Inter_Init();
	halLedInit();
	nwkAddressInit();
	PrintString("Coordinator!\r\n");
	//RX mode and print imformation
	EA = 1;
}

/*
 *\brief All system task
 */
void RunSystem(void)
{
	nwkNetworkManager();
}

void Timer0_ISR(void) interrupt 1
{
	TH0 = 0xDC;
	TL0 = 0x00;

	if(rxDetect.start == ON)
	{
		rxDetect.ticks++;
		if(rxDetect.ticks >= MAX_RX_RETRY)
		{
			rxDetect.ticks = 0;
			rxDetect.flag = 1;
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

		rx_buf[3] = SBUF;
		if(rx_buf[3] == RESET_NWK)
		{
			tx_buf[1] = RESET_NWK;
			panAddr.index = 1;
			LOWPWR_LED = 0;
		}
	}
}
