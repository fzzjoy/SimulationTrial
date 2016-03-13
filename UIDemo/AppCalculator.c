#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"
#include "EDIT.h"
#include "AppFrame.h"
#include "stdlib.h"

#define BUTTON_ID_EXIT	0
#define BUTTON_ID_CLEAR	67
#define BUTTON_ID_EQUAL 61
#define BUTTON_ID_ADD   43  
#define BUTTON_ID_SUB  45
#define BUTTON_ID_MUL  42
#define BUTTON_ID_DIV  47


static char _acText[] = {
	'7', '8', '9', '/', 'C',
	'4', '5', '6', '*', '=',
	'1', '2', '3', '-', 8,//8（BackSpace）并没有用到
	'0', '.', '+', 0
};
EDIT_Handle hEdit;
BUTTON_Handle hButton[19];

static void CreateCalculator(WM_HWIN hWin)
{
	int y0 = 55;
	int XSize = LCD_GetXSize();
	int YSize = LCD_GetYSize();
	int XStep = XSize / 5;
	int YStep = (YSize - y0 - 25) / 4;

	int Key;
	int i = 0;
	for (i = 0; i < 19; i++)
	{
		if (i == 14)
			i = 15;
		int XPos = (i % 5) * XStep + 3;
		int YPos = (i / 5) * YStep + 3 + y0;

		char ac[2] = { 0 };
		char *s = ac;
		ac[0] = _acText[i];
		if (i == 18)
		{
			hButton[i] = BUTTON_CreateAsChild(XPos, YPos, XStep * 2 - 5, YStep - 5, hWin, _acText[i], WM_CF_SHOW);
		}
		else if (i == 9)
			hButton[i] = BUTTON_CreateAsChild(XPos, YPos, XStep - 5, YStep * 2 - 5, hWin, _acText[i], WM_CF_SHOW);
		else hButton[i] = BUTTON_CreateAsChild(XPos, YPos, XStep - 5, YStep - 5, hWin, _acText[i], WM_CF_SHOW);
		BUTTON_SetBkColor(hButton[i], BUTTON_CI_UNPRESSED, 0x50A0FF);
		BUTTON_SetBkColor(hButton[i], BUTTON_CI_PRESSED, 0x004080);
		BUTTON_SetTextColor(hButton[i], BUTTON_CI_UNPRESSED, GUI_WHITE);
		BUTTON_SetTextColor(hButton[i], BUTTON_CI_PRESSED, GUI_WHITE);
		BUTTON_SetFont(hButton[i], &GUI_Font24B_ASCII);
		if (i == 18)
		{
			BUTTON_SetText(hButton[i], "EXIT");
		}
		else BUTTON_SetText(hButton[i], s);
	}
	hEdit = EDIT_CreateAsChild(0, 0, 320, 50, hWin, 0, WM_CF_SHOW, 80);
	EDIT_SetTextColor(hEdit, 0, 0x0080FF);
	EDIT_SetFont(hEdit, &GUI_Font24B_ASCII);
	EDIT_SetBkColor(hEdit, 0, GUI_RED);
}

static void  _cbCalculator(WM_MESSAGE *pMsg)
{
	int NCode, Id;
	static float num1 = 0.00f, num2 = 0.00f, result = 0.00f;
	static U8 countnum1 = 0;
	static char str1[10] = { 0 };
	static char oper = 0;
	char resultbuffer[80];
	static U8 finishflag = 0;
	static U8 pointflag = 0;

	switch (pMsg->MsgId)
	{
	case WM_PAINT:
		GUI_SetBkColor(GUI_WHITE);
		GUI_Clear();
		GUI_SetColor(GUI_RED);

		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
		NCode = pMsg->Data.v;            /* Notification code */
		switch (NCode)
		{
		case WM_NOTIFICATION_RELEASED: /* Released */
			switch (Id)
			{
			case BUTTON_ID_EXIT:
				GUI_SetAppIndex(APP_MainMenu);
				GUI_SetAppDirection(MEMDEV_ANIMATION_LEFT);
				_MoveShiftWindow(&pMsg->hWin, MEMDEV_ANIMATION_RIGHT, 1, 1);
				GUI_SetCancel(APP_Calculator);
				GUI_ClearHalt(); 
				break;
			case BUTTON_ID_CLEAR:
				memset(&str1, 0, sizeof(char)* 10);
				countnum1 = num1 = num2 = 0;
				pointflag = 0;
				EDIT_SetText(hEdit, " ");
				break;
			case BUTTON_ID_ADD:
			case BUTTON_ID_DIV:
			case BUTTON_ID_SUB:
			case BUTTON_ID_MUL:
				oper = Id;
				num1 = atof(&str1);
				memset(&str1, 0, sizeof(char)* 10);
				countnum1 = 0;
				EDIT_AddKey(hEdit, Id);
				break;
			case BUTTON_ID_EQUAL:
				num2 = atof(&str1);
				memset(&str1, 0, sizeof(char)* 10);
				countnum1 = 0;
				switch (oper)
				{
				case '+':
					result = num1 + num2;
					break;
				case '-':
					result = num1 - num2;
					break;
				case '*':
					result = num1 * num2;
					break;
				case '/':
					result = num1 / num2;
					break;
				default:
					break;
				}
				if ((pointflag == 1) || (oper == '/'))
					sprintf(resultbuffer, "%f", result);
				else sprintf(resultbuffer, "%d", (int)result);
				EDIT_SetText(hEdit, resultbuffer);
				finishflag = 1;
				pointflag = 0;
				break;
			default:
				if (finishflag == 1)
				{
					finishflag = 0;
					EDIT_SetText(hEdit, " ");
				}
				if (Id == '.')
					pointflag = 1;
				EDIT_AddKey(hEdit, Id);
				str1[countnum1] = Id;
				countnum1++;
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

void MainTask_AppCalculator(void)
{
	WM_HWIN hWinCalculator;

	GUI_Init();
	hWinCalculator = WM_CreateWindow(320, 0, 320, 240, WM_CF_SHOW, _cbCalculator, 0);
	WM_SetFocus(hWinCalculator);
	_MoveShiftWindow(&hWinCalculator, MEMDEV_ANIMATION_LEFT, WM_Shift_ToLCD, 0);
	WM_EnableMemdev(hWinCalculator);

	CreateCalculator(hWinCalculator);
	while (!GUI_CheckCancel(APP_Calculator))
	{
		GUI_Delay(10);
	}
	WM_DeleteWindow(hWinCalculator);
	hWinCalculator = 0;
}