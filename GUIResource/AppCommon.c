/*********************************************************************
*File        : AppCommon.c
*Author      : fzz
*Versions    : 2016/3/7 V1.0
*Description : App common interface
**********************************************************************/

#include "AppCommon.h"
//#include "GUI.h"
//#include "WM.h"



/*********************************************************************
*
*       Data structure
*
**********************************************************************/

/*********************************************************************
*
*       Static variables
*
**********************************************************************
*/
static int _Pressed;
static int _iAppIndex;
static int _iPreAppIndex;
static int _Next;
static int _Halt;
static GUI_CONFIG _pConfig;
static int	 _Direction = MEMDEV_ANIMATION_NUM;


/*********************************************************************
*
*       Public variables
*
**********************************************************************
*/

/*********************************************************************
*
*       Extern variables
*
**********************************************************************
*/

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

/*********************************************************************
*
*       Static functions
*
**********************************************************************
*/

/*********************************************************************
*Function name	: _Main
*Parameters     : NULL
*Return value	: NULL
*Description	: recycle run the UI demo
**********************************************************************/
static void _Main(void) 
{
	for (_iAppIndex = APP_MainMenu; _pConfig.apFunc[_iAppIndex];)
	{
		(*_pConfig.apFunc[_iAppIndex])();
		_Pressed = 0;
	}
	_iAppIndex = APP_MainMenu;
}


/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
void GUI_SetAppIndex(int appindex)
{
	if(_iAppIndex == appindex)
	{
		return;
	}
	_iPreAppIndex = _iAppIndex;
	_iAppIndex = appindex;
	return;
}

void GUI_SetAppDirection(MEMDEV_ANIMATION direction)
{
	_Direction = direction;
	return;
}



/*********************************************************************
*Function name	: GUI_SetCancel
*Parameters     : int appindex
*Return value	: 
*Description	: 
**********************************************************************/
int GUI_SetCancel(int appindex) 
{
	switch(appindex)
	{
		case APP_AllNums:
			_Next = 1;
			break;
		default:
			_Next = 1;			
			break;
	}
	return 1;
}

void GUI_ClearHalt(void) 
{
	_Halt = 0;
}

int GUI_CheckCancel(int appindex)
{
	switch (appindex)
	{
	case APP_AllNums:
		break;
	default:
		if (_Next == 1)
		{
			_Next = 0;
			return 1;
		}
		break;
	}
	return 0;
}

/*********************************************************************
*Function name	: _MoveShiftWindow
*Parameters     : WM_HWIN *hWinIn;MEMDEV_ANIMATION direction;WM_Shift_Option ShiftOption;U8 Option
*Return value	: 
*Description	: 
**********************************************************************/
void _MoveShiftWindow(WM_HWIN *hWinIn,MEMDEV_ANIMATION direction,WM_Shift_Option ShiftOption,U8 Option)
{
	int i;
	U8 count = 0;
	int offset[5] = {1,16,207,15,1};
	for (i = 0; i < 5; i++) 
	{	
		switch (direction)
		{
		case MEMDEV_ANIMATION_LEFT:
			WM_MoveWindow(*hWinIn, -offset[i], 0);
			break;
		case MEMDEV_ANIMATION_TOP:
			WM_MoveWindow(*hWinIn, 0, -offset[i]);
			break;
		case MEMDEV_ANIMATION_RIGHT:
			WM_MoveWindow(*hWinIn, offset[i], 0);
			break;
		case MEMDEV_ANIMATION_BOTTOM:
			WM_MoveWindow(*hWinIn, 0, offset[i]);
			break;
		default:
			break;
		}
		if(Option)
		{
			GUI_Delay(1);
		}
		else
		{
			WM_Paint(*hWinIn);
		}
	}
	switch(ShiftOption)
	{
		case WM_Shift_OutFromLCD:
		{
			switch (direction)
			{
				case MEMDEV_ANIMATION_LEFT:
					WM_MoveTo(*hWinIn, -320, 0);
					break;
				case MEMDEV_ANIMATION_TOP:
					WM_MoveTo(*hWinIn, 0, -320);
					break;
				case MEMDEV_ANIMATION_RIGHT:
					WM_MoveTo(*hWinIn, 320, 0);
					break;
				case MEMDEV_ANIMATION_BOTTOM:
					WM_MoveTo(*hWinIn, 0, 320);
					break;
				default:
					break;

			}
		}
		break;
		case WM_Shift_ToLCD:
			WM_MoveTo(*hWinIn, 0, 0);	
			break;
		case WM_Shift_InLCD:
			break;
		default:
			break;
	}
	return;
}

/*********************************************************************
*Function name	: MainTask
*Parameters     : NULL
*Return value	: NULL
*Description	: UITask entrance
**********************************************************************/
void MainTask(void) 
{
    GUIWATCH_Config(&_pConfig);
	while (1)
	{
		_Main();
	}
}


/*************************** End of file ****************************/



