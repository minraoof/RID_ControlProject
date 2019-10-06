/*
 * Key.c
 *
 *  Created on: 19 Sep 2018
 *      Author: min
 */
//*****************************************************************************
// INCLUDE FILES
//*****************************************************************************
#include "Key.h"
#include <stdlib.h>

#include "Module.h"
#include "UI.h"
#include "Utility.h"
#include "UI.h"
#include "stm32f1xx_hal.h"
#include "main.h"

//*****************************************************************************
// MACROS
//*****************************************************************************

extern TIM_HandleTypeDef htim2;
//*****************************************************************************
// VARIABLES
//*****************************************************************************
KEY_STATE_STRUCT g_Key_State;
BYTE g_Key_NewEvent = FALSE;
BYTE yTemp;

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
void Key_InitializeModule(void)
{

	Key_ConfigPressTimer();

	g_Key_State.yState = KEY_STATE_RELEASE;
	g_Key_State.yKey = KEY_VALUE_NONE;
	g_Key_State.yPreviousKey = KEY_VALUE_NONE;

}


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
void Key_ConfigPressTimer(void)
{
	// Debounce timer
}


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
void Key_ParseKey(void)
{
	BYTE yKey = Key_GetKeyData(), yResult = FALSE;
	
	yTemp = yKey;

	if (KEY_STATE_RELEASE == g_Key_State.yState)
	{
		if (0x03 != yKey &&
			g_Key_State.yKey != yKey)
		{
			HAL_TIM_Base_Stop_IT(&htim2);
		}

		if (0x03 == yKey)
		{
			HAL_TIM_Base_Stop_IT(&htim2);
		}
		else
		{
			g_Key_State.yKey = yKey;
			HAL_TIM_Base_Start_IT(&htim2);
		}
	}
	else if (KEY_STATE_PRESS == g_Key_State.yState ||
		KEY_STATE_LONG_PRESS == g_Key_State.yState)
	{
		if (0x03 == yKey)
		{
			g_Key_State.yState = KEY_STATE_RELEASE;

			g_Key_State.yPreviousKey = g_Key_State.yKey;
			//g_Key_State.yKey = yKey;
			g_Key_State.yPressCount = 0;

			//wType = Key_FindKeyValue(g_Key_State.yKey);
			// Disable the Timer2
			/*KEY_PRESS_TIMER_ENABLE = FALSE;
			KEY_PRESS_TIMER = 0;*/
			//RTCif_SetKeyLongPressTime(0);

			//yResult = TRUE;
		}
		else if (yKey != g_Key_State.yKey)
		{
			//KEY_PRESS_TIMER = 0;
			//RTCif_SetKeyLongPressTime(KEY_TEST_LONG_KEY_COUNT);

			g_Key_State.yPreviousKey = g_Key_State.yKey;
			g_Key_State.yKey = yKey;
			g_Key_State.yPressCount = 0;

			//wType = Key_FindKeyValue(g_Key_State.yKey);
		}
	}

	if (TRUE == yResult)
	{
		g_Key_NewEvent = TRUE;
	}
}


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
void Key_DebounceInterruptHandler(void)
{
	BYTE yKey = Key_GetKeyData();
	//WORD wType = Key_FindKeyValue(yKey);
	yTemp = yKey;
	HAL_TIM_Base_Stop_IT(&htim2);

	if (g_Key_State.yKey == yKey)
	{
		g_Key_State.yState = KEY_STATE_PRESS;

		//RTCif_SetKeyLongPressTime(KEY_TEST_LONG_KEY_COUNT);

		g_Key_State.yPreviousKey = g_Key_State.yKey;
		g_Key_State.yKey = yKey;
		g_Key_State.yPressCount = 0;

		g_Key_NewEvent = TRUE;
	}
}


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
BYTE Key_GetKeyData(void)
{
	BYTE yKey = 0;

	yKey |= HAL_GPIO_ReadPin(MOTOR_SENSE_GPIO_Port, MOTOR_SENSE_Pin);
	yKey <<= 1;

	yKey |= HAL_GPIO_ReadPin(MODE1_GPIO_Port, MODE1_Pin);
	yKey <<= 1;

	yKey |= HAL_GPIO_ReadPin(MODE2_GPIO_Port, MODE2_Pin);

	return yKey;
}


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
WORD Key_FindKeyValue(BYTE yKey)
{
	BYTE yIndex;

	for (yIndex = 1; yIndex < KEY_VALUE_TOTAL; yIndex++)
	{
		if ((yKey & 0x01) == 0)
		{
			return yIndex;
		}

		yKey >>= 1;
	}

	return KEY_VALUE_NONE;
}


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
void Key_PressTimeOut(void)
{
	g_Key_State.yState = KEY_STATE_LONG_PRESS;
	g_Key_NewEvent = TRUE;
}


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
void Keyif_NewEventHandler(void)
{
	if (TRUE == g_Key_NewEvent)
	{
		KEY_DATA* pBuffer;

		g_Key_NewEvent = FALSE;

		pBuffer = (KEY_DATA*)MALLOC(sizeof(KEY_DATA));

		if (NULL == pBuffer)
		{
			return;
		}

		pBuffer->yEvent = g_Key_State.yState;

		if (g_Key_State.yState != KEY_STATE_RELEASE)
		{
			pBuffer->wKeyValue = Key_FindKeyValue(g_Key_State.yKey);
		}
		else
		{
			pBuffer->wKeyValue = Key_FindKeyValue(g_Key_State.yPreviousKey);
			g_Key_State.yKey = KEY_VALUE_NONE;
		}

		Module_AddSignal(
			MODULE_UI,
			UI_EVENT_KEY,
			(void*)pBuffer
		);
	}
}


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
void Keyif_StopModule(void)
{
	/*KEY_PRESS_TIMER_ENABLE = FALSE;
	TMR2 = 0;*/

	g_Key_State.yState = KEY_STATE_RELEASE;
	g_Key_State.yKey = KEY_VALUE_NONE;
	g_Key_State.yPreviousKey = KEY_VALUE_NONE;

	g_Key_NewEvent = FALSE;
}


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
void Keyif_ResumeModule(void)
{

}


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
BYTE Keyif_GetKeyState(void)
{
	return g_Key_State.yState;
}




