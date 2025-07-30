/*
演示demo
*/

#include "ContextUI.h"
#include "base.h"
#include "pico/stdlib.h"

const u8 set_pic[] = {
0xFF,0xFF,0xFF,0xFF,0x3F,0x1F,0x0F,0x0F,0x1F,0x1F,0x07,0x07,0x07,0x07,0x1F,0x1F,
0x0F,0x0F,0x1F,0x3F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC3,0xC3,0x00,0x00,0x00,
0x00,0x3C,0x7E,0x7E,0x7E,0x7E,0x3C,0x00,0x00,0x00,0x00,0xC3,0xC3,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFC,0xF8,0xF0,0xF0,0xF8,0xF8,0xE0,0xE0,0xE0,0xE0,0xF8,0xF8,
0xF0,0xF0,0xF8,0xFC,0xFF,0xFF,0xFF,0xFF
};

const u8 adc_pic[] = {
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0xFF,0x7F,0x03,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xBF,0xBF,0xDF,0x0F,0xBF,0x3F,0x07,
0xE0,0x03,0xFC,0x00,0x3F,0x07,0x3F,0x87,0x1F,0x7F,0x7F,0x7F,0xBF,0xBF,0xBF,0xBF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xF0,0xFF,0xFF,0xFF,0xFF,0xC0,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFE,0xF8,0xFF,0xFF,0xFF,0xFF
};

const u8 pic1[] = {
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
};



//---- Setting ----

void change_style();

CUI_Widget ui_widget = {WIDGET_STR,"ContextUI",9,NULL};
CUI_Widget author_widget = {WIDGET_STR,"TensonC",7,NULL};
CUI_Widget version_widget = {WIDGET_STR,"v1.0",4,NULL};

const char* s[] = {"Dot","Line","Rect"};
CUI_Widget select_style_widget = {WIDGET_STR,"Rect",4,change_style};
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

CUI_List about_list = {
    "about_list",
    {&ui,&version,&author}
};

CUI_Tab about = {"Information",NULL,&about_list};
CUI_Tab select = {"Select",&select_style_widget,NULL};

void bright_set();
CUI_Widget bright_wid = {WIDGET_PERCENTBAR,"bright",50,bright_set};
void bright_set()
{
    u8 b = bright_wid.respond_value * 255 / 100;
    OLED_WriteCmd(0x81);
    OLED_WriteCmd(b);
    
}
CUI_Tab bright = {"Bright",&bright_wid,NULL};

CUI_List setting_list = {
    "setting_list",
    {&about,&select,&bright}
};


//---- Show ----

void change_check_box();
void change_dot();

CUI_Tab t2_3_1 = {"2.3.1",NULL,NULL};
CUI_Tab t2_3_2 = {"2.3.2",NULL,NULL};
CUI_List t2_3_list = {"t2_3_list",{&t2_3_1,&t2_3_2}};

CUI_Tab t2_1 = {"2.1",NULL,NULL};
CUI_Tab t2_2 = {"2.2",NULL,NULL};
CUI_Tab t2_3 = {"2.3",NULL,&t2_3_list};
CUI_List t2_list = {"t2_list",{&t2_1,&t2_2,&t2_3}};

CUI_Tab t1 = {"1",NULL,NULL};
CUI_Tab t2 = {"2",NULL,&t2_list};
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

CUI_Widget str_wid = {WIDGET_STR,"ABC",3,NULL};
CUI_Tab str = {"Str",&str_wid,NULL};

CUI_Widget num_wid = {WIDGET_NUMBER,"number",255,NULL};
CUI_Tab num = {"Number",&num_wid,NULL};

CUI_Widget per_wid = {WIDGET_PERCENTBAR,"value",20,NULL};
CUI_Tab per = {"Percent",&per_wid,NULL};

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
CUI_Page new_page = {fun,{enter,NULL,left,right}};
void open_new_page()
{
    CUI_EnterPage(&new_page);   
}
CUI_Widget page_wid = {WIDGET_PAGE,"page",0,open_new_page};
CUI_Tab page = {"Open Page",&page_wid,NULL};


CUI_List show_list = {
    "show_list",
    {&mult,&check_box,&dot,&str,&num,&per,&page}
};

//---- ADC ----

static u8 run;
static u8 wave_data[128];
static u8 scale = 20;
u8 get_new() {
    if(gpio_get(10))
        return 0;
    else
        return 1;
}
void push_exit() {
    run = 0;
}
void scale_up() {
    scale +=2;
}
void scale_down() {
    scale -=2;
}
void wave_update()
{
    u8 i;
    for(i = 0;i < 127;i++)
            wave_data[i] = wave_data[i + 1];
        wave_data[127] = scale * get_new();
        CUI_Clear();
    for(i = 0;i < 127;i++)
        CUI_PutBiasLine(i,SCREEN_H / 2 + 10 - wave_data[i],
            i + 1,SCREEN_H / 2 + 10 - wave_data[i + 1]);
    
    CUI_Refresh();
    sleep_ms(20);
}
bool wave_timer_callback(struct repeating_timer* t)
{
    if (!run) return false; // 自动停止
    wave_update();
    return true;
}
void show_wave()
{
    run = 1;
    static struct repeating_timer wave_timer;
    //每20ms更新一次
    add_repeating_timer_ms(20, wave_timer_callback, NULL, &wave_timer);
}

CUI_Page wave_page = {
    &show_wave,{NULL,push_exit,scale_up,scale_down}
};

void into_adc() {
    CUI_EnterPage(&wave_page);
}