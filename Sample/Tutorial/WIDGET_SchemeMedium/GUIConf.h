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
File        : GUIConf.h
Purpose     : Configuration of available features and default values
----------------------------------------------------------------------
*/

#ifndef GUICONF_H
#define GUICONF_H

/*********************************************************************
*
*       Configuration of available packages
*/
#define GUI_SUPPORT_TOUCH   (1)  // Support a touch screen (req. win-manager)
#define GUI_SUPPORT_MOUSE   (1)  // Support a mouse
#define GUI_WINSUPPORT      (1)  // Window manager package available
#define GUI_SUPPORT_MEMDEV  (1)  // Memory devices available

/*********************************************************************
*
*       Default font
*/
#define GUI_DEFAULT_FONT          &GUI_Font6x8

/*********************************************************************
*
*       Default widget scheme
*/
#define WIDGET_USE_SCHEME_MEDIUM 1

#endif  /* Avoid multiple inclusion */

/*************************** End of file ****************************/
