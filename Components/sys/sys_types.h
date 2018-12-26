/*
*\file types.h
*\brief  Commonly types and macros.
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


#ifndef __TYPES_H
#define __TYPES_H

typedef unsigned char 	INT8U;
typedef unsigned int	INT16U;
typedef unsigned long	INT32U;
typedef int	  INT16;
typedef char  INT8;
typedef long  INT32;

typedef unsigned char 	uint8;
typedef unsigned int	uint16;
typedef unsigned long	uint32;
typedef int	  int16;
typedef char  int8;
typedef long  int32;

typedef struct __time{
		INT16U ticks;
		INT8U sec;
		INT8U min;
		INT8U hour;
		INT8U start	:4;
		INT8U flag	:4;
}time_t;

#endif
