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
 * This file holds some structures that are used to create and parse advertising
 * packets used by the mesh.
 * */
#pragma once

#include <types.h>

//Start packing all these structures
//These are packed so that they can be transmitted savely over the air
//Smaller datatypes could be implemented with bitfields?
//Sizeof operator is not to be trusted because of padding
// Pay attention to http://www.keil.com/support/man/docs/armccref/armccref_Babjddhe.htm

#pragma pack(push)
#pragma pack(1)

//###### AD structures for advertising messages ###############################

//BLE AD Type FLAGS
#define SIZEOF_ADV_STRUCTURE_FLAGS 3
typedef struct
{
	u8 len;
	u8 type;
	u8 flags;
}advStructureFlags;

//BLE AD Type full local name
#define SIZEOF_ADV_STRUCTURE_NAME 4
typedef struct
{
	u8 len;
	u8 type;
	u8 name[2];
}advStructureName;

//BLE AD Type full list of services
#define SIZEOF_ADV_STRUCTURE_UUID128 18
typedef struct
{
	u8 len;
	u8 type;
	u8 uuid[16];
}advStructureUUID128;

//BLE AD Type Manufacturer specific data
#define SIZEOF_ADV_STRUCTURE_MANUFACTURER 4
typedef struct
{
	u8 len;
	u8 type;
	u16 companyIdentifier;
}advStructureManufacturer;

//####### Advertising packets #################################################

//Message types: Protocol defined, up to 19
#define MESSAGE_TYPE_JOIN_ME 1

//Header that is common to all mesh advertising messages
#define SIZEOF_ADV_PACKET_HEADER (SIZEOF_ADV_STRUCTURE_FLAGS + SIZEOF_ADV_STRUCTURE_MANUFACTURER + SIZEOF_ADV_STRUCTURE_NAME + 1 + 2 + 1) //11 byte
#define SIZEOF_ADV_PACKET_STUFF_AFTER_MANUFACTURER 7 //1byte type + 2byte company id + 1byte mesh identifier + 2 byte networkid + 1 byte message type
typedef struct
{
	advStructureFlags flags;
	advStructureName name;
	advStructureManufacturer manufacturer;
	u8 meshIdentifier;
	networkID networkId;
	u8 messageType;
}advPacketHeader;

// ==> This leaves us with 20 bytes payload that are saved in the manufacturer specific data field

//JOIN_ME packet that is used for cluster discovery
//TODO: Add  the current discovery mode/length,... which would allow other nodes to determine
//How long they need to wait until this node scans or advertises again?
//This is v0 of the packet, other versions will have different values in the packet,
//Future research must show which values are the most interesting to determine the
//best connection partner.
#define SIZEOF_ADV_PACKET_PAYLOAD_JOIN_ME_V0 19
typedef struct
{
	nodeID sender;
	clusterID clusterId; //Consists of the founding node's id and the connection loss / restart counter
	clusterSIZE clusterSize;
	u8 freeInConnections : 3; //Up to 8 in-connections
	u8 freeOutConnections : 5; //Up to 32 out-connections

	//Version specific content
	u8 version; //Different versions might have different parameters, this is a version 1 packet.
	u8 batteryRuntime; //batteryRuntime. Contains the expected runtime of the device (1-59=minutes, 60-83=1-23hours, 84-113=1-29days, 114-233=1-119months, 234-254=10-29year, 255=infinite)
	u8 txPower; //txPower. Send power in two's complement dbm
	u8 deviceType; //Type of device => enum deviceTypes
	u16 hopsToSink; //Number of hops to the shortest sink
	u16 meshWriteHandle; //The GATT handle for the mesh communication characteristic
	nodeID ackField;//Contains the acknowledgement from another node for the slave connection procedure
}advPacketPayloadJoinMeV0;

#define SIZEOF_ADV_PACKET_PAYLOAD_JOIN_ME_V1 5
typedef struct
{
	nodeID sender;

	//Version specific content
	//u8 txPower; //txPower. Send power in two's complement dbm
	u8 deviceType; //Type of device => enum deviceTypes
	u16 meshWriteHandle; //The GATT handle for the mesh communication characteristic
	//nodeID ackField;//Contains the acknowledgement from another node for the slave connection procedure
}advPacketPayloadJoinMeV1;

/*
 Different kind of mesh devices:
 - 0:Static - A normal node that remains static
 - 1:Roaming - A node that is moving constantly or often
 - 2:Static - A static node that wants to acquire data
 * */
enum deviceTypes{DEVICE_TYPE_STATIC=0, DEVICE_TYPE_ROAMING=1, DEVICE_TYPE_SINK=2};

/*
 * Explanation:
The JOIN_ME packet can have a number of different fields that contain different information.
The version number indicates the JOIN_ME packet type
This information can then be used in the clusterScore function to build clusters based
on different criteria
 * */

#define SIZEOF_ADV_PACKET_JOIN_ME (SIZEOF_ADV_PACKET_HEADER + SIZEOF_ADV_PACKET_PAYLOAD_JOIN_ME_V1)
typedef struct
{
	advPacketHeader header;
	advPacketPayloadJoinMeV1 payload;
}advPacketJoinMeV0;

#define SIZEOF_ADV_PACKET_JOIN_ME1 (SIZEOF_ADV_PACKET_HEADER + SIZEOF_ADV_PACKET_PAYLOAD_JOIN_ME_V1)
typedef struct
{
	advPacketHeader header;
	advPacketPayloadJoinMeV1 payload;
}advPacketJoinMeV1;

//######## Scan Response packets ##############################################
//A header that is used for scan response packets
#define SIZEOF_SCAN_PACKET_HEADER (SIZEOF_ADV_STRUCTURE_NAME + SIZEOF_ADV_STRUCTURE_MANUFACTURER)
typedef struct
{
	advStructureName name;
	advStructureManufacturer manufacturer;
}scanPacketHeader;

//End Packing
#pragma pack(pop)
