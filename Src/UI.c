/*
 * UI.c
 *
 *  Created on: 19 Sep 2018
 *      Author: min
 */
//*****************************************************************************
// INCLUDE FILES
//*****************************************************************************
#include "stm32f1xx_hal.h"
#include <string.h>
#include <stdlib.h>

#include "UI.h"
#include "Module.h"
#include "USB_Module.h"
#include "Version.h"
#include "Utility.h"
#include "ADC.h"
#include "Key.h"
#include "RTC.h"
#include "Power.h"



//*****************************************************************************
// MACROS
//*****************************************************************************



//*****************************************************************************
// VARIABLES
//*****************************************************************************
UI_STATUS g_UI_Status;


//*****************************************************************************
// FUNCTION DEFINITION
//*****************************************************************************
/********************************************************************
 * Function:		UI_InitializeModule()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Initialize the UI module
 *
 * Note:			None
 *******************************************************************/
void UI_InitializeModule(void)
{
	g_UI_Status.yState = UI_STATE_IDLE;
	g_UI_Status.yKeyPressFlag = FALSE;

	g_UI_Status.yLEDState[UI_LED1] = UI_LED_STATE_OFF;
	g_UI_Status.yLEDState[UI_LED2] = UI_LED_STATE_OFF;
}

/**********************************************************************
 * Function:		UI_SignalHandler()
 *
 * PreCondition:	None
 *
 * Input:			Module_Signal* pSignal: The event signal
 *
 * Output:		BYTE: The signal handle result.
 *
 * Side Effects:	None
 *
 * Overview:		The UI module signal handler. It control the state machine
 *				state.
 *
 * Note:			None
 *******************************************************************/
BYTE UI_SignalHandler(Module_Signal* pSignal)
{
	if (NULL == pSignal)
	{
		return MODULE_RESULT_EMPTY;
	}

	switch (pSignal->wEvent)
	{
		case UI_EVENT_KEY:
		{
			UI_KeyHandler((KEY_DATA*)pSignal->pBuffer);
			FREE((KEY_DATA*)pSignal->pBuffer);
		}
			break;
		case UI_EVENT_BATTERY_HANDLER:
		{
			UI_CheckBatteryHandler();
		}
			break;

		default:
			break;
	}

	return MODULE_RESULT_OK;
}

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
void UI_KeyHandler(KEY_DATA* pData)
{
	switch (pData->wKeyValue)
	{
		case KEY_VALUE_USR1:
		{
			g_UI_Status.yKeyPressFlag ^= TRUE;
		}
			break;
		case KEY_VALUE_USR2:
		{
			//USB_PrintMsg("User Key2 Press");
		}
			break;
		case KEY_VALUE_USR3:
		{
			//USB_PrintMsg("User Key3 Press");
		}
			break;

		default:
			break;
	}
}

/********************************************************************
 * Function:		UI_CheckBatteryHandler()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Check the battery is lowe than the low batter threshold
 *				or need blink the LED
 *
 * Note:			None
 *******************************************************************/
void UI_CheckBatteryHandler(void)
{
	if(Powerif_IsLowPowerState() == TRUE)
	{
		if(g_UI_Status.yKeyPressFlag == TRUE)
		{
			UI_StopBlinkLED(UI_LED2);
			if(RTCif_GetLEDTimer(UI_LED1) == 0)
			{
				UIif_SetBlinkLED(UI_LED1);
			}
			
		}
		else
		{
			UI_StopBlinkLED(UI_LED1);
			if(RTCif_GetLEDTimer(UI_LED2) == 0)
			{
				UIif_SetBlinkLED(UI_LED2);
			}
		}		
	}
	else
	{
		UI_StopBlinkLED(UI_LED1);
		UI_StopBlinkLED(UI_LED2);
	}

	RTCif_SetBatterySampleTime(1);
}
/********************************************************************
 * Function:		UIif_SetBlinkLED()
 *
 * PreCondition:	None
 *
 * Input:			BYTE yLED: The LED type.
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Set the blinking LED by the input type and state.
 *
 * Note:			None
 *******************************************************************/
void UIif_SetBlinkLED(BYTE yLED)
{
	if (g_UI_Status.yLEDState[yLED] == UI_LED_STATE_ON)
	{
		g_UI_Status.yLEDState[yLED] = UI_LED_STATE_OFF;

		if (UI_LED1 == yLED)
		{
			UI_LED1_OFF;			
		}
		else if(UI_LED2 == yLED)
		{
			UI_LED2_OFF;
		}
		
		RTCif_SetLEDTimer(
				yLED,
				1
			);
	}
	else if (g_UI_Status.yLEDState[yLED] == UI_LED_STATE_OFF)
	{
		g_UI_Status.yLEDState[yLED] = UI_LED_STATE_ON;
		
		if (UI_LED1 == yLED)
		{			
			UI_LED1_ON;			
		}
		else if(UI_LED2 == yLED)
		{
			UI_LED2_ON;
		}

		RTCif_SetLEDTimer(
				yLED,
				1
			);
	}
}

/********************************************************************
 * Function:		UI_StopBlinkLED()
 *
 * PreCondition:	None
 *
 * Input:			BYTE yLED: The LED type.
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Stop the blink timer for LED. Set to dim mode and reset the
 *				LED state.
 *
 * Note:			None
 *******************************************************************/
void UI_StopBlinkLED(BYTE yLED)
{
	if(yLED == UI_LED1)
	{
		UI_LED1_OFF;
	}
	else if(yLED == UI_LED2)
	{
		UI_LED2_OFF;
	}

	RTCif_SetLEDTimer(
		yLED,
		0
	);
}
