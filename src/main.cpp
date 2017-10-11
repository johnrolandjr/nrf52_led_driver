#include <stdio.h>
#include <stdlib.h>

#include <Main.h>
#include <Node.h>
#include <AdvertisingController.h>
#include <ScanController.h>
#include <GAPController.h>
#include <GATTController.h>
#include <LedWrapper.h>
#include <Module.h>
#include <Utility.h>
#include <types.h>
#include <Config.h>
#include <LedModule.h>


extern "C"{
#include <stdlib.h>
#include <nrf_soc.h>
#include <app_error.h>
#include <softdevice_handler.h>
#include <app_timer.h>
#include <malloc.h>
#include <nrf_delay.h>
#include <app_timer.h>
#include <nrf_gpio.h>
#include <nrf_nvic.h>
#include <ble_l2cap_app.h>
#include "sdk_config.h"
#include "boardPinAssignments.h"
}

extern "C"{
	void impulse_triggered_event_handler(uint32_t event_pins_low_to_high, uint32_t event_pins_high_to_low);
	void impactDetectedHandler(uint32_t event_pins_low_to_high, uint32_t event_pins_high_to_low);
	//### TIMERS ##############################################################
	//static app_timer_id_t activityTimerMsId; //FSM state timer
	static app_timer_t timer_id_0_data = { {0} };
	static app_timer_id_t mainTimerMsId;
	//app_timer_id_t mainTimerMsId;
	//APP_TIMER_DEF(mainTimerMsId);
}

//A global buffer for the current event, which must be 4-byte aligned
#pragma pack(push)
#pragma pack(4)
static uint8_t currentEventBuffer[BLE_EVT_LEN_MAX(23)];
static ble_evt_t* currentEvent = (ble_evt_t *) currentEventBuffer;
static u16 sizeOfEvent = sizeof(currentEventBuffer);
static u16 sizeOfCurrentEvent = sizeOfEvent;
#pragma pack(pop)

// Include (or do not) the service_changed characteristic.
// If not enabled, the server's database cannot be changed for the lifetime of the device
#define IS_SRVC_CHANGED_CHARACT_PRESENT 1

#define APP_TIMER_PRESCALER       99	// Value of the RTC1 PRESCALER register
#define APP_TIMER_MAX_TIMERS      1 	// Maximum number of simultaneously created timers
#define APP_TIMER_OP_QUEUE_SIZE   1 	// Size of timer operation queues

//Reference to Node
LedWrapper* pLed = NULL;
Module* mainModule = NULL;
Conf* pConfig = NULL;

//Debug variable
//bool lookingForInvalidStateErrors = false;

Conf* Conf::instance;

//volatile bool bImpactDetected = false;	//TODO:: remove both of these depreciated booleans
//volatile bool bTimedOperation = false;

int main(void)
{
	u32 err = 0;
#ifdef NRF_DK_BRD
	pLed = new LedWrapper(LED_2,false);
#else
	pLed = new LedWrapper(BRD_LED, false);
#endif

	pLed->Toggle();
	pLed->Toggle();

	//pFlash = new FlashDataStorage();

	mainModule = new LedModule(pLed, "ws2812", Config->numLeds_back, Config->numLeds_side);

	mainModule->startOperations();

	//Initialialize the SoftDevice and the BLE stack
#ifndef DISABLE_BLE
	bleInit();
#endif

	lfclk_request();
	initTimers();		//initializes state timer

	pLed->ToggleOnForMs(200);

	mainModule->startOperations();
	if(mainModule->bStartTimerAtBeginning)
		startTimers(mainModule->startingTimerPeriod);

	while (true)
	{
		u32 err = NRF_ERROR_NOT_FOUND;

		//basic blind timing of led update
#ifdef DISABLE_BLE
		while(true)
		{
			if(mainModule->bOperationRequested)
			{
				mainModule->performOperation();
				mainModule->bOperationRequested = false;
			}
			__WFI();
		}
		/*
		while(1)
		{
			nrf_delay_ms(mainModule->startingTimerPeriod);
			mainModule->TimerEventHandler(mainModule->startingTimerPeriod);
		}
		*/
#endif
		do
		{
			if(mainModule->bOperationRequested)
			{
				mainModule->performOperation();
				mainModule->bOperationRequested = false;
			}
			nrf_delay_ms(1500);

#ifndef DISABLE_BLE
			// Fetch the event
			sizeOfCurrentEvent = sizeOfEvent;
			u32* pTest = (u32*)currentEventBuffer;


			err = sd_ble_evt_get(currentEventBuffer, &sizeOfCurrentEvent);

			// Handle ble event event
			if (err == NRF_SUCCESS)
			{
				bleDispatchEventHandler(currentEvent);
			}else if (err == NRF_ERROR_NOT_FOUND)// No more events available
			{

				err = sd_app_evt_wait();
				APP_ERROR_CHECK(err);
				sd_nvic_ClearPendingIRQ(SD_EVT_IRQn);
				break;
			}else
			{
				APP_ERROR_CHECK(err);
				break;
			}
#endif
		} while (true);
	}
}

//INIT function that starts up the Softdevice and registers the needed handlers
void bleInit(void){
	u32 err = 0;

    // Initialize the SoftDevice handler with the low frequency clock source
	//And a reference to the previously allocated buffer
	//No event handler is given because the event handling is done in the main loop
	//SOFTDEVICE_HANDLER_INIT(NRF_CLOCK_LFCLKSRC_XTAL_20_PPM, NULL);

	nrf_clock_lf_cfg_t rc_cfg;
	memset(&rc_cfg, 0, sizeof(nrf_clock_lf_cfg_t));

#ifdef NRF_DK_BRD
	rc_cfg.source = NRF_CLOCK_LF_SRC_RC;
	rc_cfg.rc_ctiv = 16;
	rc_cfg.rc_temp_ctiv = 2;
	//rc_cfg.xtal_accuracy = NRF_CLOCK_LF_XTAL_ACCURACY_20_PPM;
	//err = softdevice_handler_init(NRF_CLOCK_LF_SRC_RC, currentEventBuffer, sizeOfEvent, NULL);
#endif
#ifdef CUSTOM_BRD_WASHINGTON
	rc_cfg.source = NRF_CLOCK_LF_SRC_RC;
	rc_cfg.rc_ctiv = 16;
	rc_cfg.rc_temp_ctiv = 2;
	/*
	rc_cfg.source = NRF_CLOCK_LF_SRC_XTAL;
	rc_cfg.rc_ctiv = 0;
	rc_cfg.rc_temp_ctiv = 0;
	rc_cfg.xtal_accuracy = NRF_CLOCK_LF_XTAL_ACCURACY_20_PPM;
	 */
#endif
	err = softdevice_handler_init(&rc_cfg, currentEventBuffer, sizeOfEvent, NULL);
	APP_ERROR_CHECK(err);

    // Register with the SoftDevice handler module for System events.
    err = softdevice_sys_evt_handler_set(sys_evt_dispatch);
    APP_ERROR_CHECK(err);

	//FOR THE S130 WE MUST NOW CALL sd_ble_enable() TO ENABLE BLE FUNCTIONALITY
	//Decide if we include the service changed characteristic in our services
    /*
	ble_enable_params_t bleSdEnableParams;
    memset(&bleSdEnableParams, 0, sizeof(bleSdEnableParams));
    bleSdEnableParams.common_enable_params.vs_uuid_count = 1;
    bleSdEnableParams.common_enable_params.p_conn_bw_counts = NULL;
    bleSdEnableParams.gap_enable_params.central_conn_count = 2;
    bleSdEnableParams.gap_enable_params.periph_conn_count = 1;
    bleSdEnableParams.gap_enable_params.central_sec_count = 0;
    bleSdEnableParams.gatts_enable_params.attr_tab_size = ATTR_TABLE_MAX_SIZE;
    bleSdEnableParams.gatts_enable_params.service_changed = IS_SRVC_CHANGED_CHARACT_PRESENT;
    */

    uint32_t app_ram_base;
    app_ram_base = 0x20003028;	//0x20003028 //0x20001270
    err = sd_ble_enable( &app_ram_base);
    APP_ERROR_CHECK(err);

    //Enable DC/DC (needs external LC filter, cmp. nrf51 reference manual page 43)
#ifdef CUSTOM_BRD_V2
	err = sd_power_dcdc_mode_set(NRF_POWER_DCDC_DISABLE);
#else
	//err = sd_power_dcdc_mode_set(NRF_POWER_DCDC_ENABLE);
#endif
	//APP_ERROR_CHECK(err);

	//Set power mode
	//err = sd_power_mode_set(NRF_POWER_MODE_CONSTLAT);
	//APP_ERROR_CHECK(err);
}

extern "C"
{

	void modulePinTriggerHandler(uint32_t const * event_pins_low_to_high, uint32_t const * event_pins_high_to_low)
	{
		mainModule->bOperationRequested = true;
		mainModule->immediateOperation(*event_pins_low_to_high, *event_pins_high_to_low);
	}


	/*
	//The app_error handler is called by all APP_ERROR_CHECK functions
	void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name)
	{
		//We want to debug DEADBEEF => Endless loop.
		if(error_code == 0xDEADBEEF){

			pLed->On();
			nrf_delay_ms(5000);
			NVIC_SystemReset();
			while(1){
			}
		}

		//Output Error message to UART
		if(error_code != NRF_SUCCESS){
			const char* errorString = Logger::getNrfErrorString(error_code);
			//logt("ERROR", "ERROR CODE %d: %s in file %s@%d", error_code, errorString, p_file_name, line_num);
		}


		//NRF_ERROR_BUSY is not an error(tm)
		//FIXME: above statement is not true

		//All other errors will run into endless loop for debugging
		pLed->On();
		nrf_delay_ms(5000);
		NVIC_SystemReset();
		while(1){
		}
	}
	*/

	//Called when the softdevice crashes
	void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
	{
		//Does not produce interesting filename,....
	    app_error_handler(0xDEADBEEF, 0, NULL);
	}

	//Dispatches system events
	void sys_evt_dispatch(uint32_t sys_evt)
	{
		//fs_sys_event_handler(sys_evt);

	}
	//This is, where the program will get stuck in the case of a Hard fault
	void HardFault_Handler(void)
	{
		pLed->On();
		//pHardfaultAccessorSqrlModule->updateFlashMetaData();
		nrf_delay_ms(5000);
		NVIC_SystemReset();

		for (;;)
		{
		}
	}
	static void app_led_timer_dispatch(void * p_context)
	{
		UNUSED_PARAMETER(p_context);
		mainModule->bOperationRequested = true;
	}
}


//Called by the app_timer module
static void ble_timer_dispatch(void * p_context)
{
    UNUSED_PARAMETER(p_context);
    mainModule->TimerEventHandler(Config->mainTimerTickMs);
}

// Function starting the internal LFCLK oscillator.
// This is needed by RTC1 which is used by the application timer
// (When SoftDevice is enabled the LFCLK is always running and this is not needed).
static void lfclk_request(void)
{
    uint32_t err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);
    nrf_drv_clock_lfclk_request(NULL);
}

//Starts an application timer
void initTimers(void){
	u32 err = 0;

	//APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, NULL);
    err = app_timer_init();
    APP_ERROR_CHECK(err);

	mainTimerMsId = &timer_id_0_data;
#ifndef DISABLE_BLE
	err = app_timer_create(&mainTimerMsId, APP_TIMER_MODE_REPEATED, ble_timer_dispatch);
#else
	err = app_timer_create(&mainTimerMsId, APP_TIMER_MODE_REPEATED, app_led_timer_dispatch);
#endif
	APP_ERROR_CHECK(err);

}

void startTimers(uint32_t periodMs)
{
	u32 err = 0;
	if(periodMs == 0)
		periodMs = Config->mainTimerTickMs;
	err = app_timer_start(mainTimerMsId, APP_TIMER_TICKS(periodMs), NULL);
	APP_ERROR_CHECK(err);
}

void stopTimers(void)
{
	u32 err = 0;
	err = app_timer_stop(mainTimerMsId);
	APP_ERROR_CHECK(err);
}
