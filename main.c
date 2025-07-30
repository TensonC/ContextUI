#include "ContextUI_draw.h"
#include "base.h"
#include "ContextUI.h"
#include "key.h"
#include "demo.c"


/*****
TODO: 添加动画
******/
CUI_App MainMenu[APP_NUM] = {
    {"Show",pic1,&show_list,0},
    {"ADC",adc_pic,0,into_adc},
    {"app3",pic1,0,0},
    {"app4",pic1,0,0},
    {"Setting",set_pic,&setting_list,0}
};


int main()
{
    SPI_Init();
    OLED_Init();
    key_Init();
    CUI_Init();

    CUI_Refresh();
   
    while (1) {
        
    }
}
