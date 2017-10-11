
extern "C"{
#include "nrf_drv_twi.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "i2c_api.h"
}

#ifndef _I2C_LIB_H_
#define _I2C_LIB_H_

#define MAX_TIMEOUT_LOOPS (20000UL) /**< MAX while loops to wait for RXD/TXD event */

class I2C_LIB {
    
    private:
        //char slaveAddr;
        uint8_t dataPin;
        uint8_t clkPin;
        nrf_drv_twi_t* twi;
    public:
        char slaveAddr;
    public:
        I2C_LIB();
        I2C_LIB(char addr, uint8_t data, uint8_t clk);
        

        bool I2C_Read ( uint8_t cmd, uint8_t *data, int length);
        bool I2C_Write ( uint8_t *data, int length );
        bool I2C_Write ( uint8_t reg, uint8_t val);

        uint8_t slaveAddrFinder(void);
};
#endif
