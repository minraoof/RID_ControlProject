#ifndef POWER_H
#define POWER_H

//*****************************************************************************
// INCLUDE FILES
//*****************************************************************************
//#include "p32mx450f256l.h"
#include "GenericTypeDefs.h"


//*****************************************************************************
// MACROS
//*****************************************************************************


#define POWER_BATTERY_GAUGE_1_4							1738			// Formaula (V*1023)/280, V=(v*100)/2
#define POWER_BATTERY_GAUGE_2_4							685
#define POWER_BATTERY_GAUGE_3_4							700
#define POWER_BATTERY_GAUGE_FULL						713
#define POWER_BATTERY_DRAIN_THRESHOLD					3

#define POWER_BATTERY_4V_ADC							731
#define POWER_BATTERY_39V_ADC							712
#define POWER_BATTERY_34V_ADC							621


//*****************************************************************************
// VARIABLES
//*****************************************************************************


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
void Power_Initialize(void);

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
void Powerif_SetBatteryValue(WORD wValue);

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
void Power_SampleBatteryAlgorithm(void);

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
CHAR* Powerif_GetVoltageValue(void);

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
WORD Powerif_GetLowBatteryThreshold(void);

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
void Powerif_SetLowBatteryThreshold(WORD wValue);

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
BYTE Powerif_IsLowPowerState(void);

#endif


