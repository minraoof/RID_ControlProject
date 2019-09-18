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

#include "stdlib.h"
#include <string.h>
#include "usbd_cdc_if.h"
#include "Utility.h"
#include "UserCmd.h"
#include "Module.h"
#include "usbd_cdc_if.h"

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
BYTE  g_USB_MessageNumber;

USB_STATUS g_USB_State;
USB_MESSAGE_QUEUE *g_USB_MessageHead;
USB_DATA g_USB_Data;

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
		if (USB_EVENT_USER_CMD == g_USB_State.wEvent)
		{
			USB_HandleUserCmdData((char*)buf, yLen);
		}
		else
		{
			USB_ParsingEvent((char*)buf, yLen);
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
	USBif_SetUSBState();

	/*if(g_USB_State.yUSB == FALSE)
	{
		return;
	}*/
	
	if(CDC_ISBUSY() != FALSE)
	{
		return;
	}

	if(g_USB_State.wEvent == USB_EVENT_USER_CMD_PROCESS)
	{
		g_USB_State.wEvent = USB_EVENT_NONE;

		UserCmd_ParsingCmd(
					(char*)g_USB_UserCmd,
					USERCMD_SEND_USB
				);

	}
	else if(g_USB_State.wEvent == USB_EVENT_NONE)
	{
		if (NULL != g_USB_MessageHead)
		{
			USB_TxMessage();
		}
	}


}

/********************************************************************
 * Function:		USBif_SetUSBState()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Set the USB plug in status. It will check the port value.
 *
 * Note:			None
 *******************************************************************/
void USBif_SetUSBState(void)
{

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

	if (pData[0] != 0 &&
		g_USB_State.wEvent == USB_EVENT_NONE)
	{
		g_USB_State.wEvent = USB_EVENT_USER_CMD;
		memset(g_USB_UserCmd, 0x0, USB_LENGTH_USERCMD);
		USB_HandleUserCmdData((char*)pData, yLen);
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
				g_USB_State.wEvent = USB_EVENT_USER_CMD_PROCESS;

				/*UserCmd_ParsingCmd(
					(char*)g_USB_UserCmd,
					USERCMD_SEND_USB
				);*/

				return;
			}
		}
	}
}

