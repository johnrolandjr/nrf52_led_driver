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
 * The GAP Controller wraps SoftDevice calls for initiating and accepting connections
 * It should also provide encryption in the future.
 */

#pragma once

#include "types.h"
#include <conn_packets.h>

extern "C"{
	#include <ble.h>
}

class GAPController
{

private:
	static void (*connectionTimeoutCallback)(ble_evt_t* bleEvent);
	static void (*connectionSuccessCallback)(ble_evt_t* bleEvent);
	static void (*disconnectionCallback)(ble_evt_t* bleEvent);

	//Set to true if a connection procedure is ongoing
	static bool currentlyConnecting;


public:
	//Initialize the GAP module
	static void bleConfigureGAP();
	static void updateConnectResponse(updateConnectionSettingsPacket* pSettings);

	//Configure the callbacks
	static void setConnectionTimeoutHandler(void (*callback)(ble_evt_t* bleEvent));
	static void setConnectionSuccessfulHandler(void (*callback)(ble_evt_t* bleEvent));
	static void setDisconnectionHandler(void (*callback)(ble_evt_t* bleEvent));

	//Connects to a peripheral with the specified address and calls the corresponding callbacks
	static bool connectToPeripheral(ble_gap_addr_t* address);
	//Disconnects from a peripheral when given a connection handle
	static void disconnectFromPeripheral(u16 connectionHandle);

	//This handler is called with bleEvents from the softdevice
	static bool bleConnectionEventHandler(ble_evt_t* bleEvent);
};

