/**
  ******************************************************************************
  * @file    Utility.h
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
#ifndef __UTILITY_H__
#define __UTILITY_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "GenericTypeDefs.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define MEM_MAX_HEAP 						8192

/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */


#ifdef __cplusplus
}
#endif

 //*****************************************************************************
 // FUNCTION DEFINITION
 //*****************************************************************************
 /********************************************************************
  * Function:		DelayMS()
  *
  * PreCondition:	None
  *
  * Input:			WORD	wTime
  *
  * Output:		None
  *
  * Side Effects:	None
  *
  * Overview:		Make the device idle with the input time
  *
  * Note:			None
  *******************************************************************/
 void DelayMS(WORD wTime);


 /********************************************************************
  * Function:		DelayUS()
  *
  * PreCondition:	None
  *
  * Input:			WORD	wTime
  *
  * Output:		None
  *
  * Side Effects:	None
  *
  * Overview:		Make the device idle with the input time
  *
  * Note:			None
  *******************************************************************/
 void DelayUS(WORD wTime);


 /********************************************************************
  * Function:		Convert()
  *
  * PreCondition:	None
  *
  * Input:			BYTE yData: The data want to convert the bit order
  *
  * Output:		BYTE: The converted data
  *
  * Side Effects:	None
  *
  * Overview:		Convert a data byte's bit order
  *
  * Note:			None
  *******************************************************************/
 BYTE Convert(BYTE yData);


 /********************************************************************
  * Function:		ItoStr()
  *
  * PreCondition:	None
  *
  * Input:			DWORD dwValue: The integer number
  *				CHAR sString[]: The string want to save result
  *				BYTE ySize: The result length. Not equal 0 means may fill with
  *							0 or cut the string.
  *
  * Output:		None
  *
  * Side Effects:	None
  *
  * Overview:		Convert an integer in to a text string in decimal.
  *
  * Note:			None
  *******************************************************************/
 void ItoStr(
 	DWORD dwValue,
 	CHAR sString[],
 	BYTE ySize
 );


 /********************************************************************
  * Function:		HtoStr()
  *
  * PreCondition:	None
  *
  * Input:			DWORD dwValue: The integer number
  *				CHAR* pString: The string want to save result
  *				BYTE ySize: The result length. Not equal 0 means may fill with
  *							0 or cut the string.
  *
  * Output:		None
  *
  * Side Effects:	None
  *
  * Overview:		Convert an integer in to a text string in heximal.
  *
  * Note:			None
  *******************************************************************/
 void HtoStr(
 	DWORD dwValue,
 	CHAR* pString,
 	BYTE ySize
 );


 /********************************************************************
  * Function:		BtoStr()
  *
  * PreCondition:	None
  *
  * Input:			DWORD dwValue: The integer number
  *				CHAR* pString: The string want to save result
  *				BYTE ySize: The result string length. 0 means variable
  *
  * Output:		None
  *
  * Side Effects:	None
  *
  * Overview:		Convert an integer in to a text string in binary format.
  *
  * Note:			None
  *******************************************************************/
 void BtoStr(
 	DWORD dwValue,
 	CHAR* pString,
 	BYTE ySize
 );


 /********************************************************************
  * Function:		StoI()
  *
  * PreCondition:	None
  *
  * Input:			CHAR* pString: The string want to be converted
  *
  * Output:		DWORD: The converted value
  *
  * Side Effects:	None
  *
  * Overview:		Convert the decimal text string to the real value.
  *
  * Note:			None
  *******************************************************************/
 DWORD StoI(CHAR *pString);


 /********************************************************************
  * Function:		StoH()
  *
  * PreCondition:	None
  *
  * Input:			CHAR* pString: The string want to be converted
  *				BYTE ySize: The length of string needs to translate
  *
  * Output:		DWORD: The converted value
  *
  * Side Effects:	None
  *
  * Overview:		Convert the heximal text string to the real value.
  *
  * Note:			None
  *******************************************************************/
 DWORD StoH(
 	CHAR *pString,
 	BYTE ySize
 );


 /********************************************************************
  * Function:		StrLowerCase()
  *
  * PreCondition:	None
  *
  * Input:			CHAR pString[]: The string want to be lowercase.
  *				WORD wTotal: The specified length that need to lower.
  *
  * Output:		None
  *
  * Side Effects:	None
  *
  * Overview:		To lowecase a string.
  *
  * Note:			None
  *******************************************************************/
 void StrLowerCase(
 	CHAR* pString,
 	WORD wTotal
 );


 /********************************************************************
  * Function:		StrUpperCase()
  *
  * PreCondition:	None
  *
  * Input:			CHAR pString[]: The string want to be uppercase.
  *				WORD wTotal: The specified length that need to upper.
  *
  * Output:		None
  *
  * Side Effects:	None
  *
  * Overview:		To lowecase a string.
  *
  * Note:			None
  *******************************************************************/
 void StrUpperCase(
 	CHAR *pString,
 	WORD wTotal
 );


 /********************************************************************
  * Function:		StrCmpN()
  *
  * PreCondition:	None
  *
  * Input:			CHAR *sString1: The first string
  *				CHAR *sString2: The second string
  *				BYTE yLength: The specify length need to compare
  *
  * Output:		BYTE: The string compare result
  *
  * Side Effects:	None
  *
  * Overview:		To compare two string with specify length. If the two strings are
  *				same, it will return 0. Else will return 1.
  *
  * Note:			None
  *******************************************************************/
 BYTE StrCmpN(
 	CHAR *sString1,
 	CHAR *sString2,
 	BYTE yLength
 );


 /********************************************************************
  * Function:		Strncpy()
  *
  * PreCondition:	None
  *
  * Input:			CHAR* pDst: The destination string
  *				CHAR* pSource: The source string
  *				BYTE yLength: The specify length need to copy
  *
  * Output:		None
  *
  * Side Effects:	None
  *
  * Overview:		Copy the specified number of character to the other string
  *
  * Note:			None
  *******************************************************************/
 void Strncpy(
 	CHAR* pDst,
 	CHAR* pSource,
 	BYTE yLength
 );

 /********************************************************************
  * Function:		MALLOC()
  *
  * PreCondition:	None
  *
  * Input:			WORD wSize: The size want to be allocated
  *
  * Output:		void*: The pointer of the new allocated space
  *
  * Side Effects:	None
  *
  * Overview:		Allocate and counte the heap usage.
  *
  * Note:			None
  *******************************************************************/
 void* MALLOC(WORD wSize);


 /********************************************************************
  * Function:		FREE()
  *
  * PreCondition:	None
  *
  * Input:			void* pData: The pointer that want to be free
  *
  * Output:		None
  *
  * Side Effects:	None
  *
  * Overview:		Release the heap and decrease the count
  *
  * Note:			None
  *******************************************************************/
 void FREE(void* pData);


 /********************************************************************
  * Function:		Memif_GetHeapStatus()
  *
  * PreCondition:	None
  *
  * Input:			BYTE yIndex: The heap data index
  *
  * Output:		WORD: The heap value
  *
  * Side Effects:	None
  *
  * Overview:		Get the heap's information
  *
  * Note:			None
  *******************************************************************/
 WORD Memif_GetHeapStatus(BYTE yIndex);


 /********************************************************************
  * Function:		Memif_GetMaxHeapTime()
  *
  * PreCondition:	None
  *
  * Input:			None
  *
  * Output:		DWORD: The time stamp
  *
  * Side Effects:	None
  *
  * Overview:		Get the time stamp of heap's maximum usage.
  *
  * Note:			None
  *******************************************************************/
 DWORD Memif_GetMaxHeapTime(void);


#endif /* __STM32F4xx_IT_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
