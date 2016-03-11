#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"
#include "AppFrame.h"
#include "time.h"

#define MAG 8
#define BUTTON_ID_EXIT	0

static BUTTON_Handle   _ahButton;

typedef struct PARAM
{
	GUI_AUTODEV_INFO AutoDevInfo;
	GUI_POINT aPoint1[15];
	float	  Angle1;
	GUI_POINT aPoint2[15];
	float	  Angle2;
	GUI_POINT aPoint3[15];
	float     Angle3;
}PARAM;

extern GUI_CONST_STORAGE GUI_BITMAP bmwatch;

GUI_POINT _aNeedle[3][15] = {
	{//Sec
		{ MAG * (-1), MAG * (18) },
		{ MAG * (-1), MAG * (-84) },
		{ MAG * (0), MAG * (-84) },
		{ MAG * (0), MAG * (18) },

		{ MAG * (-1), MAG * (18) },
	},

	{//Min
		{ MAG * (-1), MAG * (0) },
		{ MAG * (-1), MAG * (-70) },
		{ MAG * (1), MAG * (-70) },
		{ MAG * (1), MAG * (0) },

		{ MAG * (-1), MAG * (0) },
	},

	{//Hour
		{ MAG * (-2), MAG * (0) },
		{ MAG * (-2), MAG * (-54) },
		{ MAG * (2), MAG * (-54) },
		{ MAG * (2), MAG * (0) },

		{ MAG * (-2), MAG * (0) },
	}
};

static void _ShowButton(WM_HWIN hParent)
{
	_ahButton = BUTTON_CreateAsChild(10, 10, 30, 25, hParent, BUTTON_ID_EXIT, WM_CF_SHOW | WM_CF_STAYONTOP | WM_CF_MEMDEV);
	BUTTON_SetText(_ahButton, "EXIT");
}


/*********************************************************************
*Function name	: Draw
*Parameters     : void *p
*Return value	: NULL
*Description	: Draw the watch
**********************************************************************/
static void Draw(void * p)
{
	PARAM * pParam = (PARAM *)p;
	if (pParam->AutoDevInfo.DrawFixed)
	{
		/* Draw fixed background */
		GUI_ClearRect(37, 0, 30+ bmwatch.XSize, bmwatch.YSize);
		GUI_DrawBitmap(&bmwatch, 37, 0);
	}

	/* Draw moving objects */
	GUI_SetColor(GUI_BLACK);
	GUI_AA_FillPolygon(pParam->aPoint1, GUI_COUNTOF(_aNeedle[0]), MAG * LCD_GetXSize() / 2, MAG * (LCD_GetYSize() / 2));
	GUI_AA_FillPolygon(pParam->aPoint2, GUI_COUNTOF(_aNeedle[1]), MAG * LCD_GetXSize() / 2, MAG * (LCD_GetYSize() / 2));
	GUI_AA_FillPolygon(pParam->aPoint3, GUI_COUNTOF(_aNeedle[2]), MAG * LCD_GetXSize() / 2, MAG * (LCD_GetYSize() / 2));

	if (pParam->AutoDevInfo.DrawFixed)
	{
		/* Draw fixed foreground (if needed) */
		GUI_SetColor(GUI_BLACK);
		GUI_FillCircle(160, 120, 3);
	}
}

static void  _cbClock(WM_MESSAGE *pMsg)
{
	int NCode, Id;
	//PARAM pParam;
	switch (pMsg->MsgId)
	{
	case WM_PAINT:
		GUI_SetBkColor(GUI_WHITE); 
		GUI_Clear();
		GUI_SetColor(GUI_BLACK);
		_ShowButton(pMsg->hWin);
		GUI_DrawBitmap(&bmwatch, 37, 0);

		//GUI_AA_FillPolygon(pParam.aPoint1, GUI_COUNTOF(_aNeedle[0]), MAG * LCD_GetXSize() / 2, MAG * (LCD_GetYSize() / 2));
	//	GUI_AA_FillPolygon(pParam.aPoint2, GUI_COUNTOF(_aNeedle[1]), MAG * LCD_GetXSize() / 2, MAG * (LCD_GetYSize() / 2));
		//GUI_AA_FillPolygon(pParam.aPoint3, GUI_COUNTOF(_aNeedle[2]), MAG * LCD_GetXSize() / 2, MAG * (LCD_GetYSize() / 2));

		GUI_FillCircle(160, 120, 3);
	
		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
		NCode = pMsg->Data.v;            /* Notification code */
		switch (NCode)
		{
		case WM_NOTIFICATION_CLICKED:
			switch (Id)
			{
			case BUTTON_ID_EXIT:
				GUI_SetAppIndex(APP_MainMenu);
				GUI_SetAppDirection(MEMDEV_ANIMATION_LEFT);
				_MoveShiftWindow(&pMsg->hWin, MEMDEV_ANIMATION_RIGHT, 1, 1);
				GUI_SetCancel(APP_Clock);
				GUI_ClearHalt();
				break;
			default:
				break;
			}
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
void MainTask_AppClock(void)
{
	WM_HWIN hWinClock;
	GUI_AUTODEV AutoDev;
	PARAM       Param;

	int hour = 0;
	time_t oldtimestamp = 0;
	time_t curtimestamp = 0;
	struct tm *currUtime;

	//Init time
	curtimestamp = time(0);
	oldtimestamp = curtimestamp;
	currUtime=localtime(&curtimestamp);

	hWinClock = WM_CreateWindowAsChild(0, 0, 320, 240, WM_HBKWIN, WM_CF_SHOW, _cbClock, 0);
	WM_SelectWindow(hWinClock);
	WM_SetCreateFlags(WM_CF_MEMDEV);
	WM_SetFocus(hWinClock);
	_MoveShiftWindow(&hWinClock, MEMDEV_ANIMATION_LEFT, WM_Shift_ToLCD, 0);

	/*hWinClock = WM_CreateWindow(0, 0, 320, 240, WM_CF_SHOW, _cbClock, 0);
	WM_SetFocus(hWinClock);
	//_MoveShiftWindow(&hWinCalendar, MEMDEV_ANIMATION_LEFT, WM_Shift_ToLCD, 0);
	WM_EnableMemdev(hWinClock);*/

	/*	GUI_SetBkColor(GUI_WHITE);
	GUI_Clear();
	GUI_SetColor(GUI_BLACK);*/

	GUI_AA_EnableHiRes();
	GUI_AA_SetFactor(MAG);
	GUI_MEMDEV_CreateAuto(&AutoDev);

	while (!GUI_CheckCancel(APP_Clock))
	{
		GUI_Delay(10);
		curtimestamp = time(0);
		currUtime = localtime(&curtimestamp);
		if (1 == curtimestamp - oldtimestamp)
		{
			Param.Angle1 = (59 - currUtime->tm_sec) * 6 * (3.1415926f / 180);
			Param.Angle2 = (3599 - ((currUtime->tm_min * 60) + (currUtime->tm_sec)))*0.1*(3.1415926f / 180);
			hour = (currUtime->tm_hour>12) ? (currUtime->tm_hour - 12) : (currUtime->tm_hour);
			Param.Angle3 = (43199 - ((hour * 3600) + (currUtime->tm_min * 60) + (currUtime->tm_sec)))*0.0083333*(3.1415926f / 180);
			GUI_RotatePolygon(Param.aPoint1, _aNeedle[0], GUI_COUNTOF(_aNeedle[0]), Param.Angle1);
			GUI_RotatePolygon(Param.aPoint2, _aNeedle[1], GUI_COUNTOF(_aNeedle[1]), Param.Angle2);
			GUI_RotatePolygon(Param.aPoint3, _aNeedle[2], GUI_COUNTOF(_aNeedle[2]), Param.Angle3);
			GUI_MEMDEV_DrawAuto(&AutoDev, &Param.AutoDevInfo, &Draw, &Param);
		}
		oldtimestamp = curtimestamp;
	}
	WM_DeleteWindow(hWinClock);
	hWinClock = 0;
}