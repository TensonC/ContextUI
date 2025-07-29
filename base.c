#include "base.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"

#define SPI_PORT spi0
#define PIN_CS   17
#define PIN_SCK  18
#define PIN_MOSI 19
#define PIN_RES  16
#define PIN_DC   20

/**
 * @brief SPI通信的初始化
 */
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

/**
 * @brief 向OLED屏幕发送命令
 * @param cmd 命令
 */
void OLED_WriteCmd(u8 cmd)
{
    gpio_put(PIN_DC,0);
    gpio_put(PIN_CS,0);
    spi_write_blocking(SPI_PORT, &cmd, 1);
    gpio_put(PIN_CS,1);
} 

/**
 * @brief 向OLED屏幕发送数据
 * @param dat 数据
 */
void OLED_WriteData(u8 dat)
{
    gpio_put(PIN_DC,1);
    gpio_put(PIN_CS,0);
    spi_write_blocking(SPI_PORT, &dat, 1);
    gpio_put(PIN_CS,1);
}

/**
 * @brief OLED屏幕的初始化
 */
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
