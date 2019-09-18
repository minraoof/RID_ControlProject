/*
 * RTC.c
 *
 *  Created on: 19 Sep 2018
 *      Author: min
 */
//*****************************************************************************
// INCLUDE FILES
//*****************************************************************************
#include "stm32f1xx_hal.h"
#include "RTC.h"
#include <string.h>
#include <stdlib.h>

#include "Main.h"
#include "UI.h"
#include "Utility.h"
#include "USB_Module.h"
#include "ADC.h"

//*****************************************************************************
// MACROS
//*****************************************************************************


//*****************************************************************************
// VARIABLES
//*****************************************************************************
BYTE  g_RTC_HalfSecond =  0;
DWORD g_RTC_CurrentTime = 0, g_RTC_SystemUptime = 0;

RTC_SYSTEM_TIMER g_RTC_SystemTimer;
RTC_TIMER_FLAG g_RTC_TimerFlag;

extern TIM_HandleTypeDef htim3;


//*****************************************************************************
// FUNCTION DEFINITION
//*****************************************************************************
/********************************************************************
 * Function:		RTC_Initialize()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Initialize the RTC. Now the 32k OSC haven't put in so use timer
 *				to implement.
 *
 * Note:			None
 *******************************************************************/
void RTC_Initialize(void)
{
	memset(&g_RTC_SystemTimer.yHalfSecond, 0x0, sizeof(RTC_SYSTEM_TIMER));
	memset(&g_RTC_TimerFlag.Flags.yLED1Flag, 0x0, sizeof(RTC_TIMER_FLAG));
}


/********************************************************************
 * Function:		RTC_SetTimerEnable()
 *
 * PreCondition:	None
 *
 * Input:			BYTE yEnable: The timer is enable/disable
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Set the timer to be enable or disable
 *
 * Note:			None
 *******************************************************************/
void RTC_SetTimerEnable(BYTE yEnable)
{

}


/********************************************************************
 * Function:		RTCif_SetHalfSecondTimeout()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Set the flag to indicate that the half second timeout. It will
 *				check the timer in the normal process loop.
 *
 * Note:			None
 *******************************************************************/
void RTCif_SetHalfSecondTimeout(void)
{
	g_RTC_SystemTimer.ySysTick++;

	if(g_RTC_SystemTimer.ySysTick >= 50)
	{
		g_RTC_SystemTimer.ySysTick = 0;
		g_RTC_SystemTimer.yHalfSecond++;
		
	}
}


/********************************************************************
 * Function:		RTC_TimerHandler()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		The timer will time out every 1/2 second. Decrease the value
 *				in the RTC timer. When the value is to 0, it will set the flag
 *				and check the flag after exiting the interrupt.
 *
 * Note:			None
 *******************************************************************/
void RTC_TimerHandler(void)
{

	if (0 == g_RTC_SystemTimer.yHalfSecond)
	{
		return;
	}

	g_RTC_SystemTimer.yHalfSecond--;

	g_RTC_HalfSecond++;

	if (g_RTC_HalfSecond % 2 == 0)
	{
		g_RTC_CurrentTime++;
		g_RTC_SystemUptime++;
		g_RTC_HalfSecond = 0;
	}

	if(g_RTC_SystemTimer.yLED1 > 0)
	{
		g_RTC_SystemTimer.yLED1--;

		if(g_RTC_SystemTimer.yLED1 == 0)
		{
			g_RTC_TimerFlag.Flags.yLED1Flag = TRUE;
		}
	}

	if(g_RTC_SystemTimer.yLED2 > 0)
	{
		g_RTC_SystemTimer.yLED2--;

		if(g_RTC_SystemTimer.yLED2 == 0)
		{
			g_RTC_TimerFlag.Flags.yLED2Flag = TRUE;
		}
	}

	if(g_RTC_SystemTimer.yBatterySample > 0)
	{
		g_RTC_SystemTimer.yBatterySample--;

		if(g_RTC_SystemTimer.yBatterySample == 0)
		{
			g_RTC_TimerFlag.Flags.yBatterySampleFlag = TRUE;
		}
	}
}


/********************************************************************
 * Function:		RTC_TimeOutHandler()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Handle the time out flag.
 *
 * Note:			None
 *******************************************************************/
void RTC_TimeOutHandler(void)
{
	if(g_RTC_TimerFlag.dwFlags == FALSE)
		return;


	if(g_RTC_TimerFlag.Flags.yLED1Flag)
	{
		g_RTC_TimerFlag.Flags.yLED1Flag = FALSE;
		UIif_SetBlinkLED(UI_LED1);
	}

	if(g_RTC_TimerFlag.Flags.yLED2Flag)
	{
		g_RTC_TimerFlag.Flags.yLED2Flag = FALSE;
		UIif_SetBlinkLED(UI_LED2);
	}

	if(g_RTC_TimerFlag.Flags.yBatterySampleFlag)
	{
		g_RTC_TimerFlag.Flags.yBatterySampleFlag = FALSE;
		ADCif_SampleADC(ADC_TYPE_BATTERY);
	}
}

/********************************************************************
 * Function:		RTCif_SetLEDTimer()
 *
 * PreCondition:	None
 *
 * Input:			WORD wLED: The LED
 *				BYTE wTime: The timer value
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Set the RTC timer for LED highlighting.
 *
 * Note:			None
 *******************************************************************/
void RTCif_SetLEDTimer(
	WORD wLED,
	BYTE yTime
)
{
	if (UI_LED1 == wLED)
	{
		g_RTC_SystemTimer.yLED1 = yTime;
 
		if (0 == yTime)
		{
			g_RTC_TimerFlag.Flags.yLED1Flag = FALSE;
		}
	}
	else if (UI_LED2 == wLED)
	{
		g_RTC_SystemTimer.yLED2 = yTime;

		if (0 == yTime)
		{
			g_RTC_TimerFlag.Flags.yLED2Flag = FALSE;
		}
	}
}

/********************************************************************
 * Function:		RTCif_SetLEDTimer()
 *
 * PreCondition:	None
 *
 * Input:			WORD wLED: The LED
 *				BYTE wTime: The timer value
 *
 * Output:		BYTE: time value
 *
 * Side Effects:	None
 *
 * Overview:		Set the RTC timer for LED time;
 *
 * Note:			None
 *******************************************************************/
BYTE RTCif_GetLEDTimer(BYTE yLED)
{
	if(yLED == UI_LED1)
	{
		return g_RTC_SystemTimer.yLED1;
	}
	else
	{
		return g_RTC_SystemTimer.yLED2;
	}
}

/********************************************************************
 * Function:		RTCif_SetBatterySampleTime()
 *
 * PreCondition:	None
 *
 * Input:			BYTE yTime: The sample waiting time
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Set the battery sample timer for battery.
 *
 * Note:			None
 *******************************************************************/
void RTCif_SetBatterySampleTime(BYTE yTime)
{
	g_RTC_SystemTimer.yBatterySample = (yTime * 2);
}

/********************************************************************
 * Function:		RTCif_GetSystemUptime()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		BYTE: The timer value
 *
 * Side Effects:	None
 *
 * Overview:		Get the HTTP process timer value
 *
 * Note:			None
 *******************************************************************/
DWORD RTCif_GetSystemUptime(void)
{
	return g_RTC_SystemUptime;
}

/********************************************************************
 * Function:		RTCif_TimeSecondsToString()
 *
 * PreCondition:	None
 *
 * Input:			DWORD dwTime: The time seconds.
 *				CHAR* pString: The destination string for the time
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Convert the seconds to the string format. The format is
 *				DD:HH:MM:SS
 *
 * Note:			None
 *******************************************************************/
void RTCif_TimeSecondsToString(
	DWORD dwTime,
	CHAR* pString
)
{
	ItoStr(dwTime / RTC_DAY_TO_SECONDS, &pString[strlen(pString)], 0);
	strcat(pString, ":");

	dwTime %= RTC_DAY_TO_SECONDS;

	ItoStr(dwTime / RTC_HOUR_TO_SECONDS, &pString[strlen(pString)], 2);
	strcat(pString, ":");

	dwTime %= RTC_HOUR_TO_SECONDS;

	ItoStr(dwTime / 60, &pString[strlen(pString)], 2);
	strcat(pString, ":");
	ItoStr(dwTime % 60, &pString[strlen(pString)], 2);
}

