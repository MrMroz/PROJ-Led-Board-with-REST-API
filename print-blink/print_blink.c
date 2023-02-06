#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"


int main() {
    //stdio init do wypisywania rzeczy na konsolce
    stdio_init_all();

    // Trzeba zainicjalizować moduł WiFi aby mrugać wbudowanym LED'em
    printf("WiFi init...\n");
    if (cyw43_arch_init()) {
        printf("WiFi init failed\n");
        return -1;
    }
    printf("WiFi init successful\n");

    while (true) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(250);
        printf("Led on\n");
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(250);
        printf("Led off\n");
    }
}
