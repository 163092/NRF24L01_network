/*
*\file hal_adc.c
*\brief Analog to digital conversion functions.
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

#include"hal_adc.h"
#include"delay.h"

/*
 *\brief Initialized ADC input pins
 */
void adc_init(void)
{
	P1ASF = ADC_PIN_CFG;
}

#ifdef	POWER_SAVING

/*
 *\brief ADC power off after convertion to reduce the current consumption
 */
void AdcPowerOff(void)
{
	ADC_CONTR &= ADC_PWR_OFF;
//	SHUT_TL431();
}

#endif

/*
 *\brief Start ADC
 *
 *\param ch - Selected ADC channel
 *
 * \return result - ADC vaule if convertion successful,return ADC_FAIL if
 *         failed
 */
uint8 GetAdcValue(uint8 ch)
{
	uint8 result;
	uint8 i;
	ADC_CONTR = ADC_CFG | ch;
	ADC_RES = 0;
	ADC_RESL = 0;
	EN_TL431();
	//Wait voltage stability
	DelayMs(2);

	for(i=0;i<255;i++)
	{
		if(ADC_CONTR & ADC_FLAG)
		{
			ADC_CONTR &= ~ADC_FLAG;

			result = ADC_RES;
//			result <<= 2;
//			result |= (ADC_RESL & 3);

			#ifdef	POWER_SAVING
				AdcPowerOff();
			#endif

			return result;
		}
	}

	#ifdef	POWER_SAVING
		AdcPowerOff();
	#endif

	return ADC_FAIL;
}
