/*
 * fatfs.h
 *
 *  Created on: Aug 20, 2017
 *      Author: johnr
 */

#ifndef FATFS_H_
#define FATFS_H_

#include "SPI_slave.h"
#include <boardPinAssignments.h>
#include "animation_structs.h"


extern "C"{
	#include <integer.h>
	#include <ff.h>
	#include <ffconf.h>
	#include <sd_io.h>
	#include <diskio.h>
}

//bitmap header structs and defines

#define BMP_TYPE_ID 0x4D42


#pragma pack(push, 1)

#define BMP_FILE_HEADER_SIZE 14
typedef struct bmpFileHeader
{
	WORD fileType;		//File type
	DWORD size;			//size in bytes of the file
	WORD reserved1;
	WORD reserved2;
	DWORD dataOffset;	//offset in hex to pixel data
}BMP_FILE_HEADER;

typedef struct bmpInfoHeader
{
	DWORD size;				//size of this header
	LONG imgWidth;			//width in pixels
	LONG imgHeight;			//Height in pixels
	WORD planes;			//color planes must be 1
	WORD bitPerPixel;		//bits per pixel
	DWORD compressionType;	//type of compression
	DWORD imageSize;		//size of image in bytes
	LONG xPixelPerMeter;
	LONG yPixelPerMeter;
	DWORD clrUsed;			//number of colors used by the bitmap
	DWORD clrImportant;		//number of important colors used by the bitmap
}BMP_INFO_HEADER;

typedef struct bmpPixelData
{
	BYTE b;
	BYTE g;
	BYTE r;
}BMP_PIXEL_COLOR;

#pragma pack(pop)


class fatfs{
    private:
        SPI_slave * spi_s;
        FATFS fs;
        DIR dir;
        FILINFO fno;
        FIL file;

        uint8_t * pBmpPixelData;

    public:
        fatfs(SPI_bus* pBus, uint8_t csPin);
        BYTE init(void);
        DRESULT	read_sectors(BYTE* pRx, DWORD startSector, BYTE sectorCnt);

        void loadPixelDataFromBmp(frameCycle* pAnimStruct, uint32_t imageIdx);

        int unittest(void);
    private:
        DRESULT read_sector(BYTE* pRx, DWORD sectorIdx);
        BYTE __SD_Send_Cmd(BYTE cmd, DWORD arg);
        void __SD_Deassert() { NRF_GPIO->OUTSET = 1<<spi_s->csPin; }
        void __SD_Assert() { NRF_GPIO->OUTCLR = 1<<spi_s->csPin; }

};

#endif /* FATFS_H_ */
