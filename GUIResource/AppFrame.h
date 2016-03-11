/*********************************************************************
*File        : AppFrame.h
*Author      : fzz
*Versions    : 2016/3/7 V1.0
*Description : create a app enum and motion enum
**********************************************************************/
#ifndef APPFRAME_H
#define APPFRAME_H


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
//APP's move action
typedef enum {
	MEMDEV_ANIMATION_LEFT = 0,
	MEMDEV_ANIMATION_RIGHT,   
	MEMDEV_ANIMATION_TOP,
	MEMDEV_ANIMATION_BOTTOM,
	MEMDEV_ANIMATION_DEFAULT,
	MEMDEV_ANIMATION_NUM
} MEMDEV_ANIMATION;

typedef enum {
	WM_Shift_ToLCD = 0,
	WM_Shift_OutFromLCD,  
	WM_Shift_InLCD,
	WM_Shift_Num
} WM_Shift_Option;


//APP's index
typedef enum {
	APP_MainMenu= 0,
	APP_Calendar,
	APP_StopWatch,
	APP_Clock,
	APP_Led,
	APP_AllNums
}AppIndex_ForUISystem;


/*********************************************************************
*
*       Functions
*
**********************************************************************
*/

#endif



