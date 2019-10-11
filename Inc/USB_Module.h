/**
  ******************************************************************************
  * @file    USB_Module.h
  * @brief   This file contains the headers of the interrupt handlers.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBMODULE_H__
#define __USBMODULE_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "main.h"
#include "GenericTypeDefs.h"

/* Exported types ------------------------------------------------------------*/

#define USB_MESSAGE_PREFIX						0x02
#define USB_MESSAGE_ID_QUERY					0xC1
#define USB_MESSAGE_ID_WRITE					0xC2
#define USB_MESSAGE_ID_READ					0xC3
#define USB_MESSAGE_ID_RESET					0xC4

 typedef enum
 {
 	USB_EVENT_NONE = 0,

 	USB_EVENT_CONFIGURATOR,

	USB_EVENT_QUERY,
	USB_EVENT_WRITE,
	USB_EVENT_WRITE_PROCESS,
	USB_EVENT_READ,
	
 	USB_EVENT_USER_CMD,
 	USB_EVENT_USER_CMD_PROCESS,
 	USB_EVENT_STOP
 }USB_CONFIG_EVENT;

 typedef enum
 {
 	USB_RESULT_OK = 0,
 	USB_RESULT_ERROR,
 	USB_RESULT_FINISH
 }USB_RESULT_CODE;


 typedef struct
 {
 	BYTE yUSB;
 	DWORD wEvent;
 }USB_STATUS;

 typedef struct
 {
 	BYTE yEvent;
 	CHAR* pData;
 }USB_DATA;


 typedef struct
 {
 	char *pMessage;
 	void *pNext;
 }USB_MESSAGE_QUEUE;

#ifdef __cplusplus
}
#endif

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
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
 void USB_InitializeModule(void);
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
 void USB_PrintMsg(CHAR *pMessage);

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
 void USB_TxMessage(void);
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
 void USB_CDC_ReceiveCallBack(uint8_t *buf, uint8_t len);

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
 void USB_EventHandler(void);

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
 void USB_ParsingEvent(CHAR* pData, BYTE yLen);

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
 void USB_HandleUserCmdData(CHAR *pString, BYTE yLen);
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
void USB_HandleReadConfig(void);

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
);

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
);

#endif /* __USBMODULE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
