/*
*\file nwk_service.c
*\brief   Network service.
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
#include"nwk_service.h"
#include"hal_uart.h"
#include"nwk_defs.h"
#include"utils.h"
#include"delay.h"



int16_u	panId;
uint8 idata rx_buf[DATA_LEN];
uint8 idata tx_buf[DATA_LEN];//="This is a test data from ZIC03\r\n";//
bit retry = 1;


/*
 *\breif Assgin network address

 *\param nwkCfg - Network status initialization
   		 panCfg - Network address initialization
 */
void nwkNetworkInit(nwkStat_t *nwkCfg,pan_t *panCfg)
{
	panId.word = PAN_ID;
	panCfg->nwkAddr[0] = panId.byte.msb;
	panCfg->nwkAddr[1] = panId.byte.lsb;
	panCfg->dstAddr[0] = panId.byte.msb;
	panCfg->dstAddr[1] = panId.byte.lsb;
	#if defined(ROUTER)
		panCfg->nwkAddr[2] = ROUTER_ADDR;
		panCfg->dstAddr[2] = COORDINATOR_ADDR;
		nwkCfg->channel = MAIN_CHANNEL;
		//Router default mode
		nwkCfg->mode = ENTER_RX;
	#elif defined(END_DEVICE)
		panCfg->nwkAddr[2] = ENDEVICE_ADDR;
		panCfg->dstAddr[2] = ROUTER_ADDR; //test: ENDEVICE_ADDR;//
		nwkCfg->channel = ENDEVICE_CHANNEL;
		//End device default mode
		nwkCfg->mode = ENTER_RX; //2014.8.28 Debug change \original RX
	#endif

	nwkCfg->lock = OFF;

}

/*
 *\breif Network mode,toggle network TX/RX mdoe,return current status
 *
 *\param *addr - Dstiantion address
 		 *status - Network status
 *
 *\return ENTER_TX - Net work in TX mode.
 		  ENTER_RX - Net work in RX mode.
 */
uint8 nwkNetworkMode(pan_t *addr,nwkStat_t *status)
{
	if(status->mode == ENTER_TX)
	{
		if(status->lock == OFF)
		{
			TX_Mode(addr->dstAddr,status->channel);
			#ifdef DEBUG
				PrintString("Enter TX mode!\r\n");
			#endif
			status->lock = ON;
		}

		return ENTER_TX;
	}
	else
	{
		if(status->lock == OFF)
		{
			RX_Mode(addr->nwkAddr,status->channel);
			#ifdef DEBUG
				PrintString("Enter RX mode!\r\n");
			#endif
			status->lock = ON;


		}

		return ENTER_RX;
	}

	return ERROR;
}

/*
 *\brief Network service routine,transmition and receive data
 *
 *\param *nwkSver - Network service imformation
 *
 *\return TX_OK -  		TX Completed correctly;
  		  RX_OK - 		RX Completed correctly;
		  MAX_RT - 		Maximun retry overflow;
		  RX_STANDBY - 	Receive busy;
		  TX_STANDBY - 	TX busy;
		  SERVICE_ERR - No service mode.
 */
uint8 nwkNetworkService(nwkStat_t *nwkSver)
{
	uint8 status;

	if(nwkSver->mode == ENTER_TX)
	{
		status = WritePayloadBuf(tx_buf,DATA_LEN);
		if(status==TX_OK)
		{
			#ifdef DEBUG
				PrintString("TX Complete!\r\n");
			#endif
			#if defined(ROUTER)
				nwkSver->lock = ON;
			#else
				nwkSver->mode = ENTER_RX; //2014.8.28 debug \original RX
				nwkSver->lock = OFF;
				retry = 1;
			#endif

			return TX_OK;
		}
		else if(status==MAX_RT)
		{
			#ifdef DEBUG
				PrintString("Retrying,please check device connection!\r\n");
			#endif
			//Retry 15 times,if the coordinator not acknowledge,router enter receive mode
			//for the next communication
			#if	defined( ROUTER )
		//		nwkSver->mode = ENTER_RX;
				nwkSver->lock = ON;
			#elif defined( END_DEVICE )
				if(retry)
				{
					retry = 0;
				}
				else
				{
					nwkSver->mode = ENTER_RX; //2014.8.28 Debug change \original RX
					nwkSver->lock = OFF;
				}
			#endif

			return MAX_RT;

		}
		else
		{
			#ifdef DEBUG
				PrintString("Standby!!!\r\n");
			#endif

			return TX_STANDBY;
		}
	}
	else
	{
		status = ReadPayloadBuf(rx_buf,DATA_LEN);
		if(status == RX_OK)
		{
			PrintData(rx_buf,DATA_LEN);
			#if defined(ROUTER)
				nwkSver->lock = ON;
			#else
				nwkSver->mode = ENTER_TX;  //2014.8.28 Debug change \original TX
				nwkSver->lock = OFF;
			#endif

			return RX_OK;
		}
		else
		{
			//TODO:
			#ifdef DEBUG
				PrintString("Receive failed!\r\n");
			#endif

			return RX_STANDBY;
		}
	}

	return SERVICE_ERR;

}
