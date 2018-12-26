/*
*\file uart.h
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

#ifndef	__UART_H
#define	__UART_H

#include<STC/STC15F2K60S2.H>

#define MAIN_FOSC		11059200UL
#define	BAUD_RATE		115200

void UART_Init(void);
void Tx1Send(unsigned char dat);
void PrintString(unsigned char *puts);

#endif
