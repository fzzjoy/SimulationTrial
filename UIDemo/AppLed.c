#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"
#include "AppFrame.h"
#include "time.h"

#define BUTTON_ID_EXIT	0
#define BUTTON_ID_LED1	1
#define BUTTON_ID_LED2	2
#define BUTTON_ID_LED3	3
#define BUTTON_ID_LED4	4

typedef enum
{
	FALSE = 0, TRUE = !FALSE
}
ledbool;

static BUTTON_Handle   _ahButton[5];
static ledbool LedStatus[5] = { 0, 0, 0, 0, 0 };

extern GUI_CONST_STORAGE GUI_BITMAP bmledon;
extern GUI_CONST_STORAGE GUI_BITMAP bmledoff;


static void _ShowButton(WM_HWIN hWin)
{
	_ahButton[0] = BUTTON_CreateAsChild(10, 10, 30, 25, hWin, BUTTON_ID_EXIT, WM_CF_SHOW | WM_CF_STAYONTOP | WM_CF_MEMDEV);

	_ahButton[1] = BUTTON_CreateAsChild(35, 150, 40, 20, hWin, BUTTON_ID_LED1, WM_CF_SHOW | WM_CF_STAYONTOP | WM_CF_MEMDEV);
	_ahButton[2] = BUTTON_CreateAsChild(105, 150, 40, 20, hWin, BUTTON_ID_LED2, WM_CF_SHOW | WM_CF_STAYONTOP | WM_CF_MEMDEV);
	_ahButton[3] = BUTTON_CreateAsChild(175, 150, 40, 20, hWin, BUTTON_ID_LED3, WM_CF_SHOW | WM_CF_STAYONTOP | WM_CF_MEMDEV);
	_ahButton[4] = BUTTON_CreateAsChild(245, 150, 40, 20, hWin, BUTTON_ID_LED4, WM_CF_SHOW | WM_CF_STAYONTOP | WM_CF_MEMDEV);

	BUTTON_SetText(_ahButton[0], "EXIT");
	BUTTON_SetText(_ahButton[1], "ON/OFF");
	BUTTON_SetText(_ahButton[2], "ON/OFF");
	BUTTON_SetText(_ahButton[3], "ON/OFF");
	BUTTON_SetText(_ahButton[4], "ON/OFF");

}

static void  _cbLed(WM_MESSAGE *pMsg)
{
	int NCode, Id;

	switch (pMsg->MsgId)
	{
	case WM_PAINT:
		GUI_SetBkColor(GUI_WHITE);
		GUI_Clear();
		GUI_SetColor(GUI_RED);

		if (LedStatus[1] == 0)
			GUI_DrawBitmap(&bmledoff, 35, 80);
		else GUI_DrawBitmap(&bmledon, 35, 80);

		if (LedStatus[2] == 0)
			GUI_DrawBitmap(&bmledoff, 35 + 70, 80);
		else GUI_DrawBitmap(&bmledon, 35 + 70, 80);

		if (LedStatus[3] == 0)
			GUI_DrawBitmap(&bmledoff, 35 + 140, 80);
		else GUI_DrawBitmap(&bmledon, 35 + 140, 80);

		if (LedStatus[4] == 0)
			GUI_DrawBitmap(&bmledoff, 35 + 210, 80);
		else GUI_DrawBitmap(&bmledon, 35 + 210, 80);

		_ShowButton(pMsg->hWin);

		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
		NCode = pMsg->Data.v;            /* Notification code */
		switch (NCode)
		{
		case WM_NOTIFICATION_CLICKED:
			switch (Id)
			{
			case BUTTON_ID_LED1:
				LedStatus[1] = !LedStatus[1];
				WM_InvalidateWindow(pMsg->hWin);
				break;
			case BUTTON_ID_LED2:
				LedStatus[2] = !LedStatus[2];
				WM_InvalidateWindow(pMsg->hWin);
				break;
			case BUTTON_ID_LED3:
				LedStatus[3] = !LedStatus[3];
				WM_InvalidateWindow(pMsg->hWin);
				break;
			case BUTTON_ID_LED4:
				LedStatus[4] = !LedStatus[4];
				WM_InvalidateWindow(pMsg->hWin);
				break;
			case BUTTON_ID_EXIT:
				GUI_SetAppIndex(APP_MainMenu);
				GUI_SetAppDirection(MEMDEV_ANIMATION_LEFT);
				_MoveShiftWindow(&pMsg->hWin, MEMDEV_ANIMATION_RIGHT, 1, 1);
				GUI_SetCancel(APP_Led);
				GUI_ClearHalt();
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		break;
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
void MainTask_AppLed(void)
{
	WM_HWIN hWinLed;

	//GUI_Init();

	hWinLed = WM_CreateWindow(320, 0, 320, 240, WM_CF_SHOW, _cbLed, 0);
	WM_SetFocus(hWinLed);
	_MoveShiftWindow(&hWinLed, MEMDEV_ANIMATION_LEFT, WM_Shift_ToLCD, 0);
	WM_EnableMemdev(hWinLed);


	while (!GUI_CheckCancel(APP_Led))
	{
		GUI_Delay(10);
	}
	WM_DeleteWindow(hWinLed);
	hWinLed = 0;
}