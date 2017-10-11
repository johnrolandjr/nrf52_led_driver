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

#pragma once

#include <Module.h>
#include <Logger.h>
#include <Utility.h>
#include <Storage.h>
#include <Node.h>
#include <SPI_bus.h>
#include <I2C_LIB.h>
#include <ITG3701.h>
#include <ADXL375Z.h>
#include <AT25XE021A_flash.h>
#include <queue>

extern "C"{
#include <nrf_gpio.h>
#include <app_gpiote.h>
#include <nrf_delay.h>
#include <nrf_adc.h>
#include <app_timer.h>
#include <nrf_sdm.h>
#include <boardPinAssignments.h>
#include <app_util_platform.h>
}

extern Node* node;

//Board Specific Sqrl Module Pin Assignments
#ifdef NRF_DK_BRD
#define SQRL_SPI_MOSI	P29
#define SQRL_SPI_MISO	P4
#define SQRL_SPI_CLK	P28

//SENSORS
#define SQRL_ACCEL_CS				P27
#define SQRL_GYRO_CS				P3
#define SQRL_IMPACT_DETECTION_PIN	P31
#define SQRL_INACTIVITY_PIN			P30
#endif

#ifdef CUSTOM_BRD_V1
#define SQRL_SPI_MOSI	SPI_MOSI
#define SQRL_SPI_MISO	SPI_MISO
#define SQRL_SPI_CLK	SPI_CLK
#define SQRL_I2C_SDA	I2C_SDA
#define SQRL_I2C_CLK	I2C_CLK

//SENSORS
#define SQRL_ACCEL_CS				ACCEL_CS
#define SQRL_IMPACT_DETECTION_PIN	ACCEL_INT1
#define SQRL_INACTIVITY_PIN			ACCEL_INT2
#define SQRL_FLASH_CS				FLASH_CS
#define SQRL_FLASH_WP				FLASH_WP
#define SQRL_FLASH_HOLD				FLASH_HOLD
#endif

#ifdef CUSTOM_BRD_V2
#define SQRL_SPI_MOSI	SPI_MOSI
#define SQRL_SPI_MISO	SPI_MISO
#define SQRL_SPI_CLK	SPI_CLK
#define SQRL_I2C_SDA	I2C_SDA
#define SQRL_I2C_CLK	I2C_CLK

//SENSORS
#define SQRL_ACCEL_CS				ACCEL_CS
#define SQRL_IMPACT_DETECTION_PIN	ACCEL_INT1
#define SQRL_INACTIVITY_PIN			ACCEL_INT2
#define SQRL_GYRO_CS				GYRO_CS
#define SQRL_FLASH_CS				FLASH_CS
#define SQRL_FLASH_WP				FLASH_WP
#define SQRL_FLASH_HOLD				FLASH_HOLD
#endif

#define ACCEPTABLE_SENSOR_SPI_READ_SPEED  (freq_4Mhz)
#define ACCEPTABLE_SENSOR_SPI_WRITE_SPEED (freq_1Mhz)
#define ACCEPTABLE_ACCEL_FLAG_WRITE_SPEED (freq_1Mhz)
#define ACCEPTABLE_FLASH_SPI_SPEED		  (freq_8Mhz)

#define MAX_NUM_FIFO_DUMPS	5

#define CLEAR_GYRO_PERIOD_TIME_MS 70

typedef enum
{
	CHANGE_POWER_STATE = 1
}sqrlModuleCmds;

typedef enum
{
	MAX_POWER = 0,	//Both gyro and accel on
	MID_POWER,		//gyro disabled(sleep mode) and accel on
	LOW_POWER		//All sensors off, interrupt disabled
}powerStates;

typedef struct
{
	connPacketHeader header;
	uint8_t cmd;
	uint8_t powerState;
}moduleCmd_ChangePowerState;

class ConcussionModule: public Module
{

	public:
		SPI_bus* spi_bus;

		ADXL375Z* accel;
		ITG3701* gyro;
		flash_at25x* flash;

		u32 startTimeTicks;

		powerStates currentPwrState;

		bool bFreeToStoreAndSend = true;
		bool bProcessingData;
		bool bPreventSensorFifoClear = false;

		bool bInternalBufferFilled = false;
		impactHeader internalImpactHeader;
		std::queue< impactStoredRam > qImpacts;
		uint32_t aIdx = 0;
		uint32_t gIdx = 0;
	private:
		uint8_t* partialToPackage;
		dataDump flashStoredData[MAX_NUM_FIFO_DUMPS];	//array of accel and gyro data dumps
	public:


	private:
		//Module configuration that is saved persistently (size must be multiple of 4)
		struct sqrlModuleConfiguration : ModuleConfiguration{
			//Insert more persistent config values here
		};

		sqrlModuleConfiguration configuration;

	public:
		ConcussionModule(u16 moduleId, Node* node, ConnectionManager* cm, LedWrapper* pLed, const char* name, u16 storageSlot);
	public:
		void initPersistanceStorage(void);

		void BleEventHandler(ble_evt_t* bleEvent);
		void TimerEventHandler(u16 passedTime);
		void startOperations(void);
		void performOperation(void);
		void immediateOperation(uint32_t l2h, uint32_t h2l);

		//void storePartnerDataThroughputPacket(syncPacketSensorDump_Data* pBleDataPkt);
		void determineAmountOfSamplesToSend(uint16_t* aSamples, uint16_t* gSamples,bool* bIsLastPartial, uint64_t * impactTimestamp);
		void dumpSensorDataToFillTransmit(uint16_t numA, uint16_t numG, uint16_t sizeOfPartial, uint8_t* flashData);
		void dumpSensorDataToFillTransmit(const uint8_t numFifoDumps , dataDump flashData[MAX_NUM_FIFO_DUMPS]);
		void dumpSensorDataToFillTransmit(uint16_t numA, uint16_t numG);
		void eraseFlash(void);
		void updateFlashMetaData(void);
		//void receivedAllData(endThroughPutPacketReply* incomingETP);

		void enableImpactInterrupt();
		void disableImpactInterrupt();

		void storeData(void);
		void checkForData(void);
		void emptyBackBuffers(void);

	public:		//depreciated functions
		bool TerminalCommandHandler(string commandName, vector<string> commandArgs);
		void ConfigurationLoadedHandler();
		void ResetToDefaultConfiguration();
	private:
		void updateStartTime(void);
		void calibrateSensors(void);
		void selftest_pstore(void);
};

