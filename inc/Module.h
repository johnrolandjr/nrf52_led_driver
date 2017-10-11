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
 * The Module class can be subclassed for a number of purposes:
 * - Implement a driver for a sensor or an actuator
 * - Add functionality like parsing advertising data, etc...
 *
 * It provides a basic set of handlers that are called from the main event handling
 * routines and the received events can be used and acted upon.
 *
 */

#pragma once

#ifdef __cplusplus
#include <conn_packets.h>
#include <Config.h>
#include <ConnectionManager.h>
#include <LedWrapper.h>

extern "C"{
#include <ble.h>
#include <app_error.h>
}

extern "C"{
extern void modulePinTriggerHandler(uint32_t const * event_pins_low_to_high, uint32_t const * event_pins_high_to_low);
extern void gpio_interrupt_init(uint32_t lowToHighPinNum);
extern void gpio_interrupt_enable(void);
extern void gpio_interrupt_disable(void);
}

extern void startTimers(uint32_t periodMs);
extern void stopTimers(void);

class Node;

#define MODULE_NAME_MAX_SIZE 10

//class Module : public StorageEventListener, public TerminalCommandListener
class Module
{
	public:
		LedWrapper* led;
		bool bOperationRequested = false;
		bool bStartTimerAtBeginning = false;
		uint32_t startingTimerPeriod = 0;

	public:
		char moduleName[MODULE_NAME_MAX_SIZE];

		//Constructor is passed
		Module(LedWrapper* pLed, const char* name);
		virtual ~Module();

		//##### Handlers that can be implemented by any module, but are implemented empty here

		//Called when the module should send back its data
		virtual void GetDataHandler(u8* request, u8 length){};

		//Handle system events
		virtual void SystemEventHandler(u32 systemEvent){};

		//This handler receives all timer events
		virtual void TimerEventHandler(u16 passedTime){};

		virtual void startOperations(void){};
		virtual void performOperation(void){};
		virtual void immediateOperation(uint32_t l2h, uint32_t h2l){};

};
#endif
