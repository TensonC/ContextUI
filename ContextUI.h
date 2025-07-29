#ifndef _CONTEXTUI_H_
#define _CONTEXTUI_H_

#include "all.h"
#include "ContextUI_draw.h"

#define MAX_TAB_IN_LIST 10 //列表中的Tab数最大值
#define MAX_LAYER 5        //最大层数
#define APP_NUM 5          //应用的个数

typedef u8 WidgetType;
typedef u8 SelectStyle;
typedef struct CUI_List CUI_List;
typedef struct CUI_Tab CUI_Tab;
typedef struct CUI_Widget CUI_Widget;
typedef struct CUI_App CUI_App;
typedef struct CUI_Page CUI_Page;
typedef struct  CUI_LayerPointer CUI_LayerPointer;

/*组件类型枚举*/
enum WIDGET {
    WIDGET_CHECKBOX,    //复选框组件
    WIDGET_DOT,         //复选点组件
    WIDGET_PERCENTBAR,  //进度指示条组件
    WIDGET_NUMBER,      //数字组件
    WIDGET_STR,         //字符串组件
    WIDGET_PAGE         //打开新页面
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
    const char* widget_name;            //该组件名字,或可作为字符串显示
    u8 respond_value;                   //响应数值,作为字符串显示的时候填入字符串的长度
    void (*respond)();                  //响应函数
};

/*列表中的单个项目(Tab)*/
/*son_list和widget只能同时存在一个，如何两个都定义会优先进入新列表*/
struct CUI_Tab {
    const char* name;                         //该Tab的名字
    CUI_Widget* link_widget;                  //与该Tab相关联的组件
    CUI_List* son_list;                       //指向的下一个列表
};

/*列表结构体*/
struct CUI_List {
    const char* name;                   //该列表的名字
    CUI_Tab* Tabs[MAX_TAB_IN_LIST + 1]; //列表中的Tab
};

/*应用的结构体*/
struct CUI_App {
    const char* name;   //应用项目名
    const u8* Icon;     //应用项目图标(24*24 = 72)
    CUI_List* son_list; //应用下列表
    void (*respond)();
};

struct CUI_Page
{
    void (*fun)();
    void (*enter)();
    void (*left)();
    void (*right)();
};



/*层级指针结构体，负责确定当前打开页面的各种层级*/
struct CUI_LayerPointer
{
    CUI_List* cui_list_p;   //指向当前显示的列表
    u8 cui_tab_p;           //指向选择框当前选择的Tab索引
    u8 cui_select;          //选择框的位置
    u8 start_tab;           //当前界面起始Tab
    CUI_Page* cui_page;     //当前的页面
} ;


/*主菜单*/
extern CUI_App MainMenu[APP_NUM];
/*选择框的种类*/
extern SelectStyle select_style; 

void CUI_Init();
void CUI_Enter();
void CUI_Exit();
void CUI_Leftward();
void CUI_Rightward();
void CUI_EnterPage(CUI_Page* page);


#endif