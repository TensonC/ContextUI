#ifndef _OLED_H_
#define _OLED_H_

#include "pico/stdlib.h"

void OLED_WriteCmd(uint8_t cmd);
void OLED_WriteData(uint8_t dat);


#endif