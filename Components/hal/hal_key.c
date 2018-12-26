/*
*\file hal_key.c
*\brief Key service.
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
#include"hal_key.h"
#include"delay.h"

/*
 *\brief Detect if a key is pressed

 *\return KEY_PUSHED -  Key pressed
 		  KEY_STANDBY - No button is pressed
 *
 */
uint8 GetKeyStatus(void)
{
	if(!MODE_KEY)
	{
		Debounce();
		if(!MODE_KEY)
		{
			return KEY_PUSHED;
		}
	}

	return KEY_STANDBY;
}

/*
 *\brief Key pressed debounce and beep
 */
void Debounce(void)
{
//	BEEP = ON;
	DelayMs(200);
//	BEEP =OFF;
}
