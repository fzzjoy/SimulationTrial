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
File        : GUIDEMO_Start.c
Purpose     : GUIDEMO initialization
----------------------------------------------------------------------
*/

#include "GUIDEMO.h"

/*********************************************************************
*
*       MainTask
*/
void MainTask(void) {
  #if GUI_WINSUPPORT
    WM_SetCreateFlags(WM_CF_MEMDEV);
  #endif
  GUI_Init();
  #if GUI_WINSUPPORT
    WM_MULTIBUF_Enable(1);
  #endif
  GUIDEMO_Main();
}

/*************************** End of file ****************************/

