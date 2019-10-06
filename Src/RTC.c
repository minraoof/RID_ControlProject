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
BYTE g_RTC_TimeZoneIndex;
DWORD g_RTC_CurrentTime = 0, g_RTC_SystemUptime = 0;

RTC_SYSTEM_TIMER g_RTC_SystemTimer;
RTC_TIMER_FLAG g_RTC_TimerFlag;

extern RTC_HandleTypeDef hrtc;


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
	memset(&g_RTC_SystemTimer.yOneSecond, 0x0, sizeof(RTC_SYSTEM_TIMER));
	memset(&g_RTC_TimerFlag.Flags.yLedM2MFlag, 0x0, sizeof(RTC_TIMER_FLAG));
	
	g_RTC_CurrentTime = 1325376000;
	
	
	g_RTC_TimeZoneIndex = 48;

	RTC_SetOneSecTimerEnable(TRUE);
}


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
void RTC_SetOneSecTimerEnable(BYTE yEnable)
{
	if(yEnable == TRUE)
	{
		HAL_RTCEx_SetSecond_IT(&hrtc);
	}
	else
	{
		HAL_RTCEx_DeactivateSecond(&hrtc);
	}
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
	g_RTC_SystemTimer.yOneSecond++;
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

	if (0 == g_RTC_SystemTimer.yOneSecond)
	{
		return;
	}

	g_RTC_SystemTimer.yOneSecond--;
	g_RTC_CurrentTime++;
	g_RTC_SystemUptime++;


	if(g_RTC_SystemTimer.yLedM2M > 0)
	{
		g_RTC_SystemTimer.yLedM2M--;

		if(g_RTC_SystemTimer.yLedM2M == 0)
		{
			g_RTC_TimerFlag.Flags.yLedM2MFlag = TRUE;
		}
	}

	if(g_RTC_SystemTimer.yLedCONT > 0)
	{
		g_RTC_SystemTimer.yLedCONT--;

		if(g_RTC_SystemTimer.yLedCONT == 0)
		{
			g_RTC_TimerFlag.Flags.yLedCONTFlag = TRUE;
		}
	}

	if(g_RTC_SystemTimer.yLedPOL > 0)
	{
		g_RTC_SystemTimer.yLedPOL--;

		if(g_RTC_SystemTimer.yLedPOL == 0)
		{
			g_RTC_TimerFlag.Flags.yLedPOLFlag = TRUE;
		}
	}

	if(g_RTC_SystemTimer.yLedTest > 0)
	{
		g_RTC_SystemTimer.yLedTest--;

		if(g_RTC_SystemTimer.yLedTest == 0)
		{
			g_RTC_TimerFlag.Flags.yLedTestFlag = TRUE;
		}
	}

	if(g_RTC_SystemTimer.yLedSDcard > 0)
	{
		g_RTC_SystemTimer.yLedSDcard--;

		if(g_RTC_SystemTimer.yLedSDcard == 0)
		{
			g_RTC_TimerFlag.Flags.yLedSDcardFlag = TRUE;
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


	if(g_RTC_TimerFlag.Flags.yLedM2MFlag)
	{
		g_RTC_TimerFlag.Flags.yLedM2MFlag = FALSE;
		UIif_SetBlinkLED(UI_LED_M2M);
	}

	if(g_RTC_TimerFlag.Flags.yLedCONTFlag)
	{
		g_RTC_TimerFlag.Flags.yLedCONTFlag = FALSE;
		UIif_SetBlinkLED(UI_LED_CONT);
	}

	if(g_RTC_TimerFlag.Flags.yLedPOLFlag)
	{
		g_RTC_TimerFlag.Flags.yLedPOLFlag = FALSE;
		UIif_SetBlinkLED(UI_LED_POL);
	}

	if(g_RTC_TimerFlag.Flags.yLedTestFlag)
	{
		g_RTC_TimerFlag.Flags.yLedTestFlag = FALSE;
		UIif_SetBlinkLED(UI_LED_TEST);
	}

	if(g_RTC_TimerFlag.Flags.yLedSDcardFlag)
	{
		g_RTC_TimerFlag.Flags.yLedSDcardFlag = FALSE;
		UIif_SetBlinkLED(UI_LED_SDCARD);
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
	if (UI_LED_M2M == wLED)
	{
		g_RTC_SystemTimer.yLedM2M = yTime;
 
		if (0 == yTime)
		{
			g_RTC_TimerFlag.Flags.yLedM2MFlag = FALSE;
		}
	}
	else if (UI_LED_CONT == wLED)
	{
		g_RTC_SystemTimer.yLedCONT = yTime;

		if (0 == yTime)
		{
			g_RTC_TimerFlag.Flags.yLedCONTFlag = FALSE;
		}
	}
	else if (UI_LED_POL == wLED)
	{
		g_RTC_SystemTimer.yLedPOL = yTime;

		if (0 == yTime)
		{
			g_RTC_TimerFlag.Flags.yLedPOLFlag = FALSE;
		}
	}
	else if (UI_LED_TEST == wLED)
	{
		g_RTC_SystemTimer.yLedTest = yTime;

		if (0 == yTime)
		{
			g_RTC_TimerFlag.Flags.yLedTestFlag = FALSE;
		}
	}
	else if (UI_LED_SDCARD == wLED)
	{
		g_RTC_SystemTimer.yLedSDcard = yTime;

		if (0 == yTime)
		{
			g_RTC_TimerFlag.Flags.yLedSDcardFlag = FALSE;
		}
	}
}

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
BYTE RTCif_GetLEDTimer(BYTE yLED)
{
	if (UI_LED_M2M == yLED)
	{
		return g_RTC_SystemTimer.yLedM2M;
	}
	else if (UI_LED_CONT == yLED)
	{
		return g_RTC_SystemTimer.yLedCONT;
	}
	else if (UI_LED_POL == yLED)
	{
		return g_RTC_SystemTimer.yLedPOL;
	}
	else if (UI_LED_TEST == yLED)
	{
		return g_RTC_SystemTimer.yLedTest;
	}
	else
	{
		return g_RTC_SystemTimer.yLedSDcard;
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
DWORD RTCif_GetTimeStamp(void)
{
	return g_RTC_CurrentTime;
}

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
void RTCif_SetTimeStamp(DWORD dwTime)
{
	g_RTC_CurrentTime = dwTime;
}
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
DWORD RTC_DateToTimeStamp(RTC_DATE_FORMAT tDate)
{
	DWORD dwTime = 0, dwTemp = 0;
	WORD wDay;
	BYTE yYear, yMonth = 1, yMaxDay[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
	BYTE yTemp;

	// Decrease the time zone value
	if (g_RTC_TimeZoneIndex < 48)
	{
		yTemp = g_RTC_TimeZoneIndex;
	}
	else
	{
		yTemp = (48 - (g_RTC_TimeZoneIndex - 48)) % 48;
	}

	tDate.yMin += 15 * (yTemp  % 4);
	tDate.yHour += (yTemp / 4);

	//Check overflow
	while (60 <= tDate.yMin)
	{
		tDate.yMin -= 60;
		tDate.yHour++;
	}

	while (24 <= tDate.yHour)
	{
		tDate.yHour -= 24;
		tDate.yDay++;
	}

	while (yMaxDay[tDate.yMonth - 1] < tDate.yDay)
	{
		tDate.yDay -= yMaxDay[tDate.yMonth - 1];
		tDate.yMonth++;

		while (12 < tDate.yMonth)
		{
			tDate.yMonth -= 12;
			tDate.wYear++;
		}
	}

	if (48 < g_RTC_TimeZoneIndex)
	{
		if (tDate.yHour < 12)
		{
			tDate.yHour += 12;
			
			if (1 == tDate.yDay)
			{
				if (1 == tDate.yMonth)
				{
					tDate.wYear--;
					tDate.yMonth = 12;
					tDate.yDay = 31;
				}
				else
				{
					tDate.yMonth--;
					tDate.yDay = yMaxDay[tDate.yMonth - 1];
				}
			}
			else
			{
				tDate.yDay--;
			}
		}
		else
		{
			tDate.yHour -= 12;
		}
	}
		
	// Year
	if (tDate.wYear < 1970)
	{
		yYear = 0;
	}
	else
	{
		yYear = tDate.wYear - 1970;
	}
	
	dwTime += yYear * RTC_YEAR_TO_SECONDS;

	// Month & Day
	if (yYear > 2)
	{
		wDay = ((yYear - 3) / 4) + 1;

		if (((yYear - 2) % 4) == 0)
		{
			yMaxDay[1] = 29;
		}
	}
	
	while (yMonth != tDate.yMonth)
	{
		wDay += yMaxDay[yMonth - 1];
		yMonth++;
	}

	wDay += (tDate.yDay - 1);

	dwTime += wDay * RTC_DAY_TO_SECONDS;

	// Hour, mins and seconds
	//dwTime += (DWORD)(tDate.yHour * 60 * 60);
	yTemp = tDate.yHour;
	dwTemp = yTemp * 60;
	dwTemp = dwTemp * 60;
	dwTime += dwTemp;
	
	dwTime += tDate.yMin * 60;
	dwTime += tDate.ySec;

	return dwTime;
}

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
)
{
	DWORD dwSecond, dwDay;
	WORD wYear, wDate;
	BYTE yMonth = 1, yMaxDay[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
	BYTE yTemp;

	// Get the year
	wYear = 0;
	dwDay = (dwTime / RTC_DAY_TO_SECONDS);
	wDate = dwDay % 7;

	pDate->yWeekDate = (wDate + 4) % 7;		// 1970/1/1 is Thursday.
	
	while (dwDay >= 365)
	{
		if ((wYear % 4) == 2)
		{
			if (dwDay == 365)
			{
				break;
			}
			else
			{
				dwDay -= 366;
			}
		}
		else
		{
			dwDay -= 365;
		}

		wYear++;
	}

	if ((wYear % 4) == 2)
	{
		yMaxDay[1] = 29;
	}

	pDate->wYear = 1970 + wYear;

	// Get the month & day
	while (dwDay >= yMaxDay[yMonth - 1])
	{
		dwDay -= yMaxDay[yMonth - 1];
		yMonth++;
	}

	if (yMonth > 12)
	{
		wYear++;
		yMonth -= 12;
	}
	
	pDate->yMonth = yMonth;
	pDate->yDay = (BYTE)dwDay + 1;

	// The time value
	dwSecond = dwTime % RTC_DAY_TO_SECONDS;
	pDate->yHour = dwSecond / RTC_HOUR_TO_SECONDS;
	pDate->yMin = (dwSecond % RTC_HOUR_TO_SECONDS) / 60;
	pDate->ySec = (dwSecond % RTC_HOUR_TO_SECONDS) % 60;

	// Add the time zone value
	if (g_RTC_TimeZoneIndex < 48)
	{
		yTemp = 48 - g_RTC_TimeZoneIndex;
	}
	else
	{
		yTemp = g_RTC_TimeZoneIndex - 48;
	}

	pDate->yMin += (15 * (yTemp % 4));
	pDate->yHour += (yTemp / 4);

	//Check overflow
	while (60 <= pDate->yMin)
	{
		pDate->yMin -= 60;
		pDate->yHour++;
	}

	while (24 <= pDate->yHour)
	{
		pDate->yHour -= 24;
		pDate->yDay++;
		pDate->yWeekDate++;

		while (yMaxDay[pDate->yMonth - 1] < pDate->yDay)
		{
			pDate->yDay -= yMaxDay[pDate->yMonth - 1];
			pDate->yMonth++;

			while (12 < pDate->yMonth)
			{
				pDate->yMonth -= 12;
				pDate->wYear++;
			}
		}
	}

	if (g_RTC_TimeZoneIndex < 48)
	{
		if (pDate->yHour < 12)
		{
			pDate->yHour += 12;
			
			if (1 == pDate->yDay)
			{
				if (1 == pDate->yMonth)
				{
					pDate->wYear--;
					pDate->yMonth = 12;
					pDate->yDay = 31;
				}
				else
				{
					pDate->yMonth--;
					pDate->yDay = yMaxDay[pDate->yMonth - 1];
				}
			}
			else
			{
				pDate->yDay--;
			}

			if (pDate->yWeekDate == 0)
			{
				pDate->yWeekDate = 6;
			}
			else
			{
				pDate->yWeekDate--;
			}
		}
		else
		{
			pDate->yHour -= 12;
		}
	}
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
)
{
	BYTE yLength;
	RTC_DATE_FORMAT t_Date;

	t_Date.wYear = 2013;
	t_Date.yMonth = 1;
	t_Date.yDay = 1;
	t_Date.yWeekDate = 1;
	t_Date.yHour = 0;
	t_Date.yMin = 0;
	t_Date.ySec = 0;
	
	RTC_TimeStampToDate(
		dwTimeStamp,
		&t_Date
	);

	switch(t_Date.yMonth)
	{
		case 1:
		{
			if (t_Setting.ySimplifyMonth)
			{
				strcpy(pDate, "JAN ");
			}
			else
			{
				strcpy(pDate, "January ");
			}
		}
			break;

		case 2:
		{
			if (t_Setting.ySimplifyMonth)
			{
				strcpy(pDate, "FEB ");
			}
			else
			{
				strcpy(pDate, "February ");
			}
		}
			break;

		case 3:
		{
			if (t_Setting.ySimplifyMonth)
			{
				strcpy(pDate, "MAR ");
			}
			else
			{
				strcpy(pDate, "March ");
			}
		}
			break;

		case 4:
		{
			if (t_Setting.ySimplifyMonth)
			{
				strcpy(pDate, "APR ");
			}
			else
			{
				strcpy(pDate, "April ");
			}
		}
			break;

		case 5:
		{
			if (t_Setting.ySimplifyMonth)
			{
				strcpy(pDate, "MAY ");
			}
			else
			{
				strcpy(pDate, "May ");
			}
		}
			break;

		case 6:
		{
			if (t_Setting.ySimplifyMonth)
			{
				strcpy(pDate, "JUN ");
			}
			else
			{
				strcpy(pDate, "June ");
			}
		}
			break;

		case 7:
		{
			if (t_Setting.ySimplifyMonth)
			{
				strcpy(pDate, "JUL ");
			}
			else
			{
				strcpy(pDate, "July ");
			}
		}
			break;

		case 8:
		{
			if (t_Setting.ySimplifyMonth)
			{
				strcpy(pDate, "AUG ");
			}
			else
			{
				strcpy(pDate, "August ");
			}
		}
			break;

		case 9:
		{
			if (t_Setting.ySimplifyMonth)
			{
				strcpy(pDate, "SEP ");
			}
			else
			{
				strcpy(pDate, "September ");
			}
		}
			break;

		case 10:
		{
			if (t_Setting.ySimplifyMonth)
			{
				strcpy(pDate, "OCT ");
			}
			else
			{
				strcpy(pDate, "October ");
			}
		}
			break;

		case 11:
		{
			if (t_Setting.ySimplifyMonth)
			{
				strcpy(pDate, "NOV ");
			}
			else
			{
				strcpy(pDate, "November ");
			}
		}
			break;

		case 12:
		{
			if (t_Setting.ySimplifyMonth)
			{
				strcpy(pDate, "DEC ");
			}
			else
			{
				strcpy(pDate, "December ");
			}
		}
			break;

		default:
			break;
	}

	yLength = strlen(pDate);
	
	pDate[yLength++] = (t_Date.yDay / 10) + '0';
	pDate[yLength++] = (t_Date.yDay % 10) + '0';

	if (TRUE == t_Setting.yWeekDay)
	{
		switch(t_Date.yWeekDate)
		{
			case 0:
			{
				strcat(pDate, " SUN");
			}
				break;

			case 1:
			{
				strcat(pDate, " MON");
			}
				break;

			case 2:
			{
				strcat(pDate, " TUE");
			}
				break;

			case 3:
			{
				strcat(pDate, " WED");
			}
				break;

			case 4:
			{
				strcat(pDate, " THU");
			}
				break;

			case 5:
			{
				strcat(pDate, " FRI");
			}
				break;

			case 6:
			{
				strcat(pDate, " SAT");
			}
				break;

			default:
				break;
		}
	}

	if (TRUE == t_Setting.yYear)
	{
		strcat(pDate, ", ");

		yLength = strlen(pDate);
		ItoStr(
			t_Date.wYear,
			&pDate[yLength],
			0
		);
	}
}

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
)
{
	BYTE yTemp, yPM;
	RTC_DATE_FORMAT t_Date;

	RTC_TimeStampToDate(
		dwTimeStamp,
		&t_Date
	);

	if (t_Date.yHour >= 12)
	{
		yTemp = t_Date.yHour - 12;
		yPM = TRUE;
	}
	else
	{
		yTemp = t_Date.yHour;
		yPM = FALSE;
	}

	if (yTemp == 0)
	{
		yTemp = 12;
	}
	
	pTime[0] = '0' + (yTemp / 10);
	pTime[1] = '0' + (yTemp % 10);
		
	pTime[2] = ':';

	if (t_Date.yMin < 10)
	{
		pTime[3] = '0';
		pTime[4] = '0' + t_Date.yMin;
	}
	else
	{
		pTime[3] = '0' + (t_Date.yMin / 10);
		pTime[4] = '0' + (t_Date.yMin % 10);
	}

	pTime[5] = ':';

	if(t_Date.ySec < 10)
	{
		pTime[6] = '0';
		pTime[7] = '0'  + t_Date.ySec;
	}
	else
	{
		pTime[6] = '0' + (t_Date.ySec/10);
		pTime[7] = '0' + (t_Date.ySec % 10);
	}

	pTime[8] = ' ';

	if (yPM == TRUE)
	{
		pTime[9] = 'P';
	}
	else
	{
		pTime[9] = 'A';
	}
	
	pTime[10] = 'M';
}

