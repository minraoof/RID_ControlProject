/*
 * RTC.h
 *
 *  Created on: 19 Sep 2018
 *      Author: min
 */

#ifndef RTC_H_
#define RTC_H_
//*****************************************************************************
// INCLUDE FILES
//*****************************************************************************

#include "GenericTypeDefs.h"



//*****************************************************************************
// MACROS
//*****************************************************************************
#define RTC_YEAR_TO_SECONDS						31536000
#define RTC_DAY_TO_SECONDS						86400
#define RTC_HOUR_TO_SECONDS						3600


//*****************************************************************************
// VARIABLES
//*****************************************************************************

typedef struct
{
	BYTE ySysTick;
	
	BYTE yHalfSecond;

	BYTE yLED1;

	BYTE yLED2;

	BYTE yBatterySample;

}RTC_SYSTEM_TIMER;

/*typedef union
{
	BYTE yADCScanTimeFlag;	
}RTC_TIMER_FLAG;*/

typedef union
{
	struct
	{
		BYTE yLED1Flag;

		BYTE yLED2Flag;

		BYTE yBatterySampleFlag;
		
		DWORD yReserved:28;		
	}Flags;
	DWORD dwFlags;
}RTC_TIMER_FLAG;
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
void RTC_Initialize(void);


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
void RTC_SetTimerEnable(BYTE yEnable);


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
void RTCif_SetHalfSecondTimeout(void);

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
 * Overview:		The timer will time out every 1/2 seconds. Decrease the value
 *				in the RTC timer. When the value is to 0, it will set the flag
 *				and check the flag after exiting the interrupt.
 *
 * Note:			None
 *******************************************************************/
void RTC_TimerHandler(void);

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
void RTC_TimeOutHandler(void);

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
void RTCif_SetBatterySampleTime(BYTE yTime);

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
);

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
BYTE RTCif_GetLEDTimer(BYTE yLED);

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
DWORD RTCif_GetSystemUptime(void);

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
);
#endif /* RTC_H_ */
