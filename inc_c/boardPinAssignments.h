#pragma once

#ifdef NRF_DK_BRD
//We are in the nrf52 workspace, so this is the nrf52 dk board pin assignment
//AVAILBLE GPIOS
#define P0	0
#define P1	1
#define P2	2
#define P3	3
#define P4	4
#define P5	5
#define P6	6
#define P7  7
#define P8	8
#define P9  9
#define P10	10
#define P11	11
#define P12	12
#define P13	13
#define P14	14
#define P15	15
#define P16	16
#define P17	17
#define P18	18
#define P19	19
#define P20	20
#define P21	21
#define P22	22
#define P23	23
#define P24	24
#define P25	25
#define P26	26
#define P27	27
#define P28	28
#define P29	29
#define P30 30
#define P31 31
/* redefined in pca10040.h
#define LED_1	P17
#define LED_2	P18
#define LED_3	P19
#define LED_4	P20

#define BUTTON_1	P13
#define BUTTON_2	P14
#define BUTTON_3	P15
#define BUTTON_4	P16
*/
#define RESET	P21

#define AIN_0	P2
#define AIN_1	P3
#define AIN_2	P4
#define AIN_3	P5
#define AIN_4	P28
#define AIN_5	P29
#define AIN_6	P30
#define AIN_7	P31

#define XL_1	P0
#define XL_2	P1
#endif

#ifdef CUSTOM_BRD
#include "pin_cfg_suspenders_v3.h"
#endif
