/*
 * animation_structs.h
 *
 *  Created on: Aug 27, 2017
 *      Author: johnr
 */

#ifndef ANIMATION_STRUCTS_H_
#define ANIMATION_STRUCTS_H_

typedef struct
{
	const uint8_t  *pPixels;
	uint32_t numFrames;
	uint32_t numLeds;
	uint32_t currentFrameIdx;
}frameCycle;

#endif /* ANIMATION_STRUCTS_H_ */
