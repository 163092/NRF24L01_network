/*
*\file hal_led.h
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
#ifndef	HAL_LED_H
#define	HAL_LED_H


#include"sys_types.h"
#include"board_cfg.h"

/*
 *\brief LED service status definitions
 */
#define		LED_ON			0
#define		LED_OFF			1
#define		LED_TOGGLE		3
#define		LED_FLASH		4


void halLedInit(void);
#if	(HAL_LED == TRUE)
void halModeLedToggle(uint8 mode);
#ifdef	POWER_SAVING
void halLedEnterSleep(void);
#endif
#endif

#if defined(ROUTER) || defined(END_DEVICE)
void  halVolLedBlink(uint8 mode);
#endif


#endif
