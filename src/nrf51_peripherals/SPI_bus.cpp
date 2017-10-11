#include "SPI_bus.h"

SPI_bus::SPI_bus(uint8_t mosi, uint8_t miso, uint8_t sclk){
//SPI_bus::SPI_bus(uint8_t mosi, uint8_t miso, uint8_t sclk, uint8_t csNum){
	mosiPin = mosi;
	misoPin = miso;
	clkPin = sclk;
    //A Slave select must be set as high before setting it as output,
    //because during connect it to the pin it causes glitches.


    //Configure GPIO
	NRF_GPIO->PIN_CNF[clkPin] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos) |
								(GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)		|
								(GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)	|
								(GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)|
								(GPIO_PIN_CNF_DIR_Output);
	NRF_GPIO->PIN_CNF[mosiPin] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos) |
								(GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)		|
								(GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)	|
								(GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos)|
								(GPIO_PIN_CNF_DIR_Output);
	NRF_GPIO->PIN_CNF[misoPin] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos) |
								(GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)		|
								(GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos)	|
								(GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)|
								(GPIO_PIN_CNF_DIR_Input);

    /* Configure SPI hardware */
	NRF_SPI0->PSELSCK = clkPin;
	NRF_SPI0->PSELMOSI = mosiPin;
	NRF_SPI0->PSELMISO = misoPin;

	NRF_SPI0->FREQUENCY = SPIM_FREQUENCY_FREQUENCY_M1;

	NRF_SPI0->CONFIG = 	(SPI_CONFIG_CPOL_ActiveHigh << SPI_CONFIG_CPOL_Pos)	|
						(SPI_CONFIG_CPHA_Leading << SPI_CONFIG_CPHA_Pos) 	|
    					(SPI_CONFIG_ORDER_MsbFirst << SPI_CONFIG_ORDER_Pos);

    /* Clear waiting interrupts and events */
	NRF_SPI0->EVENTS_READY = 0;

	/*
    APP_ERROR_CHECK(sd_nvic_ClearPendingIRQ(p_spi_instance->irq_type));
    APP_ERROR_CHECK(sd_nvic_SetPriority(p_spi_instance->irq_type, p_spi_master_config->SPI_PriorityIRQ));
	 */

    /* Clear event handler */
    //p_spi_instance->callback_event_handler = NULL;

    /* Enable interrupt */
    //p_spi_instance->p_nrf_spi->INTENSET = (SPI_INTENSET_READY_Set << SPI_INTENCLR_READY_Pos);
    //APP_ERROR_CHECK(sd_nvic_EnableIRQ(p_spi_instance->irq_type));

    /* Enable SPI hardware */
	NRF_SPI0->ENABLE = (SPI_ENABLE_ENABLE_Enabled << SPI_ENABLE_ENABLE_Pos);
}

void SPI_bus::changeBusSpeed(spi_bus_speeds newFreq)
{
	NRF_SPI0->FREQUENCY = newFreq;
}

void SPI_bus::clearSpiBus(void)
{
	uint8_t rxd;
	NRF_SPI0->TXD = 0xff;
	while(!NRF_SPI0->EVENTS_READY){	};
	NRF_SPI0->EVENTS_READY = 0;
	rxd = NRF_SPI0->RXD;
}
uint8_t SPI_bus::busWrite(uint8_t tx)
{
	uint8_t rxd;
	NRF_SPI0->TXD = tx;
	while(!NRF_SPI0->EVENTS_READY){	};
	NRF_SPI0->EVENTS_READY = 0;
	rxd = NRF_SPI0->RXD;

	return rxd;
}

void SPI_bus::busWrite( uint16_t tx1, uint16_t tx2 ){
    /*this->CS = 0;
    wait_us(1);
    spi.write((tx1 & W_BIT));
    spi.write((tx2));
    this->CS = 1;
    */

	uint8_t rxd;

	NRF_SPI0->TXD = tx1 & W_BIT;
	NRF_SPI0->TXD = tx2;
	while(!NRF_SPI0->EVENTS_READY){	};

	NRF_SPI0->EVENTS_READY = 0;
	rxd = NRF_SPI0->RXD;
	while(!NRF_SPI0->EVENTS_READY){};
	NRF_SPI0->EVENTS_READY = 0;
	rxd = NRF_SPI0->RXD;
}

bool SPI_bus::busMB_transfer(uint8_t* txData, uint32_t txLen, uint8_t* rxData, uint32_t rxLen)
{
	uint32_t consecutiveZeroCount = 0;
	uint32_t discard;
	uint32_t tmp;
	NRF_SPI0->EVENTS_READY = 0;

	//transmit data
	NRF_SPI0->TXD = (uint32_t)*(txData++);
	tmp = (uint32_t)*(txData++);
	while(--txLen)
	{
		NRF_SPI0->TXD = tmp;
		tmp = (uint32_t)*(txData++);

		while(NRF_SPI0->EVENTS_READY == 0);

		NRF_SPI0->EVENTS_READY = 0;

		discard = NRF_SPI0->RXD;
	}

	while(NRF_SPI0->EVENTS_READY == 0);
	NRF_SPI0->EVENTS_READY = 0;
	discard = NRF_SPI0->RXD;

	//receive data
	while(rxLen--)
	{
		NRF_SPI0->TXD = 0;

		while(NRF_SPI0->EVENTS_READY == 0);

		NRF_SPI0->EVENTS_READY = 0;

		*(rxData++) = NRF_SPI0->RXD;
	}
	return true;
}
void SPI_bus::busWrite( uint8_t* data, uint32_t numBytes)
{
	uint32_t discard;
	uint32_t tmp;
	NRF_SPI0->EVENTS_READY = 0;

	NRF_SPI0->TXD = (uint32_t)*(data++);
	tmp = (uint32_t)*(data++);
	while(--numBytes)
	{
		NRF_SPI0->TXD = tmp;
		tmp = (uint32_t)*(data++);

		while(NRF_SPI0->EVENTS_READY == 0);

		NRF_SPI0->EVENTS_READY = 0;

		discard = NRF_SPI0->RXD;
	}

	while(NRF_SPI0->EVENTS_READY == 0);
	NRF_SPI0->EVENTS_READY = 0;
	discard = NRF_SPI0->RXD;
}
void SPI_bus::busWriteShorts( int16_t* data, uint32_t numShorts)
{

	uint8_t discard;
	NRF_SPI0->EVENTS_READY = 0;

	while(numShorts--)
	{
		NRF_SPI0->TXD = (uint8_t)((*data) & 0xff);
		NRF_SPI0->TXD = (uint8_t)(((*data) >> 8)& 0xff);

		while(!NRF_SPI0->EVENTS_READY){};
		NRF_SPI0->EVENTS_READY = 0;
		discard = NRF_SPI0->RXD;

		while(!NRF_SPI0->EVENTS_READY){};
		NRF_SPI0->EVENTS_READY = 0;
		discard = NRF_SPI0->RXD;

		data++;
	}
}
void SPI_bus::busWrite_FlashCorrection( uint8_t* cmd, uint32_t cmdNumBytes, int16_t* pData, uint32_t numShorts)
{

	uint32_t discard;
	uint32_t tmp;
	NRF_SPI0->EVENTS_READY = 0;

	NRF_SPI0->TXD = (uint32_t)*(cmd++);
	tmp = (uint32_t)*(cmd++);
	while(--cmdNumBytes)
	{
		NRF_SPI0->TXD = tmp;
		tmp = (uint32_t)*(cmd++);

		while(NRF_SPI0->EVENTS_READY == 0);

		NRF_SPI0->EVENTS_READY = 0;

		discard = NRF_SPI0->RXD;
	}

	while(NRF_SPI0->EVENTS_READY == 0);
	NRF_SPI0->EVENTS_READY = 0;
	discard = NRF_SPI0->RXD;

	for(unsigned int index = 0; index < numShorts; index++)
	{
		nrf_delay_us(1);
		NRF_SPI0->TXD = (uint32_t)((pData[index]) & 0xff);

		while(!NRF_SPI0->EVENTS_READY){};
		NRF_SPI0->EVENTS_READY = 0;
		discard = NRF_SPI0->RXD;

		nrf_delay_us(1);
		NRF_SPI0->TXD = (uint32_t)(((pData[index]) >> 8)& 0xff);
		while(!NRF_SPI0->EVENTS_READY){};
		NRF_SPI0->EVENTS_READY = 0;
		discard = NRF_SPI0->RXD;
	}
}
int SPI_bus::busRead( uint16_t tx1 ){
    int rxd;
	NRF_SPI0->TXD = tx1 | R_BIT;
	NRF_SPI0->TXD = 0;
	while(!NRF_SPI0->EVENTS_READY){};
	NRF_SPI0->EVENTS_READY = 0;
	rxd = NRF_SPI0->RXD;
	while(!NRF_SPI0->EVENTS_READY){};
	NRF_SPI0->EVENTS_READY = 0;
	rxd = NRF_SPI0->RXD;

    return rxd;
}
void SPI_bus::busRead(uint8_t tx, uint8_t* dataRx, uint32_t rxLen)
{
	NRF_SPI0->TXD = tx | R_BIT;
	while(!NRF_SPI0->EVENTS_READY){};
	NRF_SPI0->EVENTS_READY = 0;
	*dataRx = NRF_SPI0->RXD;					//throw away because we expect not to receive any data on our first transfer
	while(rxLen--)
	{
		NRF_SPI0->TXD = 0;
		while(!NRF_SPI0->EVENTS_READY){};
		NRF_SPI0->EVENTS_READY = 0;
		*dataRx = NRF_SPI0->RXD;
		dataRx++;
	};
}
void SPI_bus::busMB_Read( uint8_t reg, uint8_t* dataRx, uint32_t len){

	uint32_t discard;
	NRF_SPI0->EVENTS_READY = 0;

	NRF_SPI0->TXD = (uint32_t)(reg | R_BIT | MB_BIT);
	NRF_SPI0->TXD = 0;

	while(NRF_SPI0->EVENTS_READY == 0);
	NRF_SPI0->EVENTS_READY = 0;
	discard = NRF_SPI0->RXD;

	while(--len)
	{
		NRF_SPI0->TXD = 0;

		while(NRF_SPI0->EVENTS_READY == 0);
		NRF_SPI0->EVENTS_READY = 0;
		*(dataRx) = NRF_SPI0->RXD;
		dataRx++;
	}

	while(NRF_SPI0->EVENTS_READY == 0);
	NRF_SPI0->EVENTS_READY = 0;
	*(dataRx) = NRF_SPI0->RXD;

	/*
	//----------------------------------------------
	//all future txd writes will be 0, because we will be reading the data
	while(--len)
	{
		NRF_SPI0->TXD = 0;

		while(NRF_SPI0->EVENTS_READY == 0);

		NRF_SPI0->EVENTS_READY = 0;
		*(dataRx) = NRF_SPI0->RXD;
		dataRx++;
	}

	while(NRF_SPI0->EVENTS_READY == 0);

	NRF_SPI0->EVENTS_READY = 0;
	*(dataRx++) = NRF_SPI0->RXD;

	*/
}

void SPI_bus::busMB_Read( uint8_t* dataRx, uint32_t len)
{
	NRF_SPI0->EVENTS_READY = 0;
	while(len--)
	{
		NRF_SPI0->TXD = 0;

		while(NRF_SPI0->EVENTS_READY == 0);

		NRF_SPI0->EVENTS_READY = 0;
		*(dataRx) = NRF_SPI0->RXD;
		dataRx++;
	}

	/*
	while(len--){
		NRF_SPI0->TXD = 0;
		while(!NRF_SPI0->EVENTS_READY){};
		NRF_SPI0->EVENTS_READY = 0;
		*dataRx = (uint8_t)(NRF_SPI0->RXD&0xff);
		dataRx++;
	}
	*/
}

void SPI_bus::busMB_Read_sendff( uint8_t* dataRx, uint32_t len)
{
	NRF_SPI0->EVENTS_READY = 0;
	while(len--)
	{
		NRF_SPI0->TXD = 0xff;

		while(NRF_SPI0->EVENTS_READY == 0);

		NRF_SPI0->EVENTS_READY = 0;
		*(dataRx) = NRF_SPI0->RXD;
		dataRx++;
	}
}
