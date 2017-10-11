/**

Copyright (c) 2014-2015 "M-Way Solutions GmbH"
FruityMesh - Bluetooth Low Energy mesh protocol [http://mwaysolutions.com/]

This file is part of FruityMesh

FruityMesh is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <Module.h>
//#include <Storage.h>
#include <Node.h>

extern "C"{
#include <stdlib.h>
#include <nrf_gpio.h>
#include <app_gpiote.h>
}

extern "C"{
	static app_gpiote_user_id_t m_module_user_id;
	extern void modulePinTriggerHandler(uint32_t const * event_pins_low_to_high, uint32_t const * event_pins_high_to_low);
	void gpio_interrupt_init(uint32_t lowToHighPinNum){
		uint32_t errcode;

		nrf_gpio_cfg_sense_input(lowToHighPinNum, NRF_GPIO_PIN_PULLDOWN, NRF_GPIO_PIN_SENSE_HIGH);

		APP_GPIOTE_INIT(1);

		const uint32_t * pPinEnable;
		*(uint32_t*)pPinEnable = (1<<lowToHighPinNum);
		errcode = app_gpiote_user_register(	&m_module_user_id,					// unique user id for interrupt
											pPinEnable,				// low-to-high enable
											(0),								// high-to-low enable
											modulePinTriggerHandler);			// interrupt callback function
		APP_ERROR_CHECK(errcode);

		errcode = app_gpiote_user_enable(m_module_user_id);
		APP_ERROR_CHECK(errcode);
	}
	void gpio_interrupt_enable(void){
		u32 errcode;
		errcode = app_gpiote_user_enable(m_module_user_id);
		APP_ERROR_CHECK(errcode);
	}
	void gpio_interrupt_disable(void){
		u32 errcode;
		errcode = app_gpiote_user_disable(m_module_user_id);
		APP_ERROR_CHECK(errcode);
	}
}

Module::Module(LedWrapper* pLed, const char* name)
{
	this->led = pLed;

	memcpy(moduleName, name, MODULE_NAME_MAX_SIZE);
}

Module::~Module()
{
}

