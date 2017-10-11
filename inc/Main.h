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

/*
 * This is the main class that initializes the SoftDevice and starts the code.
 * It contains error handlers for all unfetched errors.
 */

#pragma once


#include <types.h>

extern "C"{
#include <ble.h>
#include "app_timer.h"
#include "nrf_drv_clock.h"
}

void mainProgram(void);

void bleDispatchEventHandler(ble_evt_t * p_ble_evt);
void bleDispatchEventHandlerToo(ble_evt_t * p_ble_evt);

void bleInit(void);
u32 initNodeID(void);

static void lfclk_request(void);
void initTimers(void);
void startTimers(uint32_t periodMs);
void stopTimers(void);

static void timerEventDispatch(u16 passedTime, u32 appTimer);


//These are the event handlers that are notified by the SoftDevice
//The events are then broadcasted throughout the application
extern "C"{
	void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name);
	void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name);
	void sys_evt_dispatch(uint32_t sys_evt);

	//ret_code_t FDS_init(void);
	//ret_code_t FDS_read(void);
	//ret_code_t FDS_write(void);
}




/** @} */
