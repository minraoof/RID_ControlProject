/*
 * ADC.c
 *
 *  Created on: 19 Sep 2018
 *      Author: min
 */

//*****************************************************************************
// INCLUDES
//*****************************************************************************
#include "ADC.h"
#include "UI.h"
#include "string.h"
#include "stdlib.h"

//#include "Temperature.h"
#include "Utility.h"
#include "UI.h"
#include "USB_Module.h"
#include "Power.h"
#include "Temperature.h"


//*****************************************************************************
// MACROS
//*****************************************************************************

extern ADC_HandleTypeDef hadc1;
//*****************************************************************************
// VARIABLES
//*****************************************************************************
BYTE g_ADC_Status, g_ADC_SampleTime, g_ADC_SampleFinish = FALSE;
WORD g_ADC_SampleType, g_ADC_SampleFinishType, g_ADC_SampleValue[3];

//*****************************************************************************
// FUNCTION DEFINITION
//*****************************************************************************
/********************************************************************
 * Function:		ADC_Initialize()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Initialize the ADC module. It will setup the analog to digital
 *				convert module for battery and temperature sensor.
 *
 * Note:			None
 *******************************************************************/
void ADC_Initialize(void)
{
	g_ADC_SampleFinish = FALSE;
	ADCif_CloseADC();
}


/********************************************************************
 * Function:		ADCif_CloseADC()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		This function first disables the ADC interrupt and then turns off
 *				the ADC module. The Interrupt Flag bit (ADIF) is also cleared.
 *
 * Note:			None
 *******************************************************************/
void ADCif_CloseADC(void)
{
	g_ADC_Status = ADC_SAMPLE_OFF;
	g_ADC_SampleTime = 0;

	HAL_ADC_Stop_IT(&hadc1);
}


/********************************************************************
 * Function:		ADCif_SampleADC()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		This function will enable the ADC convert process. Set the
 *				input sorce and active the auto sample and wait the interrupt.
 *
 * Note:			None
 *******************************************************************/
BYTE ADCif_SampleADC(WORD wType)
{
	ADC_ChannelConfTypeDef sConfig;
	
	if (g_ADC_Status == ADC_SAMPLE_ON)
	{
		return FALSE;
	}

	ADCif_CloseADC();

	if (wType == ADC_TYPE_BATTERY)
	{	
		sConfig.Channel = ADC_CHANNEL_0;
		sConfig.Rank = ADC_REGULAR_RANK_1;
		sConfig.SamplingTime = ADC_SAMPLETIME_13CYCLES_5;
		HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	}
	else
	{
		return FALSE;
	}

	g_ADC_SampleType = wType;
	g_ADC_SampleTime = 0;

	g_ADC_SampleValue[0] = 0;
	g_ADC_SampleValue[1] = 0;
	g_ADC_SampleValue[2] = 0;

	HAL_ADC_Start_IT(&hadc1);


	g_ADC_Status = ADC_SAMPLE_ON;
	return TRUE;
}


/********************************************************************
 * Function:		ADC_InterruptHandler()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Handle the ADC interrupt. It will pass the sample value to
 *				power or temperature module.
 *
 * Note:			None
 *******************************************************************/
void ADC_InterruptHandler(void)
{

	if (g_ADC_SampleTime < (ADC_SAMPLE_TIME - 1))
	{
		if(g_ADC_SampleType == ADC_TYPE_BATTERY ||
			g_ADC_SampleType == ADC_TYPE_TEMPERATURE)
		{
			g_ADC_SampleValue[g_ADC_SampleTime] = (WORD) HAL_ADC_GetValue(&hadc1);
			HAL_ADC_Start_IT(&hadc1);
		}

		g_ADC_SampleTime++;
	}
	else
	{
		if(g_ADC_SampleType == ADC_TYPE_BATTERY ||
			g_ADC_SampleType == ADC_TYPE_TEMPERATURE)
		{
			g_ADC_SampleValue[ADC_SAMPLE_TIME - 1] = (WORD) HAL_ADC_GetValue(&hadc1);
		}

		g_ADC_SampleFinish = TRUE;
		g_ADC_SampleFinishType = g_ADC_SampleType;

		ADCif_CloseADC();
	}
}

/********************************************************************
 * Function:		ADCif_NewEventHandler()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Handle the new event for ADC sample.
 *
 * Note:			None
 *******************************************************************/
void ADCif_NewEventHandler(void)
{
	CHAR pString[50];

	memset(pString, 0x00, 50);

	if (TRUE == g_ADC_SampleFinish)
	{
		WORD wValue;

		g_ADC_SampleFinish = FALSE;

		wValue = (g_ADC_SampleValue[0] + g_ADC_SampleValue[1] + g_ADC_SampleValue[2]) / 3;
		
		if (g_ADC_SampleFinishType == ADC_TYPE_BATTERY)
		{
			Powerif_SetBatteryValue(wValue);
		}
		else if (g_ADC_SampleFinishType == ADC_TYPE_TEMPERATURE)
		{
			TSif_HandleSampleValue(wValue);
		}
	}
}


/********************************************************************
 * Function:		ADCif_StopModule()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Stop the ADC module
 *
 * Note:			None
 *******************************************************************/
void ADCif_StopModule(void)
{

}


