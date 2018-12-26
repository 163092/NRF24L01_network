/*
*\file hal_powerMgr.h
*\brief Power management.
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
#ifndef	HAL_POWER_MGR_H
#define	HAL_POWER_MGR_H

/*
 *\brief Alarm voltage value
 */
#define		LOW_VOLTAGE_VALUE		208 /*< Consumption mode >*/
#define		NOMAL_VOLTAGE			180 /*< Charging completed value >*/

/**
 *\brief Battery status
 */
#define		CHARGING				1
#define		CONSUMPTION				0

/*
 *\brief Power status
 */
#define		VOL_NOMAL				0
#define		VOL_LOW					1


uint8 VoltageDetect(void);
uint8 GetPowerStatus(void);
void PowerManagement(uint8 mode);

#ifdef 	POWER_SAVING

void MCUPowerDown(void);
void EnterPowerConserve(void);

#endif


#endif
