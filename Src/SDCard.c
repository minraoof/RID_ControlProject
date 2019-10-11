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
#define FA_OPEN_APPEND 0x30

SDCARD_SYSTEM_SETTING g_SDcard_Setting;

//*****************************************************************************
// VARIABLES
//*****************************************************************************
SDCARD_STATUS g_SDCard_Status;
BYTE g_SDCard_Echo;
CHAR g_SD_FileDataLog[] = "Datalog.txt";
CHAR g_SD_FileConfig[] = "Config.txt";

//File System Variable
FATFS g_SD_Fats;  // file system
FIL g_SD_FIL; // File
FILINFO g_SD_Fileinfo;
FRESULT g_SD_Fresult;  // result
UINT g_SD_FileReadCnt, g_SD_FileWriteCnt;  // File read/write count
CHAR g_SD_PrintBuf[128];


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

	g_SDCard_Echo = TRUE;

	g_SDcard_Setting.pConfigurator = NULL;
	g_SDcard_Setting.yLoadData = FALSE;
	
	SDCard_Mount();
}

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
void SDCard_Mount(void)
{
	g_SD_Fresult= f_mount(&g_SD_Fats, "", 0);
	
	if (g_SD_Fresult != FR_OK) 
	{
		USB_PrintMsg("Error in mounting SD CARD");
	}
	else 
	{
		USB_PrintMsg("SD CARD mounted successfully");
	}
}

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
void SDCard_Unmount(void)
{
	g_SD_Fresult = f_mount(NULL, "", 1);
	
	if (g_SD_Fresult == FR_OK)
	{
		USB_PrintMsg ("SD CARD UNMOUNTED successfully.");
	}
	else
	{
		USB_PrintMsg("error!!! in UNMOUNTING SD CARD\n");
	}
}

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
void SDCard_Createfile (CHAR *name)
{

	/**** check whether the file exists or not ****/
	if(name == NULL)
		return;

	memset(g_SD_PrintBuf, 0x00, 128);
	
	g_SD_Fresult= f_stat (name, &g_SD_Fileinfo);
	
	if (g_SD_Fresult == FR_OK)
	{
		sprintf (g_SD_PrintBuf, "*%s* already exists!!!!\n", name);
		SDCard_PrintMsg(g_SD_PrintBuf);
	}
	else 
	{
	    g_SD_Fresult = f_open(&g_SD_FIL, name, FA_CREATE_ALWAYS|FA_READ|FA_WRITE);
		
	    if (g_SD_Fresult != FR_OK)
	    {
	    	sprintf (g_SD_PrintBuf, "error no %d in creating file *%s*\n", g_SD_Fresult, name);			
	    	SDCard_PrintMsg(g_SD_PrintBuf);
	    }
	    else
	    {
	    	sprintf (g_SD_PrintBuf, "*%s* created successfully\n",name);
	    	SDCard_PrintMsg(g_SD_PrintBuf);
	    }

		memset(g_SD_PrintBuf, 0x00, 128);
		
	    g_SD_Fresult = f_close(&g_SD_FIL);
		
	    if (g_SD_Fresult != FR_OK)
	    {
	    	sprintf (g_SD_PrintBuf, "error no %d in closing file *%s*\n", g_SD_Fresult, name);
	    	SDCard_PrintMsg(g_SD_PrintBuf);
	    }
	}
}

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
void SDCard_WriteFile (CHAR *filname, CHAR* pData)
{
	
	/**** check whether the file exists or not ****/
	if(filname == NULL || pData == NULL)
		return;

	memset(g_SD_PrintBuf, 0x00, 128);
	
	g_SD_Fresult = f_stat (filname, &g_SD_Fileinfo);
	
	if (g_SD_Fresult != FR_OK)
	{
		sprintf (g_SD_PrintBuf, "*%s* does not exists", filname);
		SDCard_PrintMsg(g_SD_PrintBuf);
	}

	else
	{
	    /* Create a file with read write access and open it */
	    g_SD_Fresult = f_open(&g_SD_FIL, filname, FA_OPEN_EXISTING | FA_WRITE );
		
	    if (g_SD_Fresult != FR_OK)
	    {
	    	sprintf (g_SD_PrintBuf, "Error no %d in opening file *%s*", g_SD_Fresult, filname);			
	    	SDCard_PrintMsg(g_SD_PrintBuf);
	    }

	    /* Writing text */
		memset(g_SD_PrintBuf, 0x00, 128);
		
	    g_SD_Fresult = f_write(&g_SD_FIL, pData, strlen(pData), &g_SD_FileWriteCnt);

	    if (g_SD_Fresult != FR_OK)
	    {
	    	sprintf (g_SD_PrintBuf, "error no %d in writing file *%s*\n", g_SD_Fresult, filname);		
	    	SDCard_PrintMsg(g_SD_PrintBuf);
	    }

	    else
	    {
	    	sprintf (g_SD_PrintBuf, "*%s* written successfully\n", filname);
	    	SDCard_PrintMsg(g_SD_PrintBuf);
	    }

	    /* Close file */
		memset(g_SD_PrintBuf, 0x00, 128);
		
	    g_SD_Fresult = f_close(&g_SD_FIL);
		
	    if (g_SD_Fresult != FR_OK)
	    {
	    	sprintf (g_SD_PrintBuf, "error no %d in closing file *%s*\n", g_SD_Fresult, filname);
	    	SDCard_PrintMsg(g_SD_PrintBuf);
	    }
	}
}

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
void SDCard_SeekWriteFile (CHAR *filname, CHAR* pData, WORD wIndex, WORD wLength)
{
	
	/**** check whether the file exists or not ****/
	if(filname == NULL || pData == NULL)
		return;

	memset(g_SD_PrintBuf, 0x00, 128);
	
	g_SD_Fresult = f_stat (filname, &g_SD_Fileinfo);
	
	if (g_SD_Fresult != FR_OK)
	{
		sprintf (g_SD_PrintBuf, "*%s* does not exists", filname);
		SDCard_PrintMsg(g_SD_PrintBuf);
	}

	else
	{
	    /* Create a file with read write access and open it */
	    g_SD_Fresult = f_open(&g_SD_FIL, filname, FA_OPEN_EXISTING | FA_WRITE );
		
	    if (g_SD_Fresult != FR_OK)
	    {
	    	sprintf (g_SD_PrintBuf, "Error no %d in opening file *%s*", g_SD_Fresult, filname);			
	    	SDCard_PrintMsg(g_SD_PrintBuf);
	    }

	    /* Seeking text */
		memset(g_SD_PrintBuf, 0x00, 128);

		g_SD_Fresult = f_lseek(&g_SD_FIL, wIndex);

		if (g_SD_Fresult != FR_OK)
	    {
	    	sprintf (g_SD_PrintBuf, "error no %d in seeking *%s*\n", g_SD_Fresult, filname);		
	    	SDCard_PrintMsg(g_SD_PrintBuf);
	    }

	    else
	    {
	    	sprintf (g_SD_PrintBuf, "*%s* seek successfully\n", filname);
	    	SDCard_PrintMsg(g_SD_PrintBuf);
	    }
		
		/* Writing text */
		memset(g_SD_PrintBuf, 0x00, 128);

	    g_SD_Fresult = f_write(&g_SD_FIL, pData, wLength, &g_SD_FileWriteCnt);

	    if (g_SD_Fresult != FR_OK)
	    {
	    	sprintf (g_SD_PrintBuf, "error no %d in writing file *%s*\n", g_SD_Fresult, filname);		
	    	SDCard_PrintMsg(g_SD_PrintBuf);
	    }

	    else
	    {
	    	sprintf (g_SD_PrintBuf, "*%s* written successfully\n", filname);
	    	SDCard_PrintMsg(g_SD_PrintBuf);
	    }

	    /* Close file */
		memset(g_SD_PrintBuf, 0x00, 128);
		
	    g_SD_Fresult = f_close(&g_SD_FIL);
		
	    if (g_SD_Fresult != FR_OK)
	    {
	    	sprintf (g_SD_PrintBuf, "error no %d in closing file *%s*\n", g_SD_Fresult, filname);
	    	SDCard_PrintMsg(g_SD_PrintBuf);
	    }
	}
}

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
void SDCard_UpdateFile (CHAR *filname, CHAR* pData)
{
	
	/**** check whether the file exists or not ****/
	if(filname == NULL || pData == NULL)
		return;

	memset(g_SD_PrintBuf, 0x00, 128);
	
	g_SD_Fresult = f_stat (filname, &g_SD_Fileinfo);
	
	if (g_SD_Fresult != FR_OK)
	{
		sprintf (g_SD_PrintBuf, "*%s* does not exists", filname);
		SDCard_PrintMsg(g_SD_PrintBuf);
	}

	else
	{
	     /* Create a file with read write access and open it */
	    g_SD_Fresult = f_open(&g_SD_FIL, filname, FA_OPEN_APPEND | FA_WRITE );
		
	    if (g_SD_Fresult != FR_OK)
	    {
	    	sprintf (g_SD_PrintBuf, "Error no %d in opening file *%s*", g_SD_Fresult, filname);			
	    	SDCard_PrintMsg(g_SD_PrintBuf);
	    }

		memset(g_SD_PrintBuf, 0x00, 128);
		
		g_SD_Fresult = f_lseek(&g_SD_FIL, f_size(&g_SD_FIL));

		if (g_SD_Fresult != FR_OK)
	    {
	    	sprintf (g_SD_PrintBuf, "error no %d in seeking *%s*\n", g_SD_Fresult, filname);		
	    	SDCard_PrintMsg(g_SD_PrintBuf);
	    }

	    else
	    {
	    	sprintf (g_SD_PrintBuf, "*%s* seek successfully\n", filname);
	    	SDCard_PrintMsg(g_SD_PrintBuf);
	    }

	    /* Writing text */
		memset(g_SD_PrintBuf, 0x00, 128);
		
	    g_SD_Fresult = f_write(&g_SD_FIL, pData, strlen(pData), &g_SD_FileWriteCnt);

	    if (g_SD_Fresult != FR_OK)
	    {
	    	sprintf (g_SD_PrintBuf, "error no %d in writing file *%s*\n", g_SD_Fresult, filname);		
	    	SDCard_PrintMsg(g_SD_PrintBuf);
	    }
	    else
	    {
	    	sprintf (g_SD_PrintBuf, "*%s* written successfully\n", filname);
	    	SDCard_PrintMsg(g_SD_PrintBuf);
	    }

	    /* Close file */
		memset(g_SD_PrintBuf, 0x00, 128);
		
	    g_SD_Fresult = f_close(&g_SD_FIL);
		
	    if (g_SD_Fresult != FR_OK)
	    {
	    	sprintf (g_SD_PrintBuf, "error no %d in closing file *%s*\n", g_SD_Fresult, filname);
	    	SDCard_PrintMsg(g_SD_PrintBuf);
	    }
	}
}

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
void SDCard_ReadFile (CHAR *filname, CHAR* pData)
{
		/**** check whether the file exists or not ****/
	if(filname == NULL || pData == NULL)
		return;

	memset(g_SD_PrintBuf, 0x00, 128);
	
	g_SD_Fresult = f_stat (filname, &g_SD_Fileinfo);
	
	if (g_SD_Fresult != FR_OK)
	{
		sprintf (g_SD_PrintBuf, "*%s* does not exists", filname);
		SDCard_PrintMsg(g_SD_PrintBuf);
	}

	else
	{
	     /* Create a file with read write access and open it */
	    g_SD_Fresult = f_open(&g_SD_FIL, filname, FA_READ);
		
	    if (g_SD_Fresult != FR_OK)
	    {
	    	sprintf (g_SD_PrintBuf, "Error no %d in opening file *%s*", g_SD_Fresult, filname);			
	    	SDCard_PrintMsg(g_SD_PrintBuf);
	    }

	    /* Writing text */
		memset(g_SD_PrintBuf, 0x00, 128);
		
	    g_SD_Fresult = f_read(&g_SD_FIL, pData, f_size(&g_SD_FIL), &g_SD_FileReadCnt);

	    if (g_SD_Fresult != FR_OK)
	    {
	    	sprintf (g_SD_PrintBuf, "error no %d in reading file *%s*\n", g_SD_Fresult, filname);		
	    	SDCard_PrintMsg(g_SD_PrintBuf);
	    }
	    else
	    {
	    	sprintf (g_SD_PrintBuf, "*%s* written successfully\n", filname);
	    	SDCard_PrintMsg(g_SD_PrintBuf);
	    }

	    /* Close file */
		memset(g_SD_PrintBuf, 0x00, 128);
		
	    g_SD_Fresult = f_close(&g_SD_FIL);
		
	    if (g_SD_Fresult != FR_OK)
	    {
	    	sprintf (g_SD_PrintBuf, "error no %d in closing file *%s*\n", g_SD_Fresult, filname);
	    	SDCard_PrintMsg(g_SD_PrintBuf);
	    }
	}
}

/********************************************************************
 * Function:		SDCard_SeekReadFile()
 *
 * PreCondition:	None
 *
 * Input:			filename: Name of the file to be write,
 					Data: write data to the file
 					WORD: seek index;
 					WORD: data length;
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Writing the data to SDcard for specific file
 *
 * Note:			None
 *******************************************************************/
void SDCard_SeekReadFile (CHAR *filname, CHAR* pData, WORD wIndex, WORD wLength)
{
		/**** check whether the file exists or not ****/
	if(filname == NULL || pData == NULL)
		return;

	memset(g_SD_PrintBuf, 0x00, 128);
	
	g_SD_Fresult = f_stat (filname, &g_SD_Fileinfo);
	
	if (g_SD_Fresult != FR_OK)
	{
		sprintf (g_SD_PrintBuf, "*%s* does not exists", filname);
		SDCard_PrintMsg(g_SD_PrintBuf);
	}

	else
	{
	     /* Create a file with read write access and open it */
	    g_SD_Fresult = f_open(&g_SD_FIL, filname, FA_READ | FA_WRITE);
		
	    if (g_SD_Fresult != FR_OK)
	    {
	    	sprintf (g_SD_PrintBuf, "Error no %d in opening file *%s*", g_SD_Fresult, filname);			
	    	SDCard_PrintMsg(g_SD_PrintBuf);
	    }

	    /*Seeking Index*/
	    memset(g_SD_PrintBuf, 0x00, 128);

		g_SD_Fresult = f_lseek(&g_SD_FIL, wIndex);

		if (g_SD_Fresult != FR_OK)
	    {
	    	sprintf (g_SD_PrintBuf, "error no %d in seeking *%s*\n", g_SD_Fresult, filname);		
	    	SDCard_PrintMsg(g_SD_PrintBuf);
	    }

	    else
	    {
	    	sprintf (g_SD_PrintBuf, "*%s* seek successfully\n", filname);
	    	SDCard_PrintMsg(g_SD_PrintBuf);
	    }

	    /* Reading text */
		memset(g_SD_PrintBuf, 0x00, 128);
		
	    g_SD_Fresult = f_read(&g_SD_FIL, pData, f_size(&g_SD_FIL), &g_SD_FileReadCnt);

	    if (g_SD_Fresult != FR_OK)
	    {
	    	sprintf (g_SD_PrintBuf, "error no %d in reading file *%s*\n", g_SD_Fresult, filname);		
	    	SDCard_PrintMsg(g_SD_PrintBuf);
	    }
	    else
	    {
	    	sprintf (g_SD_PrintBuf, "*%s* written successfully\n", filname);
	    	SDCard_PrintMsg(g_SD_PrintBuf);
	    }

	    /* Close file */
		memset(g_SD_PrintBuf, 0x00, 128);
		
	    g_SD_Fresult = f_close(&g_SD_FIL);
		
	    if (g_SD_Fresult != FR_OK)
	    {
	    	sprintf (g_SD_PrintBuf, "error no %d in closing file *%s*\n", g_SD_Fresult, filname);
	    	SDCard_PrintMsg(g_SD_PrintBuf);
	    }
	}
}
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
uint32_t SDCard_GetFreeSize (void)
{
	FATFS *pfs;
	DWORD fre_clust;
	uint32_t free_space;

    /* Check free space */
    f_getfree("", &fre_clust, &pfs);
    free_space = (uint32_t)(fre_clust * pfs->csize * 0.5);

    return free_space;
}

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
uint32_t SDCard_GetTotalSize (void)
{
	FATFS *pfs;
	DWORD fre_clust;
	uint32_t dwtotal;

    /* Check free space */
    f_getfree("", &fre_clust, &pfs);
    dwtotal = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);

    return dwtotal;
}

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
void SDCard_Removefile (CHAR *name)
{
	/**** check whether the file exists or not ****/
	memset(g_SD_PrintBuf, 0x00, 256);
	
	g_SD_Fresult = f_stat (name, &g_SD_Fileinfo);

	if (g_SD_Fresult != FR_OK)
	{
		sprintf (g_SD_PrintBuf, "*%s* does not exists", name);
		SDCard_PrintMsg (g_SD_PrintBuf);
	}

	else{

		g_SD_Fresult = f_unlink (name);
		if (g_SD_Fresult == FR_OK)
		{
			sprintf (g_SD_PrintBuf, "*%s* has been removed successfully", name);
			SDCard_PrintMsg (g_SD_PrintBuf);
		}

		else
		{
			sprintf (g_SD_PrintBuf, "error in removing *%s*", name);
			SDCard_PrintMsg (g_SD_PrintBuf);
		}
	}
}

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
)
{
	BYTE yResult = FALSE;
	//DWORD dwIndex;

	if (dwAddress >= 0x1000)
	{
		return;
	}

	if(NULL == g_SDcard_Setting.pConfigurator ||
		TRUE != g_SDcard_Setting.yLoadData)
	{
		yResult = SDCard_AllocateBuffer();

		if(yResult == FALSE)
		{
			return;
		}
	}

}

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
void SDCardif_WriteBufferToMemory(BYTE yResult)
{
	//BYTE yIndex = 0;
	//WORD wAddress;

	if(yResult == TRUE &&
		NULL != g_SDcard_Setting.pConfigurator)
	{
		if (NULL != g_SDcard_Setting.pConfigurator)
		{
			SDCard_SeekWriteFile((char*)&g_SD_FileConfig, (char*)&g_SDcard_Setting.pConfigurator[0], 0, SFLASH_ADDRESS_CON_TEMPERATURE);
		}
	}

	SDCard_ReleaseBuffer();
}

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
void SDCard_ReleaseBuffer(void)
{
	//BYTE yIndex;
	
	if (NULL != g_SDcard_Setting.pConfigurator)
	{
		FREE(g_SDcard_Setting.pConfigurator);
		g_SDcard_Setting.pConfigurator = NULL;
	}

	g_SDcard_Setting.yLoadData = FALSE;
}

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
BYTE SDCard_AllocateBuffer(void)
{
	BYTE yResult = FALSE;

	do
	{
		g_SDcard_Setting.pConfigurator = (PBYTE)MALLOC(SFLASH_LENGTH_TOTAL_CONFIG + 1);

		if(NULL == g_SDcard_Setting.pConfigurator)
		{
			break;
		}

		yResult = TRUE;

	}while(0);

	if(yResult == FALSE)
	{
		SDCard_ReleaseBuffer();
	}
	else
	{
		g_SDcard_Setting.yLoadData = FALSE;
	}

	return yResult;
}
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
void SDCard_PrintMsg (CHAR *pData)
{
	if(g_SDCard_Echo == TRUE)
	{
		USB_PrintMsg(pData);
	}
}

void SDCard_TestWrite (void)
{
	CHAR pData[255];

	memset(pData, 0x00, 256);
	
	SDCard_Mount();
	//SDCard_Createfile("datalog.txt");
	//SDCard_WriteFile("datalog.txt", "Test String");
	SDCard_UpdateFile("datalog.txt", "Test String");
	SDCard_UpdateFile("datalog.txt", "Test String");
	SDCard_UpdateFile("datalog.txt", "Test String");
	SDCard_UpdateFile("datalog.txt", "Test String");
	SDCard_UpdateFile("datalog.txt", "Test String");
	SDCard_UpdateFile("datalog.txt", "Test String");
	SDCard_UpdateFile("datalog.txt", "Test String");
	
	SDCard_ReadFile("datalog.txt", (char*)&pData);

	USB_PrintMsg(pData);
}

