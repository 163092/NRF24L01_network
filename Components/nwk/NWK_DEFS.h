/*
*\file nwk_defs.h
*\brief   Network address and channel definitions.
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
#ifndef NWK_DEFS_H
#define	NWK_DEFS_H

#include"sys_types.h"

#define		PAN_ID					0x2014	/*< Personal area network ID>*/
#define		ROUTER_ADDR				0x04	/*< Router address,specified 1~32>*/
#define		COORDINATOR_ADDR		0x00	/*< Coordinater address,specified by programer>*/

#define		MAX_ROUTER_NUM			30		/*< Maximun number of router  >*/
#define		NWK_FM_SKIP				4		/*< Frenquence skiping step length >*/

/**
 *\brief Each router and end device using a unique channel communication,router and
 		 end device never in the same mode simultaneously,so they can use the same
		 address, but if too many router in the network,we have not enough communication
		 channels can be used,then the end device using the basic channel with a unique
		 address to communicate with the coordinator
 */
#if ( MAX_ROUTER_NUM > 30)
	#define		ENDEVICE_ADDR			((ROUTER_ADDR + MAX_ROUTER_NUM) + NWK_FM_SKIP) /*< End device address >*/
#else
	/*
	 *\note: Signal interference if channel spacing is 4
	 		 Wide Band Blocking measurements��
			 30MHz to 2000MHz; 10MHz step size
			 2000MHz to 2399MHz; 3MHz step size
			 2484MHz to 3000MHz; 3MHz step size
			 3GHz to 12.75GHz; 25MHz step size
	 */
	#define		ENDEVICE_ADDR			((ROUTER_ADDR + MAX_ROUTER_NUM) + NWK_FM_SKIP)	/*< \comment: overlapping channels >*/
#endif

#define		MAIN_CHANNEL			0	 /*< Communication channel between router and coordinator>*/
#define		BASIC_CHANNEL			MAIN_CHANNEL
#if ( MAX_ROUTER_NUM > 30)
	#define		ENDEVICE_CHANNEL		BASIC_CHANNEL
#else
	#define		ENDEVICE_CHANNEL		((ROUTER_ADDR * NWK_FM_SKIP) + (BASIC_CHANNEL))  /*< Channel spacing is 4>*/
#endif

/*
 *\brief Status imformation
 */
#define		CONNECTED		0x20
#define		KILLED			0x40
#define		FOULED			0x60
#define		HANDSUP			0x80
#define		SURRENDER		0xA0
#define		DISCONNECT		0xC0
#define		RESET_NWK		0xE0

typedef struct
{
	uint8 nwkAddr[ADDR_WIDTH]; /*< PAN address>*/
	uint8 dstAddr[ADDR_WIDTH];
	uint8 index;			 /*< Address index >*/
}pan_t;

typedef struct
{
	uint8 channel; 		  /*< RF Channel 0~125>*/
	uint8 mode		:4;	  /*< Current network mode>*/
	uint8 lock		:4;	  /*< Lock current network mode utill system toggle it>*/
}nwkStat_t;

/************Network mode*************/
#define			ENTER_RX 		0
#define			ENTER_TX 		1
#define			ERROR			3

#if defined(ROUTER)
	#define			MAX_RX_RETRY	20
#else
	#define			MAX_RX_RETRY	10
#endif



#endif
