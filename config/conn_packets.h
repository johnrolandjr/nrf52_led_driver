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
 * This file contains the structs that are used for packets that are sent
 * over standard BLE connections, such as the mesh-handshake and other packets
 */

#pragma once

#include <types.h>

#pragma pack(push)
#pragma pack(1)

//########## Connection packets ###############################################
#define MESSAGE_TYPE_HANDSHAKE					24
#define MESSAGE_TYPE_BEGIN_THROUGHPUT 			25
#define MESSAGE_TYPE_END_THROUGHPUT 			26
#define MESSAGE_TYPE_BEGIN_THROUGHPUT_REPLY		27
#define MESSAGE_TYPE_END_THROUGHPUT_REPLY 		28
#define MESSAGE_TYPE_UPDATE_CHILDREN 			29
#define MESSAGE_TYPE_SET_METHOD 				30
#define MESSAGE_TYPE_DISCONNECT 				31
#define MESSAGE_TYPE_ERASE_FLASH				32
#define MESSAGE_TYPE_UPDATE_CONN_SETTINGS		33
#define MESSAGE_TYPE_SOFT_RESET					34

#define MESSAGE_TYPE_PRESSURE					35
#define MESSAGE_TYPE_MODULE_CMD					36


union directionUnion{
	uint8_t rawDirection;
	uint8_t outDir_lvl_1:1;
	uint8_t outDir_lvl_2:1;
	uint8_t outDir_lvl_3:1;
	uint8_t outDir_lvl_4:1;
	uint8_t outDir_lvl_5:1;
	uint8_t outDir_lvl_6:1;
	uint8_t outDir_lvl_7:1;
	uint8_t outDir_lvl_8:1;
};

#define SIZEOF_CONN_PACKET_HEADER 3
typedef struct
{
	u8 isData : 1; //Set to true if message is data
	u8 messageType : 7;
	nodeID receiverID;
}connPacketHeader;

#define SIZEOF_HANDSHAKE_PACKET (SIZEOF_CONN_PACKET_HEADER + 16)
typedef struct
{
	connPacketHeader header;
	nodeID parentID;
	u16 meshWriteHandle;
	clusterSIZE hopsToSink;
	nodeID sinkNodeID;
	uint64_t time;
}handShakePacket;

#define SIZEOF_BEGIN_THROUGHPUT_PACKET (SIZEOF_CONN_PACKET_HEADER + 14)
typedef struct
{
	connPacketHeader header;
	nodeID originId;
	uint64_t timestamp;
	u16 numSamplesAccel;
	u16 numSamplesGyro;
}beginThroughPutPacket;

#define SIZEOF_END_THROUGHPUT_PACKET (SIZEOF_CONN_PACKET_HEADER + 1)
typedef struct
{
	connPacketHeader header;
	u8 bIsLastPartialPacket;
}endThroughPutPacket;

#define SIZEOF_BEGIN_THROUGHPUT_PACKET_REPLY (SIZEOF_CONN_PACKET_HEADER + 3)
typedef struct
{
	connPacketHeader header;
	nodeID sender;
	u8 availability;
}beginThroughPutPacketReply;

#define SIZEOF_END_THROUGHPUT_PACKET_REPLY (SIZEOF_CONN_PACKET_HEADER + 1 + 16)
typedef struct
{
	connPacketHeader header;
	u8 numMissing;
	u8 missingIds[16];
}endThroughPutPacketReply;

#define SIZEOF_UPDATE_CHILDREN_PACKET (SIZEOF_CONN_PACKET_HEADER + 5)
typedef struct
{
	connPacketHeader header;
	directionUnion dir;
	nodeID child1;
	nodeID child2;
}updateChildrenPacket;

#define SIZEOF_SET_METHOD_PACKET (SIZEOF_CONN_PACKET_HEADER + 1)
typedef struct
{
	connPacketHeader header;
	u8 CFD;
}setMethodPacket;

#define SIZEOF_DISCONNECT_PACKET (SIZEOF_CONN_PACKET_HEADER + 5)
typedef struct
{
	connPacketHeader header;
	directionUnion dir;
	nodeID child1;
	nodeID child2;
}disconnectPacket;

#define SIZEOF_ERASE_FLASH_PACKET (SIZEOF_CONN_PACKET_HEADER)
typedef struct
{
	connPacketHeader header;
}eraseFlashPacket;

#define SIZEOF_SOFT_RESET_PACKET (SIZEOF_CONN_PACKET_HEADER)
typedef struct
{
	connPacketHeader header;
}softResetPacket;

#define SIZEOF_UPDATE_CONN_SETTINGS_PACKET (SIZEOF_CONN_PACKET_HEADER + 4)
typedef struct
{
	connPacketHeader header;
	uint16_t connectionInterval;
	uint16_t slaveLatency;
}updateConnectionSettingsPacket;

#define SIZEOF_PRESSURE_PACKET_BASE (SIZEOF_CONN_PACKET_HEADER + 6)
#define SIZEOF_PRESSURE_VALUE		(2)
typedef struct
{
	connPacketHeader header;
	uint16_t pressureValues[3];
}pressurePacket;

#define SIZEOF_MODULE_CMD_PACKET	(SIZEOF_CONN_PACKET_HEADER + 1)
typedef struct
{
	connPacketHeader header;
	uint8_t cmd;
}moduleCmd;

#define SIZEOF_ROUTING_PACKET (SIZEOF_CONN_PACKET_HEADER + 1)
typedef struct
{
	connPacketHeader header;
	directionUnion dir;
}routingPacket;

#define SIZEOF_SYNC_SENSOR_DATA2_PACKET (19)
typedef struct
{
	u8 isData : 1;
	u8 packetId : 7;
	int16_t data[9];	//18bytes
}syncPacketSensorDump_Data;

#pragma pack(pop)
