/*
 * USB_Module.c
 *
 *  Created on: Sep 16, 2018
 *      Author: Min Aung
 */

//*****************************************************************************
// INCLUDE FILES
//*****************************************************************************
//#define USB_IMPORTED
//#define _SUPPRESS_PLIB_WARNING
#include "USB_Module.h"
#include "Version.h"

#include "stdlib.h"
#include <string.h>
#include "usbd_cdc_if.h"
#include "Utility.h"
#include "UserCmd.h"
#include "Module.h"
#include "usbd_cdc_if.h"
#include "SDCard.h"

//*****************************************************************************
// MACROS
//*****************************************************************************
#define USB_LENGTH_RESPONSE					64
#define USB_LENGTH_USERCMD					256


//*****************************************************************************
// VARIABLES
//*****************************************************************************
BYTE g_USB_UserCmd[USB_LENGTH_USERCMD], g_USB_PrintOut[USB_LENGTH_RESPONSE];
WORD g_USB_DataIndex, g_USB_VGTotalSector;		// This is used for receiving data.
DWORD g_USB_TotalLength, g_USB_SectorIndex;
BYTE g_USB_CheckSum[2], g_USB_ChecksumLowBit, g_USB_MessageNumber;

USB_STATUS g_USB_State;
USB_MESSAGE_QUEUE *g_USB_MessageHead;
USB_DATA g_USB_Data;

CHAR pTemData[100];

//*****************************************************************************
// FUNCTION DEFINITION
//*****************************************************************************
/********************************************************************
 * Function:		USB_InitializeModule()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Initialize the USB module
 *
 * Note:			None
 *******************************************************************/
void USB_InitializeModule(void)
{
	//g_USB_State.yUSB = USB_SENSE;
	g_USB_State.wEvent = USB_EVENT_NONE;
	g_USB_MessageHead = NULL;
	g_USB_MessageNumber = 0;

}

/********************************************************************
 * Function:		USB_PrintMsg()
 *
 * PreCondition:	None
 *
 * Input:			CHAR *pMessage: The output message
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Print the message to hyper terminal.
 *
 * Note:			None
 *******************************************************************/
void USB_PrintMsg(CHAR *pMessage)
{
	WORD wLength;
	USB_MESSAGE_QUEUE *pBuffer, *pTemp;

	if (/*g_USB_State.yUSB == FALSE ||*/
		g_USB_State.wEvent != USB_EVENT_NONE)
	{
		return;
	}

	if (NULL == pMessage)
	{
		return;
	}

	wLength = strlen((const char*)pMessage);

	pBuffer = (USB_MESSAGE_QUEUE*)MALLOC(sizeof(USB_MESSAGE_QUEUE));

	if (NULL == pBuffer)
	{
		return;
	}

	pBuffer->pMessage = (CHAR*)MALLOC(wLength + 3);

	if (NULL == pBuffer->pMessage)
	{
		FREE(pBuffer);
		return;
	}

	memset(pBuffer->pMessage, 0x0, wLength + 3);
	strcpy(pBuffer->pMessage, pMessage);

	pBuffer->pMessage[wLength] = 0x0A;
	pBuffer->pMessage[wLength + 1] = 0x0D;

	pBuffer->pNext = NULL;

	if (NULL == g_USB_MessageHead)
	{
		g_USB_MessageHead = pBuffer;
		g_USB_MessageNumber = 1;
	}
	else
	{
		pTemp = g_USB_MessageHead;

		while (NULL != pTemp->pNext)
		{
			pTemp = (USB_MESSAGE_QUEUE*)pTemp->pNext;
		}

		pTemp->pNext = pBuffer;

		g_USB_MessageNumber++;

		if (g_USB_MessageNumber > 15)
		{
			pTemp = g_USB_MessageHead;
			g_USB_MessageHead = (USB_MESSAGE_QUEUE*)g_USB_MessageHead->pNext;

			if (NULL != pTemp)
			{
				if (NULL != pTemp->pMessage)
				{
					FREE(pTemp->pMessage);
				}

				FREE(pTemp);
			}

			g_USB_MessageNumber--;
		}
	}
}

/********************************************************************
 * Function:		USB_TxMessage()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Call the USB Driver function
 *
 * Note:			None
 *******************************************************************/
void USB_TxMessage(void)
{
	BYTE yTemp;
	WORD wLength, wIndex;
	USB_MESSAGE_QUEUE *pTemp;

	if (NULL == g_USB_MessageHead->pMessage)
	{
		pTemp = g_USB_MessageHead;
		g_USB_MessageHead = (USB_MESSAGE_QUEUE*)g_USB_MessageHead ->pNext;

		FREE(pTemp);
		g_USB_MessageNumber--;

		return;
	}

	wLength = strlen(g_USB_MessageHead->pMessage);

	memset(g_USB_PrintOut, 0x0, USB_LENGTH_RESPONSE);

	if (wLength > USB_LENGTH_RESPONSE)
	{
		yTemp = USB_LENGTH_RESPONSE;
	}
	else
	{
		yTemp = wLength;
	}

	for (wIndex = 0; wIndex < yTemp; wIndex++)
	{
		g_USB_PrintOut[wIndex] = g_USB_MessageHead->pMessage[wIndex];
	}

	if (wLength > USB_LENGTH_RESPONSE)
	{
		for (wIndex = USB_LENGTH_RESPONSE; wIndex < wLength; wIndex++)
		{
			g_USB_MessageHead->pMessage[wIndex - USB_LENGTH_RESPONSE] = g_USB_MessageHead->pMessage[wIndex];
		}

		memset(&g_USB_MessageHead->pMessage[wLength - USB_LENGTH_RESPONSE], 0x0, USB_LENGTH_RESPONSE);
		wLength = USB_LENGTH_RESPONSE;
	}
	else
	{
		pTemp = g_USB_MessageHead;
		g_USB_MessageHead = (USB_MESSAGE_QUEUE*)g_USB_MessageHead ->pNext;

		if (NULL != pTemp)
		{
			if (NULL != pTemp->pMessage)
			{
				FREE(pTemp->pMessage);
			}

			FREE(pTemp);
		}

		g_USB_MessageNumber--;
	}

	CDC_Transmit_FS(g_USB_PrintOut, wLength);
}

/********************************************************************
 * Function:		USB_CDC_ReceiveCallBack()
 *
 * PreCondition:	None
 *
 * Input:			uint8_t: Receive Buffer
 * 					uint32_t: Data Length
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Call the USB Driver function
 *
 * Note:			None
 *******************************************************************/
void USB_CDC_ReceiveCallBack(uint8_t *buf, uint8_t yLen)
{
	if (yLen != 0)
	{
		if (yLen != 0)
		{
			if (USB_EVENT_WRITE == g_USB_State.wEvent)
			{
				USB_HandleWriteConfig(
					(char*)buf,
					yLen
				);
			}
			else if (USB_EVENT_USER_CMD == g_USB_State.wEvent)
			{
				USB_HandleUserCmdData((char*)buf, yLen);
			}
			else
			{
				USB_ParsingEvent((char*)buf, yLen);
			}
		}
	}
}

/********************************************************************
 * Function:		USB_EventHandler()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		By different event to go different handler.
 *
 * Note:			None
 *******************************************************************/
void USB_EventHandler(void)
{
	//USBif_SetUSBState();
	
	if(CDC_ISBUSY() != FALSE)
	{
		return;
	}

	if (USB_EVENT_READ == g_USB_State.wEvent)
	{
		USB_HandleReadConfig();
	}
	else if (USB_EVENT_WRITE_PROCESS == g_USB_State.wEvent)
	{
		USB_HandleReadConfig();
	}
	
	else if(g_USB_State.wEvent == USB_EVENT_USER_CMD_PROCESS)
	{
		g_USB_State.wEvent = USB_EVENT_NONE;

		UserCmd_ParsingCmd(
					(char*)g_USB_UserCmd,
					USERCMD_SEND_USB
				);
	}
	else
	{
		if (NULL != g_USB_MessageHead)
		{
			USB_TxMessage();
		}
	}
}

/********************************************************************
 * Function:		USB_ParsingEvent()
 *
 * PreCondition:	None
 *
 * Input:			CHAR* pData: The new receive data
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Parsing the new incoming data when USB is idle.
 *
 * Note:			None
 *******************************************************************/
void USB_ParsingEvent(CHAR* pData, BYTE yLen)
{
	 uint8_t pResponse[20];

	memset(pTemData, 0x00, 100);
	Strncpy(pTemData, pData, yLen);
	
	if(USB_MESSAGE_PREFIX == (BYTE)pData[0])
	{
		if ((USB_MESSAGE_ID_QUERY == (BYTE)pData[4] &&
			0 == (BYTE)pData[1]) ||
			USB_MESSAGE_ID_QUERY == (BYTE)pData[2])
		{
			BYTE yLength, yResult = FALSE;
				
			//g_USB_State.wEvent = USB_EVENT_QUERY;

			pResponse[0] = USB_MESSAGE_PREFIX;				

			if (USB_EVENT_NONE == g_USB_State.wEvent ||
				USB_EVENT_CONFIGURATOR == g_USB_State.wEvent)
			{
				pResponse[1] = 0x0;
				pResponse[2] = 0x0;
				pResponse[3] = 0x6;

				yLength = 4;

				g_USB_State.wEvent = USB_EVENT_CONFIGURATOR;

				yResult = TRUE;
			}

			if (TRUE == yResult)
			{

				pResponse[yLength++] = USB_MESSAGE_ID_QUERY;

				pResponse[yLength++] = VERSION_HARDWARE_NUMBER;
				pResponse[yLength++] = VERSION_MAIN_NUMBER;
				pResponse[yLength++] = VERSION_SUB_NUMBER;
				pResponse[yLength++] = VERSION_SUB_CHAR;

				//if (USB_MESSAGE_ID_QUERY == (BYTE)pData[2])
				{
					if (0xFF == (BYTE)pResponse[yLength])
					{
					pResponse[yLength] = 0;
					}

					yLength++;
				}

				//pResponse[3] = yLength;

				CDC_Transmit_FS(&pResponse[0], yLength);
			}
		}
		else if (USB_MESSAGE_ID_READ == (BYTE)pData[4] &&
			0 == (BYTE)pData[1])
		{
			if (USB_EVENT_CONFIGURATOR == g_USB_State.wEvent)
			{
				g_USB_State.wEvent = USB_EVENT_READ;

				g_USB_DataIndex = 0;
				
				g_USB_CheckSum[0] = 0;
				g_USB_CheckSum[1] = 0;
				g_USB_ChecksumLowBit = 0;
				
				pResponse[0] = USB_MESSAGE_PREFIX;
				pResponse[1] = 0;//(SFLASH_LENGTH_TOTAL_CONFIG >> 16) & 0xff;
				pResponse[2] = (SFLASH_LENGTH_TOTAL_CONFIG  >> 8) & 0xff;
				pResponse[3] = SFLASH_LENGTH_TOTAL_CONFIG  & 0xff;
				pResponse[4] = USB_MESSAGE_ID_READ;

				CDC_Transmit_FS(&pResponse[0], 5);
			}
		}
		else if (USB_MESSAGE_ID_WRITE == (BYTE)pData[4] &&
			0 == (BYTE)pData[1])
		{
			if (USB_EVENT_CONFIGURATOR == g_USB_State.wEvent)
			{
				g_USB_State.wEvent = USB_EVENT_WRITE;

				g_USB_TotalLength = (BYTE)pData[1];
				g_USB_TotalLength <<= 8;
				g_USB_TotalLength += (BYTE)pData[2];
				g_USB_TotalLength <<= 8;
				g_USB_TotalLength += (BYTE)pData[3];
				//g_USB_TotalLength -= 2;		// The last 2 bytes is checksum so no need to write

				g_USB_DataIndex = 0;

				g_USB_CheckSum[0] = 0;
				g_USB_CheckSum[1] = 0;
				g_USB_ChecksumLowBit = 0;

				SDCardif_WriteDataToBuffer(
					(PBYTE)&pData[5],
					g_USB_DataIndex,
					28
				);

				USB_CalculateChecksum(
					&pData[5],
					28
				);
				
				//g_USB_DataIndex += 28;
				g_USB_State.wEvent = USB_EVENT_WRITE;
			}
		}
		else if (USB_MESSAGE_ID_RESET == (BYTE)pData[4] &&
			0 == (BYTE)pData[1])
		{
			// The disconnect signal
			//Reset();
			g_USB_State.wEvent = USB_EVENT_NONE;
		}
	}
	else
	{
		if (pData[0] != 0 &&
			g_USB_State.wEvent == USB_EVENT_NONE)
		{
			g_USB_State.wEvent = USB_EVENT_USER_CMD;
			memset(g_USB_UserCmd, 0x0, USB_LENGTH_USERCMD);
			USB_HandleUserCmdData((char*)pData, yLen);
		}
	}
}

/********************************************************************
 * Function:		USB_HandleUserCmdData()
 *
 * PreCondition:	None
 *
 * Input:			CHAR *pString: The data for user command.
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		To handle the user command from hyper terminal.
 *
 * Note:			None
 *******************************************************************/
void USB_HandleUserCmdData(CHAR *pString, BYTE yLen)
{
	BYTE yIndex, yLength, yTotal;
	uint8_t pTemp[4];

	yTotal = strlen((const char*)g_USB_UserCmd);
	yLength = yLen;

	memset(pTemp, 0x0, 4);

	for (yIndex = 0; yIndex < yLength; yIndex++)
	{
		if (0x08 == pString[yIndex])
		{
			if (yTotal > 0)
			{
				pTemp[0] = 0x08;
				pTemp[1] = 0x20;
				pTemp[2] = 0x08;

				CDC_Transmit_FS(pTemp, 3);

				g_USB_UserCmd[yTotal - 1] = 0;
				yTotal--;
			}
		}
		else
		{
			if (0x0D != pString[yIndex])
			{
				CDC_Transmit_FS((uint8_t*)&pString[yIndex], 1);
			}
			else
			{
				pTemp[0] = 0x0A;
				pTemp[1] = 0x0D;
				pTemp[2] = 0x00;

				CDC_Transmit_FS(pTemp, 2);
			}


			if (0x0D != pString[yIndex])
			{
				g_USB_UserCmd[yTotal] = pString[yIndex];
				yTotal++;

				if(yTotal == 255)					// to avoid string overflow
				{
					memset(g_USB_UserCmd, 0x00, 256);
				}
			}
			else
			{
				/*g_USB_State.wEvent = USB_EVENT_NONE;

				UserCmd_ParsingCmd(
							(char*)g_USB_UserCmd,
							USERCMD_SEND_USB
						);*/
				g_USB_State.wEvent = USB_EVENT_USER_CMD_PROCESS;

				
				return;
			}
		}
	}
}
/********************************************************************
 * Function:		USB_HandleReadConfig()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Read all the config in flash memory and send to the terminal.
 *
 * Note:			None
 *******************************************************************/
void USB_HandleReadConfig(void)
{
	CHAR pData[USB_LENGTH_RESPONSE + 2];
	BYTE yLength;

	memset(pData, 0x0, USB_LENGTH_RESPONSE + 2);
	
	if ((SFLASH_LENGTH_TOTAL_CONFIG - g_USB_DataIndex) < USB_LENGTH_RESPONSE)
	{
		yLength = SFLASH_LENGTH_TOTAL_CONFIG - g_USB_DataIndex;
	}
	else
	{
		yLength = USB_LENGTH_RESPONSE;
	}

	if (yLength != 0)
	{
		/*SFlash_ReadData(
			(PBYTE)pData,
			(DWORD)g_USB_DataIndex,
			(WORD)yLength
		);*/

		USB_CalculateChecksum(
			pData,
			yLength
		);
	}

	if ((SFLASH_LENGTH_TOTAL_CONFIG - g_USB_DataIndex) < USB_LENGTH_RESPONSE)
	{
		g_USB_DataIndex = 0;
		g_USB_State.wEvent = USB_EVENT_NONE;
	}
	else
	{
		g_USB_DataIndex += yLength;
	}
	
	CDC_Transmit_FS((uint8_t*)pData, yLength);
}

/********************************************************************
 * Function:		USB_HandleWriteConfig()
 *
 * PreCondition:	None
 *
 * Input:			CHAR* pString: The input config data value
 *				BYTE yLength: The length of data.
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Get the data and write into the flash. It will compare with
 *				the checksum value in the end of data.
 *
 * Note:			None
 *******************************************************************/
void USB_HandleWriteConfig(
	CHAR* pString,
	BYTE yLength
)
{
	if ((g_USB_TotalLength - g_USB_DataIndex) < USB_LENGTH_RESPONSE)
	{
		BYTE yTemp = g_USB_TotalLength - g_USB_DataIndex;

		if (yTemp != 0)
		{
			/*SFlashif_WriteDataToBuffer(
				(PBYTE)pString,
				g_USB_DataIndex,
				yTemp
			);

			g_USB_DataIndex = 0;
			g_USB_TotalLength = SFLASH_LENGTH_HTTP_SERVER * 2;
			g_USB_State.wEvent = USB_EVENT_WRITE_MDM_LINK;

			memset(g_USB_UserCmd, 0x0, USB_LENGTH_USERCMD);
			USB_HandleWriteMDMServerLink(
				&pString[yTemp],
				yLength - yTemp
			);*/
		}
	}
	else
	{
		SDCardif_WriteDataToBuffer(
			(PBYTE)pString,
			g_USB_DataIndex,
			yLength
		);

		USB_CalculateChecksum(
			pString,
			yLength
		);
		
		g_USB_DataIndex += yLength;
	}
}

/********************************************************************
 * Function:		USB_CalculateChecksum()
 *
 * PreCondition:	None
 *
 * Input:			CHAR *pString: The input data
 *				BYTE yLength: The length of input data
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Get the input data's checksum.
 *
 * Note:			None
 *******************************************************************/
void USB_CalculateChecksum(
	CHAR *pString,
	BYTE yLength
)
{
	BYTE yIndex;

	for (yIndex = 0; yIndex < yLength; yIndex++)
	{
		if ((yIndex + g_USB_ChecksumLowBit) % 2 == 0)
		{
			if (0xff - g_USB_CheckSum[0] < (BYTE)pString[yIndex])
			{
				g_USB_CheckSum[1]++;
			}
		}

		g_USB_CheckSum[(yIndex + g_USB_ChecksumLowBit) % 2] += (BYTE)pString[yIndex];
	}

	g_USB_ChecksumLowBit = (g_USB_ChecksumLowBit + yLength) % 2;
}


