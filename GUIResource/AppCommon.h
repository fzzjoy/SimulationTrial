/*********************************************************************
*File        : AppCommon.h
*Author      : fzz
*Versions    : 2016/3/7 V1.0
*Description : App Common interface
**********************************************************************/
#ifndef APPCOMMON_H
#define APPCOMMON_H

#include "GUI.h"
#include "WM.h"
#include "AppFrame.h"
/*********************************************************************
*
*       Macro definition
*
**********************************************************************
*/


/*********************************************************************
*
*       Data structure
*
**********************************************************************
*/
typedef struct GUI_CONFIG 
{
	void(** apFunc)(void);
	int       NumApps;
//	U16       Flags;
} GUI_CONFIG;


/*********************************************************************
*
*       Functions
*
**********************************************************************
*/
void MainTask(void);
void _MoveShiftWindow(WM_HWIN *hWinIn,MEMDEV_ANIMATION direction,WM_Shift_Option ShiftOption,U8 Option);
int GUI_SetCancel(int appindex);
void GUI_SetAppDirection(MEMDEV_ANIMATION direction);
void GUI_SetAppIndex(int appindex);
int GUI_CheckCancel(int appindex);

/*App Functions*/
void MainTask_AppMenu(void);
void MainTask_AppCalendar(void);
void MainTask_AppStopWatch(void);
void MainTask_AppClock(void);
void MainTask_AppLed(void);
void MainTask_AppCalculator(void);


#endif




