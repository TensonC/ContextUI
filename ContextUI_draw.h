#ifndef _CONTEXTUI_DRAW_H_
#define _CONTEXTUI_DRAW_H_

#include "all.h"

#define SCREEN_H 64
#define SCREEN_W 128
#define PAGE 8



void CUI_Refresh();
void CUI_Clear();
void CUI_Fill();
void CUI_PutChar_16(u8 x,u8 y,char ch);
void CUI_PutStr_16(u8 x,u8 y,const char* str);
void CUI_PutNum_16(u8 x,u8 y, u16 num, u8 len);
void CUI_PutPic(u8 x,u8 y,const u8* pic);
void CUI_PutPoint(u8 x,u8 y);
void CUI_ClearPoint(u8 x,u8 y);
void CUI_PutBiasLine(u8 x_s,u8 y_s,u8 x_e,u8 y_e);
void CUI_PutCircle(u8 x0,u8 y0,u8 r,u8 m);
void CUI_PutLine(u8 x_s,u8 y_s,u8 x_e,u8 y_e);
void CUI_PutRect(u8 x1,u8 y1,u8 w,u8 h,u8 m);
void CUI_ClearCircle(u8 x0,u8 y0,u8 r,u8 m);
void CUI_ClearLine(u8 x_s,u8 y_s,u8 x_e,u8 y_e);
void CUI_ClearRect(u8 x1,u8 y1,u8 w,u8 h,u8 m);
void CUI_DrawSelect(const char* title,u8 position,u8 select_style);

#endif