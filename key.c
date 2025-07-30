/*
该文件中为按键设置
*/

#include "key.h"
#include "hardware/timer.h"
#include "pico/stdlib.h"
#include "ContextUI.h"

#define PIN_ENTER 10
#define PIN_EXIT 11
#define PIN_RIGHT 12
#define PIN_LEFT 13 

#define DEBOUNCE_MS 50

static uint32_t last_state[30] = {1};
static uint32_t last_time[30]  = {0};

bool key_scan_callback(struct repeating_timer *t)
{
    const uint pins[] = {PIN_ENTER, PIN_EXIT, PIN_LEFT, PIN_RIGHT};
    for (int i = 0; i < 4; i++) {
        uint pin = pins[i];
        uint32_t now = to_ms_since_boot(get_absolute_time());
        uint32_t level = gpio_get(pin);

        if (last_state[pin] == 1 && level == 0) {
            if (now - last_time[pin] > DEBOUNCE_MS) {
                switch (pin) {
                    case PIN_ENTER:  CUI_Enter(); break;
                    case PIN_EXIT:   CUI_Exit(); break;
                    case PIN_LEFT:   CUI_Leftward(); break;
                    case PIN_RIGHT:  CUI_Rightward(); break;
                }
                last_time[pin] = now;
            }
        }

        last_state[pin] = level;
    }
    return true;
}

void key_Init()
{
    const uint pins[] = {PIN_ENTER, PIN_EXIT, PIN_LEFT, PIN_RIGHT};
    for (int i = 0; i < 4; i++)
    {
        gpio_init(pins[i]);
        gpio_set_dir(pins[i], GPIO_IN);
        gpio_pull_up(pins[i]);
    }

    static struct repeating_timer key_timer;
    // 每 20ms 扫描一次按键状态
    add_repeating_timer_ms(20, key_scan_callback, NULL, &key_timer);
}