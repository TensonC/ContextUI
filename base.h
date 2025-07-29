#ifndef _BASE_H_
#define _BASE_H_

#include "all.h"

void SPI_Init();
void OLED_Init();
void OLED_WriteCmd(u8 cmd);
void OLED_WriteData(u8 dat);

#endif