 /**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

bool pin_9_state = false;
bool pin_10_state = false;
uint32_t led_pins_bitmask = 511U;
uint32_t gpio_leds_state = 0U;

void welcome_blink() {
	for(int i=0; i<3; i++) {
		gpio_put_all(led_pins_bitmask);
		sleep_ms(1000);
		gpio_put_all(0U);
		sleep_ms(1000);
	}
}


uint32_t next_frame(uint32_t gls, bool reverse) {
	switch (gls) {
		case 0U:
			if(reverse) {
				return 1U;
			}
			return 256U;
		case 384U:
			if(reverse) {
				return 256U;
			}
			return 448U;
		case 1U:
			if(reverse) {
				return 3U;
			}
			return 0U;
		case 3U:
			if(reverse) {
				return 7U;
			return 1U;
			}
		case 256U:
			if(reverse) {
				return 1U;
			}
			return 384U;
	}

	if(reverse) {
		gls = gls << 1;
		gls = gls & led_pins_bitmask;
		return gls;
	}
	
	gls = gls >> 1;
	return gls;
}

int main() {
	stdio_init_all();
	if (cyw43_arch_init()) {
		printf("WiFi init failed");
		return -1;
	}

	for(int i=0; i<9; i++) {
		gpio_init(i);
		gpio_set_dir(i, GPIO_OUT);
	}

	gpio_init(9);
	gpio_set_dir(9, GPIO_IN);

	gpio_init(10);
	gpio_set_dir(10, GPIO_OUT);

	printf("Init successful\n");
	cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
	
	welcome_blink();

	bool reverse = false;
	bool fast_mode = false;
	while (true) {
		reverse = gpio_get(9);
		gpio_leds_state = next_frame(gpio_leds_state, reverse);
		gpio_put_all(gpio_leds_state);
		
		fast_mode = gpio_get(10);
		
		if(fast_mode) {
			sleep_ms(100);
		} else {
			sleep_ms(30);
		}
	}
	
	return 0;
}
