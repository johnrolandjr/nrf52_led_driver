/*
 * SPI_slave.h
 *
 *  Created on: Nov 16, 2015
 *      Author: Steven
 */
#include "SPI_bus.h"
#include "stdint.h"

#ifdef __cplusplus // only define if included in c++ source file
extern "C"{
	#include "nrf.h"
	#include <nrf_delay.h>
}

#ifndef SPI_SLAVE_H_
#define SPI_SLAVE_H_

class SPI_slave {
	public:
		uint8_t csPin;
		SPI_bus* pBus;
    public:
        SPI_slave(SPI_bus* bus, uint8_t cs);

        void SPI_Write( uint16_t tx1, uint16_t tx2 );
		int SPI_Read( uint16_t tx1 );
		void SPI_MB_Read( uint8_t reg, uint8_t* dataRx, uint32_t len);

		//flash spi writes
		void SPI_Write( uint8_t* data, uint32_t numBytes);
		void SPI_Write( uint8_t* dataTx1, uint32_t numBytes_1, uint8_t* dataTx2, uint32_t numBytes_2);
		void SPI_Write( uint8_t* dataTx1, uint32_t numBytes_1, int16_t* dataTx2, uint32_t numBytes_2);
		void SPI_WriteFlashCorrection( uint8_t* dataTx1, uint32_t numBytes, int16_t* dataTx2, uint32_t numShorts);
		bool SPI_READ_BYTES_AFTER_CMD(uint8_t* cmd, uint32_t sizeOfCmd, uint8_t* pRxData, uint32_t readCount);
		void SPI_TX_CMD_AND_READ_UNTIL_ZERO_RX(uint8_t* cmd, uint32_t sizeOfCmd);
};

#endif

#endif /* SPI_SLAVE_H_ */
