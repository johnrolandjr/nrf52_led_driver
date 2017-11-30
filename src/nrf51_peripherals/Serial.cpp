#include "Serial.h"

#include "pin_cfg_suspenders_v3.h"

extern "C"{
	#include <stdbool.h>
	#include <stdint.h>
	#include <stdio.h>
	#include "app_uart.h"
	#include "app_error.h"
	#include "nrf_delay.h"
	#include "nrf.h"
	#include "nrf_gpio.h"
}
#define UART_TX_BUF_SIZE 256                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE 256                         /**< UART RX buffer size. */

extern "C"{

	void uart_error_handle(app_uart_evt_t * p_event)
	{
		if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR)
		{
			APP_ERROR_HANDLER(p_event->data.error_communication);
		}
		else if (p_event->evt_type == APP_UART_FIFO_ERROR)
		{
			APP_ERROR_HANDLER(p_event->data.error_code);
		}
	}
}
Serial::Serial(uint32_t tx, uint32_t rx){
	txPin = tx;
	rxPin = rx;
	bOnlyTransmit = false;
}
Serial::Serial(uint32_t tx){
	txPin = tx;
	rxPin = UART_PIN_DISCONNECTED;
	//nrf_gpio_cfg_input(rxPin, NRF_GPIO_PIN_PULLUP);
	bOnlyTransmit = true;
}

#define RTS_PIN_DEFAULT NC_2
#define CTS_PIN_DEFAULT NC_3
void Serial::init(){
	uint32_t err_code;

	const app_uart_comm_params_t comm_params =
	{
		rxPin,
		txPin,
		RTS_PIN_DEFAULT,
		CTS_PIN_DEFAULT,
		APP_UART_FLOW_CONTROL_DISABLED,
		false,
		NRF_UART_BAUDRATE_9600
	};

	APP_UART_FIFO_INIT(&comm_params,
						 UART_RX_BUF_SIZE,
						 UART_TX_BUF_SIZE,
						 uart_error_handle,
						 APP_IRQ_PRIORITY_LOWEST,
						 err_code);

	APP_ERROR_CHECK(err_code);
}

void Serial::send(char * pData, unsigned int numBytes){

	for(int a=0; a<numBytes; a++){
		while(app_uart_put(*pData) != NRF_SUCCESS );
		pData++;
	}
}
