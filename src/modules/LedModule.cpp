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

LedModule::LedModule(LedWrapper* pLed, const char* name,  uint32_t numLedsInStrip_back, uint32_t numLedsInStrip_sides): Module(pLed, name)
{
	bStartTimerAtBeginning = true;
	startingTimerPeriod = LED_PERIOD_TIME_MS;

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

	numLeds_back = numLedsInStrip_back*2;
	numLeds_side = numLedsInStrip_sides*2;
	numLedsTotal = numLeds_back + numLeds_side*2;
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
	pFs = new fatfs(pSpiBus, SD_CARD_CS);

#endif

	//initialize the button intterupt
	/*
	gpio_interrupt_init(MODE_BUTTON_PIN);
	nrf_gpio_cfg_input(MODE_BUTTON_PIN, NRF_GPIO_PIN_NOPULL);
	gpio_interrupt_enable();
*/
	//Set up digital microphone (PDM)
	pdm_init();

	/*
	colorCycle *pColorCycle = new colorCycle();
	if(pColorCycle != NULL)
	{
		pColorCycle->numColors = 80;
		pColorCycle->currentColorIdx = 0;
		pColorCycle->colors = new ledPixel[80];
		for(unsigned int a=0; a < 10; a++){
			uint32_t rgb = 25*a/10;
			pColorCycle->colors[a] = { (uint8_t)rgb, 0 , 0 , 0};
			pColorCycle->colors[19 - a] = { (uint8_t)rgb, 0 , 0 , 0};

			pColorCycle->colors[a+20] = { 0,(uint8_t)rgb, 0, 0};
			pColorCycle->colors[39 - a] = { 0,(uint8_t)rgb, 0, 0};

			pColorCycle->colors[a+40] = { 0, 0, (uint8_t)rgb, 0};
			pColorCycle->colors[59 - a] = { 0, 0, (uint8_t)rgb, 0};

			pColorCycle->colors[a+60] = { (uint8_t)rgb,(uint8_t)rgb, (uint8_t)rgb, 0};
			pColorCycle->colors[79 - a] = { (uint8_t)rgb,(uint8_t)rgb, (uint8_t)rgb, 0};
		}
	}
	frameCycle *pCenteredRain = new frameCycle();
	if(pCenteredRain !=NULL)
	{
		pCenteredRain->pPixels = chase_rainbow_120_60f;
		pCenteredRain->numFrames = CHASE_RAINBOW_NUM_FRAMES;
		pCenteredRain->numLeds = CHASE_RAINBOW_NUM_LEDS;
		pCenteredRain->currentFrameIdx = 0;
	}
	movingPattern *pBasicDots = new movingPattern();
	if(pBasicDots != NULL)
	{
		pBasicDots->pattern = new pixelWithPos[3];
		pBasicDots->numPixels = 3;
		for(uint32_t idx=0; idx<3; idx++)
		{
			pBasicDots->pattern[idx] = DOT_PATTERN;
		}
		pBasicDots->pattern[0].pos = {0,0};
		pBasicDots->pattern[0].vel = {1,0};
		pBasicDots->pattern[0].brg = {50,0,0,0};

		pBasicDots->pattern[1].pos = {10,3};
		pBasicDots->pattern[1].vel = {2,1};
		pBasicDots->pattern[1].brg = {0,50,0,0};

		pBasicDots->pattern[2].pos = {20,5};
		pBasicDots->pattern[2].vel = {1,3};
		pBasicDots->pattern[2].brg = {0,0,50,0};
	}
	*/

	frameCycle *pSdImage = new frameCycle();
	if(pSdImage != NULL)
	{
		pFs->loadPixelDataFromBmp(pSdImage, 0);
	}

	pCurrentSequence = new ledSequence();
	if(pSdImage != NULL && pCurrentSequence != NULL)
	{
		pCurrentSequence->sequenceData.pFrameCycle = pSdImage;
		pCurrentSequence->type = FRAME_CYCLE;
		bUpdateLeds = true;
	}
}


void LedModule::BleEventHandler(ble_evt_t* bleEvent)
{

}
void LedModule::TimerEventHandler(u16 passedTime)
{
	if(bUpdateLeds)
	{
		if(pCurrentSequence != NULL)
		{
			switch(pCurrentSequence->type)
			{
				case (COLOR_CYCLE):{
					colorCycle * pStruct;
					pStruct = pCurrentSequence->sequenceData.pColorCycle;
					ledPixel newColor = pStruct->colors[pStruct->currentColorIdx];
					setAllLeds(newColor.r,newColor.g,newColor.b);
					updateLeds();
					pStruct->currentColorIdx++;
					if(pStruct->currentColorIdx >= pStruct->numColors)
					{
						pStruct->currentColorIdx = 0;
					}
				}break;
				case (FRAME_CYCLE):{
					frameCycle * pStruct;
					pStruct = pCurrentSequence->sequenceData.pFrameCycle;
					uint32_t pixelIdx = pStruct->currentFrameIdx*3*pStruct->numLeds;
					setLeds((uint8_t *)&pStruct->pPixels[pixelIdx], pStruct->numLeds);
					updateLeds();
					pStruct->currentFrameIdx++;
					if(pStruct->currentFrameIdx >= pStruct->numFrames)
					{
						pStruct->currentFrameIdx = 0;
					}
				}break;
				case (MOVING_PATTERN):{
					movingPattern * pStruct;
					pStruct = pCurrentSequence->sequenceData.pMovePatt;
					clearScreen();
					for(uint32_t ledIdx=0; ledIdx < pStruct->numPixels; ledIdx++)
					{
						uint32_t currRow,currCol;
						currRow = pStruct->pattern[ledIdx].pos.row + pStruct->pattern[ledIdx].vel.row;
						currCol = pStruct->pattern[ledIdx].pos.col + pStruct->pattern[ledIdx].vel.col;
						currCol %= 8;
						currRow %= FRONT_LENGTH;
						switch(currCol)
						{
							case(0):
							case(1):{
								if( currRow > 12 )
								{
									if(currRow > 12 + BACK_BEG_LEN)
									{
										if(currCol%2 == 0)
											*(pSuspenders.pBackR[LSIDEIDX][currRow-BACK_BEG_LEN]) = pStruct->pattern[ledIdx].brg;
										else
											*(pSuspenders.pBackR[RSIDEIDX][currRow-BACK_BEG_LEN]) = pStruct->pattern[ledIdx].brg;
									}else{
										*(pSuspenders.pBackR[currCol%2][currRow-BACK_BEG_LEN]) = pStruct->pattern[ledIdx].brg;
									}
								}
							}break;
							case(2):
							case(3):{
								*(pSuspenders.pFrontR[currCol%2][currRow]) = pStruct->pattern[ledIdx].brg;
							}break;
							case(4):
							case(5):{
								*(pSuspenders.pFrontL[currCol%2][currRow]) = pStruct->pattern[ledIdx].brg;
							}break;
							case(6):
							case(7):{
								if(currRow > 12 + BACK_BEG_LEN)
								{
									if(currCol%2 == 0)
										*(pSuspenders.pBackL[LSIDEIDX][currRow-BACK_BEG_LEN]) = pStruct->pattern[ledIdx].brg;
									else
										*(pSuspenders.pBackL[RSIDEIDX][currRow-BACK_BEG_LEN]) = pStruct->pattern[ledIdx].brg;
								}else{
									*(pSuspenders.pBackL[currCol%2][currRow-BACK_BEG_LEN]) = pStruct->pattern[ledIdx].brg;
								}
							}break;
						}

						pStruct->pattern[ledIdx].pos.col = currCol;
						pStruct->pattern[ledIdx].pos.row = currRow;
					}
					updateLeds();
				}break;
				case (MIC_UPDATE):
				{
					if(bFilledBuffer){
						bFilledBuffer = false;
						//convert buffered signed 16-bit data into signed floating point complex type( with 0 magnitude in the imaginary component)
						float cfft_input[MIC_BUF_SAMPLE_LEN*2];
						float cfft_output[MIC_BUF_SAMPLE_LEN];
						for(unsigned int i = 0 ; i < MIC_BUF_SAMPLE_LEN ; i++)
						{
								cfft_input[i*2] = (float)pPdmFilledBuffer[i] / LARGEST_POSITIVE_16B_NUM;	// Normalizes all data values between -1 and 1
								cfft_input[(i*2)+1] = 0x0;
						};


						/*static void fft_process(float32_t *                   p_input,
						                        const arm_cfft_instance_f32 * p_input_struct,
						                        float32_t *                   p_output,
						                        uint16_t                      output_size)*/
					    arm_cfft_f32(&arm_cfft_sR_f32_len256, cfft_input, 0, 1); 						// Perform cfft on buffered data
					    arm_cmplx_mag_f32(cfft_input, cfft_output, MIC_CFFT_SIZE);						// Calculate the magnitude at each bin using Complex Magnitude Module function.

						//from magnitudes, set pixel colors
						//fromFftMagToColorArray( cfft_output , MIC_CFFT_SIZE , pLedStrip);
						updateLeds();
					}
				}break;
			}
		}
	}
}
void LedModule::startOperations(void)
{
	/*
	//up the back
	for(uint32_t colIndex=0; colIndex<(BACK_BEG_LEN+BACK_SIDE_LEN); colIndex++)
	{
		nrf_delay_ms(20);
		setAllLeds(0,0,0);
		if(colIndex < BACK_BEG_LEN){
			*(pSuspenders.pBackL[0][colIndex]) = {255,255,255,0};
			*(pSuspenders.pBackR[0][colIndex]) = {255,255,255,0};
		}else
		{
			*(pSuspenders.pBackL[0][colIndex]) = {255,255,255,0};
			*(pSuspenders.pBackR[0][colIndex]) = {255,255,255,0};
			*(pSuspenders.pBackL[1][colIndex]) = {255,255,255,0};
			*(pSuspenders.pBackR[1][colIndex]) = {255,255,255,0};
		}
		updateLeds();
	}

	//down the front
	uint32_t colIndex=FRONT_LENGTH-1;
	do
	{
		nrf_delay_ms(20);
		setAllLeds(0,0,0);
		*(pSuspenders.pFrontL[0][colIndex]) = {255,255,255,0};
		*(pSuspenders.pFrontR[0][colIndex]) = {255,255,255,0};
		*(pSuspenders.pFrontL[1][colIndex]) = {255,255,255,0};
		*(pSuspenders.pFrontR[1][colIndex--]) = {255,255,255,0};
		updateLeds();
	}while(colIndex != 0);

	nrf_delay_ms(20);
	setAllLeds(0,0,0);
	*(pSuspenders.pFrontL[0][0]) = {255,255,255,0};
	*(pSuspenders.pFrontR[0][0]) = {255,255,255,0};
	*(pSuspenders.pFrontL[1][0]) = {255,255,255,0};
	*(pSuspenders.pFrontR[1][0]) = {255,255,255,0};
	updateLeds();
	*/

	nrf_delay_ms(20);

	setAllLeds(0,0,0);
	updateLeds();

	if(pCurrentSequence->type == MIC_UPDATE)	//if(we are sequence mode is initialized to use the mic, start recording
	{
		nrf_drv_pdm_start();
	}
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
