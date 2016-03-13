#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"
#include "EDIT.h"
#include "AppFrame.h"
#include "stdlib.h"
#include "FS.h"

#define BUTTON_ID_EXIT	0

static void  _cbFile(WM_MESSAGE *pMsg)
{
	int NCode, Id;

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
		case WM_NOTIFICATION_CLICKED:
			switch (Id) 
			{
			case BUTTON_ID_EXIT:
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

void MainTask(void)
{
	WM_HWIN hWinFile;

	GUI_Init();

	hWinFile=CreateTreeView(0, 0, 320, 240, WM_HBKWIN);
	WM_SetFocus(hWinFile);
	WM_SelectWindow(hWinFile);

	//添加一个子结点
	addTreeNode(hWinFile, MainTreeNode, 0);
	addTreeNode(hWinFile, MainTreeNode, 1);
	
	/*
	hWinFile = WM_CreateWindow(0, 0, 320, 240, WM_CF_SHOW,_cbFile, 0);
	WM_SetFocus(hWinFile);
	//_MoveShiftWindow(&hWinCalculator, MEMDEV_ANIMATION_LEFT, WM_Shift_ToLCD, 0);
	WM_EnableMemdev(hWinFile); */

	while (1)
	{
		GUI_Delay(10);
	}
	WM_DeleteWindow(hWinFile);
	hWinFile = 0;
}