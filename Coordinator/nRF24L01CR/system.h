/*
*\file system.h
*\brief Logic driver macros.
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
#ifndef __SYSTEM_H
#define	__SYSTEM_H

#include"sys_types.h"

typedef struct __OPERATE{
			uint8 mode		:4;
			uint8 flag		:2;
			uint8 entx		:2;
}operate_t;


void SystemInit(void);
void RunSystem(void);
void PrintData(uint8 *addr,uint8 len);
void enter_ack(void);




#endif
