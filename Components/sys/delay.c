/*
*\file delay.c
*\brief All delay service routine.
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

#include<intrins.h>
#include"delay.h"

/*
\brief No operation ,call DelayNops() is necessary for 1 T mode MCU  before read a pin value
*/
void DelayNops(INT8U t)
{
	do{
		_nop_();_nop_();_nop_();
		_nop_();_nop_();_nop_();
	}while(t--);
}

/*
\brief Delay about 1ms
*/
void DelayMs(INT16U t)
{
	INT16U x,y;
	DelayNops(1);
	for(x=t;x>0;x--)
		for(y=100;y>0;y--);

}
