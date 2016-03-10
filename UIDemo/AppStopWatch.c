#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"
#include "AppFrame.h"

#define BUTTON_ID_CLEAR		0
#define BUTTON_ID_MULTI		1
#define BUTTON_ID_EXIT		2

static BUTTON_Handle _ahButton[3];
/*********************************************************************
*
*       Data structure
*
**********************************************************************
*/
typedef struct SWTime
{
	int min;
	int sec;
	int ms;
}SWTime;

typedef enum
{
	STOPWATCH_INIT = 0,
	STOPWATCH_RUN,
	STOPWATCH_STOP,
	STOPWATCH_NUM
}STOPWATCH_MODE;

/*********************************************************************
*
*       Static variables
*
**********************************************************************/
static SWTime STime = { 0 };
static U8 ModeFlag = STOPWATCH_INIT;
static U8 OneSecflag = 0; //1s flag

/*********************************************************************
*
*       Static functions
*
**********************************************************************/
/*********************************************************************
*
*       GetTime
*/
static time_t GetTime(void)
{
	time_t timestamp;

	timestamp = GetCurrentTimestamp();
	return timestamp;//return system time stamp
}

static void _ShowButton(WM_HWIN hWin)
{
	_ahButton[0] = BUTTON_CreateAsChild(60, 150, 60, 30, hWin, BUTTON_ID_MULTI, WM_CF_SHOW | WM_CF_STAYONTOP | WM_CF_MEMDEV);
	_ahButton[1] = BUTTON_CreateAsChild(200, 150, 60, 30, hWin, BUTTON_ID_CLEAR, WM_CF_SHOW | WM_CF_STAYONTOP | WM_CF_MEMDEV);
	_ahButton[2] = BUTTON_CreateAsChild(10, 10, 30, 25, hWin, BUTTON_ID_EXIT, WM_CF_SHOW | WM_CF_STAYONTOP | WM_CF_MEMDEV);

	BUTTON_SetText(_ahButton[1], "REST");
	BUTTON_SetText(_ahButton[2], "EXIT");
}

/*********************************************************************
*
*       Static code
*
**********************************************************************/
static void _cbStopWatch(WM_MESSAGE *pMsg)
{
	int NCode, Id;

	switch (pMsg->MsgId)
	{
	case WM_PAINT:
		GUI_SetBkColor(GUI_WHITE);
		GUI_Clear();
		GUI_SetColor(GUI_BLACK);

		GUI_SetFont(&GUI_FontD48);
		GUI_GotoXY(105, 70);
		GUI_DispString(":");
		GUI_GotoXY(200, 70);
		GUI_DispString(".");

		_ShowButton(pMsg->hWin);
		switch (ModeFlag)
		{
		case STOPWATCH_INIT:
			GUI_SetTextMode(GUI_TM_NORMAL);
			GUI_SetFont(&GUI_FontD48);
			GUI_DispDecAt(STime.min, 30, 75, 2);
			GUI_DispDecAt(STime.sec, 125, 75, 2);
			GUI_DispDecAt(STime.ms, 220, 75, 2);
			BUTTON_SetText(_ahButton[0], "Start");
			break;
		case STOPWATCH_STOP:
			GUI_SetTextMode(GUI_TM_NORMAL);
			GUI_SetFont(&GUI_FontD48);
			GUI_DispDecAt(STime.min, 30, 75, 2);
			GUI_DispDecAt(STime.sec, 125, 75, 2);
			GUI_DispDecAt(STime.ms, 220, 75, 2);
			BUTTON_SetText(_ahButton[0], "Continue");
			break;
		case STOPWATCH_RUN:
			GUI_SetTextMode(GUI_TM_NORMAL);
			GUI_SetFont(&GUI_FontD48);
			GUI_DispDecAt(STime.min, 30, 75, 2);
			GUI_DispDecAt(STime.sec, 125, 75, 2);
			GUI_DispDecAt(STime.ms, 220, 75, 2);
			BUTTON_SetText(_ahButton[0], "Stop");
			break;
		default:
			break;
		}
		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
		NCode = pMsg->Data.v;            /* Notification code */
		switch (NCode)
		{
		case WM_NOTIFICATION_CLICKED:
			switch (Id)
			{
			case BUTTON_ID_MULTI:
				switch (ModeFlag)
				{
				case STOPWATCH_INIT:
					ModeFlag = STOPWATCH_RUN;
					WM_InvalidateWindow(pMsg->hWin);
					break;
				case STOPWATCH_RUN:
					ModeFlag = STOPWATCH_STOP;
					WM_InvalidateWindow(pMsg->hWin);
					break;
				case STOPWATCH_STOP:
					ModeFlag = STOPWATCH_RUN;
					WM_InvalidateWindow(pMsg->hWin);
					break;
				default:
					break;
				}
				break;
			case BUTTON_ID_CLEAR:
				STime.min = STime.ms = STime.sec = 0;
				ModeFlag = STOPWATCH_INIT;
				WM_InvalidateWindow(pMsg->hWin);
				break;
			case BUTTON_ID_EXIT:
				GUI_SetAppIndex(APP_MainMenu);
				GUI_SetAppDirection(MEMDEV_ANIMATION_LEFT);
				_MoveShiftWindow(&pMsg->hWin, MEMDEV_ANIMATION_RIGHT, 1, 1);
				GUI_SetCancel(APP_StopWatch);
				GUI_ClearHalt();
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	default:
		WM_DefaultProc(pMsg);
	}
}

/*********************************************************************
*Function name	: STOPWATCH_UI
*Parameters     : NULL
*Return value	: NULL
*Description	: ui of Stop Watch
**********************************************************************/
static void StopWatch_UI(void)
{
	switch (ModeFlag)
	{
	case STOPWATCH_RUN:
		STime.ms++;
		if ((STime.ms >= 100) || (1 == OneSecflag))
		{
			STime.sec++;
			STime.ms = 0;
			if (STime.sec >= 60)
			{
				STime.min++;
				STime.sec = 0;
				if (STime.min >= 100)
				{
					STime.ms = 99;
					STime.sec = 59;
					STime.min = 99;
				}
			}
		}
		GUI_SetBkColor(GUI_WHITE);
		GUI_SetColor(GUI_BLACK);
		GUI_SetTextMode(GUI_TM_NORMAL);
		GUI_SetFont(&GUI_FontD48);
		GUI_DispDecAt(STime.min, 30, 75, 2);
		GUI_DispDecAt(STime.sec, 125, 75, 2);
		GUI_DispDecAt(STime.ms, 220, 75, 2);
		break;
	default:
		break;
	}
}

/*********************************************************************
*
*       Public code
*
**********************************************************************/

/*******************************************************************
*
*       MainTask
*
********************************************************************
*/
void MainTask_AppStopWatch(void)
{
	WM_HWIN hWinStopWatch;

	GUI_Init();
	ModeFlag = STOPWATCH_INIT;

	hWinStopWatch = WM_CreateWindow(320, 0, 320, 240, WM_CF_SHOW, _cbStopWatch, 0);
	WM_SetFocus(hWinStopWatch);
	_MoveShiftWindow(&hWinStopWatch, MEMDEV_ANIMATION_LEFT, WM_Shift_ToLCD, 0);
	WM_EnableMemdev(hWinStopWatch);

	while (!GUI_CheckCancel(APP_StopWatch))
	{
		GUI_Delay(10);
		if (ModeFlag == STOPWATCH_RUN)
		{
			STime.ms++;
			if ((STime.ms >= 100) || (1 == OneSecflag))
			{
				STime.sec++;
				STime.ms = 0;
				if (STime.sec >= 60)
				{
					STime.min++;
					STime.sec = 0;
					if (STime.min >= 100)
					{
						STime.ms = 99;
						STime.sec = 59;
						STime.min = 99;
					}
				}
			}
			WM_InvalidateWindow(hWinStopWatch);
		}
	}
	WM_DeleteWindow(hWinStopWatch);
	hWinStopWatch = 0;
}
