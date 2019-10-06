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
UI_LED_FastBlink g_UI_LEDFastBlink;

extern TIM_HandleTypeDef htim3;
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

	g_UI_Status.yLEDState[UI_LED_M2M] = UI_LED_STATE_OFF;
	g_UI_Status.yLEDState[UI_LED_CONT] = UI_LED_STATE_OFF;
	g_UI_Status.yLEDState[UI_LED_POL] = UI_LED_STATE_OFF;
	g_UI_Status.yLEDState[UI_LED_TEST] = UI_LED_STATE_OFF;
	g_UI_Status.yLEDState[UI_LED_SDCARD] = UI_LED_STATE_OFF;

	//UIif_SetBlinkLED(UI_LED_M2M); test
	
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
	/*if(Powerif_IsLowPowerState() == TRUE)
	{
	
	}
	else
	{

	}
	RTCif_SetBatterySampleTime(1);*/
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

		if (UI_LED_M2M == yLED)
		{
			UI_LED_M2M_OFF;			
		}
		else if(UI_LED_CONT == yLED)
		{
			UI_LED_CONT_OFF;
		}
		else if(UI_LED_POL == yLED)
		{
			UI_LED_POL_OFF;
		}
		else if(UI_LED_TEST == yLED)
		{
			UI_LED_TEST_OFF;
		}
		else if(UI_LED_SDCARD == yLED)
		{
			UI_LED_SDCARD_OFF;
		}
		RTCif_SetLEDTimer(
				yLED,
				1
			);
	}
	else if (g_UI_Status.yLEDState[yLED] == UI_LED_STATE_OFF)
	{
		g_UI_Status.yLEDState[yLED] = UI_LED_STATE_ON;
		
		if (UI_LED_M2M == yLED)
		{
			UI_LED_M2M_ON;			
		}
		else if(UI_LED_CONT == yLED)
		{
			UI_LED_CONT_ON;
		}
		else if(UI_LED_POL == yLED)
		{
			UI_LED_POL_ON;
		}
		else if(UI_LED_TEST == yLED)
		{
			UI_LED_TEST_ON;
		}
		else if(UI_LED_SDCARD == yLED)
		{
			UI_LED_SDCARD_ON;
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
	if(yLED == UI_LED_M2M)
	{
		UI_LED_M2M_OFF;
	}
	else if(yLED == UI_LED_CONT)
	{
		UI_LED_CONT_OFF;
	}
	else if(yLED == UI_LED_POL)
	{
		UI_LED_POL_OFF;
	}
	else if(yLED == UI_LED_TEST)
	{
		UI_LED_TEST_OFF;
	}
	else if(yLED == UI_LED_SDCARD)
	{
		UI_LED_SDCARD_OFF;
	}

	RTCif_SetLEDTimer(
		yLED,
		0
	);
}

/********************************************************************
 * Function:		UIif_StartFastBlinkLED()
 *
 * PreCondition:	None
 *
 * Input:			BYTE yLED: The LED type.
 					BYTE yLoop
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Blink first for LED
 *
 * Note:			None
 *******************************************************************/
void UIif_StartFastBlinkLED(
	BYTE yLED,
	BYTE yLoop
)
{
	g_UI_LEDFastBlink.yLED = yLED;
	g_UI_LEDFastBlink.yLoop = yLoop;

	if (yLoop != 0)
	{
		HAL_TIM_Base_Start_IT(&htim3);

	}
	else
	{
		HAL_TIM_Base_Stop_IT(&htim3);
	}
}

/********************************************************************
 * Function:		UIif_HandleFastBlinkLEDTimeout()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Timeout for fast blink
 *
 * Note:			None
 *******************************************************************/
void UIif_HandleFastBlinkLEDTimeout(void)
{
	HAL_TIM_Base_Stop_IT(&htim3);

	if(g_UI_LEDFastBlink.yLoop > 0)
	{
		g_UI_LEDFastBlink.yLoop--;
	}

	if(g_UI_LEDFastBlink.yLoop > 0)
	{
		if(g_UI_LEDFastBlink.yLED == UI_FAST_LED_M2M)
		{
			UI_LED_M2M_TOGGLE;
		}

		if(g_UI_LEDFastBlink.yLED == UI_FAST_LED_CONT)
		{
			UI_LED_CONT_TOGGLE;
		}

		if(g_UI_LEDFastBlink.yLED == UI_FAST_LED_POL)
		{
			UI_LED_POL_TOGGLE;
		}

		if(g_UI_LEDFastBlink.yLED == UI_FAST_LED_TEST)
		{
			UI_LED_TEST_TOGGLE;
		}

		if(g_UI_LEDFastBlink.yLED == UI_FAST_LED_SDCARD)
		{
			UI_LED_SDCARD_TOGGLE;
		}

		HAL_TIM_Base_Start_IT(&htim3);
	}
	else
	{
		if(g_UI_LEDFastBlink.yLED == UI_FAST_LED_M2M)
		{
			UI_LED_M2M_OFF;
		}

		if(g_UI_LEDFastBlink.yLED == UI_FAST_LED_CONT)
		{
			UI_LED_CONT_OFF;
		}

		if(g_UI_LEDFastBlink.yLED == UI_FAST_LED_POL)
		{
			UI_LED_POL_OFF;
		}

		if(g_UI_LEDFastBlink.yLED == UI_FAST_LED_TEST)
		{
			UI_LED_TEST_OFF;
		}

		if(g_UI_LEDFastBlink.yLED == UI_FAST_LED_SDCARD)
		{
			UI_LED_SDCARD_OFF;
		}
	}
}
