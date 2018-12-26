/*
*\file utils.h
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
#ifndef UTILS_H
#define	UTILS_H

#include"sys_types.h"


/*
*  This macro is for use by other macros to form a fully valid C statement.
*  Without this, the if/else conditionals could show unexpected behavior.
*
*  For example, use...
*    #define SET_REGS()  st( ioreg1 = 0; ioreg2 = 0; )
* (The while condition below evaluates false without generating a
*  constant-controlling-loop type of warning on most compilers.)
*/
#define		st(x)			do{ x }while(__LINE__==-1)


#define		ToString(x)			#x
#define		ToPaste(x,y)		x##y

typedef union
{
	struct __byte
	{
		uint8 msb;
		uint8 lsb;
	}byte;
	uint16 word;
}int16_u;




#endif
