/*
 * fatfs.cpp
 *
 *  Created on: Aug 20, 2017
 *      Author: johnr
 */

#include "fatfs.h"
#include "fatfs_c_api.h"

#define GPS_FILE_NAME "sim.txt"

extern "C"{

}
static void * fs_s = 0;
fatfs::fatfs(SPI_bus* pBus, uint8_t csPin)
{
	FRESULT ff_result;

	spi_s = new SPI_slave(pBus, csPin);

	fs_s = (void *) this;

	ff_result = f_mount(&fs, "", 1);
	if (ff_result)
	{
		//NRF_LOG_INFO("Mount failed.\r\n");
		return;
	}

	ff_result = f_opendir(&dir, "/");
	if (ff_result)
	{
		//NRF_LOG_INFO("Directory listing failed!\r\n");
		return;
	}

	ff_result = f_readdir(&dir, &fno);
	if (ff_result != FR_OK)
	{
		//NRF_LOG_INFO("Directory read failed.");
		return;
	}
}

BYTE fatfs::init(void)
{
	BYTE res;
	BYTE n, cmd, ct, ocr[4];
	/* SC CARD for SPI INTERFACE initialize sequence
	 * 	1.	After receiving power, transmit at least 74 SPI clock cycles
			so that all internal start up operations can complete.
		2. 	Drive the CS pin low.
		3. 	Transmit a CMD0 to switch the card into SPI mode.
		4. 	Transmit 8 SPI clock cycles.
		5. 	Transmit a CMD1.
		6. 	If the response to CMD1 indicates that the card is busy, go
			back to step 4. Otherwise, the card initialization is
			successful and the card can now receive and respond to
			commands.
	 */

	spi_s->pBus->changeBusSpeed(freq_125khZ);

	nrf_delay_ms(2);

	// step 1, 74 clk pulses.. more like 160 pulses
	__SD_Deassert();
	uint8_t tmp[20];
	spi_s->pBus->busMB_Read_sendff(tmp,20);
	nrf_delay_ms(500);

	// STEP 2: Assert CS line for spi protocol selection while in sd card mode
	//__SD_Assert();
	do{
		//STEP 3: SEND RESET CMD0 to switch over to SPI protocol
		res = __SD_Send_Cmd(CMD0, 0);
	}while(res != 0x01);

	BYTE r7[4];
	nrf_delay_ms(100);
	__SD_Deassert();	//keep it asserted during the reset command, this is the time when the sd card reads this line for either spi mode or sd mode protocol
	nrf_delay_ms(2);

	res = __SD_Send_Cmd(CMD8, 0x000001AA);
	spi_s->pBus->busMB_Read(r7, 4);
	__SD_Deassert();

	if(res == 1 )
	{
		//version 2
		//we should read r7 for the return and check the voltage support and command check pattern
		if(((r7[2] & 0xf) == 1) && (r7[3] == 0xAA))
		{
			//both voltage support and check pattern found in response, command was successful
			//sd card version 2
			BYTE r3[4];
			res = __SD_Send_Cmd(CMD58, 0);
			spi_s->pBus->busMB_Read(r3, 4);
			__SD_Deassert();
			//check voltages that the card supports
			if((r3[1] == 0xff) && (r3[2] == 0x80))
			{
				//card vdd is within spec, let's start the initialization command
				//may take up to a second
				int delay_4=0;
				do{
					res = __SD_Send_Cmd(ACMD41, 0x40000000);
					nrf_delay_ms(50);
					delay_4++;
				}while( (res != 0) && (delay_4 < 20));

				if(res == 0)
				{
					//card is initialized, let's find the supported CCS
					res = __SD_Send_Cmd(CMD58, 0);
					spi_s->pBus->busMB_Read(r3, 4);
					__SD_Deassert();

					if( (r3[0] & 0xc0) == 0xc0 ){
						//CCS=1 means that the card is SDHC or SDXC.
					}
				}
			}
			r7[0]=0;
		}else
		{
			r7[1]=0;
		}
	}else
	{
		while(1){
			//this is a sdhc version 2 card... we have failed initialization
		}
	}

	//spi_s->pBus->changeBusSpeed(freq_8Mhz);
	return 0;
}

int fatfs::open_file(){
	//open the file that we wish to read from
	FRESULT ff_result;
	ff_result = f_open(&file, GPS_FILE_NAME, FA_READ);
	if (ff_result != FR_OK)
	{
		return 1;
	}
	return 0;
}
DRESULT	fatfs::read_sectors(BYTE* pRx, DWORD startSector, BYTE sectorCnt)
{
	DRESULT res;

	for(int a =0; a<sectorCnt; a++)
	{
		res=read_sector(pRx,startSector+a);
		pRx+=512;
		if(res != RES_OK)
			return res;
	}
	return RES_OK;
}

DRESULT fatfs::read_sector(BYTE* pRx, DWORD sectorIdx)
{
	BYTE r1;
	r1 = __SD_Send_Cmd(CMD17, sectorIdx);
	if(r1 == 0){
		//at some point in time, the sd card will send the data token start byte 0xfe, the following will be our data
		BYTE tmp;
		do{
			spi_s->pBus->busMB_Read_sendff(&tmp, 1);
		}while(tmp != 0xfe);

		//read data
		spi_s->pBus->busMB_Read_sendff(pRx, 512);

		//read in/ignore crc bytes
		uint8_t dummy[2] = {0xff, 0xff};
		spi_s->pBus->busWrite(dummy, 2);
		__SD_Deassert();

		return RES_OK;
	}else{
		return RES_ERROR;
	}
}

#define FILE_NAME   "TEST.BMP"

#define TEST_STRING "SD card example.\r\n"

/**
 * @brief  SDC block device definition
 * */

int fatfs::unittest(void)
{
	static FATFS fs;
	static DIR dir;
	static FILINFO fno;
	static FIL file;

	FRESULT ff_result;

	ff_result = f_mount(&fs, "", 1);
	if (ff_result)
	{
		//NRF_LOG_INFO("Mount failed.\r\n");
		return -1;
	}

	ff_result = f_opendir(&dir, "/");
	if (ff_result)
	{
		//NRF_LOG_INFO("Directory listing failed!\r\n");
		return -1;
	}

	ff_result = f_readdir(&dir, &fno);
	if (ff_result != FR_OK)
	{
		//NRF_LOG_INFO("Directory read failed.");
		return -1;
	}

	ff_result = f_open(&file, FILE_NAME, FA_READ);
	if (ff_result != FR_OK)
	{
		//NRF_LOG_INFO("Unable to open or create file: " FILE_NAME ".\r\n");
		return -1;
	}

	//time to read the data
	BYTE pImage[150];
	unsigned int bytesRead;
	ff_result = f_read(&file, pImage, 150, &bytesRead);


	(void) f_close(&file);

	while(1){};

    return 0;
}

BYTE fatfs::__SD_Send_Cmd(BYTE cmd, DWORD arg)
{
    BYTE crc, res;
    // ACMD«n» is the command sequense of CMD55-CMD«n»
    if(cmd & 0x80) {
        cmd &= 0x7F;
        res = __SD_Send_Cmd(CMD55, 0);
        if (res > 1) return (res);
    }

    // Select the card
    /*
    __SD_Deassert();
    if(cmd == CMD17)
    {
    	for(int a=0; a<7; a++)
    		spi_s->pBus->busWrite(0xff);
    }
    spi_s->pBus->busWrite(0xff);
    __SD_Assert();
    spi_s->pBus->busWrite(0xff);
     */

    // Send complete command set
    BYTE msg[6] = { cmd , (BYTE)(arg>>24),(BYTE)(arg>>16),(BYTE)(arg>>8),(BYTE)(arg>>0),(BYTE)(0)};
    //spi_s->pBus->busWrite(msg, 5);
    //SPI_RW(cmd);                        // Start and command index
    //SPI_RW((BYTE)(arg >> 24));          // Arg[31-24]
    //SPI_RW((BYTE)(arg >> 16));          // Arg[23-16]
    //SPI_RW((BYTE)(arg >> 8 ));          // Arg[15-08]
    //SPI_RW((BYTE)(arg >> 0 ));          // Arg[07-00]

    // CRC?
    crc = 0x01;                         // Dummy CRC and stop
    if(cmd == CMD0) crc = 0x95;         // Valid CRC for CMD0(0)
    if(cmd == CMD8) crc = 0x87;         // Valid CRC for CMD8(0x1AA)

    msg[5] = crc;

    spi_s->pBus->clearSpiBus();
    __SD_Assert();
    spi_s->pBus->busWrite(msg, 6);

    // Receive command response
    // Wait for a valid response in timeout of 5 milliseconds
    int timerCnt = 0;
	res = spi_s->pBus->busWrite(0xff);
	while((res & 0x80)&&(timerCnt<8)){
        nrf_delay_us(100);
    	res = spi_s->pBus->busWrite(0xff);
    }
	if(	(cmd != CMD0) &&
		(cmd != CMD8) &&
		(cmd != CMD58)&&
		(cmd != CMD17))
	{
		__SD_Deassert();
	}
    // Return with the response value
    return(res);
}

void fatfs::loadPixelDataFromBmp(frameCycle* pAnimStruct, uint32_t imageIdx)
{
	FRESULT ff_result;
	unsigned int bytesRead;

	/*
	ff_result = f_open(&file, FILE_NAME, FA_READ);
	if (ff_result != FR_OK)
	{
		return;
	}

	//first we need to read in the bitmap file header
	bmpFileHeader bmpFileHead;
	ff_result = f_read(&file, &bmpFileHead, BMP_FILE_HEADER_SIZE, &bytesRead);

	bmpInfoHeader bmpInfoHead;
	ff_result = f_read(&file, &bmpInfoHead, 4, &bytesRead);
	int sizeOfHeadRemaining = bmpInfoHead.size - 4;
	ff_result = f_read(&file, &bmpInfoHead.imgWidth, sizeOfHeadRemaining, &bytesRead);

	if(pBmpPixelData == 0)
		delete[] pBmpPixelData;

	//uint8_t pSdPixelData[(bmpInfoHead.imageSize / 3)];		// imageSize in bytes / 3 bytes per pixel ... 24 bits = 3 bytes
	//ff_result = f_read(&file, pSdPixelData, sizeOfHeadRemaining, &bytesRead);

	//we need to transpose the data so that it's organized per animation frame,
	// bottom left pixel, bottom left + 1 pixel to the right ... -> bottom pixel, pixel above it, ...
	pBmpPixelData = new uint8_t[bmpInfoHead.imageSize];
	ff_result = f_read(&file, pBmpPixelData, bmpInfoHead.imageSize, &bytesRead);
	for(int a=0; a<bmpInfoHead.imgHeight; a++)
	{
		for(int b=0; b<bmpInfoHead.imgWidth; b++)
		{
			//for every pixel, we need to reorganize the data in r,g,b order
			bmpPixelData tmp;
			tmp.b = pBmpPixelData[((a*bmpInfoHead.imgWidth + b) * 3)];
			//tmp.g = pBmpPixelData[((a*bmpInfoHead.imgWidth + b) * 3) + 1];
			tmp.r = pBmpPixelData[((a*bmpInfoHead.imgWidth + b) * 3) + 2];

			pBmpPixelData[((a*bmpInfoHead.imgWidth + b) * 3)] = tmp.r;
			//pBmpPixelData[((a*bmpInfoHead.imgWidth + b) * 3) + 1] = tmp.g;
			pBmpPixelData[((a*bmpInfoHead.imgWidth + b) * 3) + 2] = tmp.b;
		}
	}
	//we have read in all of the data now :)
	//so now, let's translate it to the animation sequence
	pAnimStruct->currentFrameIdx = 0;
	pAnimStruct->numFrames = bmpInfoHead.imgHeight;
	pAnimStruct->numLeds = bmpInfoHead.imgWidth;
	pAnimStruct->pPixels = pBmpPixelData;
	(void) f_close(&file);
	*/
}

void fatfs::readLine(char * pLine){
	char * pTest;
	pTest = f_gets(pLine, 256, &file);
}

void fatfs::rewindToBegin(){
	f_rewind(&file);
}

//c assessor functions
void * 	c_create()
{
	if(fs_s != 0)
		return fs_s;
	else
		return 0;
}
void c_destroy(void * cThisFs)
{
	delete static_cast<fatfs *>(cThisFs);
}
BYTE c_init(void * cThisFs)
{
	return static_cast<fatfs *>(cThisFs)->init();
}
DRESULT	c_read_sectors(void * cThisFs, BYTE* pRx, DWORD startSector, BYTE sectorCnt)
{
	return static_cast<fatfs *>(cThisFs)->read_sectors(pRx,startSector,sectorCnt);
}
