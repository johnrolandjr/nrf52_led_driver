#include <LedModule.h>
#pragma once

//all patterns are moved via 0,0
#define DOT_PATTERN_LEN 1
#define DOT_PATTERN 				  \
	{ {0,0} , {0,0} , { 255 , 255, 255, 0} }
#define WIDEDOT_PATTERN_LEN 1
#define WIDEDOT_PATTERN 			  \
	{ {0,0} , {0,0}, { 255 , 255, 255, 0} }, \
	{ {1,0} , {0,0}, { 255 , 255, 255, 0} }
#define BAR_PATTERN_LEN 1
#define BAR_PATTERN 				  \
	{ {0,0} , {0,0}, { 255 , 255, 255, 0} }, \
	{ {1,0} , {0,0}, { 255 , 255, 255, 0} }, \
	{ {2,0} , {0,0}, { 255 , 255, 255, 0} }, \
	{ {3,0} , {0,0}, { 255 , 255, 255, 0} }, \
	{ {4,0} , {0,0}, { 255 , 255, 255, 0} }, \
	{ {5,0} , {0,0}, { 255 , 255, 255, 0} }

