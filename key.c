#include "key.h"
#include "hardware/timer.h"
#include "pico/stdlib.h"
#include "ContextUI.h"

#define PIN_ENTER 10
#define PIN_EXIT 11
#define PIN_RIGHT 12
#define PIN_LEFT 13 

static uint32_t last_time[32] = {0}; // 每个GPIO独立防抖

void gpio_callback(uint gpio, uint32_t events)
{
    uint32_t now = to_ms_since_boot(get_absolute_time());
    if (now - last_time[gpio] < 50) return;  // 忽略 50ms 内的重复中断
    last_time[gpio] = now;

    if (events & GPIO_IRQ_EDGE_FALL)
    {
        switch (gpio)
        {
            case PIN_ENTER: CUI_Enter(); break;
            case PIN_EXIT: CUI_Exit(); break;
            case PIN_LEFT: CUI_Leftward(); break;
            case PIN_RIGHT: CUI_Rightward(); break;
        }
    }
}

void key_Init()
{
    const uint pins[] = {PIN_ENTER, PIN_EXIT, PIN_LEFT, PIN_RIGHT};
    for (int i = 0; i < 4; i++)
    {
        gpio_init(pins[i]);
        gpio_set_dir(pins[i], GPIO_IN);
        gpio_pull_up(pins[i]);
        gpio_set_irq_enabled(pins[i], GPIO_IRQ_EDGE_FALL, true);
    }

    // 注册统一回调函数（必须只调用一次）
    gpio_set_irq_callback(gpio_callback);
    irq_set_enabled(IO_IRQ_BANK0, true);
}