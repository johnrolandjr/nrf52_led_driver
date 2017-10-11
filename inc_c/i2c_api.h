/*
 * i2c_api.h
 *
 *  Created on: Oct 7, 2015
 *      Author: Beau
 */

#include "app_util_platform.h"
#include <stdint.h>
#include "app_error.h"
#include "nrf_drv_twi.h"
#include "nrf_delay.h"

#ifndef I2C_API_H_
#define I2C_API_H_

void i2c_api_init(uint8_t sdaPin, uint8_t clkPin);

#endif /* I2C_API_H_ */
