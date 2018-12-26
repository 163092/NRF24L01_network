/*
*\file hal_led.c
*\brief LED service.
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
#include"hal_led.h"

/*
 *\brief Initialized LEDs status when power up
 */
void halLedInit(void)
{
	#if (HAL_LED == TRUE)
		//Power on
		POWER_LED = LED_ON;
		#if defined (ROUTER) || defined(END_DEVICE)
		//Router default to receive mode
			RECEIVE_LED = LED_ON;
		#else
		//In other function,system in transfer mode
			TRANSMIT_LED = LED_ON;
		#endif
	#endif
}

#if (HAL_LED == TRUE)

/*
 *\brief Indicator the current network mode
 *
 * \param
 */
void halModeLedToggle(uint8 mode)
{
	//TX mode indicator
	if(mode == 1)
	{
		RECEIVE_LED = LED_OFF;
		//TRANSMIT_LED = LED_ON;
	}
	//RX mode indicator
	else
	{
		RECEIVE_LED = LED_ON;
		//TRANSMIT_LED = LED_OFF;
	}
}

#ifdef	POWER_SAVING

/*
 *\brief Turn off LEDs before enter sleep
 */
void halLedEnterSleep(void)
{
	RECEIVE_LED = LED_OFF;
	TRANSMIT_LED = LED_OFF;
	POWER_LED = LED_OFF;
}


#endif

#endif

#if defined(ROUTER) || defined(END_DEVICE)

/*
 *\brief Low voltage warning,led blink
 */
void  halVolLedBlink(uint8 mode)
{
	if(mode == ON)
	{
		POWER_LED = LED_ON;
	}
	else
	{
		POWER_LED = LED_OFF;
	}
}
#endif
