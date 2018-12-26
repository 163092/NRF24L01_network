/*
*\file hal_key.h
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
#ifndef	HAL_KEY_H
#define	HAL_KEY_H

#include"phy_port.h"

/**********Independent key *********/
#define		KEY_PUSHED		1
#define		KEY_STANDBY		0

/***********Matrix keynoard***********/
//TODO:


uint8 GetKeyStatus(void);
void Debounce(void);


#endif
