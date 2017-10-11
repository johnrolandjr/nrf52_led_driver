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
 * The Connection Class in instantiated once for every possible BLE connection,
 * either as a Master or Slave Connections. It provides methods and event handlers
 * to receive or send messages and handles the mesh-handshake.
 */

#pragma once

#include <types.h>
#include <Config.h>

extern "C"{
	#include <ble.h>
	#include <ble_gap.h>
}

#define MAX_NUM_RETRIES_MISSING_PKTS 8

void printHex(u8* srcBuffer, u32 srcLength);

class Node;
class ConnectionManager;

class Connection
{
	private:
		Node* node; //Reference to the own Node
		ConnectionManager* connectionManager;
		void Init();
	public:
		nodeID targetNodeId;
	public:
		//Types
		enum ConnectionDirection { CONNECTION_DIRECTION_IN, CONNECTION_DIRECTION_OUT };

		bool isConnected;
		bool handshakeDone;

		//Initializes connection but does not connect
		Connection(u8 id, ConnectionManager* cm, Node* node, ConnectionDirection direction);

		void DiscoverCharacteristicHandles(void);
		void StartHandshake(void);

		bool Connect(ble_gap_addr_t* address, u16 writeCharacteristicHandle);
		void Disconnect(void);
		
		//Encryption
		void EncryptConnection();
		
		//Handler
		void ConnectionSuccessfulHandler(ble_evt_t* bleEvent);
		void DisconnectionHandler(ble_evt_t* bleEvent);
		void ReceivePacketHandler(connectionPacket* inPacket);

		//void SendNextMessageHandler(ble_evt_t* bleEvent);
		
		//Helpers
		void PrintStatus(void);

		u8 GetAverageRSSI();

		//Variables
		u8 connectionId;
		ConnectionDirection direction;
		
		//Buffers
		u8 reliableBuffersFree; //reliable transmit buffers that are available currently to this connection
		u8 packetSendBuffer[PACKET_SEND_BUFFER_SIZE];

		u8 packetSendPosition; //Is used to send messages that consist of multiple parts

		u8 packetReassemblyBuffer[PACKET_REASSEMBLY_BUFFER_SIZE];
		u8 packetReassemblyPosition; //Set to 0 if no reassembly is in progress

		//Partner
		nodeID partnerId;
		u16 connectionHandle; //The handle that is given from the BLE stack to identify a connection
		ble_gap_addr_t partnerAddress;
		u16 writeCharacteristicHandle;
		
		//Mesh variables
		clusterID connectedClusterId;
		clusterSIZE connectedClusterSize;
		clusterSIZE hopsToSink;
		u8 count;


};
