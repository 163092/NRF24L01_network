/*
*\file uart.c
*\brief monitor and test!
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
#include"uart.h"

/*
 *\brief Open UART and set baudrate
 */
void UART_Init(void)
{
	SCON = 0x50; //Enable receive
	T2L = (65536-(MAIN_FOSC/4/BAUD_RATE));
	T2H = (65536-(MAIN_FOSC/4/BAUD_RATE))>>8;
	AUXR = 0x14;
	AUXR |= 0x01;
	ES  = 1;

}

/*
 *\brief Send 1 byte
 */
void Tx1Send(unsigned char dat)
{
	SBUF = dat;
	while(!TI);
	TI = 0;
}

/*
 *brief Send a string to display imformation
 */
void PrintString(unsigned char *puts)
{
    for (; *puts != 0;	puts++)  Tx1Send(*puts);
}
