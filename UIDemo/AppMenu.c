#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"
#include "AppFrame.h"
#include "AppCommon.h"

#define BUTTON_ID_CALENDAR		0
#define BUTTON_ID_STOPWATCH		1
#define BUTTON_ID_CLOCK			2
#define BUTTON_ID_LED			3
#define BUTTON_ID_CALCULATOR	4
#define BUTTON_ID_FILESYSTEM	5


extern GUI_CONST_STORAGE GUI_BITMAP bmcalendar;
extern GUI_CONST_STORAGE GUI_BITMAP bmbackground;
extern GUI_CONST_STORAGE GUI_BITMAP bmSecWatch;
extern GUI_CONST_STORAGE GUI_BITMAP bmclock;
extern GUI_CONST_STORAGE GUI_BITMAP bmcalculator;
extern GUI_CONST_STORAGE GUI_BITMAP bmfileSystem;
extern GUI_CONST_STORAGE GUI_BITMAP bmled;

//typedef enum
//{
//	APP_CALENDAR = 0,
//	APP_STOPWATCH,
//	APP_CLOCK,
//	APP_LED,
//	APP_CALCULATOR,
//	APP_FILESYSTEM,
//	APP_NUM
//}BUTTONMEAN;

static BUTTON_Handle   _ahButton[6];

static _ShowButton(WM_HWIN hWin)
{
	_ahButton[0] = BUTTON_CreateAsChild(50, 50, 40, 40, hWin, BUTTON_ID_CALENDAR, WM_CF_SHOW | WM_CF_STAYONTOP | WM_CF_MEMDEV);
	_ahButton[1] = BUTTON_CreateAsChild(140, 50, 40, 40, hWin, BUTTON_ID_STOPWATCH, WM_CF_SHOW | WM_CF_STAYONTOP | WM_CF_MEMDEV);
	_ahButton[2] = BUTTON_CreateAsChild(230, 50, 40, 40, hWin, BUTTON_ID_CLOCK, WM_CF_SHOW | WM_CF_STAYONTOP | WM_CF_MEMDEV);
	_ahButton[3] = BUTTON_CreateAsChild(50, 130, 40, 40, hWin, BUTTON_ID_LED, WM_CF_SHOW | WM_CF_STAYONTOP | WM_CF_MEMDEV);
	_ahButton[4] = BUTTON_CreateAsChild(140, 130, 40, 40, hWin, BUTTON_ID_CALCULATOR, WM_CF_SHOW | WM_CF_STAYONTOP | WM_CF_MEMDEV);
	_ahButton[5] = BUTTON_CreateAsChild(230, 130, 40, 40, hWin, BUTTON_ID_FILESYSTEM, WM_CF_SHOW | WM_CF_STAYONTOP | WM_CF_MEMDEV);

	BUTTON_SetBitmap(_ahButton[0], BUTTON_BI_UNPRESSED, &bmcalendar);
	BUTTON_SetBitmap(_ahButton[1], BUTTON_BI_UNPRESSED, &bmSecWatch);
	BUTTON_SetBitmap(_ahButton[2], BUTTON_BI_UNPRESSED, &bmclock);
	BUTTON_SetBitmap(_ahButton[3], BUTTON_BI_UNPRESSED, &bmled);
	BUTTON_SetBitmap(_ahButton[4], BUTTON_BI_UNPRESSED, &bmcalculator);
	BUTTON_SetBitmap(_ahButton[5], BUTTON_BI_UNPRESSED, &bmfileSystem);
}

static void _cbMenu(WM_MESSAGE *pMsg)
{
	int NCode, Id;
	switch (pMsg->MsgId)
	{
	case WM_PAINT:
		GUI_SetBkColor(GUI_WHITE);
		GUI_Clear();
		GUI_SetColor(GUI_RED);

		GUI_DrawBitmap(&bmbackground, 0, 0);
		_ShowButton(pMsg->hWin);
		GUI_SetFont(&GUI_Font8_ASCII);
		GUI_SetTextMode(GUI_TM_XOR);
		GUI_DispStringHCenterAt("CALENDAR", 70, 100);
		GUI_DispStringHCenterAt("STOPWATCH", 160, 100);
		GUI_DispStringHCenterAt("CLOCK", 250, 100);
		GUI_DispStringHCenterAt("LED", 70, 180);
		GUI_DispStringHCenterAt("CALCULATOR", 160, 180);
		GUI_DispStringHCenterAt("FILESYSTEM", 250, 180);
		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
		NCode = pMsg->Data.v;            /* Notification code */
		switch (NCode) 
		{
		case WM_NOTIFICATION_CLICKED:
			switch (Id)
			{
			case BUTTON_ID_CALENDAR:
				GUI_SetAppIndex(APP_Calendar);
				GUI_SetAppDirection(MEMDEV_ANIMATION_BOTTOM);
				GUI_SetCancel(APP_MainMenu);
				GUI_ClearHalt(); // Clear _Halt, so 
				break;
			case BUTTON_ID_STOPWATCH:
				GUI_SetAppIndex(APP_StopWatch);
				GUI_SetAppDirection(MEMDEV_ANIMATION_BOTTOM);
				GUI_SetCancel(APP_MainMenu);
				GUI_ClearHalt(); // Clear _Halt, so 
				break;
			case BUTTON_ID_CLOCK:
				GUI_SetAppIndex(APP_Clock);
				GUI_SetAppDirection(MEMDEV_ANIMATION_BOTTOM);
				GUI_SetCancel(APP_MainMenu);
				GUI_ClearHalt(); // Clear _Halt, so 
				break;
			case BUTTON_ID_LED:
				GUI_SetAppIndex(APP_Led);
				GUI_SetAppDirection(MEMDEV_ANIMATION_BOTTOM);
				GUI_SetCancel(APP_MainMenu);
				GUI_ClearHalt(); // Clear _Halt, so 
				break;
			case BUTTON_ID_CALCULATOR:
				GUI_SetAppIndex(APP_Calculator);
				GUI_SetAppDirection(MEMDEV_ANIMATION_BOTTOM);
				GUI_SetCancel(APP_MainMenu);
				GUI_ClearHalt(); // Clear _Halt, so 
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
/*******************************************************************
*
*       MainTask
*
********************************************************************
*/
void MainTask_AppMenu(void)
{
	WM_HWIN hWinMenu;

	GUI_Init();
	hWinMenu = WM_CreateWindow(0, 0, 320, 240, WM_CF_SHOW, _cbMenu, 0);
	WM_SetFocus(hWinMenu);
	WM_EnableMemdev(hWinMenu);

	while (!GUI_CheckCancel(APP_MainMenu))
	{
		GUI_Delay(10);
	}
	WM_DisableWindow(hWinMenu);
}

