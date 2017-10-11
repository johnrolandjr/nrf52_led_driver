#pragma once

#define SIZEOF_IMPACT_HEADER (12)
typedef struct
{
	uint32_t hitCounter;
	uint32_t timestamp;
	uint32_t nextImpactStartAddress;
}impactHeader;

typedef struct
{
	uint8_t* 	pAccel;
	uint8_t* 	pGyro;
	uint32_t	numAccelBytes;
	uint32_t	numGyroBytes;
}dataDump;

struct sensorSample
{
	sensorSample(int16_t xVal, int16_t yVal, int16_t zVal) : x(xVal), y(yVal), z(zVal){}
	sensorSample(void) : x(0),y(0),z(0){}

	int16_t x;
	int16_t y;
	int16_t z;
};

typedef struct
{
	sensorSample* pAccel;
	sensorSample* pGyro;
	uint32_t numSamples_A;
	uint32_t numSamples_G;
	uint64_t timestamp;
}impactStoredRam;
