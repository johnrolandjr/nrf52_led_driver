#pragma once

#ifdef __cplusplus

#include <Module.h>
#include <Node.h>
#include <stdlib.h>
#include <SPI_bus.h>
#include <Serial.h>
#include <fatfs.h>
#include <LIS3DH.h>
#include <nrf_drv_pdm.h>
#include <queue>
#include <stdio.h>
#include <string.h>

//led patterns
#include "animation_structs.h"

#include "pixelPatterns.h"

extern "C"{
#include <nrf_gpio.h>
#include <app_gpiote.h>
#include <nrf_delay.h>
#include <app_timer.h>
#include <nrf_sdm.h>
#include <boardPinAssignments.h>
#include <app_util_platform.h>
#include "arm_common_tables.h"
#include "arm_const_structs.h"
#include "arm_math.h"
}

extern Node* node;

//Board specific pin assignments for module
#ifdef NRF_DK_BRD
	#define LED_LINE_1_PIN_NUM LED_4

#define LED_SPI_MOSI	P4
#define LED_SPI_MISO	P28
#define LED_SPI_CLK		P3

#define LED_ACCEL_CS	P30
#define LED_MOVEMENT_DETECT_PIN P29

#define MIC_CLK_PIN			LED_1	//Used to see whether the led blinks rapidly for oversampling digital signal
#define MIC_DOUT_PIN		P1

#endif
#ifdef CUSTOM_BRD_V3
	#define LED_LINE_1_PIN_NUM GPIO

#define LED_SPI_MOSI	SPI_MOSI
#define LED_SPI_MISO	SPI_MISO
#define LED_SPI_CLK		SPI_CLK

#define LED_ACCEL_CS	ACCEL_CS
#define LED_MOVEMENT_DETECT_PIN ACCEL_INT1

#define MIC_CLK_PIN			P2
#define MIC_DOUT_PIN		P1
#endif

#ifdef CUSTOM_BRD
#define LED_LINE_1_PIN_NUM WS_LED_2

#define LED_SPI_MOSI	SPI_MOSI
#define LED_SPI_MISO	SPI_MISO
#define LED_SPI_CLK		SPI_CLK

#define LED_ACCEL_CS			SPI_CS_ACC
#define SD_CARD_CS				SPI_CS_SD
#define LED_MOVEMENT_DETECT_PIN ACCEL_INT
#define MIC_CLK_PIN				MIC_CLK
#define MIC_DOUT_PIN			MIC_DO

#define MODE_BUTTON_PIN			BUTTON_1
#define OPTION_BUTTON_PIN		BUTTON_2
#endif

#define NUM_LED_LINES 1
const uint8_t ledLinePinNums[NUM_LED_LINES] = { LED_LINE_1_PIN_NUM };

#define NUM_LEDS 100

#define MIC_BUF_SAMPLE_LEN 512
#define MIC_CFFT_SIZE (MIC_BUF_SAMPLE_LEN>>1)
#define LARGEST_POSITIVE_16B_NUM 32767

#define MAX_BIN_MAG 50

typedef enum
{
	OFF=0,
	ALL_WHITE
}ledModuleCmds;


typedef struct
{
	uint8_t b;
	uint8_t r;
	uint8_t g;
	uint8_t dummy;
}ledPixel;
#define BACK_BEG_LEN 13
#define BACK_SIDE_LEN 7
#define BACK_LENGTH (BACK_BEG_LEN + BACK_SIDE_LEN)
#define FRONT_LENGTH 33
#define RSIDEIDX 0
#define LSIDEIDX 1
typedef struct
{
	ledPixel * pBackR[2][BACK_LENGTH];
	ledPixel * pBackL[2][BACK_LENGTH];
	ledPixel * pFrontR[2][FRONT_LENGTH];
	ledPixel * pFrontL[2][FRONT_LENGTH];
}ledSuspenders;
typedef struct
{
	uint32_t row;
	uint32_t col;
}pattCoor;
typedef struct
{
	pattCoor pos;
	pattCoor vel;
	ledPixel brg;
}pixelWithPos;

typedef struct
{
	uint32_t index;
	ledPixel color;
}updatePixel;
typedef enum
{
	COLOR_CYCLE	= 0,
	FRAME_CYCLE,
	STROBE,
	MOVING_PATTERN,
	MIC_UPDATE,
	SEND_PULSE
}ledSquenceTypes;

typedef struct
{
	ledPixel *colors;
	uint32_t numColors;
	uint32_t currentColorIdx;
}colorCycle;
typedef struct
{
	pixelWithPos* pattern;
	uint32_t numPixels;
}movingPattern;
typedef struct{
	ledPixel *leds;
	uint32_t numLeds;
}pulseState;

typedef struct
{
	uint8_t type;
	union
	{
		colorCycle *pColorCycle;
		frameCycle *pFrameCycle;
		movingPattern *pMovePatt;
		pulseState *pPulseState;
	}sequenceData;
}ledSequence;

#define UPDATE_LED_LOOP 12
#define UPDATE_LED_PERIOD_MS 50
#define GPS_UPDATE_PERIOD_MS 600

class LedModule: public Module
{
	public:
		SPI_bus* pSpiBus;
		Serial* pUart;
		fatfs* pFs;
		Adafruit_LIS3DH* pAcc;
		u32 startTimeTicks=0;


	private:
		ledPixel *pLedStrip;
		ledSuspenders pSuspenders;
		uint32_t numLeds_back=0;
		uint32_t numLeds_side=0;
		uint32_t numLedsTotal=0;
		ledSequence *pCurrentSequence = NULL;
		bool bUpdateLeds = true;

		bool bInternalBufferFilled = false;
		bool bProcessingData = false;
		uint32_t numMovementsStored = 0;
		uint32_t bufferRdIdx = 0;
		uint32_t bufferWrIdx = 0;
		uint32_t buffersFilled = 0;

		char sGpsData[256];
		char sCurrLat[256];
		char sCurrLon[256];
	public:
		LedModule(LedWrapper* pLed, const char* name, uint32_t numLedsInStrip_track);

	public:
		void BleEventHandler(ble_evt_t* bleEvent);
		void TimerEventHandler(u16 passedTime);
		void startOperations(void);
		void performOperation(void);
		void immediateOperation(uint32_t event_pins_low_to_high, uint32_t event_pins_high_to_low);

		void updateStartTime(void);

	private:
		void clearScreen(void);
		void updateLeds(void);
		void updateUpToLed(uint32_t ledEndIndex);
		void setLed(uint32_t ledIndex, uint8_t rVal, uint8_t gVal, uint8_t bVal);
		void setLeds(uint8_t *pLedArray, uint32_t updateNumLeds);
		void setLeds(ledPixel *pLedArray, uint32_t updateNumLeds);
		void setAllLeds(uint8_t rVal, uint8_t gVal, uint8_t bVal);
		void overwriteLeds(updatePixel *pArrayUpdate, uint32_t updateLen);

		void fromFftMagToColorArray(float * magnitudes, uint32_t numMagnitudes, ledPixel * pLeds);

	public:
		void selftest(void);
};
#endif

