//*****************************************************************************
// INCLUDES
//*****************************************************************************
#include "Power.h"
#include <stdlib.h>
#include <string.h>

#include "ADC.h"
#include "UI.h"
#include "Module.h"
#include "RTC.h"
#include "Utility.h"
#include "Power.h"
#include "USB_Module.h"

//*****************************************************************************
// MACROS
//*****************************************************************************


//*****************************************************************************
// VARIABLES
//*****************************************************************************
BYTE g_Power_ACStatus, g_Power_BatterySample, g_Power_SampleCount, g_Power_ShutDown;
WORD g_Power_CurrentVoltage, g_Power_LowBatteryThreshold, g_Power_TempBattery[3];


//*****************************************************************************
// FUNCTION DEFINITION
//*****************************************************************************
/********************************************************************
 * Function:		Power_Initialize()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Initialize the power module.
 *                  
 * Note:			None
 *******************************************************************/
void Power_Initialize(void)
{
	g_Power_SampleCount = 0;
	g_Power_LowBatteryThreshold = POWER_BATTERY_GAUGE_1_4;

	RTCif_SetBatterySampleTime(2);
}

/********************************************************************
 * Function:		Powerif_SetBatteryValue()
 *
 * PreCondition:	None
 *
 * Input:			WORD wValue: The ADC value sample by MCU.
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Set the ADC value. If this is called during initialization,
 *				it will not run the battery sample algorithm.
 *                  
 * Note:			None
 *******************************************************************/
void Powerif_SetBatteryValue(WORD wValue)
{
	g_Power_TempBattery[g_Power_SampleCount] = wValue;
	g_Power_SampleCount++;

	if (g_Power_SampleCount >= 3)
	{
		Power_SampleBatteryAlgorithm();
		g_Power_SampleCount = 0;

		Module_AddSignal(
			MODULE_UI,
			UI_EVENT_BATTERY_HANDLER,
			NULL
		);
	}
	else
	{
		ADCif_SampleADC(ADC_TYPE_BATTERY);
	}	
}

/********************************************************************
 * Function:		Power_SampleBatteryAlgorithm()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Calculate the voltage. With the three sample value to get
 *				the more accurate value.
 *                  
 * Note:			None
 *******************************************************************/
void Power_SampleBatteryAlgorithm(void)
{

	WORD wVoltage = 0;
	CHAR pString[15];

	memset(pString, 0x00, 15);
	
	g_Power_CurrentVoltage = (g_Power_TempBattery[0] + g_Power_TempBattery[1] + g_Power_TempBattery[2]) / 3;

	//Print Voltage
	wVoltage = g_Power_CurrentVoltage * 0.806;

	if ((wVoltage % 10) >= 5)
	{
		wVoltage = (wVoltage / 10) + 1;
	}
	else
	{
		wVoltage /= 10;
	}

	strcpy(pString, "Volt = ");

	pString[0] = (wVoltage / 100) + '0';
	pString[1] = '.';

	ItoStr(
		(wVoltage % 100),
		&pString[2],
		0
	);

	strcat(pString, "V");
	USB_PrintMsg(pString);
}
/********************************************************************
 * Function:		Powerif_GetVoltageValue()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		CHAR*: The valtage value sense by MCU
 *
 * Side Effects:	None
 *
 * Overview:		Get the voltage value.
 *                  
 * Note:			None
 *******************************************************************/
CHAR* Powerif_GetVoltageValue(void)
{
	CHAR *pValue = NULL;
	WORD wTemp = 0;
	DWORD dwTemp = g_Power_CurrentVoltage * 806;

	dwTemp /= 1000;
	
	if ((dwTemp % 10) >= 5)
	{
		wTemp = (dwTemp / 10) + 1;
	}
	else
	{
		wTemp = dwTemp / 10;
	}
	
	pValue = (CHAR*)MALLOC(5);

	if (NULL != pValue)
	{
		memset(pValue, 0x0, 5);

		ItoStr(wTemp / 100, pValue, 0);
		strcat(pValue, ".");
		ItoStr(wTemp % 100, &pValue[strlen(pValue)], 2);
	}
	
	return pValue;
}


/********************************************************************
 * Function:		Powerif_GetLowBatteryThreshold()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		WORD: The low battery threshold.
 *
 * Side Effects:	None
 *
 * Overview:		Get the low battery threshold.
 *                  
 * Note:			None
 *******************************************************************/
WORD Powerif_GetLowBatteryThreshold(void)
{
	WORD wTemp = g_Power_LowBatteryThreshold * 0.806;

	if ((wTemp % 10) >= 5)
	{
		wTemp = (wTemp / 10) + 1;
	}
	else
	{
		wTemp /= 10;
	}
	
	return wTemp;
}


/********************************************************************
 * Function:		Powerif_SetLowBatteryThreshold()
 *
 * PreCondition:	None
 *
 * Input:			WORD: The low battery threshold.
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Set the low battery threshold.
 *                  
 * Note:			None
 *******************************************************************/
void Powerif_SetLowBatteryThreshold(WORD wValue)
{
	g_Power_LowBatteryThreshold = wValue;
}

/********************************************************************
 * Function:		Powerif_IsLowPowerState()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		BYTE: The status for low battery.
 *
 * Side Effects:	None
 *
 * Overview:		Check the power module is in the low power state or not.
 *                  
 * Note:			None
 *******************************************************************/
BYTE Powerif_IsLowPowerState(void)
{
	if (g_Power_CurrentVoltage <= g_Power_LowBatteryThreshold)
	{
		return TRUE;
	}

	return FALSE;
}

