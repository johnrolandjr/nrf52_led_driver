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
 * The Connection manager manages all connections and schedules their packet
 * transmissions. It is the interface that must be used to send data.
 */

#pragma once

#include <Connection.h>
#include <string>
#include <types.h>
#include <conn_packets.h>
#include <forward_list>

extern "C"{
#include <ble.h>
}

//defines
#define EXPECTED_MAX_NUMBER_BLE_PACKETS_PER_THROUGHPUT 200

class ConnectionManagerCallback{
	public:
		ConnectionManagerCallback();
		virtual ~ConnectionManagerCallback();
		virtual void DisconnectionHandler(ble_evt_t* bleEvent) = 0;
		virtual void ConnectionSuccessfulHandler(ble_evt_t* bleEvent) = 0;
		virtual void ConnectionTimeoutHandler(ble_evt_t* bleEvent) = 0;
		//virtual void messageReceivedCallback(connectionPacket* inPacket) = 0;
};

#pragma pack(push)
#pragma pack(4)
class ConnectionManager
{
	private:
		ConnectionManager();
		static ConnectionManager* instance;

		//Used within the send methods
		bool QueuePacket(Connection* connection, u8* data, u16 dataLength, bool reliable, bool notify = false);

		Connection* pendingConnection;

	public:
		static ConnectionManager* getInstance(){
			if(!instance){
				instance = new ConnectionManager();
			}
			return instance;
		}

		ConnectionManagerCallback* connectionManagerCallback;

		//This method is called when empty buffers are available and there is data to send
		void fillTransmitBuffers();

		void setConnectionManagerCallback(ConnectionManagerCallback* cb);

		bool doHandshake;

		//network throughput message storage
		bool bDataThroughputLocked = false;
		beginThroughPutPacket currentThroughputBeginBlePacket;
		std::forward_list<syncPacketSensorDump_Data> throughputPackets;

		//Keep track of outgoing packets and the transmit buffers
		u16 pendingPackets;
		u8 pendingNotifies;
		bool triggerNextMessage;
		u8 txBufferFreeCount;
		bool bLastPartialPacket;

		u8 freeInConnections;
		u8 freeOutConnections;

		Connection* inConnection;
		Connection* outConnections[MAXIMUM_OUT_CONNECTIONS];

		Connection* connections[MAXIMUM_CONNECTIONS];

		Connection* getFreeConnection();


		Connection* ConnectAsMaster(nodeID partnerId, ble_gap_addr_t* address, u16 writeCharacteristicHandle);

		void Disconnect(u16 connectionHandle);
		void DisconnectOtherConnections(Connection* connection);

		//Functions used for sending messages
		bool SendMessage(Connection* connection, u8* data, u16 dataLength, bool reliable);
		bool SyncMessage(u8* data, u16 dataLength, bool reliable);
		void SyncMessage(std::forward_list<syncPacketSensorDump_Data> *packetArrayVector, uint16_t numConsecutiveDataPackets, uint16_t sizePerPacket, bool reliable, bool notify);
		bool SyncMessage(syncPacketSensorDump_Data* packetArray, uint8_t numConsecutiveDataPackets, uint16_t sizePerPacket, bool reliable, bool notify);
		void SendMessageOverConnections(Connection* ignoreConnection, u8* data, u16 dataLength, bool reliable);
		void SendMessageToReceiver(Connection* originConnection, u8* data, u16 dataLength, bool reliable);
		void SendMessageToChildren(u8* data, u16 dataLength, bool reliable);
		void SendMessageToParent(u8* data, u16 dataLength, bool reliable);
		Connection* GetConnectionFromHandle(u16 connectionHandle);
		Connection* GetFreeOutConnection(nodeID nodeId);

		Connection* GetConnectionToShortestSink(Connection* excludeConnection);
		clusterSIZE GetHopsToShortestSink(Connection* excludeConnection);

		//These methods can be accessed by the Connection classes

		//GAPController Handlers
		static void DisconnectionHandler(ble_evt_t* bleEvent);
		static void ConnectionSuccessfulHandler(ble_evt_t* bleEvent);
		static void ConnectionTimeoutHandler(ble_evt_t* bleEvent);

		//GATTController Handlers
		static void messageReceivedCallback(ble_evt_t* bleEvent);
		static void handleDiscoveredCallback(u16 connectionHandle, u16 characteristicHandle);
		static void dataTransmittedCallback(ble_evt_t* bleEvent);

		//Helper functions
		uint32_t getNumberOfPackets(void);

};
#pragma pack(pop)
