/*
*\file system.c
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


#include"system.h"
#include"hal_uart.h"
#include"hal_radio.h"
#include"phy_port.h"
#include"delay.h"

uint8 const edv_addr[3] = {0x20,0x14,0x04};

uint8  idata tx_buf[DATA_LEN] = "This is a test data from ZIC05\r\n";
uint8  idata rx_buf[DATA_LEN];


operate_t operate;
uint8 rx_cnt;
//#ifdef COORDINATOR
	uint8 end_device_addr;
//#endif

/*
 *\brief Interrupt initialized
 */
void Inter_Init(void)
{
	INT_CLKO = 0x20; //External interrupt 3
}

/*
 *\brief System initialized,run system with call this function,
 */
void SystemInit(void)
{
	Port_Init();
	UART_Init();
	Inter_Init();
	//RX mode and print imformation
	operate.flag = 1;
	EA = 1;
}

/*
 *\brief Process a key envent
 */
void GetKeyValue(void)
{
	if(MODE_KEY==0)
	{
		DelayMs(200);
		while(MODE_KEY==0);
		/*
			operate.mode:
						1: TX mode
						0: RX mode
		 */
		operate.mode++;
		if(operate.mode >= 2)
		{
			operate.mode = 0;
		}
		operate.flag = 1;
	}
}

/*
 *\brief Key logic process
 */
void KeyProcess(void)
{
	GetKeyValue();

	if(operate.mode==0)
	{
		if(operate.flag==1)
		{
			RX_Mode(edv_addr,45);
			PrintString("enter RX mode!\r\n");
			operate.flag = 0;
			TRANSMIT_LED = 1;
			RECEIVE_LED = 0;
		}
	}
	else
	{
		if(operate.flag==1)
		{
			TX_Mode(edv_addr,45);
			PrintString("enter TX mode!\r\n");
			operate.flag = 0;
			TRANSMIT_LED = 0;
			RECEIVE_LED = 1;
		}
	}
}

/*
 *\brief All system task
 */
void RunSystem(void)
{
	uint8 status;

	KeyProcess();
	if(operate.mode == 1)
	{
		status = WritePayloadBuf(tx_buf,DATA_LEN);
		if(status==TX_OK)
		{
			operate.mode = 0;
			operate.flag = 1;
			PrintString("TX Complete!\r\n");
		}
		else if(status==MAX_RT)
		{
			//Reuse TX;
			PrintString("Retrying,please check device connection!\r\n");
			operate.mode = 0;
			operate.flag = 1;

		}
		else
		{
			PrintString("Standby!!!\r\n");
			status = STANDBY;
		}
	}
	else
	{
		status = ReadPayloadBuf(rx_buf,DATA_LEN);
		if(status==RX_OK)
		{
			PrintData(rx_buf,DATA_LEN);
			PrintString("Recived!\r\n");
			operate.mode = 1;
			operate.flag = 1;
		}
		else
		{
		 //TODO:
		 	PrintString("Recive failed!\r\n");
		}
	}
	if(operate.entx==1)
	{
		PrintString(rx_buf);
		operate.entx = 0;
	}
}

/*
 *\brief Print a specified data with serial terminal
 *
 *\param *addr - Buffer address
 		 len   - Specified data length
 */
//void PrintData(uint8 *addr,uint8 len)
//{
//	uint8 i;
//
//	for(i=0;i<len;i++)
//	{
//		Tx1Send(*addr++);
//	}
//}

/*
 *\brief UART0 interrupt service routine
 */
void UART0_ISR(void) interrupt 4
{
	if(RI)
	{
		RI = 0;

		rx_buf[rx_cnt] = SBUF;
		rx_cnt++;
		//Buffer is full
		if(rx_cnt >= PAYLOAD_BUF_LEN)
		{
			rx_cnt = 0;
			///Print received data
			operate.entx = 1;
		}
	}
}
