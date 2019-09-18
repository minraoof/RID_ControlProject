//*****************************************************************************
// INCLUDE FILES
//*****************************************************************************
#include "UserCmd.h"
#include "Version.h"
#include "Utility.h"
#include "USB_Module.h"
#include "UI.h"
#include "Module.h"
#include "RTC.h"
#include "Power.h"
#include "stm32f1xx_hal.h"
#include <stdlib.h>
#include <string.h>



//*****************************************************************************
// MACROS
//*****************************************************************************
#define USERCMD_RESPONSE_LENGTH				256


//*****************************************************************************
// VARIABLES
//*****************************************************************************
CHAR g_UserCmd_Response[USERCMD_RESPONSE_LENGTH];
BYTE g_UserCmd_ATCommand = TRUE, g_UserCmd_SendType;

//*****************************************************************************
// FUNCTION DEFINITION
//*****************************************************************************
/********************************************************************
 * Function:		UserCmd_ParsingCmd()
 *
 * PreCondition:	None
 *
 * Input:			CHAR* pCmd: The input command
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Parsing the command.
 *
 * Note:			None
 *******************************************************************/
BYTE UserCmd_ParsingCmd(
 	CHAR* pCmd,
 	WORD wSend
 )
{
	StrUpperCase(
		pCmd,
		2
	);

	if (0 == StrCmpN(pCmd, "AT+", 3))
	{
		WORD wCmd;

		memset(g_UserCmd_Response, 0x0, USERCMD_RESPONSE_LENGTH);

		wCmd = UserCmd_ParsingATCommand(&pCmd[3]);
		g_UserCmd_SendType = wSend;

		if (wCmd != USERCMD_CMD_NONE)
		{
			UserCmd_HandleATCommand(
				&pCmd[3],
				wCmd
			);
		}
		else
		{
			strcpy(g_UserCmd_Response, "ERROR");
		}

		if (USERCMD_SEND_USB == wSend)
		{
			USB_PrintMsg(g_UserCmd_Response);
		}
	}
	else
	{
		memset(g_UserCmd_Response, 0x0, USERCMD_RESPONSE_LENGTH);
		strcpy(g_UserCmd_Response, "ERROR");

		if (USERCMD_SEND_USB == wSend)
		{
			USB_PrintMsg(g_UserCmd_Response);
		}
	}

	return TRUE;
}


/********************************************************************
 * Function:		UserCmd_ParsingATCommand()
 *
 * PreCondition:	None
 *
 * Input:			CHAR* pCmd: The input command
 *
 * Output:		WORD: The command type.
 *
 * Side Effects:	None
 *
 * Overview:		Parsing the AT command and return the command type value.
 *
 * Note:			None
 *******************************************************************/
WORD UserCmd_ParsingATCommand(CHAR* pCmd)
{
	BYTE yIndex = 0;
	WORD wCmd = USERCMD_CMD_NONE;

	while (pCmd[yIndex] != 0 &&
		pCmd[yIndex] != '=')
	{
		yIndex++;
	}

	if (yIndex == 0)
	{
		return wCmd;
	}

	StrUpperCase(
		pCmd,
		yIndex
	);

	if (0 == StrCmpN(pCmd, "VER", 3))
	{
		wCmd = USERCMD_CMD_VER;
	}
	else if (0 == StrCmpN(pCmd, "LBAT", 4))
	{
		wCmd = USERCMD_CMD_LBAT;
	}
	else if (pCmd[0] == 'S')
	{		
		if (0 == StrCmpN(&pCmd[1], "YSUP", 4))
		{
			wCmd = USERCMD_CMD_SYSTEMUPTIME;
		}
	}

	return wCmd;
}

/********************************************************************
 * Function:		UserCmd_HandleM3Command()
 *
 * PreCondition:	None
 *
 * Input:			CHAR* pCmd: The input command
 *				WORD wCmd: The command text.
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Handle the M3 command.
 *
 * Note:			None
 *******************************************************************/
BYTE UserCmd_HandleATCommand(
	CHAR* pCmd,
	WORD wCmd
)
{
	BYTE yResult = FALSE;

	switch (wCmd)
	{
		case USERCMD_CMD_VER:
		{
			yResult = UserCmd_VersionCommand(pCmd);
		}
			break;
			
		case USERCMD_CMD_LBAT:
		{
			yResult = UserCmd_LBATCommand(&pCmd[4]);
		}
			break;
			
		case USERCMD_CMD_SYSTEMUPTIME:
		{
			yResult = UserCmd_SystemUptimeCommand(&pCmd[5]);;
		}
			break;

		default:
			break;
	}

	if (FALSE == yResult)
	{
		strcpy(g_UserCmd_Response, "ERROR");
	}

	return yResult;
}

/********************************************************************
 * Function:		UserCmd_VersionCommand()
 *
 * PreCondition:	None
 *
 * Input:			CHAR* pCmd: The input command
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		To return the firmware version.
 *
 * Note:			None
 *******************************************************************/
BYTE UserCmd_VersionCommand(CHAR *pCmd)
{
	if (pCmd[3] != '?' &&
		pCmd[3] != 0)
	{
		return FALSE;
	}
	else if (pCmd[3] == '?' &&
		pCmd[4] != 0)
	{
		return FALSE;
	}
	else
	{
		strcpy(g_UserCmd_Response, "VijayPrj Ver ");
		ItoStr(VERSION_HARDWARE_NUMBER, &g_UserCmd_Response[strlen(g_UserCmd_Response)], 0);
		strcat(g_UserCmd_Response, ".");
		ItoStr(VERSION_MAIN_NUMBER, &g_UserCmd_Response[strlen(g_UserCmd_Response)], 0);
		strcat(g_UserCmd_Response, ".");
		ItoStr(VERSION_SUB_NUMBER, &g_UserCmd_Response[strlen(g_UserCmd_Response)], 0);
		g_UserCmd_Response[strlen(g_UserCmd_Response)] = VERSION_SUB_CHAR;
	}

	return TRUE;
}

/********************************************************************
 * Function:		UserCmd_LBATCommand()
 *
 * PreCondition:	None
 *
 * Input:			CHAR* pCmd: The input command
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		The WBS LBAT function. It can query and set the low battery
 *				threshold.
 *
 * Note:			None
 *******************************************************************/
BYTE UserCmd_LBATCommand(CHAR* pCmd)
{
	BYTE yResult = FALSE;
	WORD wLBAT;
	
	if (pCmd[0] == 0 ||
		(pCmd[0] == '?' &&
		pCmd[1] == 0))
	{
		CHAR *pVot;
		
		wLBAT = Powerif_GetLowBatteryThreshold();

		g_UserCmd_Response[0] = (wLBAT / 100) + '0';
		g_UserCmd_Response[1] = '.';

		ItoStr(
			(wLBAT % 100),
			&g_UserCmd_Response[2],
			0
		);

		strcat(g_UserCmd_Response, "TV, ");

		pVot = Powerif_GetVoltageValue();

		if (NULL != pVot)
		{
			strcat(g_UserCmd_Response, pVot);
			strcat(g_UserCmd_Response, "CV");
			FREE(pVot);
		}
	}
	else if (pCmd[0] == '=')
	{
		do
		{
			if (pCmd[2] != '.' &&
				pCmd[2] != 0)
			{
				break;
			}

			if (pCmd[1] < '0' ||
				pCmd[1] > '9')
			{
				break;
			}
			
			wLBAT = pCmd[1] - '0';
			wLBAT *= 100;

			if (pCmd[2] != 0)
			{
				if (pCmd[3] != 0)
				{
					if (pCmd[3] < '0' ||
						pCmd[3] > '9')
					{
						break;
					}
					
					wLBAT += (pCmd[3] - '0') * 10;

					if (pCmd[4] != 0)
					{
						if (pCmd[5] != 0)
						{
							break;
						}

						if (pCmd[4] < '0' ||
							pCmd[4] > '9')
						{
							break;
						}

						wLBAT += (pCmd[4] - '0');
					}
				}
			}
			
			if (wLBAT > 330)
			{
				break;
			}

			wLBAT *= 12.412;

			yResult = TRUE;
		}while (0);

		if (yResult == TRUE)
		{
			strcpy(g_UserCmd_Response, "OK");
			Powerif_SetLowBatteryThreshold(wLBAT);
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

/********************************************************************
 * Function:		UserCmd_SystemUptimeCommand()
 *
 * PreCondition:	None
 *
 * Input:			CHAR* pCmd: The input command
 *
 * Output:			BYTE
 *
 * Side Effects:	None
 *
 * Overview:		Handle the Sys up time command
 *
 * Note:			None
 *******************************************************************/
BYTE UserCmd_SystemUptimeCommand(CHAR* pCmd)
{
	if (pCmd[0] == 0)
	{
		DWORD dwTime = RTCif_GetSystemUptime();
		
		RTCif_TimeSecondsToString(dwTime, g_UserCmd_Response);
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}
