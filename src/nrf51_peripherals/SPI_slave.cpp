#include "SPI_slave.h"
#include <stddef.h>

SPI_slave::SPI_slave(SPI_bus* bus,uint8_t csNum)
{
	pBus = bus;
	csPin = csNum;

	NRF_GPIO->OUTSET = (1 << csPin);
	NRF_GPIO->PIN_CNF[csPin] = 	(GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos) |
								(GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)		|
								(GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)	|
								(GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos)|
								(GPIO_PIN_CNF_DIR_Output);
	NRF_GPIO->OUTSET = (1 << csPin);
}
void SPI_slave::SPI_Write( uint16_t tx1, uint16_t tx2 )
{

	NRF_GPIO->OUTCLR = 1 << (this->csPin);
	asm("nop\n"
		"nop\n"
		"nop");
	pBus->busWrite(tx1,tx2);
	NRF_GPIO->OUTSET = 1 << (this->csPin);
}
void SPI_slave::SPI_Write( uint8_t* data, uint32_t numBytes)
{
	NRF_GPIO->OUTCLR = 1 << csPin;
	asm("nop\n"
		"nop\n"
		"nop");
	pBus->busWrite(data,numBytes);
	NRF_GPIO->OUTSET = 1 << csPin;
}
void SPI_slave::SPI_Write( uint8_t* dataTx1, uint32_t numBytes_1, uint8_t* dataTx2, uint32_t numBytes_2)
{
	NRF_GPIO->OUTCLR = 1 << csPin;
	asm("nop\n"
		"nop\n"
		"nop");
	pBus->busWrite(dataTx1,numBytes_1);
	pBus->busWrite(dataTx2,numBytes_2);
	NRF_GPIO->OUTSET = 1 << csPin;
}
void SPI_slave::SPI_Write( uint8_t* dataTx1, uint32_t numBytes, int16_t* dataTx2, uint32_t numShorts)
{
	NRF_GPIO->OUTCLR = 1 << csPin;
	asm("nop\n"
		"nop\n"
		"nop");
	pBus->busWrite(dataTx1,numBytes);
	pBus->busWriteShorts(dataTx2,numShorts);
	NRF_GPIO->OUTSET = 1 << csPin;
}
void SPI_slave::SPI_WriteFlashCorrection( uint8_t* dataTx1, uint32_t numBytes, int16_t* dataTx2, uint32_t numShorts)
{
	NRF_GPIO->OUTCLR = 1 << csPin;
	asm("nop\n"
		"nop\n"
		"nop");
	pBus->busWrite_FlashCorrection(dataTx1, numBytes ,dataTx2,numShorts);
	NRF_GPIO->OUTSET = 1 << csPin;
}
int SPI_slave::SPI_Read( uint16_t tx1 )
{
	int rx;
	NRF_GPIO->OUTCLR = 1 << csPin;
	asm("nop\n"
		"nop\n"
		"nop");
	rx = pBus->busRead(tx1);
	NRF_GPIO->OUTSET = 1 << csPin;
	return rx;
}
bool SPI_slave::SPI_READ_BYTES_AFTER_CMD(uint8_t* cmd, uint32_t sizeOfCmd, uint8_t* pRxData, uint32_t readCount)
{
	bool bSuccess;
	NRF_GPIO->OUTCLR = 1 << csPin;
	asm("nop\n"
		"nop\n"
		"nop");
	bSuccess = pBus->busMB_transfer(cmd,sizeOfCmd,pRxData,readCount);
	NRF_GPIO->OUTSET = 1 << csPin;
	return bSuccess;
}
void SPI_slave::SPI_TX_CMD_AND_READ_UNTIL_ZERO_RX(uint8_t* cmd, uint32_t sizeOfCmd)
{
	uint8_t rx = 0;
	NRF_GPIO->OUTCLR = 1 << csPin;
	asm("nop\n"
		"nop\n"
		"nop");
	pBus->busWrite(cmd,sizeOfCmd);
	do
	{
		//busRead(uint8_t tx, uint8_t* dataRx, uint32_t rxLen);
		pBus->busRead(*cmd,&rx,1);
	}while(rx == 0xff);
	NRF_GPIO->OUTSET = 1 << csPin;
}
void SPI_slave::SPI_MB_Read( uint8_t reg, uint8_t* dataRx, uint32_t len)
{
	NRF_GPIO->OUTCLR = 1 << csPin;
	asm("nop\n"
		"nop\n"
		"nop");
	pBus->busMB_Read( reg, dataRx, len);
	NRF_GPIO->OUTSET = 1 << csPin;
}

