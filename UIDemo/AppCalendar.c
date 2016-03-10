#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"
#include "AppFrame.h"


#define BUTTON_ID_LEFT		0
#define BUTTON_ID_RIGHT		1
#define BUTTON_ID_EXIT		2

typedef struct
{
	U16 X;       // X
	U16 Y;       // Y
	U32 Color;   // 颜色
	U8  Date;    // 日期
} CLD_DAY;

typedef struct
{
	int year;
	U8  month;
	U8  day;
	U8  weekday;
}CLD_DATE;

extern GUI_CONST_STORAGE GUI_BITMAP bmRightarrows;
extern GUI_CONST_STORAGE GUI_BITMAP bmLeftarrows;

static CLD_DATE			cldDate = { 2016, 3, 31, 4 };//1表示周一，7表示周日
static BUTTON_Handle	_ahButton[3];
static CLD_DAY			tmpDay[42];

/**************************************************************************************
* FunctionName   : ReturnWeekDay
* Description    : 通过日期获取周
* EntryParameter : year, month, day - 蔡勒公式
* ReturnValue    : 返回周
**************************************************************************************/
int ReturnWeekDay(int iYear, U8 iMonth, U8 iDay)
{
	int iWeek = 0;
	unsigned int y = 0, c = 0, m = 0, d = 0;

	if (iMonth == 1 || iMonth == 2)
	{
		c = (iYear - 1) / 100;
		y = (iYear - 1) % 100;
		m = iMonth + 12;
		d = iDay;
	}
	else
	{
		c = iYear / 100;
		y = iYear % 100;
		m = iMonth;
		d = iDay;
	}

	iWeek = y + y / 4 + c / 4 - 2 * c + 26 * (m + 1) / 10 + d - 1;    //蔡勒公式
	iWeek = iWeek >= 0 ? (iWeek % 7) : (iWeek % 7 + 7);    //iWeek为负时取模
	if (iWeek == 0)    //星期日不作为一周的第一天
	{
		iWeek = 7;
	}

	return iWeek;
}

/**************************************************************************************
* FunctionName   : _DrawCalendar()
* Description    : 绘制日历表格
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void _DrawCalendar(CLD_DAY *pDay)
{
	const U8 cstWek[][4] = { "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT" };
	U8 tmpWth = 320 / 7;
	U8 tmpHgh = 180 / 6;
	U8 i = 0, n = 0;

	GUI_SetColor(GUI_GRAY);                         // 设置灰色
	for (i = 0; i < 7; i++)                      // 画水平线
	{
		GUI_DrawHLine((60 + i*tmpHgh) - 1, 0, 320);
		for (n = 0; n < 7; n++)
		{
			if (i < 6)
			{
				pDay[i * 7 + n].Y = (60 + i*tmpHgh) + tmpHgh / 2;
			}
		}
	}
	for (i = 0; i < 7; i++)                      // 画垂直平线
	{
		if (i < 6) GUI_DrawVLine((i + 1)*tmpWth, 60, 240);
		for (n = 0; n < 6; n++)
		{
			pDay[i + 7 * n].X = ((i + 1)*tmpWth) - tmpWth / 2;
		}
	}
	for (i = 0; i < 7; i++)                      // 显示星期
	{
		GUI_DispStringHCenterAt(cstWek[i], i*tmpWth + tmpWth / 2, 45);
	}
}

/**************************************************************************************
* FunctionName   : _FillCalendar()
* Description    : 日历填写
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void _FillCalendar(CLD_DAY *pDay, int year, U8 month, U8 day, U8 wekday)
{
	const U8 cstmonth[][4] = { "JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEQ", "OCT", "NOV", "DEC" };
	const U8 monthDay[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };//非闰年
	const U8 monthDayLY[] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };//闰年
	int tmpWek;
	U8 i = 0;

	GUI_SetFont(&GUI_Font32_ASCII);
	GUI_SetColor(GUI_BLACK);                                      // 设置蓝色
	GUI_DispStringHCenterAt(cstmonth[month - 1], 120, 5);                      // 显示月份
	GUI_GotoXY(170, 5);
	GUI_DispDecSpace(year, 5); //显示年份 最大可显示9999
	tmpWek = wekday - (day % 7) + 1;                       // 获取本月第一天是星期几？
	if (tmpWek <= 0)
		tmpWek = 7 + wekday - (day % 7) + 1;

	for (i = 0; i < tmpWek; i++)        // 存储上月的
	{
		if (month == 1)
		{
			if ((year % 4) == 0 && (year % 100) != 0 || (year % 400) == 0)//闰年
				pDay[i].Date = monthDayLY[12 - 1] - tmpWek + i + 1;
			else pDay[i].Date = monthDay[12 - 1] - tmpWek + i + 1;
		}
		else
		{
			if ((year % 4) == 0 && (year % 100) != 0 || (year % 400) == 0)//闰年
				pDay[i].Date = monthDayLY[month - 2] - tmpWek + i + 1;
			else pDay[i].Date = monthDay[month - 2] - tmpWek + i + 1;
		}
		pDay[i].Color = GUI_GRAY;
	}

	if ((year % 4) == 0 && (year % 100) != 0 || (year % 400) == 0)//闰年
	{
		for (i = 0; i < monthDayLY[month - 1]; i++)
		{
			pDay[i + tmpWek].Date = i + 1;                               // 存储本月的
			pDay[i + tmpWek].Color = (day == i + 1) ? GUI_GREEN : GUI_BLACK;
		}
	}
	else
	{
		for (i = 0; i < monthDay[month - 1]; i++)
		{
			pDay[i + tmpWek].Date = i + 1;                               // 存储本月的
			pDay[i + tmpWek].Color = (day == i + 1) ? GUI_GREEN : GUI_BLACK;
		}
	}

	if ((year % 4) == 0 && (year % 100) != 0 || (year % 400) == 0)//闰年
	{
		for (i = 0; i < 42 - tmpWek - monthDayLY[month - 1]; i++)
		{
			if (month == 0x12)
			{
				pDay[i + tmpWek + monthDayLY[1 - 1]].Date = i + 1;
			}
			else
			{
				pDay[i + tmpWek + monthDayLY[month - 1]].Date = i + 1;    // 存储下月的
			}
			pDay[i + tmpWek + monthDayLY[month - 1]].Color = GUI_GRAY;
		}
	}
	else
	{
		for (i = 0; i < 42 - tmpWek - monthDay[month - 1]; i++)
		{
			if (month == 0x12)
			{
				pDay[i + tmpWek + monthDay[1 - 1]].Date = i + 1;
			}
			else
			{
				pDay[i + tmpWek + monthDay[month - 1]].Date = i + 1;    // 存储下月的
			}
			pDay[i + tmpWek + monthDay[month - 1]].Color = GUI_GRAY;
		}
	}

	for (i = 0; i < 42; i++)
	{
		GUI_SetFont(&GUI_Font24_ASCII);
		GUI_SetColor(pDay[i].Color);
		GUI_SetTextAlign(GUI_TA_HCENTER | GUI_TA_VCENTER);
		GUI_DispDecAt(pDay[i].Date, pDay[i].X, pDay[i].Y, ((pDay[i].Date > 10) ? 2 : 1));
	}
}
/**************************************************************************************
* FunctionName   : DisCalendar()
* Description    : 日历显示
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void DisCalendar(int year, U8 month, U8 day, U8 wekday)
{
	_DrawCalendar(tmpDay);
	_FillCalendar(tmpDay, year, month, day, wekday);
}

static void _ShowButton(WM_HWIN hParent)
{
	_ahButton[0] = BUTTON_CreateAsChild(60, 15, 13, 14, hParent, BUTTON_ID_LEFT, WM_CF_SHOW | WM_CF_STAYONTOP | WM_CF_MEMDEV);
	_ahButton[1] = BUTTON_CreateAsChild(260, 15, 13, 14, hParent, BUTTON_ID_RIGHT, WM_CF_SHOW | WM_CF_STAYONTOP | WM_CF_MEMDEV);
	_ahButton[2] = BUTTON_CreateAsChild(10, 10, 30, 25, hParent, BUTTON_ID_EXIT, WM_CF_SHOW | WM_CF_STAYONTOP | WM_CF_MEMDEV);

	BUTTON_SetBitmap(_ahButton[0], BUTTON_BI_UNPRESSED, &bmLeftarrows);
	BUTTON_SetBitmap(_ahButton[1], BUTTON_BI_UNPRESSED, &bmRightarrows);
	BUTTON_SetText(_ahButton[2], "EXIT");
}

static void _cbCalendar(WM_MESSAGE *pMsg)
{
	int NCode, Id;

	switch (pMsg->MsgId)
	{
	case WM_PAINT:
		GUI_SetBkColor(GUI_WHITE);
		GUI_Clear();
		GUI_SetColor(GUI_RED);
		_ShowButton(pMsg->hWin);
		DisCalendar(cldDate.year, cldDate.month, cldDate.day, cldDate.weekday);
		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
		NCode = pMsg->Data.v;            /* Notification code */
		switch (NCode)
		{
		case WM_NOTIFICATION_CLICKED:
			switch (Id)
			{
			case BUTTON_ID_LEFT:
				cldDate.month -= 1;
				if (cldDate.month == 0)
				{
					cldDate.month = 12;
					cldDate.year -= 1;
					if (cldDate.year == 1969)
					{
						cldDate.year = 1970;
					}
				}
				cldDate.weekday = ReturnWeekDay(cldDate.year, cldDate.month, cldDate.day);
				WM_InvalidateWindow(pMsg->hWin);
				break;
			case BUTTON_ID_RIGHT:
				cldDate.month += 1;
				if (cldDate.month == 13)
				{
					cldDate.month = 1;
					cldDate.year += 1;
					if (cldDate.year == 10000)
					{
						cldDate.year = 9999;
					}
				}
				cldDate.weekday = ReturnWeekDay(cldDate.year, cldDate.month, cldDate.day);
				WM_InvalidateWindow(pMsg->hWin);
				break;
			case BUTTON_ID_EXIT:
				GUI_SetAppIndex(APP_MainMenu);
				GUI_SetAppDirection(MEMDEV_ANIMATION_LEFT);
				_MoveShiftWindow(&pMsg->hWin, MEMDEV_ANIMATION_RIGHT, 1, 1);
				GUI_SetCancel(APP_Calendar);
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


/*******************************************************************
*
*       MainTask
*
********************************************************************
*/
void MainTask_AppCalendar(void)
{
	WM_HWIN hWinCalendar;

	//GUI_Init();
	hWinCalendar = WM_CreateWindow(320, 0, 320, 240, WM_CF_SHOW, _cbCalendar, 0);
	WM_SetFocus(hWinCalendar);
	_MoveShiftWindow(&hWinCalendar, MEMDEV_ANIMATION_LEFT, WM_Shift_ToLCD, 0);
	WM_EnableMemdev(hWinCalendar);

	while (!GUI_CheckCancel(APP_Calendar))
	{
		GUI_Delay(10);
	}
	WM_DeleteWindow(hWinCalendar);
	hWinCalendar = 0;
}