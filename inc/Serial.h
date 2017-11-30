#ifndef SERIAL_H_
#define SERIAL_H_

#include "stdint.h"

extern "C"{
	#include "nrf.h"
	#include <nrf_delay.h>
	#include <sdk_config.h>
	#include "nrf_drv_uart.h"
}



class Serial {
	private:
		uint32_t txPin;
		uint32_t rxPin;
		bool bOnlyTransmit;
    public:
        Serial(uint32_t tx, uint32_t rx);
        Serial(uint32_t tx);

    public:
        void init(void);
        void send(char * pData, unsigned int numBytes);
};

#endif
