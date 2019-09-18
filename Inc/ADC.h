/*
 * ADC.h
 *
 *  Created on: 19 Sep 2018
 *      Author: min
 */

#ifndef ADC_H_
#define ADC_H_
//*****************************************************************************
// INCLUDE FILES
//*****************************************************************************
#include "GenericTypeDefs.h"


//*****************************************************************************
// MACROS
//*****************************************************************************

#define ADC_SAMPLE_TIME							3


//*****************************************************************************
// VARIABLES
//*****************************************************************************
typedef enum
{
	ADC_TYPE_BATTERY = 0,
	ADC_TYPE_TEMPERATURE
}ADC_TYPE;


typedef enum
{
	ADC_SAMPLE_OFF = 0,
	ADC_SAMPLE_ON
}ADC_SAMPLE_STATUS;

typedef struct
{
	WORD wData;
	BYTE yType;
}ADC_SCAN_DATA;


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
void ADC_Initialize(void);


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
void ADCif_CloseADC(void);


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
BYTE ADCif_SampleADC(WORD wType);


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
void ADC_InterruptHandler(void);


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
void ADCif_NewEventHandler(void);


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
void ADCif_StopModule(void);

#endif /* ADC_H_ */
