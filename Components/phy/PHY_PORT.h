/*
*\file phy_port.h
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

#if defined(ROUTER) || defined(END_DEVICE)

/*********nRF24L01 INTERFACE **********/
// sbit	MOSI = P1^3;
// sbit	MISO = P1^4;
// sbit	SCLK = P1^5;
// sbit	CE = P1^2;
// sbit	CSN = P1^1;
// sbit	NRF_IRQ = P3^7;

sbit	MOSI = P1^3;
sbit	MISO = P1^5;
sbit	SCLK = P1^4;
sbit	CE = P1^2;
sbit	CSN = P1^1;
sbit	NRF_IRQ = P3^7;


sbit	MODE_KEY = P2^6;
//sbit	POWER_LED = P5^5;
sbit	POWER_LED   =       P5^4;
sbit	RF = P1^7;
sbit	RECEIVE_LED = P1^6;
sbit 	LOWPWR_LED = P5^4;

/**********CHARGE CONTROL************/
sbit  	CHARGE_EN = P3^4;
sbit 	CHARGE_CTL = P3^5;
sbit 	VOLTAGE_REF = P2^7;
sbit 	CHARGE_INT = P3^3;

/***********FOULD DETECT ************/
sbit	HANDS_DETECT = P3^6;

#else
/*********nRF24L01 INTERFACE **********/
sbit	MOSI = P1^3;
sbit	MISO = P1^4;
sbit	SCLK = P1^5;
sbit	CE = P1^2;
sbit	CSN = P1^1;
sbit	NRF_IRQ = P3^7;

sbit	MODE_KEY = P3^5;
sbit	POWER_LED = P5^4;
sbit	TRANSMIT_LED = P1^7;
sbit	RECEIVE_LED = P1^6;
sbit 	LOWPWR_LED = P5^5;

/******** 485 COMMUNICATION **********/
/*
 *\brief 0: RE
 		 1: DE
 */
sbit    SN75LBC_MODE = P2^7;
#endif



#define		ON			1
#define		OFF			0

void Port_Init(void);
void SPI_WriteData(uint8 byte);
uint8 SPI_ReadData(void);

#endif
