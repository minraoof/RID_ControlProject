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

#define UI_LED1_ON	HAL_GPIO_WritePin(GPIOB, USR_LED1_Pin, GPIO_PIN_SET)
#define UI_LED1_OFF	HAL_GPIO_WritePin(GPIOB, USR_LED1_Pin, GPIO_PIN_RESET)

#define UI_LED2_ON	HAL_GPIO_WritePin(GPIOB, USR_LED2_Pin, GPIO_PIN_SET)
#define UI_LED2_OFF	HAL_GPIO_WritePin(GPIOB, USR_LED2_Pin, GPIO_PIN_RESET)

//*****************************************************************************
// MACROS
//*****************************************************************************


typedef enum
{
	UI_LED1 = 0,
	UI_LED2
}UI_LED;

typedef enum
{
	UI_LED_STATE_OFF = 0,
	UI_LED_STATE_ON
}UI_LED_STATE;

//*****************************************************************************
// VARIABLES
//*****************************************************************************


typedef enum
{
	UI_STATE_IDLE = 0,
	UI_STATE_STOP
}UI_STATE;

typedef struct
{
	BYTE yState;

	// Used for the LED blinking process.
	BYTE yLEDState[2];

	//For Key press Flag
	BYTE yKeyPressFlag;
}UI_STATUS;

typedef struct
{
	WORD wData;
}UI_ADCSCAN_DATA;

typedef struct
{
	BYTE yData;
	BYTE yEvent;
}UI_CHANNEL_DATA;

typedef enum
{
	UI_EVENT_KEY = 0,
	UI_EVENT_BATTERY_HANDLER
}UI_EVENT;



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
#endif /* UI_H_ */
