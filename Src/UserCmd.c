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

#include "SDCard.h"
#include "RTC.h"
#include "SDCard.h"


//*****************************************************************************
// MACROS
//*****************************************************************************
#define USERCMD_RESPONSE_LENGTH				256

#define IsUpper(c)	(((c)>='A')&&((c)<='Z'))
#define IsLower(c)	(((c)>='a')&&((c)<='z'))
#define IsDigit(c)	(((c)>='0')&&((c)<='9'))

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
	else if (0 == StrCmpN(pCmd, "TIMESTAMP", 9))
	{
		wCmd =  USERCMD_CMD_TIMESTAMP;
	}
	else if (pCmd[0] == 'S')
	{		
		if (0 == StrCmpN(&pCmd[1], "YSTEMUPTIME", 11))
		{
			wCmd = USERCMD_CMD_SYSTEMUPTIME;
		}
		else if (0 == StrCmpN(&pCmd[1], "DCHKSIZE", 8))
		{
			wCmd = USERCMD_CMD_SDCHKSIZE;
		}
		else if (0 == StrCmpN(&pCmd[1], "DMKFILE", 7))
		{
			wCmd = USERCMD_CMD_SDMKFILE;
		}
		else if (0 == StrCmpN(&pCmd[1], "DWRFILE", 7))
		{
			wCmd = USERCMD_CMD_SDWRFILE;
		}		
		else if (0 == StrCmpN(&pCmd[1], "DRDFILE", 7))
		{
			wCmd = USERCMD_CMD_SDRDFILE;
		}
		else if (0 == StrCmpN(&pCmd[1], "DRMFILE", 7))
		{
			wCmd = USERCMD_CMD_SDRMFILE;
		}
		else if (0 == StrCmpN(&pCmd[1], "DSKWRFILE", 9))
		{
			wCmd = USERCMD_CMD_SDSKWRFILE;
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
			yResult = UserCmd_SystemUptimeCommand(&pCmd[12]);
		}
			break;

		case USERCMD_CMD_TIMESTAMP:
		{
			yResult = UserCmd_TimeStampCommand(&pCmd[9]);
		}
			break;

		case USERCMD_CMD_SDCHKSIZE:
		{
			yResult =UserCmd_SDChksizeCommand(&pCmd[9]);
		}
			break;

		case USERCMD_CMD_SDMKFILE:
		{
			yResult =UserCmd_SDmkfileCommand(&pCmd[8]);
		}
			break;

		case USERCMD_CMD_SDWRFILE:
		{
			yResult =UserCmd_SDwrfileCommand(&pCmd[8]);
		}
			break;

		case USERCMD_CMD_SDRDFILE:
		{
			yResult =UserCmd_SDrdfileCommand(&pCmd[8]);
		}
			break;

		case USERCMD_CMD_SDRMFILE:
		{
			yResult =UserCmd_SDrmfileCommand(&pCmd[8]);
		}
			break;

		case USERCMD_CMD_SDSKWRFILE:
		{
			yResult =UserCmd_SDskwrfileCommand(&pCmd[10]);
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
		strcpy(g_UserCmd_Response, "RIDControl_Ver ");
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
	if (pCmd[0] == 0 ||
		(pCmd[0] == '?' &&
		pCmd[1] == 0))
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

/********************************************************************
 * Function:		UserCmd_TimeStampCommand()
 *
 * PreCondition:	None
 *
 * Input:			CHAR* pCmd: The input command
 *
 * Output:			BYTE
 *
 * Side Effects:	None
 *
 * Overview:		Handle the Current Time command time command
 *
 * Note:			None
 *******************************************************************/
BYTE UserCmd_TimeStampCommand(CHAR* pCmd)
{
	BYTE yResult = FALSE;

	if (pCmd[0] == 0 ||
	(pCmd[0] == '?' &&
	pCmd[1] == 0))
	{
		RTC_Date_String_Setting t_Setting;
		DWORD dwTime = RTCif_GetTimeStamp();
		BYTE yLength;

		t_Setting.ySimplifyMonth = TRUE;
		t_Setting.yWeekDay = TRUE;
		t_Setting.yYear = FALSE;
		
		RTCif_GetDateString(
			g_UserCmd_Response,
			dwTime,
			t_Setting);

		yLength= strlen(g_UserCmd_Response);
		g_UserCmd_Response[yLength++] = ' ';

		RTCif_GetTimeString(
			&g_UserCmd_Response[yLength],
			dwTime);

		yResult = TRUE;
	}
	else if (pCmd[0] == '=')
	{	
		BYTE yIndex = 1;
		DWORD dwTimeVal = 0;

		do
		{
			while(pCmd[yIndex] != 0)
			{
				if (pCmd[yIndex] < '0' ||
					pCmd[yIndex] > '9')
				{
					break;
				}
					
				yIndex++;
			}

			if (0 != pCmd[yIndex])
			{
				break;
			}

			dwTimeVal = StoI(&pCmd[1]);

			if(dwTimeVal < 1325376000)
			{
				break;
			}

			RTCif_SetTimeStamp(dwTimeVal);
			
			strcpy(g_UserCmd_Response, "OK");
			yResult = TRUE;
			
		}while(0);
	}
	else
	{
		return FALSE;
	}

	return yResult;
}

/********************************************************************
 * Function:		UserCmd_SDChksizeCommand()
 *
 * PreCondition:	None
 *
 * Input:			CHAR* pCmd: The input command
 *
 * Output:			BYTE
 *
 * Side Effects:	None
 *
 * Overview:		Handle the check size command.
 *
 * Note:			None
 *******************************************************************/
BYTE UserCmd_SDChksizeCommand(CHAR* pCmd)
{
	BYTE yResult = FALSE;

	if (pCmd[0] == 0 ||
	(pCmd[0] == '?' &&
	pCmd[1] == 0))
	{
		//SDCard_CheckSize();
		strcpy(g_UserCmd_Response, "Free Size = ");

		ItoStr(SDCard_GetFreeSize(),
				&g_UserCmd_Response[strlen(g_UserCmd_Response)],
				0);

		strcat(g_UserCmd_Response,"\r\nTotal Size = ");

		ItoStr(SDCard_GetTotalSize(),
				&g_UserCmd_Response[strlen(g_UserCmd_Response)],
				0);
		
		yResult = TRUE;
	}
	else
	{
		return FALSE;
	}

	return yResult;
}

/********************************************************************
 * Function:		UserCmd_SDmkfileCommand()
 *
 * PreCondition:	None
 *
 * Input:			CHAR* pCmd: The input command
 *
 * Output:			BYTE
 *
 * Side Effects:	None
 *
 * Overview:		Handle the create file command
 *
 * Note:			None
 *******************************************************************/
BYTE UserCmd_SDmkfileCommand(CHAR* pCmd)
{
	BYTE yResult = FALSE, yLength = 0, yIndex = 0;

	if (pCmd[0] == 0 ||
	(pCmd[0] == '?' &&
	pCmd[1] == 0))
	{
		return FALSE;
	}
	else if (pCmd[0] == '=')
	{
			yIndex = 1;
			yLength = 0;

			while (pCmd[yIndex] != 0)
			{
				if (pCmd[yIndex] >= '0' &&
					pCmd[yIndex] <= '9')
				{
					yIndex++;
					yLength++;
				}
				else if (pCmd[yIndex] >= 'A' &&
					pCmd[yIndex] <= 'Z')
				{
					yIndex++;
					yLength++;
				}
				else if (pCmd[yIndex] >= 'a' &&
					pCmd[yIndex] <= 'z')
				{
					yIndex++;
					yLength++;
				}
				else if (pCmd[yIndex] == '.')
				{
					yIndex++;
					yLength++;
				}
				else
				{
					yIndex = 0;
					break;
				}
			}

			if(yIndex == 0)
			{
				return FALSE;
			}

			yResult = TRUE;

			SDCard_Createfile(&pCmd[1]);
	}
	else
	{
		return FALSE;
	}

	return yResult;

}

/********************************************************************
 * Function:		UserCmd_SDwrfileCommand()
 *
 * PreCondition:	None
 *
 * Input:			CHAR* pCmd: The input command
 *
 * Output:			BYTE
 *
 * Side Effects:	None
 *
 * Overview:		Handle write command for SD card.
 *
 * Note:			None
 *******************************************************************/
BYTE UserCmd_SDwrfileCommand(CHAR* pCmd)
{
	BYTE yResult = FALSE, yIndex = 0 , yLength = 0;
	CHAR pFilename[100], pData[100];

	if (pCmd[0] == 0 ||
	(pCmd[0] == '?' &&
	pCmd[1] == 0))
	{
		return FALSE;
	}
	else if (pCmd[0] == '=')
	{
		memset(pFilename, 0x00, 100);
		memset(pData, 0x00, 100);

		do
		{
			yIndex = 1;
			yLength = 0;

			while (pCmd[yIndex] != 0)
			{
				if (pCmd[yIndex] >= '0' &&
					pCmd[yIndex] <= '9')
				{
					yIndex++;
					yLength++;
				}
				else if (pCmd[yIndex] >= 'A' &&
					pCmd[yIndex] <= 'Z')
				{
					yIndex++;
					yLength++;
				}
				else if (pCmd[yIndex] >= 'a' &&
					pCmd[yIndex] <= 'z')
				{
					yIndex++;
					yLength++;
				}
				else if (pCmd[yIndex] == '.')
				{
					yIndex++;
					yLength++;
				}
				else if(pCmd[yIndex] == ',')
				{
					 Strncpy(pFilename, &pCmd[1], yLength);
					 yIndex++;
					 strcpy(pData, &pCmd[yIndex]);
					 strcat(pData, "\r\n");
					 
					 break;
				}
				else
				{
					yIndex = 0;
					break;
				}
			}

			if(yIndex == 0)
			{
				break;
			}

			yResult = TRUE;

			SDCard_UpdateFile(pFilename, pData);

		}while(0);
	}
	else
	{
		return FALSE;
	}

	return yResult;

}
/********************************************************************
 * Function:		UserCmd_SDskwrfileCommand()
 *
 * PreCondition:	None
 *
 * Input:			CHAR* pCmd: The input command
 *
 * Output:			BYTE
 *
 * Side Effects:	None
 *
 * Overview:		Handle write command for SD card.
 *
 * Note:			None
 *******************************************************************/
BYTE UserCmd_SDskwrfileCommand(CHAR* pCmd)
{

	BYTE yResult = FALSE, yIndex = 0 , yLength = 0;
	BYTE xIndex;
	CHAR pFilename[100], pData[100];
	WORD wSekIndex;

	if (pCmd[0] == 0 ||
	(pCmd[0] == '?' &&
	pCmd[1] == 0))
	{
		return FALSE;
	}
	else if (pCmd[0] == '=')
	{
		memset(pFilename, 0x00, 100);
		memset(pData, 0x00, 100);

		do
		{
			yIndex = 1;
			yLength = 0;

			while (pCmd[yIndex] != 0)
			{
				if (pCmd[yIndex] >= '0' &&
					pCmd[yIndex] <= '9')
				{
					yIndex++;
					yLength++;
				}
				else if (pCmd[yIndex] >= 'A' &&
					pCmd[yIndex] <= 'Z')
				{
					yIndex++;
					yLength++;
				}
				else if (pCmd[yIndex] >= 'a' &&
					pCmd[yIndex] <= 'z')
				{
					yIndex++;
					yLength++;
				}
				else if (pCmd[yIndex] == '.')
				{
					yIndex++;
					yLength++;
				}
				else if(pCmd[yIndex] == ',')
				{
					 Strncpy(pFilename, &pCmd[1], yLength);
					 yIndex++;
					 //strcpy(pData, &pCmd[yIndex]);					 
					 break;
				}
				else
				{
					yIndex = 0;
					break;
				}
			}			

			if(yIndex == 0)
			{
				break;
			}
			
			yLength = 0;
			xIndex = yIndex;

			while (pCmd[yIndex] != 0)
			{
				if(pCmd[yIndex] == ',')
				{
					 Strncpy(pData, &pCmd[xIndex], yLength);
					 yIndex++;
					 //strcpy(pData, &pCmd[yIndex]);					 
					 break;
				}
				else
				{
					yIndex++;
					yLength++;
				}
			}
			
			if(yLength == 0)
			{
				break;
			}
			
			yLength = 0;
			xIndex = yIndex;
			
			while (pCmd[yIndex] != 0)
			{
				if (pCmd[yIndex] >= '0' &&
					pCmd[yIndex] <= '9')
				{
					yIndex++;
					yLength++;
				}
				else
				{
					yIndex = 0;
					break;
				}
			}
			
			if(yIndex == 0)
			{
				break;
			}

			wSekIndex = StoI(&pCmd[xIndex]);		

			yResult = TRUE;

			
			SDCard_SeekWriteFile (pFilename, pData, wSekIndex, strlen(pData));

		}while(0);
	}
	else
	{
		return FALSE;
	}

	return yResult;
}
/********************************************************************
 * Function:		UserCmd_SDrdfileCommand()
 *
 * PreCondition:	None
 *
 * Input:			CHAR* pCmd: The input command
 *
 * Output:			BYTE
 *
 * Side Effects:	None
 *
 * Overview:		Handle read command for SD card
 *
 * Note:			None
 *******************************************************************/
BYTE UserCmd_SDrdfileCommand(CHAR* pCmd)
{
	BYTE yResult = FALSE;

	if (pCmd[0] == 0 ||
	(pCmd[0] == '?' &&
	pCmd[1] == 0))
	{
		return FALSE;
	}
	else if (pCmd[0] == '=')
	{
		SDCard_ReadFile(&pCmd[1], &g_UserCmd_Response[0]);

		yResult = TRUE;
	}
	else
	{
		return FALSE;
	}

	return yResult;

}

/********************************************************************
 * Function:		UserCmd_SDrmfileCommand()
 *
 * PreCondition:	None
 *
 * Input:			CHAR* pCmd: The input command
 *
 * Output:			BYTE
 *
 * Side Effects:	None
 *
 * Overview:		Handle delete command for SD card
 *
 * Note:			None
 *******************************************************************/
BYTE UserCmd_SDrmfileCommand(CHAR* pCmd)
{
	BYTE yResult = FALSE;

	if (pCmd[0] == 0 ||
	(pCmd[0] == '?' &&
	pCmd[1] == 0))
	{
		return FALSE;
	}
	else if (pCmd[0] == '=')
	{
		SDCard_Removefile(&pCmd[1]);
		
		yResult = TRUE;
	}
	else
	{
		return FALSE;
	}

	return yResult;

}
