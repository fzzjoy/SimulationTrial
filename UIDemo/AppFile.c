#include "GUI.h"
#include "WM.h"
#include "DIALOG.h"
#include "BUTTON.h"
#include "AppFrame.h"
#include <windows.h>
#include <stdio.h>

#define TREEVIEW_ID 0
#define BUTTON_ID_EXIT	1

static WM_HWIN hTree;
static BUTTON_Handle _ahButton;

static void _ShowButton(WM_HWIN hWin)
{
	_ahButton = BUTTON_CreateAsChild(320 - 10 - 30, 10, 30, 25, hWin, BUTTON_ID_EXIT, WM_CF_SHOW | WM_CF_STAYONTOP | WM_CF_MEMDEV);
	BUTTON_SetText(_ahButton, "EXIT");
}

void enum_path(char *cpath, TREEVIEW_ITEM_Handle hItemTree)
{
	WIN32_FIND_DATA wfd;
	HANDLE hfd;
	char cdir[MAX_PATH];
	char subdir[MAX_PATH];
	int r;
	TREEVIEW_ITEM_Handle tmphTreeNode;
	TREEVIEW_ITEM_Handle tmphTreeLeaf;

	GetCurrentDirectory(MAX_PATH, cdir);
	SetCurrentDirectory(cpath);
	hfd = FindFirstFile("*.*", &wfd);
	if (hfd != INVALID_HANDLE_VALUE) {
		do{
			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)//is a directory
			{
				if (wfd.cFileName[0] != '.') {
					// 合成完整路径名 
					sprintf_s(subdir, 200, "%s//%s", cpath, wfd.cFileName);
					// 递归枚举子目录 
					tmphTreeNode = TREEVIEW_InsertItem(hTree, TREEVIEW_ITEM_TYPE_NODE, hItemTree, 2, wfd.cFileName);
					enum_path(subdir, tmphTreeNode);
				}
			}
			else{
				printf("%s\\%s\n", cpath, wfd.cFileName);
				tmphTreeNode = TREEVIEW_InsertItem(hTree, TREEVIEW_ITEM_TYPE_LEAF, hItemTree, 2, wfd.cFileName);
				// 病毒可根据后缀名判断是 
				// 否要感染相应的文件 
			}
		} while (r = FindNextFile(hfd, &wfd), r != 0);
	}
	SetCurrentDirectory(cdir);
}

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
				GUI_SetAppIndex(APP_MainMenu);
				GUI_SetAppDirection(MEMDEV_ANIMATION_LEFT);
				_MoveShiftWindow(&pMsg->hWin, MEMDEV_ANIMATION_RIGHT, 1, 1);
				GUI_SetCancel(APP_File);
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

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*******************************************************************
*
*       MainTask
*/
void MainTask_AppFile(void)
{
	WM_HWIN hWinFile;
	TREEVIEW_ITEM_Handle hNode;
	const char path[100] = "D:/SD卡文件/font";

	hWinFile = WM_CreateWindow(320, 0, 320, 240, WM_CF_SHOW, _cbFile, 0);
	WM_SetFocus(hWinFile);
	_MoveShiftWindow(&hWinFile, MEMDEV_ANIMATION_LEFT, WM_Shift_ToLCD, 0);
	WM_EnableMemdev(hWinFile);

	_ShowButton(hWinFile);

	hTree = TREEVIEW_CreateEx(0, 0, 320, 240, hWinFile, WM_CF_SHOW, 0, TREEVIEW_ID);
	TREEVIEW_SetAutoScrollV(hTree, 1);//管理自动使用垂直滚动条。
	TREEVIEW_SetSelMode(hTree, TREEVIEW_SELMODE_ROW);

	hNode = TREEVIEW_InsertItem(hTree, TREEVIEW_ITEM_TYPE_NODE, 0, 0, "font");
	enum_path(path, hNode);

	while( !GUI_CheckCancel(APP_File))
	{
		GUI_Delay(10);
	}
	WM_DeleteWindow(hWinFile);
	hWinFile = 0;
}

/*************************** End of file ****************************/

