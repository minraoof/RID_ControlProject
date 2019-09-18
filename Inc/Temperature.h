#ifndef TEMPERATURE_H
#define TEMPERATURE_H

//*****************************************************************************
// INCLUDE FILES
//*****************************************************************************
//#include "p32mx450f256l.h"
#include "GenericTypeDefs.h"


//*****************************************************************************
// MACROS
//*****************************************************************************


//*****************************************************************************
// VARIABLES
//*****************************************************************************
typedef enum
{
	TS_TYPE_FAHRENHEIT = 0,
	TS_TYPE_CELSIUS
}TS_TYPE;


typedef struct
{
	BYTE yMode;
	BYTE yType;
	BYTE yCurrentDegree;
	BYTE yMinusDegree;

	BYTE yOutOfRange;

	BYTE yLowLimit;
	BYTE yHighLimit;
	BYTE yFactor;
}TS_STATUS;


typedef struct
{
	BYTE yLow;
	BYTE yHigh;
}TS_LIMIT;


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
void TS_Initialize(void);


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
void TSif_ReadSettingValue(void);


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
void TSif_HandleSampleValue(WORD wValue);


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
BYTE TSif_GetModeValue(void);


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
void TSif_SetModeValue(BYTE yValue);


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
BYTE TSif_GetTypeValue(void);


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
void TSif_SetTypeValue(BYTE yValue);


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
void TSif_SetLimitValue(TS_LIMIT t_Setting);


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
 * Overview:		Getthe degree limitation by puting into the input data buffer.
 *                  
 * Note:			None
 *******************************************************************/
void TSif_GetLimitValue(TS_LIMIT* pSetting);


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
BYTE TSif_GetCurrentDegree(void);


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
BYTE TSif_GetShiftFactor(void);


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
void TSif_SetShiftFactor(BYTE yFactor);


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
BYTE TSif_IceTemperature(void);


#endif

