/*
 * fatfs_c_api.h
 *
 *  Created on: Aug 23, 2017
 *      Author: johnr
 */

#ifndef FATFS_C_API_H_
#define FATFS_C_API_H_

#ifdef __cplusplus
	extern "C"{
#endif

	void * 	c_create();
	void	c_destroy(void * cThisFs);
	BYTE 	c_init(void * cThisFs);
	DRESULT	c_read_sectors(void * cThisFs, BYTE* pRx, DWORD startSector, BYTE sectorCnt);

#ifdef __cplusplus
	}
#endif

#endif /* FATFS_C_API_H_ */
