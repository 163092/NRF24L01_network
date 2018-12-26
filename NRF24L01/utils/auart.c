/*
*\file uart.c
*\brief Analog UART print for debug.
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
 *\brief Generate baudrate 9600Bps
 */
void	BitTime(void)
{
	unsigned int i;
	i = ((MAIN_Fosc / 100) * 104) / 140000 - 1;		//������ʱ��������λʱ��
	while(--i);
}

/*
 *\brief Send 1 byte
 */
void	Tx1Send(unsigned char dat)		//9600��N��8��1
{
	unsigned char	i;
	EA = 0;
	P_TXD1 = 0;
	BitTime();
	for(i=0; i<8; i++)
	{
		if(dat & 1)		P_TXD1 = 1;
		else			P_TXD1 = 0;
		dat >>= 1;
		BitTime();
	}
	P_TXD1 = 1;
	BitTime();
	BitTime();
	EA = 1;
}

/*
 *brief Send a string to display imformation
 */
void PrintString(unsigned char code *puts)
{
    for (; *puts != 0;	puts++)  Tx1Send(*puts);
}
