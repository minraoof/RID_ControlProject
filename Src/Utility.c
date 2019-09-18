//*****************************************************************************
// INCLUDE FILES
//*****************************************************************************
#include "Utility.h"
#include "string.h"
#include "stdlib.h"

//#include "Main.h"



//*****************************************************************************
// MACROS
//*****************************************************************************


//*****************************************************************************
// VARIABLES
//*****************************************************************************
DWORD g_Mem_Allocated = 0, g_Mem_MaxAllocated = 0;
DWORD g_Mem_MaximumTime = 0;


//*****************************************************************************
// FUNCTIONS
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
void DelayMS(WORD wTime)
{
	HAL_Delay(wTime);
}

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
void DelayUS(WORD wTime)
{
	wTime *= (SystemCoreClock / 1000000) / 5;
	/* Wait till done */
	while (wTime--);
}


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
BYTE Convert(BYTE yData)
{
	BYTE yBuffer = 0, yIndex;

	for (yIndex = 0; yIndex < 8; yIndex++)
	{
		yBuffer <<= 1;
		yBuffer |= (yData & 0x1);
		yData >>= 1;
	}

	return yBuffer;
}


/********************************************************************
 * Function:		ItoStr()
 *
 * PreCondition:	None
 *
 * Input:			DWORD dwValue: The integer number
 *				CHAR sString[]: The string want to save result
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
)
{
	BYTE yLength = 0, yTotal;
	DWORD dwTemp = dwValue;

	if (0 == dwValue)
	{
		if (ySize == 0)
		{
			sString[0] = '0';
			sString[1] = 0;
		}
		else
		{
			for (yLength = 0; yLength < ySize; yLength++)
			{
				sString[yLength] = '0';
			}

			sString[yLength] = 0;
		}

		return;
	}

	if (ySize == 0)
	{
		while (dwTemp != 0)
		{
			yLength++;
			dwTemp /= 10;
		}
	}
	else
	{
		yLength = ySize;
	}

	yTotal = yLength;
	dwTemp = dwValue;

	for (; yLength > 0; yLength--)
	{
		if (dwTemp != 0)
		{
			sString[yLength - 1] = '0' + (dwTemp % 10);
			dwTemp /= 10;
		}
		else
		{
			sString[yLength - 1] = '0';
		}
	}

	sString[yTotal] = 0;
}


/********************************************************************
 * Function:		HtoStr()
 *
 * PreCondition:	None
 *
 * Input:			DWORD dwValue: The integer number
 *				CHAR* pString: The string want to save result
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
)
{
	BYTE yLength = 0, yTemp, yTotal;
	DWORD dwTemp = dwValue;

	if (0 == dwValue)
	{
		if (ySize == 0)
		{
			pString[0] = '0';
			pString[1] = 0;
		}
		else
		{
			for (yTemp = 0; yTemp < ySize; yTemp++)
			{
				pString[yTemp] = '0';
			}

			pString[yTemp] = 0;
		}

		return;
	}

	if (ySize == 0)
	{
		while (dwTemp != 0)
		{
			yLength++;
			dwTemp >>= 4;
		}
	}
	else
	{
		yLength = ySize;
	}

	yTotal = yLength;
	dwTemp = dwValue;

	for (; yLength > 0; yLength--)
	{
		if (dwTemp != 0)
		{
			yTemp = (dwTemp & 0x0F);

			if (yTemp < 10)
			{
				pString[yLength - 1] = '0' + yTemp;
			}
			else
			{
				pString[yLength - 1] = 'A' + yTemp - 10;
			}

			dwTemp >>= 4;
		}
		else
		{
			pString[yLength - 1] = '0';
		}
	}

	pString[yTotal] = 0;
}


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
)
{
	BYTE yLength = 0, yTemp, yTotal;
	DWORD dwTemp = dwValue;

	if (0 == dwValue)
	{
		if (ySize == 0)
		{
			pString[0] = '0';
			pString[1] = 0;
		}
		else
		{
			for (yTemp = 0; yTemp < ySize; yTemp++)
			{
				pString[yTemp] = '0';
			}

			pString[yTemp] = 0;
		}

		return;
	}

	if (ySize == 0)
	{
		while (dwTemp != 0)
		{
			yLength++;
			dwTemp >>= 1;
		}
	}
	else
	{
		yLength = ySize;
	}

	yTotal = yLength;
	dwTemp = dwValue;

	for (; yLength > 0; yLength--)
	{
		if (dwTemp != 0)
		{
			yTemp = (dwTemp % 2);

			pString[yLength - 1] = '0' + yTemp;
			dwTemp >>= 1;
		}
		else
		{
			pString[yLength - 1] = '0';
		}
	}

	pString[yTotal] = 0;
}


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
DWORD StoI(CHAR *pString)
{
	BYTE yIndex = 0;
	DWORD dwValue = 0;

	while (pString[yIndex] != 0)
	{
		dwValue *= 10;

		if (pString[yIndex] >= '0' &&
			pString[yIndex] <= '9')
		{
			dwValue += (pString[yIndex] - '0');
		}
		else
		{
			return 0;
		}

		yIndex++;
	}

	return dwValue;
}


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
)
{
	BYTE yIndex = 0;
	DWORD dwValue = 0;

	while (pString[yIndex] != 0)
	{
		dwValue <<= 4;

		if (pString[yIndex] >= '0' &&
			pString[yIndex] <= '9')
		{
			dwValue += (pString[yIndex] - '0');
		}
		else if (pString[yIndex] >= 'A' &&
			pString[yIndex] <= 'F')
		{
			dwValue += (pString[yIndex] - 'A' + 0x0A);
		}
		else if (pString[yIndex] >= 'a' &&
			pString[yIndex] <= 'f')
		{
			dwValue += (pString[yIndex] - 'a' + 0x0A);
		}
		else
		{
			return 0;
		}

		yIndex++;

		if (yIndex == ySize)
		{
			break;
		}
	}

	return dwValue;
}


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
 * Overview:		To lower case a string.
 *
 * Note:			None
 *******************************************************************/
void StrLowerCase(
	CHAR *pString,
	WORD wTotal
)
{
	WORD wIndex;
	WORD wLength;

	if (NULL == pString)
	{
		return;
	}

	if (0 == wTotal)
	{
		wLength = strlen((const char*)pString);
	}
	else
	{
		if (wTotal > strlen((const char*)pString))
		{
			wLength = strlen((const char*)pString);
		}
		else
		{
			wLength = wTotal;
		}
	}

	for (wIndex = 0; wIndex < wLength; wIndex++)
	{
		if (pString[wIndex] >=0x41 &&
			pString[wIndex] <= 0x5A)
		{
			pString[wIndex] += 0x20;
		}
	}
}


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
)
{
	WORD wIndex;
	WORD wLength;

	if (NULL == pString)
	{
		return;
	}

	if (0 == wTotal)
	{
		wLength = strlen((const char*)pString);
	}
	else
	{
		if (wTotal > strlen((const char*)pString))
		{
			wLength = strlen((const char*)pString);
		}
		else
		{
			wLength = wTotal;
		}
	}

	for (wIndex = 0; wIndex < wLength; wIndex++)
	{
		if (pString[wIndex] >=0x61 &&
			pString[wIndex] <= 0x7A)
		{
			pString[wIndex] -= 0x20;
		}
	}
}


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
)
{
	BYTE yIndex;

	if (NULL == sString1 ||
		NULL == sString2)
	{
		return 1;
	}

	for (yIndex = 0; yIndex < yLength; yIndex++)
	{
		if (sString1[yIndex] != sString2[yIndex])
		{
			return 1;
		}

		if (sString1[yIndex] == 0)
		{
			return 0;
		}
	}

	return 0;
}


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
)
{
	BYTE yIndex = 0;

	if (NULL == pDst ||
		NULL == pSource)
	{
		return;
	}

	for (yIndex = 0; yIndex < yLength; yIndex++)
	{
		pDst[yIndex] = pSource[yIndex];
	}
}

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
void* MALLOC(WORD wSize)
{
	void* pData = NULL;

	if (wSize == 0)
	{
		return NULL;
	}

	pData = malloc(wSize);


	return pData;
}


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
void FREE(void* pData)
{
	free(pData);
}


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
WORD Memif_GetHeapStatus(BYTE yIndex)
{
	if (yIndex == 0)
	{
		return g_Mem_Allocated;
	}
	else if (yIndex == 1)
	{
		return g_Mem_MaxAllocated;
	}

	return 0;
}


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
DWORD Memif_GetMaxHeapTime(void)
{
	return g_Mem_MaximumTime;
}



