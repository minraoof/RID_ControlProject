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
	WORD wYear;
	BYTE yMonth;
	BYTE yDay;
	BYTE yWeekDate;
	BYTE yHour;
	BYTE yMin;
	BYTE ySec;
}RTC_DATE_FORMAT;

typedef struct
{
	BYTE ySimplifyMonth:1;
	BYTE yYear:1;
	BYTE yWeekDay:1;
	BYTE yReserved:5;
}RTC_Date_String_Setting;

typedef struct
{	
	BYTE yOneSecond;

	BYTE yLedM2M;

	BYTE yLedCONT;

	BYTE yLedPOL;

	BYTE yLedTest;

	BYTE yLedSDcard;

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
		BYTE yLedM2MFlag;

		BYTE yLedCONTFlag;

		BYTE yLedPOLFlag;

		BYTE yLedTestFlag;

		BYTE yLedSDcardFlag;

		BYTE yBatterySampleFlag;
		
		DWORD yReserved:25;		
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
 * Function:		RTC_SetOneSecTimerEnable()
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
void RTC_SetOneSecTimerEnable(BYTE yEnable);


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
 * Function:		RTCif_GetLEDTimer()
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
 * Function:		RTCif_GetTimeStamp()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		DWORD: The timestamp
 *
 * Side Effects:	None
 *
 * Overview:		Get the current timestamp
 *
 * Note:			None
 *******************************************************************/
DWORD RTCif_GetTimeStamp(void);

/********************************************************************
 * Function:		RTCif_SetTimeStamp()
 *
 * PreCondition:	None
 *
 * Input:			DWORD:Time
 *
 * Output:		None:
 *
 * Side Effects:	None
 *
 * Overview:		Set the current timestamp
 *
 * Note:			None
 *******************************************************************/
void RTCif_SetTimeStamp(DWORD dwTime);

/********************************************************************
 * Function:		RTC_TimeStampToDate()
 *
 * PreCondition:	None
 *
 * Input:			RTC_DATE_FORMAT tDate: The date time value.
 *
 * Output:		DWORD: The time stamp.
 *
 * Side Effects:	None
 *
 * Overview:		Translate the date time value to time stamp.
 *
 * Note:			None
 *******************************************************************/
DWORD RTC_DateToTimeStamp(RTC_DATE_FORMAT tDate);

/********************************************************************
 * Function:		RTC_TimeStampToDate()
 *
 * PreCondition:	None
 *
 * Input:			DWORD dwTime: The time stamp.
 *				RTC_DATE_FORMAT *pDate: The result pointer.
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Translate the timestamp to the date-time format.
 *
 * Note:			None
 *******************************************************************/
void RTC_TimeStampToDate(
	DWORD dwTime,
	RTC_DATE_FORMAT *pDate
);

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

/********************************************************************
 * Function:		RTCif_GetDateString()
 *
 * PreCondition:	None
 *
 * Input:			CHAR*: The current date string
 *				DWORD dwTimeStamp: The time stamp
 *				RTC_Date_String_Setting t_Setting: The string format setting
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Get the current date string. It needs to release the memory
 *				after using this string.
 *
 * Note:			None
 *******************************************************************/
void RTCif_GetDateString(
	CHAR* pDate,
	DWORD dwTimeStamp,
	RTC_Date_String_Setting t_Setting
);

/********************************************************************
 * Function:		RTCif_GetTimeString()
 *
 * PreCondition:	None
 *
 * Input:			CHAR*: The current time string
 *				DWORD dwTimeStamp: The time stamp
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Get the current time string. It needs to release the memory
 *				after using this string.
 *
 * Note:			None
 *******************************************************************/
void RTCif_GetTimeString(
	CHAR* pTime,
	DWORD dwTimeStamp
);
#endif /* RTC_H_ */
