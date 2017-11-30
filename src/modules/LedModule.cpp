#include <LedModule.h>

extern "C"{
void neo_out(ledPixel*, int);
// FastIO register address and bitmask for the GPIO pin
// because these are imported in the assembly
	uint32_t * neo_fio_reg;
	uint32_t neo_bitmask;
	uint32_t * neo_gpio_set_reg;
	uint32_t * neo_gpio_clr_reg;

	int16_t mic_double_buffer[2][MIC_BUF_SAMPLE_LEN];

	int16_t * 	pPdmFilledBuffer;
	uint16_t  	pdmFilledBufferLen;
	bool		bFilledBuffer=false;
	void pdm_event_handler(uint32_t * p_buffer, uint16_t length)
	{
		if(bFilledBuffer == false){
			bFilledBuffer = true;
			// Process the data.
			pPdmFilledBuffer = (int16_t *) p_buffer;
			pdmFilledBufferLen = length;
		}

	}
	void pdm_init(void)
	{
		nrf_drv_pdm_config_t pdm_cfg;
		pdm_cfg.mode = NRF_PDM_MODE_MONO;
		pdm_cfg.edge = NRF_PDM_EDGE_LEFTFALLING;
		pdm_cfg.pin_clk = MIC_CLK_PIN;
		pdm_cfg.pin_din = MIC_DOUT_PIN;
		pdm_cfg.clock_freq = NRF_PDM_FREQ_1032K;
		pdm_cfg.gain_l = NRF_PDM_GAIN_DEFAULT;
		pdm_cfg.gain_r = NRF_PDM_GAIN_DEFAULT;
		pdm_cfg.interrupt_priority = 0;	//PDM_CONFIG_IRQ_PRIORITY;
		pdm_cfg.buffer_length = MIC_BUF_SAMPLE_LEN;
		pdm_cfg.buffer_a = mic_double_buffer[0];
		pdm_cfg.buffer_b = mic_double_buffer[1];
		ret_code_t err_code = nrf_drv_pdm_init(&pdm_cfg, &pdm_event_handler);
	}

}

LedModule::LedModule(LedWrapper* pLed, const char* name,  uint32_t numLedsInStrip_track): Module(pLed, name)
{
	bStartTimerAtBeginning = true;
	startingTimerPeriod = UPDATE_LED_PERIOD_MS;

	for(int a=0; a < NUM_LED_LINES; a++)
	{
		//nrf_gpio_cfg_output(11);
		NRF_GPIO->OUTCLR = 1<<ledLinePinNums[a];
		//NRF_GPIO->OUTSET = 1<<11;

#ifndef NRF_DK_BRD
	    NRF_GPIO->PIN_CNF[ledLinePinNums[a]] = ((uint32_t)NRF_GPIO_PIN_DIR_OUTPUT   << GPIO_PIN_CNF_DIR_Pos)
	                                		  | ((uint32_t)NRF_GPIO_PIN_INPUT_DISCONNECT << GPIO_PIN_CNF_INPUT_Pos)
											  | ((uint32_t)NRF_GPIO_PIN_NOPULL  << GPIO_PIN_CNF_PULL_Pos)
											  | ((uint32_t)NRF_GPIO_PIN_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
											  | ((uint32_t)NRF_GPIO_PIN_NOSENSE << GPIO_PIN_CNF_SENSE_Pos);
#else
	    //done to protect any leds we connect to on board the nrf52 dk,... my led p2 (p18) burnt out.. possibly due to hi current drive
	    NRF_GPIO->PIN_CNF[ledLinePinNums[a]] = ((uint32_t)NRF_GPIO_PIN_DIR_OUTPUT   << GPIO_PIN_CNF_DIR_Pos)
	    	                                		  | ((uint32_t)NRF_GPIO_PIN_INPUT_DISCONNECT << GPIO_PIN_CNF_INPUT_Pos)
	    											  | ((uint32_t)NRF_GPIO_PIN_NOPULL  << GPIO_PIN_CNF_PULL_Pos)
	    											  | ((uint32_t)NRF_GPIO_PIN_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
	    											  | ((uint32_t)NRF_GPIO_PIN_NOSENSE << GPIO_PIN_CNF_SENSE_Pos);
#endif
		//NRF_GPIO->OUTCLR = 1<<ledLinePinNums[a];
		//NRF_GPIO->OUTSET = 1<<ledLinePinNums[a];
	}

	//configure buttons
	NRF_GPIO->PIN_CNF[BUTTON_1] = ((uint32_t)NRF_GPIO_PIN_DIR_INPUT   << GPIO_PIN_CNF_DIR_Pos)
		                                		  | ((uint32_t)NRF_GPIO_PIN_INPUT_CONNECT << GPIO_PIN_CNF_INPUT_Pos)
												  | ((uint32_t)NRF_GPIO_PIN_NOPULL  << GPIO_PIN_CNF_PULL_Pos)
												  | ((uint32_t)NRF_GPIO_PIN_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
												  | ((uint32_t)NRF_GPIO_PIN_NOSENSE << GPIO_PIN_CNF_SENSE_Pos);

	numLeds_back = 0;
	numLeds_side = 0;
	numLedsTotal = numLedsInStrip_track;
	pLedStrip = new ledPixel[numLedsTotal];

	if(numLedsTotal == 186)
	{
		//then they are suspenders
		for(uint32_t ledIdx=0; ledIdx < BACK_BEG_LEN; ledIdx++)
		{
			pSuspenders.pBackR[0][ledIdx] = &(pLedStrip[ledIdx]);
			pSuspenders.pBackL[0][ledIdx] = &pLedStrip[numLedsTotal - 1 - ledIdx];
			pSuspenders.pBackR[1][ledIdx] = &pLedStrip[ledIdx];
			pSuspenders.pBackL[1][ledIdx] = &pLedStrip[numLedsTotal - 1 - ledIdx];
		}
		for(uint32_t ledIdx=0; ledIdx < BACK_SIDE_LEN; ledIdx++)
		{
			pSuspenders.pBackR[RSIDEIDX][(BACK_BEG_LEN) + ledIdx] = &pLedStrip[ledIdx + BACK_BEG_LEN ];
			pSuspenders.pBackL[LSIDEIDX][(BACK_BEG_LEN) + ledIdx] = &pLedStrip[numLedsTotal - 1 - BACK_BEG_LEN - ledIdx];

			pSuspenders.pBackR[LSIDEIDX][(BACK_BEG_LEN) + ledIdx] = &pLedStrip[(BACK_BEG_LEN + BACK_SIDE_LEN + (2*FRONT_LENGTH)) + (BACK_SIDE_LEN - ledIdx - 1)];
			pSuspenders.pBackL[RSIDEIDX][(BACK_BEG_LEN) + ledIdx] = &pLedStrip[(BACK_BEG_LEN + (2*BACK_SIDE_LEN) + (2*FRONT_LENGTH)) + ledIdx];
		}
		for(uint32_t colIdx=0; colIdx < FRONT_LENGTH; colIdx++)
		{
			pSuspenders.pFrontR[LSIDEIDX][colIdx] = &pLedStrip[(BACK_BEG_LEN + BACK_SIDE_LEN + FRONT_LENGTH + colIdx)];
			pSuspenders.pFrontR[RSIDEIDX][colIdx] = &pLedStrip[(BACK_BEG_LEN + BACK_SIDE_LEN + (FRONT_LENGTH - 1 - colIdx))];
			pSuspenders.pFrontL[LSIDEIDX][colIdx] = &pLedStrip[(BACK_BEG_LEN + (3*BACK_SIDE_LEN) + (3*FRONT_LENGTH) + colIdx)];
			pSuspenders.pFrontL[RSIDEIDX][colIdx] = &pLedStrip[(BACK_BEG_LEN + (3*BACK_SIDE_LEN) + ((3*FRONT_LENGTH) - 1 - colIdx))];
		}
	}
	neo_fio_reg = (uint32_t *)&NRF_GPIO->OUT;
	neo_gpio_clr_reg = (uint32_t *)&NRF_GPIO->OUTCLR;
	neo_gpio_set_reg = (uint32_t *)&NRF_GPIO->OUTSET;
	neo_bitmask = (1 << ledLinePinNums[0]) & 0xFFFFFFFF;

#ifdef NRF_DK_BRD
	pSpiBus = NULL;
	pAccel = NULL;
#else
	pSpiBus = new SPI_bus(LED_SPI_MOSI, LED_SPI_MISO, LED_SPI_CLK);
	pUart = new Serial(WS_LED_1);
	pFs = new fatfs(pSpiBus, SD_CARD_CS);

	pAcc = NULL;

	#endif

	bUpdateLeds = true;
}


void LedModule::BleEventHandler(ble_evt_t* bleEvent)
{

}
void LedModule::TimerEventHandler(u16 passedTime)
{
	static int loopCount = 0;
	static bool bOrange = false;
	static int track_pos_idx = 0;

	uint8_t orange_r[UPDATE_LED_LOOP] = {184,160,136,114,92,72,54,38,25,14,6,2};//{230,215,162,103,47,10,0,22,68,127,183,220};
	uint8_t orange_g[UPDATE_LED_LOOP] = {32,28,24,20,16,13,9,7,4,2,1,1};//{40,37,28,18,8,2,0,4,12,22,32,38};
	uint8_t orange_b = 0;
	uint8_t white_rgb[UPDATE_LED_LOOP] = {112,97,83,69,56,44,33,23,15,9,4,1};//{140,131,98,63,29,6,0,13,42,77,111,134};

	int wave_idx = loopCount % UPDATE_LED_LOOP;
	if(wave_idx == 0){	//every 6th iteration, switch colors
		if(bOrange == true)
			bOrange = false;
		else
			bOrange = true;
	}

	// every 12th iteration, we need to update our position, grab the new led idx
	if( wave_idx == 0){
		pFs->readLine(sGpsData);
		if(sGpsData[0] == '\0'){
			pFs->rewindToBegin();
			pFs->readLine(sGpsData);
			pFs->readLine(sGpsData);
		}

		char * pTok;
		pTok = strtok(sGpsData, ","); 	//pTok -> 1st
		strcpy(sCurrLat,pTok);
		pTok = strtok(NULL, ",");		//2nd
		strcpy(sCurrLon,pTok);
		pTok = strtok(NULL, ",");		//3rd = led index

		track_pos_idx = atoi(pTok);
	}

	setAllLeds(0,0,0);
	uint8_t r,g,b;
	if(bOrange == true){
		r=orange_r[wave_idx];
		g=orange_g[wave_idx];
		b=orange_b;


		if(wave_idx != 0){
			int back_wave_idx,front_wave_idx;
			back_wave_idx = track_pos_idx-(wave_idx>>1);
			front_wave_idx = track_pos_idx+(wave_idx>>1);
			if(back_wave_idx < 0 )
				back_wave_idx += 206;
			if(front_wave_idx > 205 )
				front_wave_idx -= 206;

			setLed(back_wave_idx, r>>1, g>>1, b>>1);
			setLed(front_wave_idx, r>>1, g>>1, b>>1);
		}
	}else{
		r=white_rgb[wave_idx];
		g=r;
		b=r;

		if(wave_idx != 0){
			int back_wave_idx,front_wave_idx;
			back_wave_idx = track_pos_idx-(wave_idx>>1);
			front_wave_idx = track_pos_idx+(wave_idx>>1);
			if(back_wave_idx < 0 )
				back_wave_idx += 206;
			if(front_wave_idx > 205 )
				front_wave_idx -= 206;

			setLed(back_wave_idx, r>>2, g>>2, b>>2);
			setLed(front_wave_idx, r>>2, g>>2, b>>2);
		}
	}

	setLed(track_pos_idx, r, g, b);

	updateLeds();
	//static int slow_wave_loop = 0;
	//if(slow_wave_loop % 2 == 0)
		loopCount++;
	//slow_wave_loop++;
	//check to see if we need to send this current lat long to the nucleo board
	if( !(NRF_GPIO->IN & (1<<BUTTON_1))){
		pUart->send(sCurrLat, strlen(sCurrLat));
		char delim = ',';
		pUart->send(&delim, 1);
		pUart->send(sCurrLon, strlen(sCurrLon));
		delim = '\n';
		pUart->send(&delim, 1);
	}
}
void LedModule::startOperations(void)
{
	pUart->init();

	pFs->open_file();
	char firstLineHeader[256];
	pFs->readLine(firstLineHeader);	//throw it away
	nrf_delay_ms(20);

	setAllLeds(0,0,0);
	updateLeds();
}

void LedModule::performOperation(void)
{
	this->TimerEventHandler(0);
}
void LedModule::immediateOperation(uint32_t event_pins_low_to_high, uint32_t event_pins_high_to_low)
{
}

void LedModule::updateStartTime(void)
{
	/*uint32_t err;
	//err = app_timer_cnt_get(&startTimeTicks);

	if(err != NRF_SUCCESS)
		startTimeTicks = 0;
		*/
	startTimeTicks = app_timer_cnt_get();
}

void LedModule::clearScreen(void){
	for(unsigned int a=0; a < numLedsTotal; a++)
	{
		pLedStrip[a] = {0,0,0,0};
	}
}
void LedModule::updateLeds(void){
	CRITICAL_REGION_ENTER();
	neo_out(pLedStrip,numLedsTotal);
	CRITICAL_REGION_EXIT();
	nrf_delay_us(50);
}
void LedModule::updateUpToLed(uint32_t ledEndIndex)
{
	CRITICAL_REGION_ENTER();
	neo_out(pLedStrip,ledEndIndex+1);
	CRITICAL_REGION_EXIT();
	nrf_delay_us(50);
}
void LedModule::setLed(uint32_t ledIndex,uint8_t rVal, uint8_t gVal, uint8_t bVal)
{
	if(ledIndex < numLedsTotal)
	{
		pLedStrip[ledIndex].dummy = 0;
		pLedStrip[ledIndex].r = rVal;
		pLedStrip[ledIndex].g = gVal;
		pLedStrip[ledIndex].b = bVal;
	}
}
void LedModule::setLeds(uint8_t *pLedArray, uint32_t updateNumLeds)
{
	uint32_t endLedIdx = updateNumLeds;
	if(endLedIdx > numLedsTotal)
		endLedIdx = numLedsTotal;
	for(uint32_t ledIdx=0; ledIdx < endLedIdx; ledIdx++)
	{
		pLedStrip[ledIdx].dummy = 0;
		pLedStrip[ledIdx].r = pLedArray[ledIdx*3];
		pLedStrip[ledIdx].g = pLedArray[ledIdx*3+1];
		pLedStrip[ledIdx].b = pLedArray[ledIdx*3+2];
	}
}
void LedModule::setLeds(ledPixel *pLedArray, uint32_t updateNumLeds)
{
	uint32_t endLedIdx = updateNumLeds;
	if(endLedIdx > numLedsTotal)
		endLedIdx = numLedsTotal;
	for(uint32_t ledIdx=0; ledIdx < endLedIdx; ledIdx++)
	{
		pLedStrip[ledIdx].dummy = 0;
		pLedStrip[ledIdx].r = pLedArray[ledIdx].r;
		pLedStrip[ledIdx].g = pLedArray[ledIdx].g;
		pLedStrip[ledIdx].b = pLedArray[ledIdx].b;
	}
}
void LedModule::setAllLeds(uint8_t rVal, uint8_t gVal, uint8_t bVal)
{
	for(unsigned int a=0; a < numLedsTotal; a++)
	{
		pLedStrip[a].dummy = 0;
		pLedStrip[a].r = rVal;
		pLedStrip[a].g = gVal;
		pLedStrip[a].b = bVal;
	}
}
void LedModule::overwriteLeds(updatePixel *pArrayUpdate, uint32_t updateLen)
{
	for(unsigned int a=0; a < updateLen; a++)
	{
		pLedStrip[pArrayUpdate[a].index].dummy = 0;
		pLedStrip[pArrayUpdate[a].index].r = pArrayUpdate[a].color.r;
		pLedStrip[pArrayUpdate[a].index].g = pArrayUpdate[a].color.g;
		pLedStrip[pArrayUpdate[a].index].b = pArrayUpdate[a].color.b;
	}
}

void LedModule::fromFftMagToColorArray(float * magnitudes, uint32_t numMagnitudes, ledPixel * pLeds)
{
	for(unsigned int a=0; a < numMagnitudes; a++)
	{
		if(a == numLedsTotal)
			break;	//if index a is greater than the number of leds we have, exit

		float brightness = 1;
		if(magnitudes[a] < MAX_BIN_MAG)
			brightness = magnitudes[a] / MAX_BIN_MAG;
		uint8_t value = (uint8_t)(brightness * 255);
		pLeds[a].b = value;
		pLeds[a].g = value;
		pLeds[a].r = value;
	}
}

void LedModule::selftest(void)
{
	int res=0;
	res = pFs->unittest();

	res++;
	while(1){}

	/*
	//test loop to send 1 led color, incrementally changing the number from 0 to 255
	while(1)
	{
		for(int i=0; i<10; i++)
		{
			int bright = i * 255 / 10;
			nrf_delay_ms(10);
			setAllLeds(0,0,0);
			setLed(0, bright, bright, bright);
			updateUpToLed(0);

			nrf_delay_ms(1000);	//wait 1 sec between updates
		}
	};


	startOperations();

	while(1)
	{
		nrf_delay_ms(233);
		TimerEventHandler(133);
	}
	*/
}
