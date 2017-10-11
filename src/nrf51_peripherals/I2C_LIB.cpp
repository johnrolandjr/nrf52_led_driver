#include "I2C_LIB.h"

extern nrf_drv_twi_t m_twi;

/** Specific address constructor.
 * @param address I2C address
 * @see MPU6050_DEFAULT_ADDRESS
 * @see MPU6050_ADDRESS_AD0_LOW
 * @see MPU6050_ADDRESS_AD0_HIGH
 */
I2C_LIB::I2C_LIB(char addr, uint8_t data, uint8_t clk){
    slaveAddr = addr;
    dataPin = data;
    clkPin = clk;

    twi = &m_twi;
    i2c_api_init(data,clk);
}

bool I2C_LIB::I2C_Write ( uint8_t *data, int length ) {
	uint32_t err;
	while( 	err = nrf_drv_twi_tx(twi, slaveAddr, data, length, false)  != NRF_SUCCESS ){
		nrf_delay_us(100);
	};
	return true;
}
bool I2C_LIB::I2C_Write( uint8_t reg, uint8_t val){
	uint8_t data[2];
	uint32_t err;
	data[0] = reg;
	data[1] = val;
	err = nrf_drv_twi_tx(twi, slaveAddr, data, 2, false);
	if(err != NRF_SUCCESS ){
		NRF_GPIO->OUTSET = (1<<this->clkPin) | (1<<this->dataPin);
		nrf_delay_us(10);

		uint16_t clearBusCount = 0;
		uint32_t port = NRF_GPIO->OUT & ((1<<this->clkPin)|(1<<this->dataPin));
		while( port != 0x60){
			port = NRF_GPIO->OUT & 0x60;
			nrf_delay_us(10000);
			NRF_GPIO->OUTCLR = 1<<this->clkPin;
			nrf_delay_us(10000);
			NRF_GPIO->OUTSET = 1<<this->clkPin;
			nrf_delay_us(10000);
			if(clearBusCount > 100){
				clearBusCount = 0;
			}
		}
	}
	return true;
}

bool I2C_LIB::I2C_Read ( uint8_t cmd, uint8_t *data, int length ){
	uint32_t err_code;
	err_code = 0;

	nrf_drv_twi_tx(twi, slaveAddr, &cmd, 1 , true);
	err_code = nrf_drv_twi_rx(twi, slaveAddr, data, length);

	return (err_code==NRF_SUCCESS?true:false);
}
uint8_t I2C_LIB::slaveAddrFinder(void)
{
	uint8_t addr = 0;
	uint8_t data[2];
	uint32_t err;
	data[0] = 0x13;
	data[1] = 0x80;
	for(uint32_t testSlaveAddr = 0x4c; testSlaveAddr < 128; testSlaveAddr++)
	{
		err = nrf_drv_twi_tx(twi, testSlaveAddr, data, 2, false);
		if(err == NRF_SUCCESS )
		{
			addr = testSlaveAddr;
			break;
		}
		nrf_delay_us(20);
	}

	return addr;
}

