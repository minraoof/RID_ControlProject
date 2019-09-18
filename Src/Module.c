/*
 * Module.c
 *
 *  Created on: 19 Sep 2018
 *      Author: min
 */
//*****************************************************************************
// INCLUDE FILES
//*****************************************************************************
#define MODULE_IMPORTED
#include "Module.h"
#include <stdlib.h>
#include "Utility.h"
#include "UI.h"
#include "Key.h"



//*****************************************************************************
// MACROS
//*****************************************************************************


//*****************************************************************************
// VARIABLES
//*****************************************************************************
void* g_Module_Handler[] = {
	NULL,//Main_SignalHandler,

	UI_SignalHandler,
	//USB_SignalHandler,
	
	NULL
};

Module_Signal* g_Module_SignalQueue[MODULE_TOTAL];


//*****************************************************************************
// FUNCTIONS
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
void Module_InitSignalQueue(void)
{
	BYTE yIndex;

	// Initialize the signal queue for the modules
	for (yIndex = 0; yIndex < MODULE_TOTAL; yIndex++)
	{
		g_Module_SignalQueue[yIndex] = NULL;
	}
}


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
 	void *pBuffer
)
{
	Module_Signal *pSignal = NULL;

	pSignal = (Module_Signal*)MALLOC(sizeof(Module_Signal));
	if (NULL == pSignal)
	{
		return FALSE;
	}

	// If the MCU is low clock rate, switch back to normal rate
	//Main_WakeUpFromPowerSave();

	pSignal->yModuleID = yModule;
	pSignal->wEvent = wEvent;
	pSignal->pBuffer = pBuffer;
	pSignal->pNext = NULL;

	if (g_Module_SignalQueue[yModule] == NULL)
	{
		g_Module_SignalQueue[yModule] = pSignal;
	}
	else
	{
		Module_Signal *pTemp = g_Module_SignalQueue[yModule];

		while (pTemp->pNext != NULL)
		{
			pTemp = (Module_Signal*)pTemp->pNext;
		}

		pTemp->pNext = pSignal;
	}

	return TRUE;
}


/********************************************************************
 * Function:		Module_RemoveSignal()
 *
 * PreCondition:	None
 *
 * Input:			BYTE yModule: The module that first signal will be removed
 *
 * Output:			None
 *
 * Side Effects:	None
 *
 * Overview:		Remove the first signal from a module's signal queue
 *
 * Note:			None
 *******************************************************************/
void Module_RemoveSignal(BYTE yModule)
{
	Module_Signal *pTemp = g_Module_SignalQueue[yModule];

	g_Module_SignalQueue[yModule] = (Module_Signal*)pTemp->pNext;
	FREE(pTemp);
}


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
void Module_RemoveAllSignal(BYTE yModule)
{
	Module_Signal *pTemp = g_Module_SignalQueue[yModule];

	while (NULL != pTemp)
	{
		g_Module_SignalQueue[yModule] = (Module_Signal*)pTemp->pNext;

		if (MODULE_UI == yModule)
		{
			if (NULL != pTemp->pBuffer)
			{
				switch (pTemp->wEvent)
				{
					case UI_EVENT_KEY:
					{
						FREE((KEY_DATA*)pTemp->pBuffer);
					}
						break;
					default:
						break;
				}
			}
		}

		FREE(pTemp);

		pTemp = g_Module_SignalQueue[yModule];
	}
}


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
void Module_HandleSignal(void)
{
BYTE yIndex, yResult;
	
	for(yIndex=0; yIndex < MODULE_TOTAL ; yIndex++)
	{
	
			if(NULL != g_Module_SignalQueue[yIndex])
			{	
					switch(yIndex)
					{
						
							case MODULE_UI:
							{
								yResult = UI_SignalHandler(g_Module_SignalQueue[yIndex]);
							}
							break;
							
							default:
								break;	
					
					}					
					if ((yResult != MODULE_RESULT_BUSY)
					 	&& (yResult != MODULE_RESULT_EMPTY))
					{
							Module_RemoveSignal(yIndex);
					}
			}
	}
}


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
BYTE Module_CheckQueueEmpty(void)
{
	BYTE yIndex;

	for (yIndex = 0; yIndex < MODULE_TOTAL; yIndex++)
	{
		if (g_Module_SignalQueue[yIndex] != NULL)
		{
			return FALSE;
		}
	}

	return TRUE;
}




