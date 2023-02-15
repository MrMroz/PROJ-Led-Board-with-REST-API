/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/util/queue.h"
#include "pico/multicore.h"

queue_t queue;

uint32_t led_pins_bitmask = 511U;

uint32_t next_frame(uint32_t gpio_leds_state, bool load) {
	uint32_t gls = gpio_leds_state;
	gls = gls << 1;
	gls = gls & led_pins_bitmask;
	
	uint32_t load_i32 = load ? 1U : 0U;
	
	return gls + load_i32;
	
}

int core1_led_controller() {
	uint32_t gpio_leds_state = 0U;
	
	// LEDy powinny być podpięte na pinach GPIO 0-8
	for(int i=0; i<9; i++) {
		gpio_init(i);
		gpio_set_dir(i, GPIO_OUT);
	}
	gpio_put_all(gpio_leds_state);
	while(1) {
		sleep_ms(100);
		bool q_input;
		bool received;
		received = queue_try_remove(&queue, &q_input);
		
		if(received) {
			gpio_leds_state = next_frame(gpio_leds_state, q_input);
			gpio_put_all(gpio_leds_state);
		}
		else {
			gpio_leds_state = next_frame(gpio_leds_state, false);
			gpio_put_all(gpio_leds_state);
		}
	}
}

int main() {
    stdio_init_all();
    if (cyw43_arch_init()) {
        printf("WiFi init failed");
        return -1;
    }

    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    
    gpio_init(9);
    gpio_set_dir(9, GPIO_IN);
    
    queue_init(&queue, sizeof(bool), 1);
    
    multicore_launch_core1(core1_led_controller);
    
    bool gpio_9_state;
    
    while(1) {
    	gpio_9_state = gpio_get(9);
    	if(gpio_9_state) {
    		queue_try_add(&queue, &gpio_9_state);
    		gpio_9_state = false;
    		while(gpio_get(9)) {
			sleep_ms(1);    		
		}
    	}
    }


}
