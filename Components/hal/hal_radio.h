/*
*\file hal_radio.h
*\brief nRF24L01 configuration macros.
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
#ifndef	__NRF24L01_H
#define	__NRF24L01_H


#include"phy_port.h"
#include"utils.h"

/*
 *\brief NRF24L01 instruction set
 */
#define		WRITE_REG			0x20 /* Write data to register,instruction set format is 010AAAAA*/
#define		READ_REG			0x00 /* Read status form a specified register,register adress combination by AAAAA*/
#define		READ_PAYLOAD_BUF	0x61 /* Read receive data form payload buffer,should flush after read data completed*/
#define		WRITE_PAYLOAD_BUF	0xA0 /* Write data to payload buffer,and transmit data when CE = 1*/
#define		FLUSH_RX			0xE2 /* Flush payload buffer for receive mode*/
#define		FLUSH_TX			0xE1 /* Flush payload buffer for transmit mode*/
#define		REUSE_TX_PL			0xE3 /* Use a previous data packet and send it when CE = 1*/
#define		NO_OPERATE			0xFF /* No operation,can be read the status register*/


/*
 *\brief Register adress definitions
 */
#define 	CONFIG			0x00 /* Configuration register,to configure NRF24l01 */
#define		EN_AA			0x01 /* Enable auto-acknowledge*/
#define		EN_RXADDR		0x02 /* Enable receive address,0~5 channle is available*/
#define		SETUP_AW		0x03 /* Set adress width*/
#define		SETUP_RETR		0x04 /* Enable retry mode*/
#define		RF_CH			0x05 /* Set RF channel*/
#define		RF_SETUP		0x06 /* Set RF power ,LNA,etc.*/
#define		STATUS			0x07 /* Status register*/
#define		OBSERVE_TX		0x08 /* Deiecting transmit*/
#define		CD				0x09 /* Carry detecting*/
#define		RX_ADDR_P0		0x0A /* RX Pipeline 0,Default adress is 0xE7E7E7E7E7*/
#define		RX_ADDR_P1		0x0B /* RX Pipeline 1,Default adress is0xC2C2C2C2C2*/
#define		RX_ADDR_P2		0x0C /* RX Pipeline 2,shoud be 0xC2C2C2C2xx*/
#define		RX_ADDR_P3		0x0D /* RX Pipeline 3*/
#define		RX_ADDR_P4		0x0E /* RX Pipeline 4*/
#define		RX_ADDR_P5		0x0F /* RX Pipeline 5*/
#define		TX_ADDR			0x10 /* TX adress,it has the same adress with RX_ADDR_P0 in ShockBurst mode*/
#define		RX_PW_P0		0x11 /* RX Pipeline width set,0~32 bytes mode(down through)*/
#define		RX_PW_P1		0x12 /* RX Pipeline width set,0~32 bytes mode*/
#define		RX_PW_P2		0x13 /* RX Pipeline width set,0~32 bytes mode*/
#define		RX_PW_P3		0x14 /* RX Pipeline width set,0~32 bytes mode*/
#define		RX_PW_P4		0x15 /* RX Pipeline width set,0~32 bytes mode*/
#define		RX_PW_P5		0x16 /* RX Pipeline width set,0~32 bytes mode*/
#define		FIFO_STATUS		0x17 /* Read FIFO status*/

#define		RX_P0			0x01
#define		RX_P1			0x02
#define		RX_P2 			0x04
#define		RX_P3			0x08
#define		RX_P4			0x10
#define		RX_P5			0x20
#define		RX_ALL_PS		0x3F  /* All pipelines */

/*  Address width definitions   reg: SETUP_AW    */
#define		SET_ADDR_WIDTH_3	0x01
#define		SET_ADDR_WIDTH_4	0x02
#define		SET_ADDR_WIDTH_5	0x03
#define		DFT_ADDR_WIDTH		SET_ADDR_WIDTH_3 /* Default address width*/

#if (DFT_ADDR_WIDTH == 0x03)
	#define 	ADDR_WIDTH			5 /*< 5 bytes length >*/
#elif (DFT_ADDR_WIDTH == 0x02)
	#define		ADDR_WIDTH			4 /*< 4 bytes length >*/
#else
	#define		ADDR_WIDTH			3 /*< 3 bytes length >*/
#endif

#define		PIPES_NUM			6 /*< Data pipes number >*/

/*
 *\brief Radio command definitions
 */
#define		PAYLOAD_BUF_LEN				32 /*< Payload buffer length >*/
#define		TX_PAYLOAD_BUF 				1
#define		RX_PAYLOAD_BUF				2
#define		DATA_LEN					32
/* Configuration of radio mode */
#define		TX_MODE					0x00	/*< PRIM_RX bit clear i.e. Bit0=0>*/
#define		RX_MODE					0x01
#define		PWR_UP					0x02
#define		CRCO_16					0x04
#define		EN_CRC					0x08
#define		MASK_MAX_RT				0x10  /*< Max retry transmit counter overflow>*/
#define		MASK_TX_DS				0x20  /*< TX completed >*/
#define		MASK_RX_DS				0x40  /*< RX completed >*/

/*
 *\brief Configure RX mode,power up,16-bits CRC,enable CRC,enable all IRQ..
 */
#define		CONFIG_RX  ( \
			RX_MODE | \
			PWR_UP  | \
			CRCO_16 | \
			EN_CRC  \
				)
/*
 *\brief Configure TX mode,power up,16-bits CRC,enable CRC,enable all IRQ..
 */
#define		CONFIG_TX  ( \
			TX_MODE | \
			PWR_UP  | \
			CRCO_16 | \
			EN_CRC   \
				)
/*
 *!\brief Retries set up(!\note Change value half byte 0~F)
 */
#define		RETR_PERIOD			0x20	 /*< Retries period 700+86us>*/
#if defined(COORDINATOR)
	#define		RETR_MAX_NUM		0x07	/*< The maximum number of retries>*/
#elif defined(ROUTER) || defined(END_DEVICE)
	#define		RETR_MAX_NUM		0x0F
#endif

#define		RETR_CONFIG	( \
			RETR_PERIOD |\
			RETR_MAX_NUM \
					)

/*!\note Change value
	[RF_DR_LOW(bit 5), RF_DR_HIGH(bit 3)]:
	��00�� �C 1Mbps
	��01�� �C 2Mbps
	��10�� �C 250kbps
	��11�� �C Reserved
    Set RF output power in TX mode:
	[1,0]
    '00' �C -18dBm
    '01' �C -12dBm
    '10' �C -6dBm
	'11' �C -0dBm
*/
#define 	RF_CONFIG			0x27 /*< 0dbm gain,low noise amprifer,data rate 250Kbps>*/

/*
 *\brief Status register
 */
#define		TX_FULL			 	0x01 /*< TX FIFO full>*/
#define		TX_OK				0x20
#define		RX_OK				0x40
#define		MAX_RT				0x10 /*< Maximum Retries overflow >*/
#define		TX_STANDBY			0x2F
#define		RX_STANDBY			0x4F
#define		STANDBY				0x0F
#define		SERVICE_ERR			0xFF


#define			radio(x) 				st(CE = x;)
#define			enter_critical()		st(CSN = 0;) //To perform instruction form low to high
#define			exit_critical()			st(CSN = 1;)



void WriteRegister(uint8 reg,uint8 cfg);
uint8 ReadRegister(uint8 reg);
void ClearPayload(uint8 reg);
void WriteRegisterBuffer(uint8 reg,uint8 *rBuf,uint8 len);
void ReadRegisterBuffer(uint8 reg,uint8 *rBuf,uint8 len);
void RadioPowerDown(void);
void TX_Mode(uint8 *addr,uint8 ch);
void RX_Mode(uint8 *endvAddr,uint8 ch);
uint8 WritePayloadBuf(uint8 *pBuf,uint8 len);
uint8 ReadPayloadBuf(uint8 *pBuf,uint8 len);
uint8 Read_reg(uint8 reg);
uint8 Write_reg(uint8 reg,uint8 cfg);



#endif
