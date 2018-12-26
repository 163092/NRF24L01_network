/*
*\file phy_port.c
*\brief   Set up gpio and necessary interface protocol.
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
#include"phy_port.h"

/*
 *\brief Initialize communication interface and status of powerup
 */
void Port_Init(void)
{
	CE = 0;
	CSN = 0;
	SCLK = 0;
	POWER_LED = 0;
	#if defined(ROUTER) || defined(END_DEVICE)
		CHARGE_CTL = OFF;
         RF  = 1;
        P5M0 = 0x20;
        P1M0 = 0x80;
        P3M0 = 0x14;
        P3M1 = 0x04;
// 		P3M1 = 0x58; //P3.4,P3.6,P3.3 open drain
// 		P3M0 = 0x78; //P3.5 pull-push
// 		P2M0 = 0x80; //P2.7 pull-push ADC TL431
	#else
		SN75LBC_MODE = 0;
		P2M0 = 0x80; //P2.7 pull-push SN75LPC184 RE/DE
	#endif
}

/*
 *\brief SPI protocol,write 1 byte data
 *
 *\param byte - Data for write
 */
void SPI_WriteData(uint8 byte)
{
	uint8 i;

	for(i=0;i<8;i++)
	{
		MOSI = (byte & 0x80);
		byte <<= 1;
		SCLK = 1;
		SCLK = 0;
	}

}

/*
 *\brief SPI protocol,Read 1 byte data
 *
 *\return byte - Read data
 */
uint8 SPI_ReadData(void)
{
	uint8 i;
	uint8 dat;

	for(i=0;i<8;i++)
	{
		dat <<= 1;
		SCLK = 1;
		dat |= MISO;
		SCLK = 0;
	}

   return dat;
}
