#ifndef _CONTEXTUI_H_
#define _CONTEXTUI_H_

#include "all.h"

#define MAX_TAB_IN_LIST 10 //列表中的Tab数最大值
#define APP_NUM 5         //应用的个数

typedef u8 WidgetType;
typedef struct CUI_List CUI_List;
typedef struct CUI_Tab CUI_Tab;
typedef struct CUI_Widget CUI_Widget;
typedef struct CUI_App CUI_App;

/*组件类型枚举*/
enum WIDGET {
    WIDGET_CHECKBOX,
    WIDGET_DOT,
    WIDGET_PERCENTBAR,
    WIDGET_NUMBER
};

/*光标类型 */
enum SELECT {
    SELECT_POINT,
    SELECT_LINE,
    SELECT_RECT
};

/*组件结构体*/
struct CUI_Widget
{
    WidgetType widget_type;             //组件类型,构建画面时根据组件类型确定相应绘制函数
    u8 respond_value;                   //响应数值
    void (*respond)();                  //响应函数
};


/*列表中的单个项目(Tab)*/
/*son_list和widget只能同时存在一个，如何两个都定义会优先进入新列表*/
struct CUI_Tab {
    const char* name;                         //该Tab的名字
    CUI_Widget* link_widget;                  //与该Tab相关联的组件
    CUI_List* son_list;                       //指向的下一个列表
    void (*respond)(); //Tab的widget响应函数
};

/*列表结构体*/
struct CUI_List {
    const char* name;               //该列表的名字
    CUI_List* father_list;          //该列表的前一个列表
    CUI_Tab* Tabs[MAX_TAB_IN_LIST];  //列表中的Tab
};

/*应用的结构体*/
struct CUI_App {
    const char* name;   //应用项目名
    const u8* Icon;       //应用项目图标(24*24 = 72)
    CUI_List* son_list; //应用下列表
    void (*respond)();  //应用响应函数
};

typedef struct CUI_Page
{
    const char* page_name;
    void (*enter)();
    void (*exit)();
    void (*left)();
    void (*right)();
} CUI_Page;


/*主菜单*/
extern CUI_App MainMenu[APP_NUM];

void CUI_Init();
void CUI_Enter();
void CUI_Exit();
void CUI_Leftward();
void CUI_Rightward();


#endif