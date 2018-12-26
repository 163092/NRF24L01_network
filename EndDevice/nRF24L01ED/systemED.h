/*
*\file systemED.h
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
#ifndef	__SYSTEMED_H
#define	__SYSTEMED_H


/*
 *\brief Task events priority definition
 */
#define		MAX_TASK_NUM		   3
#define		MASSAGE_MGR			   0
#define		KEY_PROCESS			   1
#define		POWER_MGR			   2
//#define		FOUL_DETECT			   1
#define     enable_wdt()			st(WDT_CONTR = 0x37;)
#define		clr_wdt()				st(WDT_CONTR = 0x37;)



void timer0_init(void);
void inter_init(void);
void DataInit(void);
void SystemInit(void);
void KeyProcess(void);
void NetworkManagement(void);
void RunSystem(void);
void FlushRxTicks(void);
void FlushFoulDetectTicks(void);
void FoulDetect(void);





#endif
