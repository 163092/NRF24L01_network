/*
*\file nwk_addrMgr.c
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

#include"nwk_addrMgr.h"
#include"hal_uart.h"
#include"hal_led.h"
#include"utils.h"


uint8  idata tx_buf[DATA_LEN];// = "This is a test data from ZIC00\r\n";
uint8  idata rx_buf[DATA_LEN];
int16_u panId;			/*< PAN identity >*/
nwkStat_t nwkStatus;
pan_t panAddr;	/*< PAN address [PAN_ID,address]>*/
time_t rxDetect; /*< Detect RX time counter overflow>*/
/*
 *\brief When several data packets are lost on the link, the PID fields may become equal to the last received PID.
		 If a packet has the same PID as the previous packet, nRF24L01+ compares the CRC sums from both
		 packets. If the CRC sums are also equal, the last received packet is considered a copy of the previously
		received packet and discarded.
 */
bit retry = 1; /*< Write data to TX buffer again,the the PID increase >*/


/*
 *\breif Assign a network address
 */
void nwkAddressInit(void)
{
	panId.word = PAN_ID;
 	tx_buf[0] = COORDINATOR_ADDR; //Primary TX address imformation
	tx_buf[1] = CONNECTED;		  //Data request
	panAddr.nwkAddr[0] = panId.byte.msb;
	panAddr.nwkAddr[1] = panId.byte.lsb;
	panAddr.nwkAddr[2] = COORDINATOR_ADDR;
	panAddr.index = 1;
	nwkStatus.lock = 0;
	nwkStatus.mode = ENTER_TX;
}

/*
 *\brief Network address index
 */
void nwkAddressManager(void)
{
	if(panAddr.index > MAX_ROUTER_NUM)
	{
		panAddr.index = 1;
		tx_buf[1] = CONNECTED; //Reset completed
	}
	panAddr.dstAddr[0] = panId.byte.msb;
	panAddr.dstAddr[1] = panId.byte.lsb;
	panAddr.dstAddr[2] = panAddr.index;
	panAddr.index++;
}

/*
 *\brief Network mode initialized
 *
 *\return ENTER_RX - RX mode
 		  ENTER_TX - TX mode
		  ERROR - Network error
 */
uint8 nwkNetworkMode(void)
{
	if(nwkStatus.mode == ENTER_TX)
	{
		if(nwkStatus.lock == OFF)
		{
			TX_Mode(panAddr.dstAddr,MAIN_CHANNEL);
			#ifdef DEBUG
				PrintString("enter TX mode!\r\n");
			#endif
			nwkStatus.lock = ON;
		}

		return ENTER_TX;
	}
	else
	{
		if(nwkStatus.lock == OFF)
		{
			RX_Mode(panAddr.nwkAddr,MAIN_CHANNEL);
			#ifdef DEBUG
				PrintString("enter RX mode!\r\n");
			#endif
			nwkStatus.lock = ON;
			//Get RX time-out
			rxDetect.start = ON;
			rxDetect.ticks = 0;
			rxDetect.flag = 0;
		}

		return ENTER_RX;
	}

	return ERROR;
}

/*
 *\biref Network service routine,automatic network toggle after RX/TX completed
 */
void nwkNetworkServices(void)
{
	uint8 status;

	if(nwkStatus.mode == ENTER_TX)
	{
		status = WritePayloadBuf(tx_buf,DATA_LEN); //Write data to TX FIFO,PID increase
		if(status == TX_OK)
		{
			#ifdef DEBUG
				PrintString("TX Complete!\r\n");
			#endif
			nwkStatus.mode = ENTER_RX;
			nwkStatus.lock = OFF;  //Unlock network
			retry = 1;
		}
		else if(status == MAX_RT)
		{
			#ifdef DEBUG
				PrintString("Retrying,please check device connection!\r\n");
			#endif
			if(retry)
			{
				nwkStatus.mode = ENTER_TX;
		//		nwkStatus.lock = OFF;
				retry = 0;
			}
			else
			{

				nwkAddressManager();
				nwkStatus.lock = OFF;
				retry = 1;
			}

		}
		else
		{
			#ifdef DEBUG
				PrintString("Standby!!!\r\n");
			#endif
		}
	}
	else
	{
		status = ReadPayloadBuf(rx_buf,DATA_LEN);
		if(status==RX_OK)
		{
			SendData(rx_buf[1],rx_buf[0]);
//			PrintData(rx_buf,DATA_LEN);	 //XXX: PC protocol
			#ifdef DEBUG
				PrintString("Received!\r\n");
			#endif
			nwkAddressManager();
			nwkStatus.mode = ENTER_TX;
			nwkStatus.lock = OFF;
		}
		else
		{
		 	//TODO:If RX overflow flag assert,enter TX mode to communication with the next router
		 	if(rxDetect.flag == 1)
			{
				nwkAddressManager();
				nwkStatus.mode = ENTER_TX;
				nwkStatus.lock = OFF;
			}
		}
	}
}

/*
 *\brief  Network management
 */
void nwkNetworkManager(void)
{
	uint8 status;

	//Get a network mode
	status = nwkNetworkMode();
	//Indicate current network mode
	#if (HAL_LED == TRUE)
		halModeLedToggle(status);
	#endif
	//Net work service
	nwkNetworkServices();

}
