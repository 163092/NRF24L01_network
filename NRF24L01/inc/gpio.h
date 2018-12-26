/*
*\file gpio.h
*\brief   Port map and IO macros.
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
#ifndef	__GPIO_H
#define	__GPIO_H

#include"sys_types.h"
#include<STC/STC15F2K60S2.H>


sbit	MOSI = P1^3;
sbit	MISO = P1^4;
sbit	SCLK = P1^5;
sbit	CE = P1^2;
sbit	CSN = P1^1;
sbit	NRF_IRQ = P3^7;

sbit	MODE_KEY = P3^5;
sbit	POWER_LED = P5^4;
sbit	TANSMIT_LED = P1^7;
sbit	RECEIVE_LED = P1^6;
sbit 	LOWPWR_LED = P5^5;

/*
*  This macro is for use by other macros to form a fully valid C statement.
*  Without this, the if/else conditionals could show unexpected behavior.
*
*  For example, use...
*    #define SET_REGS()  st( ioreg1 = 0; ioreg2 = 0; )
* (The while condition below evaluates false without generating a
*  constant-controlling-loop type of warning on most compilers.)
*/
#define		st(x)			do{ x }while(__LINE__==-1)

#define		ON			1
#define		OFF			0

void Port_Init(void);
void SPI_WriteData(uint8 byte);
uint8 SPI_ReadData(void);

#endif
