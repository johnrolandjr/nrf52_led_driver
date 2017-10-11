/*
 * i2c_api.c
 *
 *  Created on: Oct 7, 2015
 *      Author: Beau
 */

#include "i2c_api.h"
#include "nrf_gpio.h"
#include "nrf_drv_uart.h"

#define TWI0_USE_EASY_DMA 	0
#define TWI0_INSTANCE_INDEX	0
nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(0);

#define MAX_PENDING_TRANSACTIONS 10
void i2c_api_init(uint8_t sdaPin, uint8_t clkPin)
{
	/*
	uint32_t err_code;

	p_config.frequency = TWI_FREQUENCY_FREQUENCY_K400;
	p_config.interrupt_priority = APP_IRQ_PRIORITY_LOW;
	p_config.sda = (uint32_t)sdaPin;
	p_config.scl = (uint32_t)clkPin;

	err_code = nrf_drv_twi_init(&twi, &p_config, NULL);
	APP_ERROR_CHECK(err_code);
	nrf_delay_us(10);
	nrf_drv_twi_enable(&twi);
	*/
    ret_code_t err_code;

    const nrf_drv_twi_config_t m_twi_config = {
       .scl                = (uint32_t)clkPin,
       .sda                = (uint32_t)sdaPin,
       .frequency          = NRF_TWI_FREQ_100K,
       .interrupt_priority = APP_IRQ_PRIORITY_LOW
    };

    err_code = nrf_drv_twi_init(&m_twi, &m_twi_config, NULL, NULL);
    APP_ERROR_CHECK(err_code);

    nrf_delay_us(10);
    nrf_drv_twi_enable(&m_twi);
}
