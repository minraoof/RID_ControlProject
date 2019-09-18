/*
 * Module.h
 *
 *  Created on: 19 Sep 2018
 *      Author: min
 */

#ifndef MODULE_H_
#define MODULE_H_


//*****************************************************************************
// INCLUDE FILES
//*****************************************************************************

#include "GenericTypeDefs.h"


//*****************************************************************************
// MACROS
//*****************************************************************************


//*****************************************************************************
// VARIABLES
//*****************************************************************************
typedef enum
{
	MODULE_MAIN = 0,

	MODULE_UI,
	//MODULE_USB,

	MODULE_TOTAL
}MODULE_NAME;


typedef enum
{
	MODULE_RESULT_OK = 0,
	MODULE_RESULT_ERROR,

	MODULE_RESULT_BUSY,
	MODULE_RESULT_EMPTY
}MODULE_RESULT_CODE;


typedef struct
{
	BYTE yModuleID;
	WORD wEvent;
	void *pBuffer;
	void *pNext;
}Module_Signal;


//*****************************************************************************
// FUNCTION DEFINITION
//*****************************************************************************
/********************************************************************
 * Function:		Module_InitSignalQueue()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Initialize the module queue
 *
 * Note:			None
 *******************************************************************/
void Module_InitSignalQueue(void);


/********************************************************************
 * Function:		Module_AddSignal()
 *
 * PreCondition:	None
 *
 * Input:			BYTE yModule: The module that new signal will be input
 *				WORD wEvent: The event ID
 *				void* pBuffer: The data for module handler.
 *
 * Output:		The result to add a new signal to the queue
 *
 * Side Effects:	None
 *
 * Overview:		Add a new signal to a module's signal queue
 *
 * Note:			None
 *******************************************************************/
BOOL Module_AddSignal(
	BYTE yModule,
	WORD wEvent,
	void* pBuffer
);


/********************************************************************
 * Function:		ModuleRemoveSignal()
 *
 * PreCondition:	None
 *
 * Input:			BYTE yModule: The module that first signal will be removed
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Remove the first signal from a module's signal queue
 *
 * Note:			None
 *******************************************************************/
void Module_RemoveSignal(BYTE yModule);


/********************************************************************
 * Function:		Module_RemoveAllSignal()
 *
 * PreCondition:	None
 *
 * Input:			BYTE yModule: The module that first signal will be removed
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Remove all the signal in the module's signal queue
 *
 * Note:			None
 *******************************************************************/
void Module_RemoveAllSignal(BYTE yModule);


/********************************************************************
 * Function:		Module_HandleSignal()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Handle the signal in each module's queue
 *
 * Note:			None
 *******************************************************************/
void Module_HandleSignal(void);


/********************************************************************
 * Function:		Module_CheckQueueEmpty()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		BYTE: The status of the signal queue.
 *
 * Side Effects:	None
 *
 * Overview:		Check the signal queue is all empty or not.
 *
 * Note:			None
 *******************************************************************/
BYTE Module_CheckQueueEmpty(void);


#endif /* MODULE_H_ */
