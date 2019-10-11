/*
 * UI.h
 *
 *  Created on: 19 Sep 2018
 *      Author: min
 */

#ifndef SDCARD_H_
#define SDCARD_H_
//*****************************************************************************
// INCLUDE FILES
//*****************************************************************************

#include "Module.h"
#include "Key.h"
#include "stm32f1xx_hal.h"


//*****************************************************************************
// MACROS
//*****************************************************************************
// Configuration Address
#define SFLASH_ADDRESS_M2M_RESISTANCE	0x00
#define SFLASH_ADDRESS_M2M_DECTTIME		0x02
#define SFLASH_ADDRESS_M2M_INCIDENT		0x04
#define SFLASH_ADDRESS_M2M_WITHINTIME	0x06
#define SFLASH_ADDRESS_M2M_TIMERELAY	0x08

#define SFLASH_ADDRESS_CIP_RESISTANCE	0x0A
#define SFLASH_ADDRESS_CIP_DECTTIME		0x0C
#define SFLASH_ADDRESS_CIP_INCIDENT		0x0E
#define SFLASH_ADDRESS_CIP_WITHINTIME	0x10
#define SFLASH_ADDRESS_CIP_TIMERELAY	0x12

#define SFLASH_ADDRESS_CON_DECLEVEL		0x14
#define SFLASH_ADDRESS_CON_MINDURATION	0x16
#define SFLASH_ADDRESS_CON_TIMERELAY	0x18
#define SFLASH_ADDRESS_CON_TEMPERATURE	0x1A


#define SFLASH_LENGTH_TOTAL_CONFIG	0x1C

typedef enum
{
	SDCARD_STATE_IDLE = 0,
	SDCARD_STATE_READY,
	SDCARD_STATE_BUSY
}SDCARD_STATE;

//*****************************************************************************
// VARIABLES
//*****************************************************************************
typedef struct
{
	BYTE yState;

}SDCARD_STATUS;

typedef struct
{
	PBYTE pConfigurator;

	BYTE yLoadData;
}SDCARD_SYSTEM_SETTING;

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
void SDCard_InitializeModule(void);

/********************************************************************
 * Function:		SDCard_Mount()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Mount the file system.
 *
 * Note:			None
 *******************************************************************/
void SDCard_Mount(void);

/********************************************************************
 * Function:		SDCard_Unmount()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		UnMount the SD card
 *
 * Note:			None
 *******************************************************************/
void SDCard_Unmount(void);

/********************************************************************
 * Function:		SDCard_Createfile()
 *
 * PreCondition:	None
 *
 * Input:			name: Name of file to be created.
 *
 * Output:			None
 *
 * Side Effects:	None
 *
 * Overview:		None
 *
 * Note:			None
 *******************************************************************/
void SDCard_Createfile (CHAR *name);

/********************************************************************
 * Function:		SDCard_WriteFile()
 *
 * PreCondition:	None
 *
 * Input:			filename: Name of the file to be write,
 					Data: write data to the file
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Writing the data to SDcard for specific file
 *
 * Note:			None
 *******************************************************************/
void SDCard_WriteFile (CHAR *filname, CHAR* pData);

/********************************************************************
 * Function:		SDCard_SeekWriteFile()
 *
 * PreCondition:	None
 *
 * Input:			filename: Name of the file to be write,
 					Data: write data to the file
 					WORD: Seek pointer
 					WORD: Write Data Len;
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Writing the data to SDcard for specific file start
 					from specific index and length
 *
 * Note:			None
 *******************************************************************/
void SDCard_SeekWriteFile (CHAR *filname, CHAR* pData, WORD wIndex, WORD wLen);

/********************************************************************
 * Function:		SDCard_UpdateFile()
 *
 * PreCondition:	None
 *
 * Input:			filename: Name of the file to be write,
 					Data: write data to the file
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Update the data in the existing file
 *
 * Note:			None
 *******************************************************************/
void SDCard_UpdateFile (CHAR *filname, CHAR* pData);

/********************************************************************
 * Function:		SDCard_WriteFile()
 *
 * PreCondition:	None
 *
 * Input:			filename: Name of the file to be write,
 					Data: write data to the file
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Writing the data to SDcard for specific file
 *
 * Note:			None
 *******************************************************************/
void SDCard_ReadFile (CHAR *filname, CHAR* pData);

/********************************************************************
 * Function:		SDCard_SeekFile()
 *
 * PreCondition:	None
 *
 * Input:			filename: Name of the file to be write,
 					Data: write data to the file
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Set the file Position to write the data;
 *
 * Note:			None
 *******************************************************************/
void SDCard_SeekFile (CHAR *filname, DWORD dwIndex);
/********************************************************************
 * Function:		SDCard_GetFreeSize()
 *
 * PreCondition:	None
 *
 * Input:			None:
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		return the SDCard Free  Size
 *
 * Note:			None
 *******************************************************************/
uint32_t SDCard_GetFreeSize (void);

/********************************************************************
 * Function:		SDCard_GetTotalSize()
 *
 * PreCondition:	None
 *
 * Input:			None:
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		return the SDCard SD total size
 *
 * Note:			None
 *******************************************************************/
uint32_t SDCard_GetTotalSize (void);

/********************************************************************
 * Function:		SDCard_Removefile()
 *
 * PreCondition:	None
 *
 * Input:			None:
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Delete the file from the SD card.
 *
 * Note:			None
 *******************************************************************/
void SDCard_Removefile (CHAR *name);

/********************************************************************
 * Function:		SDCardif_WriteDataToBuffer()
 *
 * PreCondition:	None
 *
 * Input:		PBYTE pData: The data that will write into the memory.
 *				WORD wAddress: The flash memory's address.
 *				BYTE yLength: Total data length.
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Write the data into the buffer. It will write in different buffer
 *				depend on the address. It will not write anything if the
 *				address is illegal.
 *                  
 * Note:			None
 *******************************************************************/
void SDCardif_WriteDataToBuffer(
	PBYTE pData,
	DWORD dwAddress,
	WORD wLength
);

/********************************************************************
 * Function:		SDCardif_WriteBufferToMemory()
 *
 * PreCondition:	None
 *
 * Input:			BYTE yResult: To write in or not
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Write in the data from buffer to external memory. After write
 *				in it will release the buffer's memory
 *                  
 * Note:			None
 *******************************************************************/
void SDCardif_WriteBufferToMemory(BYTE yResult);

/********************************************************************
 * Function:		SDCard_ReleaseBuffer()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Release the buffer's memory
 *                  
 * Note:			None
 *******************************************************************/
void SDCard_ReleaseBuffer(void);

/********************************************************************
 * Function:		SFlash_AllocateBuffer()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:			BYTE: The result od allocate memory for buffer
 *
 * Side Effects:	None
 *
 * Overview:		Allocate the memory to use to buffer. The external flash
 *				need to write data to the buffer first and call the write in
 *				function to write back all the data as now all the data put
 *				to the same block.
 *                  
 * Note:			None
 *******************************************************************/
BYTE SDCard_AllocateBuffer(void);

/********************************************************************
 * Function:		SDCard_PrintMsg()
 *
 * PreCondition:	None
 *
 * Input:			*pData
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Print only when echo is on
 *
 * Note:			None
 *******************************************************************/
void SDCard_PrintMsg (CHAR *pData);
void SDCard_TestWrite (void);
#endif /* SDCARD_H_ */
