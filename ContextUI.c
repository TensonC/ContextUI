#include "ContextUI.h"
#include "ContextUI_draw.h"
#include "ContextUI_build.h"

typedef struct  CUI_LayerPointer CUI_LayerPointer;

/*层级指针结构体，负责确定当前打开页面的各种层级*/
struct CUI_LayerPointer
{
    u8 cui_app_p;           //指向当前应用
    CUI_List* cui_list_p;   //指向当前显示的列表
    u8 cui_tab_p;           //指向选择框当前选择的Tab索引
    CUI_Page* cui_page;      //当前的页面
} ;

/*ui指针实例化*/
static CUI_LayerPointer cui_p;
/*ui当前层级*/
static u8 cui_layer;
/*ui当前状态*/
static u8 cui_state;
/*状态枚举*/
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
    cui_p.cui_app_p = 0;
    cui_p.cui_list_p = (void*)0;
    cui_p.cui_tab_p = 0;
    cui_state = MAINMENU;
    cui_layer = 0;
    CUI_BuildMainMenu(cui_p.cui_app_p);
}

/**
 * @brief 进入某级列表
 * @param list 要进入的列表指针
 */
void CUI_EnterList(CUI_List* list)
{
    cui_state = INLIST;
    cui_layer++;
    cui_p.cui_list_p = list;                      //list指针指向要打开的列表
    cui_p.cui_tab_p = 0;                          //新列表的Tab指针指向第一个
    CUI_BuildList(cui_p.cui_list_p);
    CUI_Refresh();
}

/**
 * @brief 进入一个新页面
 * @param page 要进入的页面
 */
void CUI_EnterPage(CUI_Page* page)
{
    cui_state = INNEWPAGE;
    cui_layer++;
    cui_p.cui_page = page;
    CUI_Clear();
    CUI_Refresh();
}

/**
 * @brief 从某级列表退出
 * @param list 当前的列表指针
 */
void CUI_ExitList(CUI_List* list)
{
    cui_p.cui_list_p = list->father_list;        //list指针指向其父级指针
    cui_layer--;
    if(cui_layer == 1) cui_state = INFIRSTLIST;//如果层级是1，状态变成第一级列表
    CUI_BuildList(cui_p.cui_list_p);
    CUI_Refresh();
}

/**
 * @brief 确定操作
 */
void CUI_Enter() {
    switch (cui_state)
    {
        //在主菜单时确定键执行应用响应函数或进入列表
    case MAINMENU:
        if(MainMenu[cui_p.cui_app_p].son_list) {
            CUI_EnterList(MainMenu[cui_p.cui_app_p].son_list);
            cui_state = INFIRSTLIST;
        }
        else {
            MainMenu[cui_p.cui_app_p].respond();
        }
        break;
        //在列表中时，确定键响应当前Tab中的函数或进入列表
    case INFIRSTLIST:
    case INLIST:
        //如果有son_list就进入该list
        if (cui_p.cui_list_p->Tabs[cui_p.cui_tab_p]->son_list)  {
            CUI_EnterList(cui_p.cui_list_p->Tabs[cui_p.cui_tab_p]->son_list);
        }
        //否则执行Tab函数
        else {
            cui_p.cui_list_p->Tabs[cui_p.cui_tab_p]->respond();
        CUI_BuildList(cui_p.cui_list_p);
        }
        break;
        //在新页面中，执行新页面的确认键函数
    case INNEWPAGE:
        cui_p.cui_page->enter();
        break;
    default:
        break;
    }
    CUI_Refresh();
}

/**
 * @brief 退出操作
 */
void CUI_Exit() {
    switch (cui_state)
    {
        //在初级列表中，返回到主菜单
    case INFIRSTLIST:
        CUI_BuildMainMenu(cui_p.cui_app_p);
        cui_state = MAINMENU;
        cui_layer--;
        break;
        //在列表中，返回到父级列表
    case INLIST:
        CUI_ExitList(cui_p.cui_list_p);
        break;
        //在新页面中返回其父列表或主菜单
    case INNEWPAGE:
        cui_layer--;
        if(cui_layer) CUI_BuildMainMenu(cui_p.cui_app_p);
        else 
            CUI_BuildList(cui_p.cui_list_p);
        cui_state = INLIST;
        break;
    default:
        break;
    }
}



/**
 * @brief 向左操作
 */
void CUI_Leftward() {
    switch (cui_state)
    {
        //左选应用
    case MAINMENU:
        if(cui_p.cui_app_p > 0)
            cui_p.cui_app_p--;
        CUI_BuildMainMenu(cui_p.cui_app_p);
        break;
        //下移选择框
    case INFIRSTLIST:
    case INLIST:
        if(cui_p.cui_tab_p > 0)
            cui_p.cui_tab_p--;
        CUI_BuildList(cui_p.cui_list_p);
        break;
    case INNEWPAGE:
        cui_p.cui_page->left();
        break;
    default:
        break;
    }
}

/**
 * @brief 向右操作
 */
void CUI_Rightward() {
    switch (cui_state)
    {
    case MAINMENU:
        if(cui_p.cui_app_p < APP_NUM - 1)
            cui_p.cui_app_p++;
        CUI_BuildMainMenu(cui_p.cui_app_p);
        break;
    case INFIRSTLIST:
    case INLIST:
        if(cui_p.cui_list_p->Tabs[cui_p.cui_tab_p])
            cui_p.cui_tab_p++;
        CUI_BuildList(cui_p.cui_list_p);
        break;
    case INNEWPAGE:
        cui_p.cui_page->right();
        break;
    default:
        break;
    }
}