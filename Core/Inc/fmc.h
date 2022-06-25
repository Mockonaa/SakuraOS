/*
*
*   Author: Matthias Guthörl (Github - fliescher)
*   Based on fmc.h published on https://github.com/NickNagy/Cortet/blob/master/Inc/fmc.h
*
*/

#ifndef FMC_H
#define FMC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f7xx_hal.h" //_sram.h"

//FSMC Write addresses (16bits)
#define ST7789_REG			*(__IO uint16_t *)(((uint32_t)0x60000000))
#define ST7789_RAM			*(__IO uint16_t *)(((uint32_t)0x60000000) + (1 << (18+1))) //A18

#define MIN_ADDSET_TIME  0
#define MAX_ADDSET_TIME  15
#define MIN_ADDHOLD_TIME 1
#define MAX_ADDHOLD_TIME 15
#define MIN_DATAST_TIME  1
#define MAX_DATAST_TIME  256


typedef struct ExternalSRAMSpecStruct {
	uint8_t dataSize;
	uint8_t tCycRead;
	uint8_t tCycWrite;
	uint8_t tWRLW;
	uint8_t tACC;
	uint8_t tAS;
	uint8_t writeOnly;
} ExternalSRAMSpecStruct;

void FMCComputeAddsetDatast(ExternalSRAMSpecStruct * sramSpec, FMC_NORSRAM_TimingTypeDef * timing);
void FMCSRAMInit(ExternalSRAMSpecStruct * sramSpec, uint8_t bank);
void FMC_Error_Handler();

#ifdef __cplusplus
}
#endif

#endif
