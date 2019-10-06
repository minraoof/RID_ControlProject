/*
 * Key.h
 *
 *  Created on: 19 Sep 2018
 *      Author: min
 */

#ifndef KEY_H_
#define KEY_H_
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
	KEY_STATE_RELEASE = 0,
	KEY_STATE_PRESS,
	KEY_STATE_LONG_PRESS
}KEY_STATE;


typedef enum
{
	KEY_VALUE_NONE = 0,
	KEY_VALUE_USR1,
	KEY_VALUE_USR2,
	KEY_VALUE_TOTAL
}KEY_VALUE;


typedef struct
{
	BYTE yState;
	BYTE yKey;
	BYTE yPreviousKey;
	BYTE yPressCount;
}KEY_STATE_STRUCT;


typedef struct
{
	BYTE yEvent;
	WORD wKeyValue;
}KEY_DATA;


//*****************************************************************************
// FUNCTION DEFINITION
//*****************************************************************************
/********************************************************************
 * Function:		Key_InitializeModule()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Init the key module
 *
 * Note:			None
 *******************************************************************/
void Key_InitializeModule(void);

/********************************************************************
 * Function:		Key_ConfigPressTimer()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Config the key press timer.
 *
 * Note:			None
 *******************************************************************/
void Key_ConfigPressTimer(void);

/********************************************************************
 * Function:		Key_ParseKey()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Handle the input key and change the state machine states.
 *
 * Note:			None
 *******************************************************************/
void Key_ParseKey(void);

/********************************************************************
 * Function:		Key_DebounceInterruptHandler()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		The SW debounce handler. If the key value still no change
 *				it will send a press event to UI module
 *
 * Note:			None
 *******************************************************************/
void Key_DebounceInterruptHandler(void);

/********************************************************************
 * Function:		Key_GetKeyData()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		BYTE: The key data
 *
 * Side Effects:	None
 *
 * Overview:		Get the current key data.
 *
 * Note:			None
 *******************************************************************/
BYTE Key_GetKeyData(void);

/********************************************************************
 * Function:		Key_FindKeyValue()
 *
 * PreCondition:	None
 *
 * Input:			BYTE yKey: The input key value
 *
 * Output:		WORD: The type value of the key in WORD.
 *
 * Side Effects:	None
 *
 * Overview:		Transform the key value to a WORD type value.
 *
 * Note:			None
 *******************************************************************/
WORD Key_FindKeyValue(BYTE yKey);

/********************************************************************
 * Function:		Key_PressTimeOut()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		The long key press timer time out event. If time out happened,
 *				it active the long key press event.
 *
 * Note:			None
 *******************************************************************/
void Key_PressTimeOut(void);

/********************************************************************
 * Function:		Keyif_NewEventHandler()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		The interface for main loop to process the new key event.
 *
 * Note:			None
 *******************************************************************/
void Keyif_NewEventHandler(void);

/********************************************************************
 * Function:		Keyif_StopModule()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Stop the key module. Disable the CN interrupt for all the
 *				keys. And also stop the key press timer.
 *
 * Note:			None
 *******************************************************************/
void Keyif_StopModule(void);

/********************************************************************
 * Function:		Keyif_ResumeModule()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Enable all the Key's CN interrupt to implement key function
 *
 * Note:			None
 *******************************************************************/
void Keyif_ResumeModule(void);

/********************************************************************
 * Function:		Keyif_GetKeyState()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		BYTE: The key's current status
 *
 * Side Effects:	None
 *
 * Overview:		Return the key's current status
 *
 * Note:			None
 *******************************************************************/
BYTE Keyif_GetKeyState(void);

/********************************************************************
 * Function:		UI_KeyHandler()
 *
 * PreCondition:	None
 *
 * Input:			KEY_DATA* pData: The key's detail.
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Handle the key event.
 * ||
							FALSE == g_GSM_Status.yConfigFinish
 * Note:			None
 *******************************************************************/
void UI_KeyHandler(KEY_DATA* pData);

#endif /* KEY_H_ */
