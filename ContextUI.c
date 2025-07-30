/*
该文件中为UI主要逻辑
*/

#include "ContextUI.h"
#include "ContextUI_draw.h"
#include "ContextUI_build.h"

//当前层级指针
#define CUI_P (cui_stack[cui_layer])
//当前层数下的列表指针
#define CUI_LIST (cui_stack[cui_layer].cui_list_p)
//当前层数下的Tab指针
#define CUI_TAB (cui_stack[cui_layer].cui_tab_p)
//当前层数下的选择框指针
#define CUI_SELECT (cui_stack[cui_layer].cui_select)
//当前层数下的显示起始TAB
#define CUI_START_TAB (cui_stack[cui_layer].start_tab)
//当前Tab的widget指针
#define CUI_WIDGET (CUI_LIST->Tabs[CUI_TAB]->link_widget)

/*层级指针堆*/
static CUI_LayerPointer cui_stack[MAX_LAYER];

/*当前层数*/
static u8 cui_layer;

/*当前状态*/
static u8 cui_state;

/*选择框的样式*/
SelectStyle select_style;

/*当前应用*/
static u8 cui_app;

/*当前页面*/
static CUI_Page* cui_page;

/*当选的状态枚举*/
enum STATE {
    MAINMENU,   //在主菜单
    INFIRSTLIST,//在初级列表
    INLIST,     //在列表中
    INWIN,      //在弹窗中
    INNEWPAGE   //在新建页面
};

/**
 * @brief UI的初始化
 */
void CUI_Init()
{
    cui_app = 0;                        //初始化应用:第一个
    cui_state = MAINMENU;               //初始化状态:在主菜单中
    cui_layer = 0;                      //初始化层数:第0层
    select_style = SELECT_RECT;         //初始化选择框:矩形样式
    CUI_BuildMainMenu(cui_app);         //初始化后构建主菜单
}

/**
 * @brief 进入个列表
 * @param list 要进入的列表指针
 */
static void CUI_EnterList(CUI_List* list)
{
    if(!list) return;
    if(cui_layer + 1 > MAX_LAYER) return;

    cui_layer++;

    CUI_LIST = list;
    CUI_TAB = 0;
    CUI_START_TAB = 0;
    CUI_SELECT = 0;

    cui_state = (cui_layer == 1) ? INFIRSTLIST : INLIST;

    CUI_BuildList(&CUI_P);
    CUI_Refresh();
}

/**
 * @brief 进入一个新页面
 * @param page 要进入的页面
 */
void CUI_EnterPage(CUI_Page* page)
{
    cui_page = page;
    cui_state = INNEWPAGE;
    CUI_Clear();
    CUI_Refresh();
    cui_page->fun();
}

/**
 * @brief 退出一个新页面
 */
static void CUI_ExitPage()
{
    cui_page = 0;
    cui_state = (cui_layer == 0) ? MAINMENU :  (cui_layer == 1) ? INFIRSTLIST : INLIST;
    (cui_layer == 0) ? CUI_BuildMainMenu(cui_app) : CUI_BuildList(&CUI_P);
}

/**
 * @brief 从某级列表退出
 */
static void CUI_ExitList()
{
    if(cui_layer == 0) return;

    cui_layer--;

    cui_state = (cui_layer == 0) ? MAINMENU :  (cui_layer == 1) ? INFIRSTLIST : INLIST;
    (cui_layer == 0) ? CUI_BuildMainMenu(cui_app) : CUI_BuildList(&CUI_P);
    CUI_Refresh();
}

/**
 * @brief 进入进度条界面
 * @param percent 
 */
static void CUI_EnterPercent(CUI_Widget* percent)
{
    CUI_BuildPercentWIndow(percent->respond_value);
    cui_state = INWIN;
}

/**
 * @brief 确定操作
 */
void CUI_Enter() {
    switch (cui_state)
    {
        //在主菜单时确定键执行应用响应函数或进入列表
    case MAINMENU:
        if(MainMenu[cui_app].son_list)
            CUI_EnterList(MainMenu[cui_app].son_list);
        if(MainMenu[cui_app].respond)
            MainMenu[cui_app].respond();
        break;
        //在列表中时，确定键响应当前Tab中的函数或进入列表
    case INFIRSTLIST:
    case INLIST:
        //如果有son_list就进入该list 
        CUI_EnterList(CUI_LIST->Tabs[CUI_TAB]->son_list);
        //否则执行Tab关联的widget的响应函数
        if(!CUI_WIDGET) break;
        if(CUI_WIDGET->widget_type == WIDGET_PERCENTBAR)  
            CUI_EnterPercent(CUI_LIST->Tabs[CUI_TAB]->link_widget);
        if(!CUI_WIDGET->respond) break;
            CUI_WIDGET->respond();
        if(CUI_WIDGET->widget_type != WIDGET_PAGE && CUI_WIDGET->widget_type != WIDGET_PERCENTBAR)
            CUI_BuildList(&CUI_P);
        break;
        //在新页面中，执行新页面的确认键函数
    case INNEWPAGE:
        if(cui_page->control[0])
            cui_page->control[0]();
        break;
    case INWIN:
        if(CUI_WIDGET->respond)
            CUI_WIDGET->respond();
        CUI_BuildList(&CUI_P);
        cui_state = (cui_layer == 1) ? INFIRSTLIST : INLIST;
        break;
    default:
        break;
    }
}

/**
 * @brief 退出操作
 */
void CUI_Exit() {
    switch (cui_state)
    {
        //在列表中，退出该列表
    case INFIRSTLIST:
    case INLIST:
        CUI_ExitList();
        break;
        //在新页面中返回其父列表或主菜单
    case INNEWPAGE:
        if(cui_page->control[1])
            cui_page->control[1]();
        CUI_ExitPage();
        break;
    case INWIN:
        if(CUI_WIDGET->respond)
            CUI_WIDGET->respond();
        CUI_BuildList(&CUI_P);
        cui_state = (cui_layer == 1) ? INFIRSTLIST : INLIST;
        break;
    default:
        break;
    }
}



/**
 * @brief 向上移动光标或向左滚动列表
 */
void CUI_Leftward() {
    switch (cui_state)
    {
        //主菜单中左移
    case MAINMENU:
        if (cui_app > 0)
            cui_app--;
        CUI_BuildMainMenu(cui_app);
        break;
        //列表中向下移动
    case INFIRSTLIST:
    case INLIST:
        // 不能再往上
        if (CUI_TAB == 0)
            break;
        CUI_TAB--;
        // 判断移动光标 还是 滚动窗口
        if (CUI_SELECT > 0) {
            CUI_SELECT--;
        } else if (CUI_START_TAB > 0) {
            CUI_START_TAB--;
        }
        CUI_BuildList(&CUI_P);
        break;
        //新建页中执行对应函数
    case INNEWPAGE:
        if(cui_page->control[2])
            cui_page->control[2]();
        break;
    case INWIN:
        if(CUI_WIDGET->respond_value <= 0) break;
        CUI_WIDGET->respond_value--;
        CUI_BuildPercentWIndow(CUI_WIDGET->respond_value);
        break;
    default:
        break;
    }
}

/**
 * @brief 向下移动光标或右滚动列表
 */
void CUI_Rightward() {
    switch (cui_state)
    {
    case MAINMENU:
        if (cui_app < APP_NUM - 1)
            cui_app++;
        CUI_BuildMainMenu(cui_app);
        break;
    case INFIRSTLIST:
    case INLIST: 
        // 判断是否还有下一个Tab
        if (!CUI_LIST->Tabs[CUI_TAB+ 1])
            break;
        CUI_TAB++;
        // 判断移动光标还是滚动窗口
        if (CUI_SELECT < 3) {
            CUI_SELECT++;
        } else {
            // 说明当前光标已在最底行
            CUI_START_TAB++;
        }
        CUI_BuildList(&CUI_P);
        break;
    case INNEWPAGE:
        if(cui_page->control[3])
            cui_page->control[3]();
        break;
    case INWIN:
        if(CUI_WIDGET->respond_value >= 100) break;
        CUI_WIDGET->respond_value++;
        CUI_BuildPercentWIndow(CUI_WIDGET->respond_value);
    default:
        break;
    }
}