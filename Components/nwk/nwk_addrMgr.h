/*
*\file nwk_addrMgr.h
*\brief   Network address management.
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
#ifndef NWK_ADDRMGR_H
#define	NWK_ADDRMGR_H


#include"sys_types.h"
#include"hal_radio.h"
#include"nwk_defs.h"


extern uint8  idata tx_buf[DATA_LEN];
extern uint8  idata rx_buf[DATA_LEN];
extern time_t	rxDetect;
extern pan_t panAddr;



void nwkAddressInit(void);
void nwkAddressManager(void);
uint8 nwkNetworkMode(void);
void nwkNetworkServices(void);
void nwkNetworkManager(void);



#endif
