/*
 * UI.h
 *
 *  Created on: 19 Sep 2018
 *      Author: min
 */

#ifndef SDCARD_H_
#define SDCARD_H_
//*****************************************************************************
// INCLUDE FILES
//*****************************************************************************

#include "Module.h"
#include "Key.h"
#include "stm32f1xx_hal.h"


//*****************************************************************************
// MACROS
//*****************************************************************************
// Configuration Address
#define SFLASH_ADDRESS_M2M_RESISTANCE	0x00
#define SFLASH_ADDRESS_M2M_DECTTIME		0x02
#define SFLASH_ADDRESS_M2M_INCIDENT		0x04
#define SFLASH_ADDRESS_M2M_WITHINTIME	0x06
#define SFLASH_ADDRESS_M2M_TIMERELAY	0x08

#define SFLASH_ADDRESS_CIP_RESISTANCE	0x0A
#define SFLASH_ADDRESS_CIP_DECTTIME		0x0C
#define SFLASH_ADDRESS_CIP_INCIDENT		0x0E
#define SFLASH_ADDRESS_CIP_WITHINTIME	0x10
#define SFLASH_ADDRESS_CIP_TIMERELAY	0x12

#define SFLASH_ADDRESS_CON_DECLEVEL		0x14
#define SFLASH_ADDRESS_CON_MINDURATION	0x16
#define SFLASH_ADDRESS_CON_TIMERELAY	0x18
#define SFLASH_ADDRESS_CON_TEMPERATURE	0x1A


#define SFLASH_LENGTH_TOTAL_CONFIG	0x1C

typedef enum
{
	SDCARD_STATE_IDLE = 0,
	SDCARD_STATE_READY,
	SDCARD_STATE_BUSY
}SDCARD_STATE;

//*****************************************************************************
// VARIABLES
//*****************************************************************************
typedef struct
{
	BYTE yState;

}SDCARD_STATUS;

//*****************************************************************************
// FUNCTION DEFINITION
//*****************************************************************************
/********************************************************************
 * Function:		SDCard_InitializeModule()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Initialize the SDCard module
 *
 * Note:			None
 *******************************************************************/
void SDCard_InitializeModule(void);

/**********************************************************************
 * Function:		SDCard_SignalHandler()
 *
 * PreCondition:	None
 *
 * Input:			Module_Signal* pSignal: The event signal
 *
 * Output:			BYTE: The signal handle result.
 *
 * Side Effects:	None
 *
 * Overview:		The SDCard module signal handler. It control the state machine
 *				state.
 *
 * Note:			None
 *******************************************************************/
BYTE SDCard_SignalHandler(Module_Signal* pSignal);

/********************************************************************
 * Function:		SDCard_TestWrite()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Test for SDcard
 *
 * Note:			None
 *******************************************************************/
void SDCard_TestWrite(void);

/********************************************************************
 * Function:		SDCard_TimerHandler()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Time out handler
 *
 * Note:			None
 *******************************************************************/
void SDCard_TimerHandler(void);

#endif /* SDCARD_H_ */
