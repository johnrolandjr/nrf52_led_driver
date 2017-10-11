/*
 * pin_cfg_suspenders_v3.h
 *
 *  Created on: Aug 11, 2017
 *      Author: johnr
 */

#ifndef PIN_CFG_SUSPENDERS_V3_H_
#define PIN_CFG_SUSPENDERS_V3_H_

#define NC_1	3
#define NC_2	4
#define NC_3	5
#define NC_4	6
#define NC_5	7
#define NC_6	8
#define NC_7	11
#define NC_8	12
#define NC_9 	15
#define NC_10	16
#define NC_11	17
#define NC_12	20
#define NC_13	21
#define NC_14	27
#define NC_15	28

#define BRD_LED		31

//communication
#define SPI_MOSI 	18
#define SPI_MISO 	24
#define SPI_CLK		23
#define SPI_CS_ACC	14
#define SPI_CS_SD	19

//devices
#define ACCEL_INT 	13
#define WS_LED_1	9
#define WS_LED_2	10
#define V_IN_PIN	2	//analog 0
#define MIC_DO		29
#define MIC_CLK		30
#define BUTTON_1	26
#define BUTTON_2	25

#endif /* PIN_CFG_SUSPENDERS_V3_H_ */
