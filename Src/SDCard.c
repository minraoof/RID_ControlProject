/*
 * UI.c
 *
 *  Created on: 19 Sep 2018
 *      Author: min
 */
//*****************************************************************************
// INCLUDE FILES
//*****************************************************************************
#include "stm32f1xx_hal.h"
#include <string.h>
#include <stdlib.h>

#include "UI.h"
#include "Module.h"
#include "USB_Module.h"
#include "Version.h"
#include "Utility.h"
#include "ADC.h"
#include "Key.h"
#include "RTC.h"
#include "Power.h"
#include "SDCard.h"

#include "ff.h"



//*****************************************************************************
// MACROS
//*****************************************************************************



//*****************************************************************************
// VARIABLES
//*****************************************************************************
SDCARD_STATUS g_SDCard_Status;

FATFS fs;
FATFS *pfs;
FIL fil;
FRESULT fres;
DWORD fre_clust;
uint32_t totalSpace, freeSpace;
char buffer[100];


uint32_t wbytes; /* File write counts */
uint8_t wtext[] = "text to write logical disk"; /* File write buffer */
CHAR readbuf[20] = {0};


BYTE g_File_Result;
//*****************************************************************************
// FUNCTION DEFINITION
//*****************************************************************************
/********************************************************************
 * Function:		SDCard_InitializeModule()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Initialize the SDCard module
 *
 * Note:			None
 *******************************************************************/
void SDCard_InitializeModule(void)
{
  g_SDCard_Status.yState = SDCARD_STATE_IDLE;
	
	//g_File_Result = f_mount(&SDFatFS, (TCHAR const*)SDPath, 0);
}

/**********************************************************************
 * Function:		SDCard_SignalHandler()
 *
 * PreCondition:	None
 *
 * Input:			Module_Signal* pSignal: The event signal
 *
 * Output:			BYTE: The signal handle result.
 *
 * Side Effects:	None
 *
 * Overview:		The SDCard module signal handler. It control the state machine
 *				state.
 *
 * Note:			None
 *******************************************************************/
BYTE SDCard_SignalHandler(Module_Signal* pSignal)
{
	if (NULL == pSignal)
	{
		return MODULE_RESULT_EMPTY;
	}

	switch (pSignal->wEvent)
	{

		default:
			break;
	}

	return MODULE_RESULT_OK;
}
/********************************************************************
 * Function:		SDCard_TestWrite()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Test for SDcard
 *
 * Note:			None
 *******************************************************************/
void SDCard_TestWrite(void)
{	
	/* Mount SD Card */
	if(f_mount(&fs, "", 0) != FR_OK)
		_Error_Handler(__FILE__, __LINE__);

	/* Open file to write */
	if(f_open(&fil, "first.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE) != FR_OK)
	{
			_Error_Handler(__FILE__, __LINE__);
			return;
	}
		

	/* Check freeSpace space */
	if(f_getfree("", &fre_clust, &pfs) != FR_OK)
		_Error_Handler(__FILE__, __LINE__);

	totalSpace = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
	freeSpace = (uint32_t)(fre_clust * pfs->csize * 0.5);

	/* free space is less than 1kb */
	if(freeSpace < 1)
		_Error_Handler(__FILE__, __LINE__);

	/* Writing text */
	f_puts("STM32 SD Card I/O Example via SPI\n", &fil);
	f_puts("Save the world!!!", &fil);

	/* Close file */
	if(f_close(&fil) != FR_OK)
		_Error_Handler(__FILE__, __LINE__);

	/* Open file to read */
	if(f_open(&fil, "first.txt", FA_READ) != FR_OK)
		_Error_Handler(__FILE__, __LINE__);

	/*while(f_gets(buffer, sizeof(buffer), &fil))
	{
		// SWV output 
		printf("%s", buffer);
		fflush(stdout);
	}*/

	/* Close file */
	if(f_close(&fil) != FR_OK)
		_Error_Handler(__FILE__, __LINE__);

	/* Unmount SDCARD */
	if(f_mount(NULL, "", 1) != FR_OK)
		_Error_Handler(__FILE__, __LINE__);
  //FATFS_UnLinkDriver(SDPath);

}


