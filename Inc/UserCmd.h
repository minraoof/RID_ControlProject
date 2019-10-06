/**
  ******************************************************************************
  * @file           : UserCmd.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V.
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __USERCMD_H__
#define __USERCMD_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "main.h"
#include "GenericTypeDefs.h"

/* Exported types ------------------------------------------------------------*/
typedef enum
{
	USERCMD_SEND_NONE = 0,

	USERCMD_SEND_USB
}UserCmd_SendWay;


typedef enum
{
	USERCMD_CMD_NONE = 0,
	USERCMD_CMD_LBAT,
	USERCMD_CMD_VER,
	USERCMD_CMD_SYSTEMUPTIME,
  USERCMD_CMD_TIMESTAMP,

	USERCMD_CMD_SDCARDTEST
}USERCMD_CMD_TYPE;


typedef union
{
	BYTE yData[4];
	DWORD dwValue;
}USERCMD_DEBUG_DATA;

#ifdef __cplusplus
}
#endif

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
 //*****************************************************************************
 // FUNCTION DEFINITION
 //*****************************************************************************
 /********************************************************************
  * Function:		UserCmd_ParsingCmd()
  *
  * PreCondition:	None
  *
  * Input:			CHAR* pCmd: The input command
  *
  * Output:		None
  *
  * Side Effects:	None
  *
  * Overview:		Parsing the command.
  *
  * Note:			None
  *******************************************************************/
 BYTE UserCmd_ParsingCmd(
  	CHAR* pCmd,
  	WORD wSend
  );

 /********************************************************************
  * Function:		UserCmd_ParsingATCommand()
  *
  * PreCondition:	None
  *
  * Input:			CHAR* pCmd: The input command
  *
  * Output:		WORD: The command type.
  *
  * Side Effects:	None
  *
  * Overview:		Parsing the AT command and return the command type value.
  *
  * Note:			None
  *******************************************************************/
 WORD UserCmd_ParsingATCommand(CHAR* pCmd);


 /********************************************************************
  * Function:		UserCmd_HandleM3Command()
  *
  * PreCondition:	None
  *
  * Input:			CHAR* pCmd: The input command
  *				WORD wCmd: The command text.
  *
  * Output:		None
  *
  * Side Effects:	None
  *
  * Overview:		Handle the M3 command.
  *
  * Note:			None
  *******************************************************************/
 BYTE UserCmd_HandleATCommand(
 	CHAR* pCmd,
 	WORD wCmd
 );


 /********************************************************************
  * Function:		UserCmd_VersionCommand()
  *
  * PreCondition:	None
  *
  * Input:			CHAR* pCmd: The input command
  *
  * Output:		None
  *
  * Side Effects:	None
  *
  * Overview:		To return the firmware version.
  *
  * Note:			None
  *******************************************************************/
 BYTE UserCmd_VersionCommand(CHAR *pCmd);

/********************************************************************
 * Function:		UserCmd_LBATCommand()
 *
 * PreCondition:	None
 *
 * Input:			CHAR* pCmd: The input command
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		The WBS LBAT function. It can query and set the low battery
 *				threshold.
 *
 * Note:			None
 *******************************************************************/
BYTE UserCmd_LBATCommand(CHAR* pCmd);

/********************************************************************
 * Function:		UserCmd_SystemUptimeCommand()
 *
 * PreCondition:	None
 *
 * Input:			CHAR* pCmd: The input command
 *
 * Output:			BYTE
 *
 * Side Effects:	None
 *
 * Overview:		Handle the Sys up time command
 *
 * Note:			None
 *******************************************************************/
BYTE UserCmd_SystemUptimeCommand(CHAR* pCmd);

/********************************************************************
 * Function:    UserCmd_TimeStampCommand()
 *
 * PreCondition:  None
 *
 * Input:     CHAR* pCmd: The input command
 *
 * Output:      BYTE
 *
 * Side Effects:  None
 *
 * Overview:    Handle the Sys up time command
 *
 * Note:      None
 *******************************************************************/
BYTE UserCmd_TimeStampCommand(CHAR* pCmd);

/********************************************************************
 * Function:		UserCmd_SDCardTestCommand()
 *
 * PreCondition:	None
 *
 * Input:			CHAR* pCmd: The input command
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Test for SDCard.
 *
 * Note:			None
 *******************************************************************/
BYTE UserCmd_SDCardTestCommand(CHAR* pCmd);

#endif /* __USERCMD_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
