/*
*\file main.c
*\brief   Program entry,main loop for task polling.
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
#include"systemRT.h"

int main(void)
{
	SystemInit();

	while(1)
	{
		RunSystem();
	}

	return 0;
}
