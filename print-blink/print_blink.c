#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

int main() {
    stdio_init_all();
    printf("WiFi init...");
    if (cyw43_arch_init()) {
        printf("WiFi init failed");
        return -1;
    }
    printf("WiFi init successful");
    while (true) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(250);
        printf("Led on\n");
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(250);
        printf("Led off\n");
    }
}
