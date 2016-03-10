/*
*********************************************************************************************************
*                                             uC/GUI V5.24
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2014, Micrium Inc., Weston, FL
*                       (c) Copyright 2014, SEGGER Microcontroller GmbH & Co. KG
*
*              µC/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : SIM_GetRegions.h
Purpose     : Interface definition of bitmap region functions
---------------------------END-OF-HEADER------------------------------
*/

#ifndef SIM_GETREGIONS_H
#define SIM_GETREGIONS_H

#include <windows.h>

/*********************************************************************
*
*       Interface
*
**********************************************************************
*/
int  SIM_GetRegions     (HBITMAP hBmp, DWORD rgbTrans, int MaxNumRegions);
void SIM_GetRegionRect  (int Index, RECT * pRect);
int  SIM_AddBitmapRegion(HWND hWnd, int Index, int xOff, int yOff);
void SIM_FreeRegions    (void);

#endif

/*************************** End of file ****************************/
