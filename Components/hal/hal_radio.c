/*
*\file nrf24l01.c
*\brief nRF24L01 API.
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

#include"hal_radio.h"
#include"delay.h"
#ifdef DEBUG
	#include"hal_uart.h"
#endif


bit rx_buf_rd;

/*
 *\brief Write configuration vlaue to a specified register
 *
 *\param reg - register address
 		 cfg - Configure data
 */
void WriteRegister(uint8 reg,uint8 cfg)
{

	//Enter critical area
	enter_critical();
	//Selecte a register
	SPI_WriteData(reg);
	//Configure data
	SPI_WriteData(cfg);
	//Take effect
	exit_critical();
}

/*
 *\brief When several data packets are lost on the link, the PID fields may become equal to the last received PID.
		 If a packet has the same PID as the previous packet, nRF24L01+ compares the CRC sums from both
		 packets. If the CRC sums are also equal, the last received packet is considered a copy of the previously
		received packet and discarded.
 *
 *\note: payload reuse must not be activated or deactivated during package transmission.
 */
//void Resending(void)
//{
//	//Enter critical area
//	enter_critical();
//	//Reuse the TX payload
//	SPI_WriteData(REUSE_TX_PL);
//	//Take effect
//	exit_critical();
//}

/*
 *\brief Read status from a selected register
 *
 *\param reg - Select register address
 *
 *\return status - Register status
 */
uint8 ReadRegister(uint8 reg)
{
	uint8 status;

	//Enter critical area
	enter_critical();
	//Selecte a register
	SPI_WriteData(reg);
	//Read register current status
	status = SPI_ReadData();
	//Enable
	exit_critical();

	return status;
}

/*
 *\brief Clear TX and RX payload buffer,clear interrupt flag,it's  necessary to invoke this
         function when transmit or receice data successdully or failed,flush the buffer so
		 that nRF24L01 can  continue to transmit or receive the next packet
 */
void ClearPayload(uint8 reg)
{
	if(reg == TX_PAYLOAD_BUF)
	{
		enter_critical();
		SPI_WriteData(FLUSH_TX);
		exit_critical();
	}
	/*If the RX FIFOs are full, the received packet is discarded*/
	else if(reg == RX_PAYLOAD_BUF)
	{
		enter_critical();
		SPI_WriteData(FLUSH_RX);
		exit_critical();
	}
	//Clear interrup flags
	DelayMs(1);
	WriteRegister(WRITE_REG+STATUS,0xff);
	NRF_IRQ = 1;
}

/*
 *\brief Write packet to a slected register,The maximun data length is 32 bytes
 *
 *\param reg   - Selected register address
         *rBuf - Data address
		 len   - Data length
 */
void WriteRegisterBuffer(uint8 reg,uint8 *rBuf,uint8 len)
{
	uint8 i;

	enter_critical();
	//Selecte a register
	SPI_WriteData(reg);
	//Write data in buffer
	for(i=0;i<len;i++)
		SPI_WriteData(*rBuf++);
	exit_critical();

}

/*
 *\brief Read packet from a slected register,The maximun data length is 32 bytes
 *
 *\param reg   - Selected register address
         *rBuf - Data buffer
		 len   - Data length
 */
void ReadRegisterBuffer(uint8 reg,uint8 *rBuf,uint8 len)
{
	uint8 i;

	enter_critical();
	//Selecte a register
	SPI_WriteData(reg);
	//Write data in buffer
	for(i=0;i<len;i++)
		rBuf[i] = SPI_ReadData();
	exit_critical();
}

/*
 *\brief nRF24L01 enter power down mode,configure nRF24L01 need call this function first.
 */
void RadioPowerDown(void)
{
	/*
		In power down mode nRF24L01+ is disabled using minimal current consumption.
         All register values available are maintained and the SPI is kept active,
		 enabling change of configuration and the uploading/downloading of data
		 registers,
	 */
	WriteRegister(WRITE_REG+CONFIG,0x0D);
	DelayMs(1);
}

/*
 *\brief  Configure nRF24L01 in TX mode
 */
void TX_Mode(uint8 *addr,uint8 ch)
{
	/*
		It is important never to keep the nRF24L01+ in TX mode for more than 4ms at a time. If the
		Enhanced ShockBurst. features are enabled, nRF24L01+ is never in TX mode longer than 4ms.
	 */
	RadioPowerDown();
	radio(OFF);

	//Write TX address
	WriteRegisterBuffer(WRITE_REG+TX_ADDR,addr,ADDR_WIDTH);
	//Write the same address to RX ,enable the auto-ack mode
	WriteRegisterBuffer(WRITE_REG+RX_ADDR_P0,addr,ADDR_WIDTH);
	//Enable auto-ack
	WriteRegister(WRITE_REG+EN_AA,RX_P0);
	//Enable data pipe
	WriteRegister(WRITE_REG+EN_RXADDR,RX_P0);
	//Set up address width
	WriteRegister(WRITE_REG+SETUP_AW,DFT_ADDR_WIDTH);
	//Set up retries mode
	WriteRegister(WRITE_REG+SETUP_RETR,RETR_CONFIG);
	//Set up RF channel
	WriteRegister(WRITE_REG+RF_CH,ch);
	//Set up RF mode,0dbm gain,low noise amprifer,data rate 2Mbps
	WriteRegister(WRITE_REG+RF_SETUP,RF_CONFIG);
	//Power up,16-bits CRC,enable CRC,enable all IRQ..
	WriteRegister(WRITE_REG+CONFIG,CONFIG_TX);
	//Clear TX FIFO
	ClearPayload(TX_PAYLOAD_BUF);
	DelayMs(1);

	radio(ON);
}

/*
 *\brief  Configure	nRF24L01+ in RX mode
 */
void RX_Mode(uint8 *endvAddr,uint8 ch)
{
	/*
		In RX mode the receiver demodulates the signals from the RF channel, constantly
		presenting the demodulated data to the baseband protocol engine
	 */
	RadioPowerDown();
	radio(OFF);

	//Write the same address to RX ,enable the auto-ack mode
	WriteRegisterBuffer(WRITE_REG+RX_ADDR_P0,endvAddr,ADDR_WIDTH);
	//Enable auto-ack
	WriteRegister(WRITE_REG+EN_AA,RX_P0);
	//Enable data pipe
	WriteRegister(WRITE_REG+EN_RXADDR,RX_P0);
	//Set up address width
	WriteRegister(WRITE_REG+SETUP_AW,DFT_ADDR_WIDTH);
	//Set up retries mode
	WriteRegister(WRITE_REG+SETUP_RETR,RETR_CONFIG);
	//Set up RX pipe width
	WriteRegister(WRITE_REG+RX_PW_P0,DATA_LEN);
	//Set up RF channel
	WriteRegister(WRITE_REG+RF_CH,ch);
	//Set up RF mode,0dbm gain,low noise amprifer,data rate 2Mbps
	WriteRegister(WRITE_REG+RF_SETUP,RF_CONFIG);
	//Power up,16-bits CRC,enable CRC,enable all IRQ..
	WriteRegister(WRITE_REG+CONFIG,CONFIG_RX);
	//Clear RX FIFO
	ClearPayload(RX_PAYLOAD_BUF);
	//Enable receive
	radio(ON);

	DelayMs(2);
}

/*
 *\brief Read specified length packet from RX payload buffer
 *
 *\param *pBuf -  Read data from payload buffer
  		 len   - Specified packet length
 *
 *\return RX_OK - Receive compelete
          STANDBY - Wait receive compelete
 */
uint8 ReadPayloadBuf(uint8 *pBuf,uint8 len)
{
	uint8 status;

	if(rx_buf_rd == 1)
	{
		status = ReadRegister(STATUS);
		//Clear flags(Asserted when new data arrives RX FIFO,and write 1 to clear interrupt flag)
		WriteRegister(WRITE_REG+STATUS,status);
		rx_buf_rd = 0;
		if(status&RX_OK)
		{
			ReadRegisterBuffer(READ_PAYLOAD_BUF,pBuf,len);
			ClearPayload(RX_PAYLOAD_BUF);

			return RX_OK;
		}

	}

	return RX_STANDBY;
}

/*
 *\brief Write specified length packet fto TX payload buffer
 *
 *\param *pBuf - Data to payload buffer
  		 len   - Specified packet length
 */
uint8 WritePayloadBuf(uint8 *pBuf,uint8 len)
{
	uint8 status;
	WriteRegisterBuffer(WRITE_PAYLOAD_BUF,pBuf,len);
	//Open the radio
	radio(ON);
	//Wait data transmit complete
	while(NRF_IRQ);
	status = ReadRegister(STATUS);
	/*Clear flags(Asserted whenpacket transmitted on TX,and write 1 to clear interrupt flag)*/
	WriteRegister(WRITE_REG+STATUS,status);
	if(status&TX_OK)
	{
		//Flush TX FIFO
		ClearPayload(TX_PAYLOAD_BUF);
		return TX_OK;
	}
	else if(status&MAX_RT)
	{
		ClearPayload(TX_PAYLOAD_BUF);
		return MAX_RT;
	}
	//TX FIFO is empty
	return TX_STANDBY;
}

void nRF24L01_ISR(void) interrupt 11
{
	uint8 byte;

	//TODO: Receive mode,count the payload length totally 32 bytes
	byte++;
	rx_buf_rd = 1;
}
