/*
*\file hal_powerMgr.c
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

#include"phy_port.h"
#include"hal_adc.h"
#include"hal_led.h"
#include"hal_radio.h"
#include"hal_powerMgr.h"
#include"delay.h"


/*
 *\brief Voltage detect
 *
 *\return VOL_LOW - Low voltage
 * 		  VOL_NOMAL - Voltage is normal
 */
uint8 VoltageDetect(void)
{
	uint8 value;
	uint8 mode;

	value = GetAdcValue(ADC_CHS0);
	mode = GetPowerStatus();

	if(mode == CONSUMPTION)
	{
		if(value > LOW_VOLTAGE_VALUE)
		{
			return	VOL_LOW;
		}
		else
		{
			return	VOL_NOMAL;
		}
	}
	else //charge
	{
		if(value > NOMAL_VOLTAGE)
		{
			return	VOL_LOW;
		}
		else
		{
			return	VOL_NOMAL;
		}
	}
}

/*
 *\brief Get power status
 *
 *\return CHARGING - Charging mode
    	  CONSUMTPION - Power consumption
 */
uint8 GetPowerStatus(void)
{
	if(CHARGE_EN == 1)	//Debuge change 0 \original 1
	{
		return	CHARGING;
	}
	else
	{
		return	CONSUMPTION;
	}

}

/*
 *\brief Connect with charger if battery is not full,else disconnect
 */
void PowerManagement(uint8 mode)
{
	if(GetPowerStatus()==CHARGING)
	{
		if(mode == ON)
		{
			CHARGE_CTL = ON;
		}
		else
		{
			CHARGE_CTL = OFF;
		}
	}
	else
	{
		CHARGE_CTL = OFF;
	}
}

#ifdef 	POWER_SAVING

/*
 *\brief Active MCU enter sleep
 */
void MCUPowerDown(void)
{
	PCON |= 0x02;
	DelayNops(2); /* \comment: Implement from here when system wake up >*/

}

/*
 *\brief Enter power conserve mode
 */
void EnterPowerConserve(void)
{
	//Shut all peripherals
	halLedEnterSleep();
	SHUT_TL431();
	//nRF24L01+ enter power down mode
	RadioPowerDown();
	//MCU enter power mode
	MCUPowerDown();
}

#endif
