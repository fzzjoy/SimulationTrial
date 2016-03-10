/*
*********************************************************************************************************
*                                             uC/GUI V5.24
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2014, Micrium Inc., Weston, FL
*                       (c) Copyright 2014, SEGGER Microcontroller GmbH & Co. KG
*
*              �C/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : MEMDEV_Printing.c
Purpose     : Demonstrates the use of GUI_MEMDEV_CreateFixed
Requirements: WindowManager - ( )
              MemoryDevices - (x)
              AntiAliasing  - ( )
              VNC-Server    - ( )
              PNG-Library   - ( )
              TrueTypeFonts - ( )
----------------------------------------------------------------------
*/

#include "GUI.h"
#include <string.h>

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

/********************************************************************
*
*       Static data
*
*********************************************************************
*/

static const char * _acText[] = {
   "Demonstrates how to use\n"
  ,"GUI_MEMDEV_CreateFixed() for printing"
};

/********************************************************************
*
*       Static data, test picture 4bpp
*
*********************************************************************
*/

/*   Palette
The following are the entries of the palette table.
Every entry is a 32-bit value (of which 24 bits are actually used)
the lower   8 bits represent the Red component,
the middle  8 bits represent the Green component,
the highest 8 bits (of the 24 bits used) represent the Blue component
as follows:   0xBBGGRR
*/

GUI_CONST_STORAGE GUI_COLOR ColorsTestPicture8[] = {
     0xFFFFFF,0x0000FF,0x000000,0x00FFFF
    ,0xFF0000,0xFF1400,0xFF2800,0xFF3C00
    ,0xFF5000,0xFF6400,0xFF7800,0xFF8C00
    ,0xFFA000,0xFFB400,0xFFC800,0xFFDC00
    ,0xFFF000,0xFFFF00,0x2B40FF,0x2B55FF
    ,0x2B80FF,0x2B95FF,0x2BAAFF,0x2BBFFF
    ,0x2BD5FF
};

GUI_CONST_STORAGE GUI_LOGPALETTE PalTestPicture8 = {
  25,	// number of entries
  0, 	// Has transparency
  &ColorsTestPicture8[0]
};

GUI_CONST_STORAGE unsigned char acTestPicture8[] = {
  0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
  0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
  0x02, 0x01, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
  0x02, 0x01, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x03, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
  0x02, 0x01, 0x03, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x03, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
  0x02, 0x01, 0x03, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x03, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
  0x02, 0x01, 0x03, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x03, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
  0x02, 0x01, 0x03, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x03, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
  0x02, 0x01, 0x03, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x03, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
  0x02, 0x01, 0x03, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x03, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
  0x02, 0x01, 0x03, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x03, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
  0x02, 0x01, 0x03, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x03, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
  0x02, 0x01, 0x03, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x03, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
  0x02, 0x01, 0x03, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x03, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
  0x02, 0x01, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
  0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
  0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
  0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02,
  0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x02,
  0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02,
  0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x02,
  0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02,
  0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x02,
  0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02
};

GUI_CONST_STORAGE GUI_BITMAP bmTestPicture8 = {
 24,               // XSize
 24,               // YSize
 24,               // BytesPerLine
 8,                // BitsPerPixel
 acTestPicture8,   // Pointer to picture data (indices)
 &PalTestPicture8  // Pointer to palette
};

/********************************************************************
*
*       Static data, test picture 4bpp
*
*********************************************************************
*/

GUI_CONST_STORAGE GUI_COLOR ColorsTestPicture4[] = {
     0xFFFFFF,0x0000FF,0x000000,0x00FFFF
    ,0xFF8000,0xFF8080,0xFF0040,0x00FF00
    ,0xFFFF00,0x4080FF
};

GUI_CONST_STORAGE GUI_LOGPALETTE PalTestPicture4 = {
  10,	// number of entries
  0, 	// Has transparency
  &ColorsTestPicture4[0]
};

GUI_CONST_STORAGE unsigned char acTestPicture4[] = {
  0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
  0x21, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x20, 0x00, 0x00, 0x02,
  0x21, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x31, 0x22, 0x22, 0x22, 0x22,
  0x21, 0x34, 0x44, 0x44, 0x45, 0x55, 0x56, 0x31, 0x20, 0x00, 0x00, 0x02,
  0x21, 0x34, 0x44, 0x45, 0x55, 0x56, 0x66, 0x31, 0x22, 0x22, 0x22, 0x22,
  0x21, 0x34, 0x45, 0x55, 0x56, 0x66, 0x67, 0x31, 0x20, 0x00, 0x00, 0x02,
  0x21, 0x35, 0x55, 0x56, 0x66, 0x67, 0x78, 0x31, 0x22, 0x22, 0x22, 0x22,
  0x21, 0x35, 0x56, 0x66, 0x67, 0x77, 0x88, 0x31, 0x20, 0x00, 0x00, 0x02,
  0x21, 0x36, 0x66, 0x67, 0x77, 0x78, 0x81, 0x31, 0x22, 0x22, 0x22, 0x22,
  0x21, 0x36, 0x67, 0x77, 0x78, 0x88, 0x11, 0x31, 0x20, 0x00, 0x00, 0x02,
  0x21, 0x37, 0x77, 0x78, 0x88, 0x81, 0x11, 0x31, 0x22, 0x22, 0x22, 0x22,
  0x21, 0x37, 0x78, 0x88, 0x88, 0x11, 0x19, 0x31, 0x20, 0x00, 0x00, 0x02,
  0x21, 0x38, 0x88, 0x88, 0x81, 0x11, 0x99, 0x31, 0x22, 0x22, 0x22, 0x22,
  0x21, 0x38, 0x88, 0x88, 0x11, 0x19, 0x99, 0x31, 0x20, 0x00, 0x00, 0x02,
  0x21, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x31, 0x22, 0x22, 0x22, 0x22,
  0x21, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x20, 0x00, 0x00, 0x02,
  0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x02, 0x02, 0x02,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x02, 0x02, 0x02,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x02, 0x02, 0x02,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22,
  0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22
};

GUI_CONST_STORAGE GUI_BITMAP bmTestPicture4 = {
 24,               // XSize
 24,               // YSize
 12,               // BytesPerLine
 4,                // BitsPerPixel
 acTestPicture4,   // Pointer to picture data (indices)
 &PalTestPicture4  // Pointer to palette
};

/********************************************************************
*
*       Static data, test picture 2bpp
*
*********************************************************************
*/

GUI_CONST_STORAGE GUI_COLOR ColorsTestPicture2[] = {
     0xFFFFFF,0xFF0000,0xFFFF00,0x000000
};

GUI_CONST_STORAGE GUI_LOGPALETTE PalTestPicture2 = {
  4,	// number of entries
  0, 	// Has transparency
  &ColorsTestPicture2[0]
};

GUI_CONST_STORAGE unsigned char acTestPicture2[] = {
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xD5, 0x55, 0x55, 0x55, 0xC0, 0x03,
  0xDA, 0xAA, 0xAA, 0xA9, 0xFF, 0xFF,
  0xDA, 0xAA, 0xAA, 0x99, 0xC0, 0x03,
  0xDA, 0xAA, 0xA9, 0x59, 0xFF, 0xFF,
  0xDA, 0xAA, 0x95, 0x59, 0xC0, 0x03,
  0xDA, 0xA9, 0x55, 0x69, 0xFF, 0xFF,
  0xDA, 0x95, 0x55, 0xA9, 0xC0, 0x03,
  0xD9, 0x55, 0x56, 0x99, 0xFF, 0xFF,
  0xD9, 0x55, 0x6A, 0x59, 0xC0, 0x03,
  0xD9, 0x56, 0xA9, 0x59, 0xFF, 0xFF,
  0xD9, 0x6A, 0xA5, 0x69, 0xC0, 0x03,
  0xDA, 0xAA, 0x95, 0xA9, 0xFF, 0xFF,
  0xDA, 0xAA, 0x56, 0xA9, 0xC0, 0x03,
  0xDA, 0xAA, 0xAA, 0xA9, 0xFF, 0xFF,
  0xD5, 0x55, 0x55, 0x55, 0xC0, 0x03,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xCC, 0xCC, 0xCC, 0xCC, 0xF3, 0x33,
  0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCF,
  0xCC, 0xCC, 0xCC, 0xCC, 0xF3, 0x33,
  0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCF,
  0xCC, 0xCC, 0xCC, 0xCC, 0xF3, 0x33,
  0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

GUI_CONST_STORAGE GUI_BITMAP bmTestPicture2 = {
 24,               // XSize
 24,               // YSize
 6,                // BytesPerLine
 2,                // BitsPerPixel
 acTestPicture2,   // Pointer to picture data (indices)
 &PalTestPicture2  // Pointer to palette
};

/********************************************************************
*
*       Static data, test picture 1bpp
*
*********************************************************************
*/

GUI_CONST_STORAGE GUI_COLOR ColorsTestPicture1[] = {
     0xFFFFFF,0x000000
};

GUI_CONST_STORAGE GUI_LOGPALETTE PalTestPicture1 = {
  2,	// number of entries
  0, 	// Has transparency
  &ColorsTestPicture1[0]
};

GUI_CONST_STORAGE unsigned char acTestPicture1[] = {
  XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, X______X,
  XX______, _______X, XXXXXXXX,
  XX______, _____X_X, X______X,
  XX______, ___XXX_X, XXXXXXXX,
  XX______, _XXXXX_X, X______X,
  XX_____X, XXXXX__X, XXXXXXXX,
  XX___XXX, XXXX___X, X______X,
  XX_XXXXX, XXX__X_X, XXXXXXXX,
  XX_XXXXX, X___XX_X, X______X,
  XX_XXXX_, ___XXX_X, XXXXXXXX,
  XX_XX___, __XXX__X, X______X,
  XX______, _XXX___X, XXXXXXXX,
  XX______, XXX____X, X______X,
  XX______, _______X, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, X______X,
  XXXXXXXX, XXXXXXXX, XXXXXXXX,
  X_X_X_X_, X_X_X_X_, XX_X_X_X,
  X_X_X_X_, X_X_X_X_, X_X_X_XX,
  X_X_X_X_, X_X_X_X_, XX_X_X_X,
  X_X_X_X_, X_X_X_X_, X_X_X_XX,
  X_X_X_X_, X_X_X_X_, XX_X_X_X,
  X_X_X_X_, X_X_X_X_, X_X_X_XX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX
};

GUI_CONST_STORAGE GUI_BITMAP bmTestPicture1 = {
 24,               // XSize
 24,               // YSize
 3,                // BytesPerLine
 1,                // BitsPerPixel
 acTestPicture1,   // Pointer to picture data (indices)
 &PalTestPicture1  // Pointer to palette
};

/********************************************************************
*
*       Static code
* 
*********************************************************************
*/
/********************************************************************
*
*       _DrawBitmap
*/
static void _DrawBitmap(const GUI_BITMAP* pBitmap, int x, int y, int Trans, int XOr) {
  GUI_LOGPALETTE Pal;
  GUI_BITMAP     Bitmap;
  int            PrevDM;

  memcpy(&Bitmap, pBitmap, sizeof(GUI_BITMAP));
  memcpy(&Pal, Bitmap.pPal, sizeof(GUI_LOGPALETTE));
  Bitmap.pPal  = &Pal;
  Pal.HasTrans = Trans;
  PrevDM = GUI_SetDrawMode((XOr) ? GUI_DM_XOR : 0);
  GUI_DrawBitmap(&Bitmap, x, y);
  GUI_SetDrawMode(PrevDM);
}

/********************************************************************
*
*       _Draw
*
* Function description
*   Draws some coloured output to the current device (display or memory device)
*/
static void _Draw(void) {
  int i;

  GUI_SetBkColor(GUI_YELLOW);
  GUI_ClearRect(0, 75, 319, 138);
  GUI_SetColor(GUI_RED);
  GUI_SetFont(&GUI_Font32B_ASCII);
  GUI_DispStringHCenterAt("Printing demo", 160, 75);
  for (i = 0; i < 12; i += 2) {
    GUI_SetColor(GUI_GREEN);
    GUI_DrawRect(5 + i, 80 + i, 50 - i, 101 - i);
    GUI_SetColor(GUI_BLUE);
    GUI_DrawRect(269 + i, 80 + i, 314 - i, 101 - i);
  }
  for (i = 0; i < 32; i += 8) {
    GUI_SetColor(GUI_BLUE);
    GUI_FillRect(0, 107 + i, 319, 110 + i);
    GUI_SetColor(GUI_YELLOW);
    GUI_FillRect(0, 111 + i, 319, 114 + i);
  }
  i = 8;
  _DrawBitmap(&bmTestPicture1, i +   0, 111, 0, 0);
  _DrawBitmap(&bmTestPicture1, i +  35, 111, 1, 0);
  _DrawBitmap(&bmTestPicture1, i +  70, 111, 0, 1);
  _DrawBitmap(&bmTestPicture2, i + 105, 111, 0, 0);
  _DrawBitmap(&bmTestPicture2, i + 140, 111, 1, 0);
  _DrawBitmap(&bmTestPicture4, i + 175, 111, 0, 0);
  _DrawBitmap(&bmTestPicture4, i + 210, 111, 1, 0);
  _DrawBitmap(&bmTestPicture8, i + 245, 111, 0, 0);
  _DrawBitmap(&bmTestPicture8, i + 280, 111, 1, 0);
}

/********************************************************************
*
*       _SendToPrinter
*
* Function description
*   Shows the contents of a memory device on the display
*   In "real life", this routine would send the contents of the memory
*   device to the printer.
*/
static void _SendToPrinter(GUI_MEMDEV_Handle hMem, int yOff) {
  int  xSize;
  int  ySize;
  int  x;
  int  y;
  int  Offset;
  int  Bit;
  U8  * pData;

  //
  // Get the size of the memory device
  //
  xSize = GUI_MEMDEV_GetXSize(hMem);
  ySize = GUI_MEMDEV_GetYSize(hMem);
  pData = (U8 *)GUI_MEMDEV_GetDataPtr(hMem); // Get the data pointer of the memory device
  //
  // Draw the pixels of the memory device on the screen
  //
  for (y = 0; y < ySize; y++) {
    for (x = 0; x < xSize; x++) {
      U8 Index;
      Offset = x >> 3;
      Bit    = 7 - (x & 7);
      Index  = *(pData + Offset);
      Index &= (1 << Bit);
      //
      // The index could be 0 or 1, so use black and white
      //
      if (Index == 0 ) {
        GUI_SetColor(GUI_BLACK);
      } else {
        GUI_SetColor(GUI_WHITE);
      }
      //
      // Draw the pixel. At this point for example a printer output routine can be called
      //
      GUI_DrawPixel(x, y + yOff);
    }
    pData += (xSize + 7) / 8;
  }
}

/********************************************************************
*
*       _Explain
*/
static void _Explain(void) {
  unsigned i;

  GUI_SetTextMode(GUI_TM_XOR);
  GUI_SetFont(&GUI_Font16B_ASCII);
  GUI_DispStringHCenterAt("MEMDEV_Printing", 160, 0);
  GUI_GotoXY(0,20);
  GUI_SetFont(&GUI_Font6x8);
  for (i = 0; i < GUI_COUNTOF(_acText); i++) {
    GUI_DispString(_acText[i]);
    GUI_DispNextLine();
  }
  GUI_SetTextMode(GUI_TM_TRANS);
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       MainTask
*/
void MainTask(void) {
  GUI_MEMDEV_Handle hMem;

  GUI_Init();
  _Explain();               // Explain the sample
  GUI_SetFont(&GUI_Font16B_ASCII);
  GUI_SetColor(GUI_WHITE);
  GUI_DispStringAt("Screen output:", 5, 59);
  _Draw();                  // Draw "Preview" on the screen
  //
  // Create and select a memory device with 1bpp color conversion (B/W)
  //
  hMem = GUI_MEMDEV_CreateFixed(0, 75, 320, 64, 0, 
                                GUI_MEMDEV_APILIST_1, // Used API list
                                GUI_COLOR_CONV_1);    // Black/white color conversion
  GUI_MEMDEV_Select(hMem);
  _Draw();                  // Call the same routine as before for drawing into memory device
  GUI_MEMDEV_Select(0);     // Switch back from memory device to the display
  GUI_SetFont(&GUI_Font16B_ASCII);
  GUI_SetColor(GUI_WHITE);
  GUI_DispStringAt("Printer output:", 5, 154);
  _SendToPrinter(hMem, 170);
  //
  // Cleanup, end-of sample
  //
  GUI_MEMDEV_Delete(hMem);
  while(1) {
    GUI_Exec();
  }
}

/*************************** End of file ****************************/
