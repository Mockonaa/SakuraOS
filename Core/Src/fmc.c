/*
*
*   Copied from https://github.com/NickNagy/Cortet/blob/master/Src/fmc.c
*
*/

#include "fmc.h"

/*
 * @brief computes DATAST and ADDSET for FMC timing config (sets DATAST to its lowest legal value)
 * @param
 * 	sramSpec: ptr to sram device spec struct, provides time constraint info for func
 * 	timing: ptr to timing typedef struct, used to config fmc in hardware
 * @return None
 *
 * NOTE: this function has not proven to work as well as expected for ILI9341 interface, which has worked best with DATAST=6, ADDSET=6 @168MHz
 */
void FMCComputeAddsetDatast(ExternalSRAMSpecStruct * sramSpec, FMC_NORSRAM_TimingTypeDef * timing) {
	uint16_t tCyc, datast, addset1, addset2;
	float tHCLK;

	// TODO: need to make this generic, not guaranteed that clock is 168MHz
	//tHCLK = 5.95;//1000.0/168.0;
    tHCLK = 6.25; 

	if (sramSpec->writeOnly) {
		tCyc = sramSpec->tCycWrite;
	} else {
		tCyc = (sramSpec->tCycWrite > sramSpec->tCycRead) ? sramSpec->tCycWrite : sramSpec->tCycRead;
	}

	// DATAST*tHCLK >= tWRLW
	datast = (uint16_t)(sramSpec->tWRLW/tHCLK) + 1;
	timing->DataSetupTime = datast;

	// (ADDSET + DATAST + 2)*tHCLK >= MAX(tCYCREAD, tCYCWRITE)
	addset1 = (int)(tCyc/tHCLK) - 1- datast;

	// DATAST >= (((tACC + tAS) + (tsu(Data_NE) + tv(A_NE)))/tHCLK) - ADDSET - 4
	addset2 = (int)((sramSpec->tACC + sramSpec->tAS + tHCLK - 0.5)/tHCLK) - datast - 3;

	// set to the maximum of the two
	timing->AddressSetupTime = (addset1 > addset2) ? addset1 : addset2;
}

void FMCSRAMInit(ExternalSRAMSpecStruct * sramSpec, uint8_t bank) {
	/* FMC configuration */
	SRAM_HandleTypeDef sram;
	FMC_NORSRAM_TimingTypeDef timing = {0};

	sram.Instance = FMC_NORSRAM_DEVICE;
	sram.Extended = FMC_NORSRAM_EXTENDED_DEVICE;

	if (bank==1) {
		sram.Init.NSBank = 0;
	} else {
		sram.Init.NSBank = 1 << (bank-1);
	}

	if (sramSpec -> dataSize == 8) {
		sram.Init.MemoryDataWidth = FMC_NORSRAM_MEM_BUS_WIDTH_8;
	} else if (sramSpec -> dataSize == 16) {
		sram.Init.MemoryDataWidth = FMC_NORSRAM_MEM_BUS_WIDTH_16;
	} else {
		sram.Init.MemoryDataWidth = FMC_NORSRAM_MEM_BUS_WIDTH_32;
	}

	sram.Init.DataAddressMux = FMC_DATA_ADDRESS_MUX_DISABLE;
	sram.Init.MemoryType = FMC_MEMORY_TYPE_SRAM;
	sram.Init.BurstAccessMode = FMC_BURST_ACCESS_MODE_DISABLE;
	sram.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
	sram.Init.WaitSignalActive = FMC_WAIT_TIMING_BEFORE_WS;
	sram.Init.WriteOperation = FMC_WRITE_OPERATION_ENABLE;
	sram.Init.WaitSignal = FMC_WAIT_SIGNAL_DISABLE;
	sram.Init.ExtendedMode = FMC_EXTENDED_MODE_DISABLE;
	sram.Init.AsynchronousWait = FMC_ASYNCHRONOUS_WAIT_DISABLE;
	sram.Init.WriteBurst = FMC_WRITE_BURST_DISABLE;
	sram.Init.ContinuousClock = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;
	sram.Init.WriteFifo = FMC_WRITE_FIFO_DISABLE;
	sram.Init.PageSize = FMC_PAGE_SIZE_NONE;

	/* Timing */
	timing.BusTurnAroundDuration = 0;
	timing.AccessMode = FMC_ACCESS_MODE_A;

	timing.AddressSetupTime = 6; //MAX_ADDSET_TIME;
	timing.DataSetupTime = 6;//MAX_DATAST_TIME;

	//FMCComputeAddsetDatast(sramSpec, &timing);

	if (HAL_SRAM_Init(&sram, &timing, NULL) != HAL_OK)
	{
		FMC_Error_Handler( );
	}

#if MEM_SWAPPING
	HAL_EnableFMCMemorySwapping();
#endif
}

void FMC_Error_Handler() {

}
