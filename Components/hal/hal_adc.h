/*
*\file hal_adc.h
*\brief ADC	definitions.
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
#ifndef	HAL_ADC_H
#define	HAL_ADC_H

#include"sys_types.h"
#include"utils.h"
#include"phy_port.h"

/*
 *\brief ADC channel and ADC pin correspondence
 */
#define		ADC_CHS0		0x00	/*< select channel 1>*/
#define		ADC_CHS1		0x01
#define		ADC_CHS2		0x02
#define		ADC_CHS3		0x03
#define		ADC_CHS4		0x04
#define		ADC_CHS5		0x05
#define		ADC_CHS6		0x06
#define		ADC_CHS7		0x07

/*
 *\ P1 Analog function configure
 */
#define		AFS_P10			0x01
#define		AFS_P11			0x02
#define		AFS_P12			0x04
#define		AFS_P13			0x08
#define		AFS_P14			0x10
#define		AFS_P15			0x20
#define		AFS_P16			0x40
#define		AFS_P17			0x80




#define		ADC_PIN_CFG		AFS_P10	/*< P1.0 set as adc input pin>*/
#define		ADC_FLAG		0x10	/*< ADC compeleted flag,clear by write 0>*/
#define		ADC_PWR_ON		0x80	/*<	ADC power on >*/
#define		ADC_PWR_OFF		0x7F	/*< ADC power off,reduce current consuption,turn off before enter power down mode>*/
#define		ADC_START		0x08  	/*< Start AD conversion >*/
/*
 *\brief AD conversion speed
 */
#define		ADC_SPEED1		0x60   /*< 90 clocks >*/
#define		ADC_SPEED2		0x40   /*< 180 clocks >*/
#define		ADC_SPEED3		0x20   /*< 360 clocks >*/
#define		ADC_SPEED4		0x00   /*< 520 clocks >*/


#define		ADC_CFG	 ( \
			ADC_PWR_ON | \
			ADC_START  | \
			ADC_SPEED1  \
					 )

#define		ADC_FAIL		255			/*< Concertion failed after 255 cycles >*/

#define		EN_TL431()		st(VOLTAGE_REF = 1;)
#define		SHUT_TL431()	st(VOLTAGE_REF = 0;)



void adc_init(void);

#ifdef POWER_SAVING

void AdcPowerOff(void);

#endif
uint8 GetAdcValue(uint8 ch);




#endif
