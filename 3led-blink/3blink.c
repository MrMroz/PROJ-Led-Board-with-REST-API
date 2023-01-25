 /**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

int main() {

    stdio_init_all();
    if (cyw43_arch_init()) {
        printf("WiFi init failed");
        return -1;
    }
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
            
    gpio_init(0);
    gpio_set_dir(0, GPIO_OUT);
    
    gpio_init(10);
    gpio_set_dir(10, GPIO_OUT);
    
    gpio_init(20);
    gpio_set_dir(20, GPIO_OUT);
    
    while (true) {
        gpio_put(0, 1);
        sleep_ms(250);
        gpio_put(10, 1);
        sleep_ms(250);
        gpio_put(20, 1);
        sleep_ms(250);
        
        gpio_put(0, 0);
        sleep_ms(250);
        gpio_put(10, 0);
        sleep_ms(250);
        gpio_put(20, 0);
        sleep_ms(250);
    }
}
