/*
 * UI.h
 *
 *  Created on: 19 Sep 2018
 *      Author: min
 */

#ifndef UI_H_
#define UI_H_
//*****************************************************************************
// INCLUDE FILES
//*****************************************************************************

#include "Module.h"
#include "Key.h"
#include "stm32f1xx_hal.h"

#define UI_LED_M2M_ON	HAL_GPIO_WritePin(GPIOB, LED_M2M_Pin, GPIO_PIN_SET)
#define UI_LED_M2M_OFF	HAL_GPIO_WritePin(GPIOB, LED_M2M_Pin, GPIO_PIN_RESET)
#define UI_LED_M2M_TOGGLE HAL_GPIO_TogglePin(GPIOB, LED_M2M_Pin)

#define UI_LED_CONT_ON	HAL_GPIO_WritePin(GPIOB, LED_CONT_Pin, GPIO_PIN_SET)
#define UI_LED_CONT_OFF	HAL_GPIO_WritePin(GPIOB, LED_CONT_Pin, GPIO_PIN_RESET)
#define UI_LED_CONT_TOGGLE HAL_GPIO_TogglePin(GPIOB, LED_CONT_Pin)

#define UI_LED_POL_ON	HAL_GPIO_WritePin(GPIOB, LED_POL_Pin, GPIO_PIN_SET)
#define UI_LED_POL_OFF	HAL_GPIO_WritePin(GPIOB, LED_POL_Pin, GPIO_PIN_RESET)
#define UI_LED_POL_TOGGLE HAL_GPIO_TogglePin(GPIOB, LED_POL_Pin)

#define UI_LED_TEST_ON	HAL_GPIO_WritePin(GPIOB, LED_TEST_Pin, GPIO_PIN_SET)
#define UI_LED_TEST_OFF	HAL_GPIO_WritePin(GPIOB, LED_TEST_Pin, GPIO_PIN_RESET)
#define UI_LED_TEST_TOGGLE HAL_GPIO_TogglePin(GPIOB, LED_TEST_Pin)


#define UI_LED_SDCARD_ON	HAL_GPIO_WritePin(GPIOC, LED_SDCARD_Pin, GPIO_PIN_SET)
#define UI_LED_SDCARD_OFF	HAL_GPIO_WritePin(GPIOC, LED_SDCARD_Pin, GPIO_PIN_RESET)
#define UI_LED_SDCARD_TOGGLE HAL_GPIO_TogglePin(GPIOC, LED_SDCARD_Pin)
//*****************************************************************************
// MACROS
//*****************************************************************************

typedef enum
{
	UI_STATE_IDLE = 0,
	UI_STATE_STOP
}UI_STATE;

typedef enum
{
	UI_LED_M2M = 0,
	UI_LED_CONT,
	UI_LED_POL,
	UI_LED_TEST,
	UI_LED_SDCARD,
	UI_LED_TOTAL
}UI_LED;

typedef enum
{
	UI_FAST_LED_M2M = 0,
	UI_FAST_LED_CONT,
	UI_FAST_LED_POL,
	UI_FAST_LED_TEST,
	UI_FAST_LED_SDCARD,
	UI_FAST_LED_TOTAL
}UI_FAST_LED;

typedef enum
{
	UI_LED_STATE_OFF = 0,
	UI_LED_STATE_ON
}UI_LED_STATE;

typedef enum
{
	UI_EVENT_KEY = 0,
	UI_EVENT_BATTERY_HANDLER
}UI_EVENT;

//*****************************************************************************
// VARIABLES
//*****************************************************************************

typedef struct
{
	BYTE yState;

	// Used for the LED blinking process.
	BYTE yLEDState[UI_LED_TOTAL];

	//For Key press Flag
	BYTE yKeyPressFlag;
}UI_STATUS;

typedef struct
{
	BYTE yLED;
	BYTE yLoop;
}UI_LED_FastBlink;

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
void UI_InitializeModule(void);

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
BYTE UI_SignalHandler(Module_Signal* pSignal);

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
void UI_CheckBatteryHandler(void);

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
void UIif_SetBlinkLED(BYTE yLED);

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
void UI_StopBlinkLED(BYTE yLED);

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
);

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
void UIif_HandleFastBlinkLEDTimeout(void);
#endif /* UI_H_ */
