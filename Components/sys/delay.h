/*
*\file delay.h
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

#ifndef __DELAY_H
#define __DELAY_H

#include"sys_types.h"
#include<STC/STC15F2K60S2.H>

void DelayNops(INT8U t);
void DelayMs(INT16U t);

#endif
