
#include "hardware/spi.h"
#include "ContextUI_draw.h"
#include "oled.h"
#include "ContextUI.h"

#define SPI_PORT spi0
#define PIN_CS   17
#define PIN_SCK  18
#define PIN_MOSI 19
#define PIN_RES  16
#define PIN_DC   20

void SPI_Init()
{
    spi_init(SPI_PORT, 1000*1000);
    spi_set_format(SPI_PORT, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
    gpio_set_function(PIN_CS,   GPIO_FUNC_SIO);
    gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    gpio_init(PIN_RES);gpio_set_dir(PIN_RES, GPIO_OUT);
    gpio_init(PIN_CS);gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_init(PIN_DC);gpio_set_dir(PIN_DC, GPIO_OUT);
}



void OLED_WriteCmd(uint8_t cmd)
{
    gpio_put(PIN_DC,0);
    gpio_put(PIN_CS,0);
    spi_write_blocking(SPI_PORT, &cmd, 1);
    gpio_put(PIN_CS,1);
} 

void OLED_WriteData(uint8_t dat)
{
    gpio_put(PIN_DC,1);
    gpio_put(PIN_CS,0);
    spi_write_blocking(SPI_PORT, &dat, 1);
    gpio_put(PIN_CS,1);
}

void OLED_Init()
{
    gpio_put(PIN_RES,0);
    sleep_ms(20);
    gpio_put(PIN_RES,1);

	OLED_WriteCmd(0xae);
	OLED_WriteCmd(0x20);
	OLED_WriteCmd(0x10);
	OLED_WriteCmd(0xb0);
	OLED_WriteCmd(0xc8);
	OLED_WriteCmd(0x00);
	OLED_WriteCmd(0x10);
	OLED_WriteCmd(0x40);
	OLED_WriteCmd(0x81);
	OLED_WriteCmd(0x7f);
	OLED_WriteCmd(0xa1);
	OLED_WriteCmd(0xa6);
	OLED_WriteCmd(0xa8);
	OLED_WriteCmd(0x3F);
	OLED_WriteCmd(0xa4);
	OLED_WriteCmd(0xd3);
	OLED_WriteCmd(0x00);
	OLED_WriteCmd(0xd5);
	OLED_WriteCmd(0xf0);
	OLED_WriteCmd(0xd9);
	OLED_WriteCmd(0x22);
	OLED_WriteCmd(0xda);
	OLED_WriteCmd(0x12);
	OLED_WriteCmd(0xdb);
	OLED_WriteCmd(0x20);
	OLED_WriteCmd(0x8d);
	OLED_WriteCmd(0x14);
	OLED_WriteCmd(0xaf);
}

extern CUI_List list2;


CUI_Widget test1_widget = {
    WIDGET_CHECKBOX,1,(void*)0
};
CUI_Tab test1 = {
    "ABCD1",
    &test1_widget,
    &list2,
    (void*)0
};

CUI_Widget test2_widget = {
WIDGET_DOT,1,(void*)0
};
CUI_Tab test2 = {
    "EFG",
    &test2_widget,
    (void*)0,
    (void*)0
};

CUI_Widget test3_widget = {
WIDGET_CHECKBOX,0,(void*)0
};
void check_test3()
{
    test3_widget.respond_value = !test3_widget.respond_value; 
}

CUI_Tab test3 = {
    "GHI",
    &test3_widget,
    (void*)0,
    check_test3,
};

CUI_Widget test4_widget = {
WIDGET_DOT,1,(void*)0
};
CUI_Tab test4 = {
    "JKL",
    &test4_widget,
    (void*)0,
    (void*)0
};


CUI_List list1 = {
    "list1",
    (void*)0,
    {&test1,&test2}
};

CUI_List list2 = {
    "list2",
    &list1,
    {&test3,&test4}
};

const u8 pic1[] = {
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff

};


CUI_App MainMenu[APP_NUM] = {
    {"app1",pic1,&list1,0},
    {"app2",pic1,0,0},
    {"app3",pic1,0,0},
    {"app4",pic1,0,0},
    {"app5",pic1,0,0}
};

int main()
{
    SPI_Init();
    OLED_Init();
    
    CUI_Clear();
    sleep_ms(200);
    CUI_Fill();
    CUI_Refresh();
    sleep_ms(200);
    CUI_Clear();

    CUI_Init();
    CUI_Refresh();
    sleep_ms(1000);
    CUI_Enter();
    CUI_DrawSelect(5,1,SELECT_RECT);
    CUI_Refresh();

   
    while (1) {
        
    }
}


/*

*/