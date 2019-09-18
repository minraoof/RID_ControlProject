//*****************************************************************************
// INCLUDES
//*****************************************************************************
#include "Temperature.h"
#include <string.h>

#include "RTC.h"
#include "UI.h"

#include "Utility.h"
#include "Power.h"
#include "USB_Module.h"


//*****************************************************************************
// MACROS
//*****************************************************************************


//*****************************************************************************
// VARIABLES
//*****************************************************************************
TS_STATUS g_TS_Status;
BYTE g_TS_DelayDisplay = 21, g_TS_DissipateCount = 0;


//*****************************************************************************
// FUNCTION DEFINITION
//*****************************************************************************
/********************************************************************
 * Function:		TS_Initialize()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Read the temperature setting.
 *                  
 * Note:			None
 *******************************************************************/
void TS_Initialize(void)
{

}


/********************************************************************
 * Function:		TSif_ReadSettingValue()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Read the temperature setting.
 *                  
 * Note:			None
 *******************************************************************/
void TSif_ReadSettingValue(void)
{

}


/********************************************************************
 * Function:		TSif_HandleSampleValue()
 *
 * PreCondition:	None
 *
 * Input:			WORD wValue: The ADC value for TS.
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Handle the temperature value. It needs to convert to the
 *				normal number.
 *                  
 * Note:			None
 *******************************************************************/
void TSif_HandleSampleValue(WORD wValue)
{	
	CHAR pStr[8];

	memset(pStr, 0x0, 8);
	strcpy(pStr, "TEP:");
	ItoStr(wValue, &pStr[4], 0);
	USB_PrintMsg(pStr);

}


/********************************************************************
 * Function:		TSif_GetModeValue()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		BYTE: The sensor mode
 *
 * Side Effects:	None
 *
 * Overview:		Get the sensor is on or off state.
 *                  
 * Note:			None
 *******************************************************************/
BYTE TSif_GetModeValue(void)
{
	return g_TS_Status.yMode;
}


/********************************************************************
 * Function:		TSif_SetModeValue()
 *
 * PreCondition:	None
 *
 * Input:			BYTE yValue: The sensor mode
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Set the sensor is on or off state.
 *                  
 * Note:			None
 *******************************************************************/
void TSif_SetModeValue(BYTE yValue)
{
	g_TS_Status.yMode = yValue;
}


/********************************************************************
 * Function:		TSif_GetTypeValue()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		BYTE yValue: The degree type
 *
 * Side Effects:	None
 *
 * Overview:		Get the sensor is using which type of degree
 *                  
 * Note:			None
 *******************************************************************/
BYTE TSif_GetTypeValue(void)
{
	return g_TS_Status.yType;
}


/********************************************************************
 * Function:		TSif_GetTypeValue()
 *
 * PreCondition:	None
 *
 * Input:			BYTE yValue: The degree type
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Set the sensor is using which type of degree
 *                  
 * Note:			None
 *******************************************************************/
void TSif_SetTypeValue(BYTE yValue)
{
	g_TS_Status.yType = yValue;
}


/********************************************************************
 * Function:		TSif_SetLimitValue()
 *
 * PreCondition:	None
 *
 * Input:			TS_LIMIT t_Setting: The degree limitation
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Set the degree limitation for alert.
 *                  
 * Note:			None
 *******************************************************************/
void TSif_SetLimitValue(TS_LIMIT t_Setting)
{
	g_TS_Status.yLowLimit = t_Setting.yLow;
	g_TS_Status.yHighLimit = t_Setting.yHigh;
}


/********************************************************************
 * Function:		TSif_GetLimitValue()
 *
 * PreCondition:	None
 *
 * Input:			TS_LIMIT* pSetting: The degree limitation
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Get the degree limitation by puting into the input data buffer.
 *                  
 * Note:			None
 *******************************************************************/
void TSif_GetLimitValue(TS_LIMIT* pSetting)
{
	pSetting->yLow = g_TS_Status.yLowLimit;
	pSetting->yHigh = g_TS_Status.yHighLimit;
}


/********************************************************************
 * Function:		TSif_GetCurrentDegree()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		BYTE: Current degree
 *
 * Side Effects:	None
 *
 * Overview:		Get the current degree.
 *                  
 * Note:			None
 *******************************************************************/
BYTE TSif_GetCurrentDegree(void)
{
	BYTE yResult, yMinus = FALSE, yTemp = 0;

	yResult = g_TS_Status.yCurrentDegree;

	if (g_TS_DelayDisplay == 0)
	{
		yTemp = 4;
	}
	else
	{
		yTemp = (20 - g_TS_DelayDisplay) / 5;
	}

	if (g_TS_Status.yFactor < 128)
	{
		if (g_TS_Status.yFactor < yTemp)
		{
			yTemp = 127 + (yTemp - g_TS_Status.yFactor);
		}
		else if (g_TS_Status.yFactor == yTemp)
		{
			yTemp = 0;
		}
		else
		{
			yTemp = g_TS_Status.yFactor - yTemp;
		}
	}
	else
	{
		yTemp += g_TS_Status.yFactor;
	}

	if (g_TS_Status.yMinusDegree == TRUE)
	{
		if (yTemp > 127)
		{
			yResult += (yTemp - 127);
		}
		else
		{
			if (yTemp > yResult)
			{
				yResult = yTemp - yResult;
				yMinus = FALSE;
			}
			else
			{
				yResult -= yTemp;
			}
		}
	}
	else
	{
		if (yTemp > 127)
		{
			if (yResult < (yTemp - 127))
			{
				yResult = (yTemp - 127) - yResult;
				yMinus = TRUE;
			}
			else
			{
				yResult -= (yTemp - 127);
			}
		}
		else
		{
			yResult += yTemp;
		}
	}

	if (TS_TYPE_CELSIUS == g_TS_Status.yType)
	{
		if (g_TS_Status.yMinusDegree == TRUE ||
			yMinus == TRUE)
		{
			yResult += 32;
		}
		else if (yResult >= 32)
		{
			yResult -= 32;
		}
		else
		{
			yResult = 32 -yResult;
		}
		
		yResult *= 0.56;
	}
	
	return yResult;
}


/********************************************************************
 * Function:		TSif_GetShiftFactor()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		BYTE: Shift factor
 *
 * Side Effects:	None
 *
 * Overview:		Get the shift factor
 *                  
 * Note:			None
 *******************************************************************/
BYTE TSif_GetShiftFactor(void)
{
	return g_TS_Status.yFactor;
}


/********************************************************************
 * Function:		TSif_SetShiftFactor()
 *
 * PreCondition:	None
 *
 * Input:			BYTE yFactor: The shift factor
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Set the shift factor
 *                  
 * Note:			None
 *******************************************************************/
void TSif_SetShiftFactor(BYTE yFactor)
{
	g_TS_Status.yFactor = yFactor;
}


/********************************************************************
 * Function:		TSif_IceTemperature()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		BYTE: Current degree is lower than ice temperature.
 *
 * Side Effects:	None
 *
 * Overview:		Get the result of current degree is lower than ice temperature.
 *                  
 * Note:			None
 *******************************************************************/
BYTE TSif_IceTemperature(void)
{
	BYTE yResult = FALSE;
	
	if (g_TS_Status.yMinusDegree == TRUE)
	{
		yResult = TRUE;;
	}
	else if (g_TS_Status.yType == TS_TYPE_CELSIUS)
	{
		if (g_TS_DelayDisplay == 0)
		{
			if (g_TS_Status.yCurrentDegree < 39)
			{
				yResult = TRUE;
			}
		}
		else
		{
			if (g_TS_Status.yCurrentDegree < 32)
			{
				yResult = TRUE;
			}
		}
	}
	else
	{
		if (g_TS_DelayDisplay == 0)
		{
			if (g_TS_Status.yCurrentDegree < 7)
			{
				yResult = TRUE;
			}
		}
	}

	return yResult;
}


