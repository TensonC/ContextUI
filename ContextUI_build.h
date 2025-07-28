#ifndef _CONTEXTUI_BUILD_H_
#define _CONTEXTUI_BUILD_H_

#include "all.h"
#include "ContextUI.h"


void CUI_BuildRRect(u8 x,u8 y,u8 w,u8 h);
void CUI_BuildFillRRect(u8 x,u8 y,u8 w,u8 h);
void CUI_BuildDot(u8 x,u8 y, u8 m);
void CUI_BuildCheckBox(u8 x,u8 y,u8 m);
void CUI_BuildPercentBar(u8 x,u8 y,u8 len,u8 precent);
void CUI_BuildNum(u8 x,u8 y,u16 num,u8 len);
void CUI_BuildList(CUI_LayerPointer* l_p);
void CUI_BuildMainMenu(u8 current_app);

#endif