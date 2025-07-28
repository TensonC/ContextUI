#include "ContextUI.h"


#define NULL (void*)0

void change_style();
void change_check_box();
void change_dot();


CUI_Widget ui_widget = {WIDGET_STR,"ContextUI",9,NULL};
CUI_Widget author_widget = {WIDGET_STR,"TensonC",7,NULL};
CUI_Widget version_widget = {WIDGET_STR,"v 0.7",5,NULL};
CUI_Widget device_widget = {WIDGET_STR,"oled",4,NULL};
CUI_Widget core_widget = {WIDGET_STR,"C",1,NULL};

const char* s[] = {"dot","line","rect"};
CUI_Widget select_style_widget = {WIDGET_STR,"rect",4,change_style};
void change_style()
{
    static u8 i = 0;
    i++;
    select_style = i % 3;
    select_style_widget.widget_name = s[select_style];
}



CUI_Tab ui = {"ui",&ui_widget,NULL};
CUI_Tab author = {"author",&author_widget,NULL};
CUI_Tab version = {"version",&version_widget,NULL};
CUI_Tab device = {"device",&device_widget,NULL};
CUI_Tab core = {"core",&core_widget,NULL};

CUI_List about_list = {
    "about_list",
    {&ui,&author,&version,&device,&core}
};

CUI_Tab about = {"- Information",NULL,&about_list};
CUI_Tab select = {"select",&select_style_widget,NULL};

CUI_List setting_list = {
    "setting_list",
    {&about,&select}
};

CUI_Tab t1 = {"1",NULL,NULL};
CUI_Tab t2 = {"2",NULL,NULL};
CUI_Tab t3 = {"3",NULL,NULL};
CUI_Tab t4 = {"4",NULL,NULL};
CUI_Tab t5 = {"5",NULL,NULL};
CUI_Tab t6 = {"6",NULL,NULL};
CUI_Tab t7 = {"7",NULL,NULL};
CUI_Tab t8 = {"8",NULL,NULL};
CUI_Tab t9 = {"9",NULL,NULL};
CUI_Tab t10 = {"10",NULL,NULL};
CUI_List mult_show = {
    "mult_show",
    {&t1,&t2,&t3,&t4,&t5,&t6,&t7,&t8,&t9,&t10}
};
CUI_Tab mult = {"Multi list",NULL,&mult_show};

CUI_Widget check = {WIDGET_CHECKBOX,"check_box",0,change_check_box};
void change_check_box()
{
    check.respond_value = !check.respond_value;
}
CUI_Tab check_box = {"Check Box",&check,NULL};

CUI_Widget check_dot = {WIDGET_DOT,"dot",0,change_dot};
void change_dot()
{
    check_dot.respond_value = !check_dot.respond_value;
}
CUI_Tab dot = {"Check Dot",&check_dot,NULL};

CUI_Widget str_wid = {WIDGET_STR,"a string",8,NULL};
CUI_Tab str = {"Str",&str_wid,NULL};

CUI_Widget num_wid = {WIDGET_NUMBER,"number",255,NULL};
CUI_Tab num = {"Number",&num_wid,NULL};

void fun()
{
    CUI_PutStr_16(0,0,"A new page!");
    CUI_Refresh();
}
void enter()
{
    CUI_PutStr_16(0,2,"ENTER!");
    CUI_Refresh();
}
void right()
{
    CUI_PutStr_16(0,2,"RIGHT!");
    CUI_Refresh();
}
void left()
{
    CUI_PutStr_16(0,2,"LEFT!");
    CUI_Refresh();
}
CUI_Page new_page = {fun,enter,right,left};
void open_new_page()
{
    CUI_EnterPage(&new_page);   
}
CUI_Widget page_wid = {WIDGET_PAGE,"page",0,open_new_page};
CUI_Tab page = {"Open Page",&page_wid,NULL};


CUI_List show_list = {
    "show_list",
    {&mult,&check_box,&dot,&str,&num,&page}
};