#include "ContextUI_build.h"
#include "ContextUI_draw.h"


/*
该文件中为UI所有物体的构建函数
*/


#define TITLE_LEFT_OFFEST 6     //标题的左侧偏移值
#define WIDGET_RIGHT_OFFEST 25  //组件的右侧偏移值


#define CHECKBOX_LEN 8  //复选框的边长
#define DOT_R 3         //指示点的半径
#define PERCENTBAR_H 6  //进度指示条的宽度

#define DOT_SPACING 6   //主菜单应用指示点间距
#define ICON_W 24

static u16 Pow10[] = {
 1,10,100,1000,10000
};



/*复选框的位置偏移参数 */
const u8 checkbox_offest[] = {
    (WIDGET_RIGHT_OFFEST - CHECKBOX_LEN / 2),4
};
/*复选点的位置偏移参数*/
const u8 dot_offest[] = {
    (WIDGET_RIGHT_OFFEST - DOT_R - 5),8
};

static void CUI_PutStr(u8 x,u8 y,const char* str)
{
    u8 i;
    while (*str && i < 10)
    {
        CUI_PutChar_16(x,y,*str++);
        x += 8;
        i++;
    }
}

/**
 * @brief 绘制略带弧形的框
 * @param x 横坐标
 * @param y 纵坐标
 * @param w 宽
 * @param h 高
 */
void CUI_BuildRRect(u8 x,u8 y,u8 w,u8 h)
{
    CUI_PutLine(x + 3,y,x + w - 3, y);
    CUI_PutPoint(x + 2,y + 1);
    CUI_PutPoint(x + 1,y + 2);
    CUI_PutLine(x + 3,y + h,x + w - 3, y + h);
    CUI_PutPoint(x + 2,y + h - 1);
    CUI_PutPoint(x + 1,y + h - 2);
    CUI_PutLine(x, y + 3, x, y + h - 3);
    CUI_PutPoint(x + w - 2,y + 1);
    CUI_PutPoint(x + w - 1,y + 2);
    CUI_PutLine(x + w, y + 3, x + w, y + h - 3);
    CUI_PutPoint(x + w - 2,y + h - 1);
    CUI_PutPoint(x + w - 1,y + h - 2);
}

/**
 * @brief 绘制略带弧形的实心框
 * @param x 横坐标
 * @param y 纵坐标
 * @param w 宽
 * @param h 高
 */
void CUI_BuildFillRRect(u8 x,u8 y,u8 w,u8 h)
{
    u8 i;
    CUI_PutLine(x + 3, y,     x + w - 3, y);
    CUI_PutLine(x + 2, y + 1, x + w - 2, y + 1);
    CUI_PutLine(x + 1, y + 2, x + w - 1, y + 2);
    for (i = y + 3; i <= y + h - 3; i++)
    {
        CUI_PutLine(x, i, x + w, i);
    }
    CUI_PutLine(x + 1, y + h - 2, x + w - 1, y + h - 2);
    CUI_PutLine(x + 2, y + h - 1, x + w - 2, y + h - 1);
    CUI_PutLine(x + 3, y + h,     x + w - 3, y + h);
}

/**
 * @brief 绘制一个指示点(长度为6)
 * @param x 横坐标
 * @param y 纵坐标
 * @param m 选择状态 1 选中 0 未选中
 */
void CUI_BuildDot(u8 x,u8 y, u8 m)
{
    CUI_PutCircle(x,y,DOT_R,0);
    switch (m)
    {
    case 0:
        CUI_ClearCircle(x,y,DOT_R - 2,1);
        break;
    case 1:
        CUI_PutCircle(x,y,DOT_R - 2,1);
        break;
    default:
        break;
    }
}

/**
 * @brief 绘制复选框(长度为8)
 * @param x 横坐标
 * @param y 纵坐标
 * @param m 选择状态 1 选中 0 未选中
 */
void CUI_BuildCheckBox(u8 x,u8 y,u8 m)
{
    CUI_PutRect(x,y,CHECKBOX_LEN,CHECKBOX_LEN,0);
    switch (m)
    {
    case 0:
        CUI_ClearRect(x + 2,y + 2,CHECKBOX_LEN - 4,CHECKBOX_LEN - 4,1);
        break;
    case 1:
        CUI_PutRect(x + 2,y + 2,CHECKBOX_LEN - 4,CHECKBOX_LEN - 4,1);
        break;
    default:
        break;
    }
}


/**
 * @brief 绘制数字
 * @param x 横坐标
 * @param y 纵坐标
 * @param num 数字
 * @param len 数字长度
 */
void CUI_BuildNum(u8 x,u8 y,u16 num,u8 len)
{
    CUI_PutNum_16(x,y,num,len);
}

/**
 * @brief 绘制进度指示条(高度为6)
 * @param x 横坐标
 * @param y 纵坐标
 * @param len 指示条长度
 * @param precent 进度百分比
 */
void CUI_BuildPercentBar(u8 x,u8 y,u8 len,u8 percent)
{
    u8 inner = len - 4;
    u16 per;
    if (percent > 100) percent = 100;
    per = (u16)percent * inner / 100;
    CUI_PutRect(x, y, len, PERCENTBAR_H, 0);
    if (per > 0) {
        CUI_PutRect(x + 2, y + 2, per, PERCENTBAR_H - 4, 1);
    }
    if (per < inner) {
        CUI_ClearRect(x + 2 + per, y + 2,
                       inner - per, PERCENTBAR_H - 4, 1);
    }
}

/**
 * @brief 构建一个Tab
 * @param tab 该Tab
 * @param col 所在列数
 */
static void CUI_BuildTab(CUI_Tab* tab,u8 col)
{
    //绘制标题
    CUI_PutStr_16(TITLE_LEFT_OFFEST,col,tab -> name);
    //绘制组件
    if(!tab->link_widget)
        return;
    switch (tab->link_widget->widget_type)
    {
    case WIDGET_CHECKBOX:
        CUI_BuildCheckBox(SCREEN_W - checkbox_offest[0],col * PAGE + checkbox_offest[1],
        tab->link_widget->respond_value);
        break;
    case WIDGET_DOT:
        CUI_BuildDot(SCREEN_W - dot_offest[0],col * PAGE + dot_offest[1],
        tab->link_widget->respond_value);
        break;
    case WIDGET_PERCENTBAR:
    case WIDGET_NUMBER:
        CUI_BuildNum(SCREEN_W - 30,col,
        tab->link_widget->respond_value, 3);
        break;
    case WIDGET_STR:
        CUI_PutStr_16(SCREEN_W - (8 * tab->link_widget->respond_value),col,
        tab->link_widget->widget_name);
        break;
    case WIDGET_PAGE:
        CUI_PutChar_16(SCREEN_W - WIDGET_RIGHT_OFFEST,col,'>');
        break;
    default:
        break;
    }
}

/**
 * @brief 构建列表
 * @param list 要构建的列表的指针
 */
void CUI_BuildList(CUI_LayerPointer* l_p)
{
    u8 i,j;
    CUI_Tab* tab;
    CUI_List* list = l_p->cui_list_p;
    CUI_Clear();
    for(i = l_p->start_tab,j = 0 ;list->Tabs[i] != 0; i++,j++) {
        tab = list->Tabs[i];
        CUI_BuildTab(tab,2 * j);
    }
    if(list->Tabs[l_p->cui_tab_p] != 0) {
        CUI_DrawSelect(list->Tabs[l_p->cui_tab_p]->name, l_p->cui_select, select_style);
    }
    CUI_Refresh();
}



/**
 * @brief 主菜单的构建
 */
void CUI_BuildMainMenu(u8 current_app)
{
    u8 len,i,x;
    u8 total_w,start_x;
    const char* str = MainMenu[current_app].name;
    for(len = 0; *str++; len++);
    CUI_Clear();
    CUI_PutPic((SCREEN_W - ICON_W) / 2 - 1,2,MainMenu[current_app].Icon);  //绘制图片
    CUI_PutStr_16(64 - 8 * (len / 2),6,MainMenu[current_app].name);    //绘制标题
    CUI_PutLine(8,47,120,47);
    total_w = APP_NUM * DOT_R + (APP_NUM - 1) * DOT_SPACING;
    start_x = (SCREEN_W - total_w) / 2;

    for (i = 0; i < APP_NUM; i++) {
        x = start_x + i * (DOT_R + DOT_SPACING);
        if (i == current_app)
            CUI_BuildDot(x, 4, 1);  // 当前APP的高亮点
        else
            CUI_BuildDot(x, 4, 0);
    }
    CUI_Refresh();
}
