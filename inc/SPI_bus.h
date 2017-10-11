#include "stdint.h"

#ifdef __cplusplus // only define if included in c++ source file
extern "C"{
	#include "nrf.h"
	#include <nrf_delay.h>
}

#ifndef SPI_LIB_H_
#define SPI_LIB_H_

//itg3701 spi defines
 #define R_BIT   0x80
 #define W_BIT  ~0x80
 #define MB_BIT  0x40
 
typedef enum
{
	freq_125khZ = SPIM_FREQUENCY_FREQUENCY_K125,
	freq_1Mhz=SPIM_FREQUENCY_FREQUENCY_M1,
	freq_2Mhz=SPIM_FREQUENCY_FREQUENCY_M2,
	freq_4Mhz=SPIM_FREQUENCY_FREQUENCY_M4,
	freq_8Mhz=SPIM_FREQUENCY_FREQUENCY_M8,
}spi_bus_speeds;

class SPI_bus {
	private:
		uint8_t mosiPin;
		uint8_t misoPin;
		uint8_t clkPin;
    public:
        SPI_bus(uint8_t mosi, uint8_t miso, uint8_t sclk);
        
        void changeBusSpeed(spi_bus_speeds newFreq);

        void clearSpiBus(void);
        uint8_t busWrite(uint8_t tx);
        void busWrite( uint16_t tx1, uint16_t tx2 );
        int busRead( uint16_t tx1 );
        void busRead(uint8_t tx, uint8_t* dataRx, uint32_t rxLen);
        void busMB_Read( uint8_t reg, uint8_t* dataRx, uint32_t len);
        void busMB_Read( uint8_t* dataRx, uint32_t len);	//doesn't use the MB bit  ... i.e. flash uses an internal register counter for multibyte reads
        void busMB_Read_sendff( uint8_t* dataRx, uint32_t len);
        bool busMB_transfer(uint8_t* txData, uint32_t txLen, uint8_t* rxData, uint32_t rxLen);

        void busWrite( uint8_t* data, uint32_t numBytes);
        void busWriteShorts( int16_t* data, uint32_t numBytes);
        void busWrite_FlashCorrection( uint8_t* cmd, uint32_t cmdNumBytes, int16_t* pData, uint32_t numShorts);

        int getClkPin()	{ return clkPin;}
        int getMisoPin(){ return misoPin; }
        int getMosiPin(){ return mosiPin; }
};
#endif

#endif
