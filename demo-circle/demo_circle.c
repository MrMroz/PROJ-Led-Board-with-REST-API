#include "picow_http/http.h"
#include <stdio.h>
#include <stdint.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/multicore.h"
#include "pico/sync.h"
#include "pico/util/queue.h"


#include "handlers.h"

static struct netinfo netinfo;
queue_t queue;
uint32_t led_pins_bitmask = 4095U;
uint32_t last_pin_bitmask = 2048U;

#define HTTP_METHODS_PUT_DELETE (1U << HTTP_METHOD_PUT | (1U << HTTP_METHOD_DELETE))

void gpio_put_safe(uint32_t gls) {
	uint32_t trollmask = 1U;
	for(int i=0; i < 12; i++) {
		gpio_put((uint32_t) i ,trollmask & (gls >> i)); 
	}
}


int next_frame(uint32_t prev_frame, bool payload, bool did_receive) {
	uint32_t gls = prev_frame;
	if(did_receive) {
		if(payload) {
			gls = (gls << 1) | 1U;
		}
		else {
			gls = gls << 1;
		}
	}
	else {
		gls = (gls << 1) | ((last_pin_bitmask & gls) >> 11);
	}
	gls = gls & led_pins_bitmask;
	gpio_put_safe(gls);
	return gls;
}

void core1_led_controller() {
	uint32_t gls = 0U;
	bool did_receive = false;
	bool payload;
	bool able_to_change = false;
	
	while(1){
		sleep_ms(200);
		if(!queue_is_empty(&queue)) {
			did_receive = queue_try_peek(&queue, &payload);
			
			if(!did_receive) {
				gls = next_frame(gls, false, false);
				continue;			
			}
			
			able_to_change = ((bool)((last_pin_bitmask & gls) >> 11)) ^ payload;
			
			gls = next_frame(gls, payload, true);
			
			if(able_to_change){
				queue_remove_blocking(&queue, &payload);
			}
		}
		else {
			gls = next_frame(gls, false, false);
		}
	}
}


int main() {
	struct server *srv;
	struct server_cfg cfg;
	err_t err;
	
	stdio_init_all();
	
	sleep_ms(5000);
	
	printf("WiFi init in progress...\n");
    if (cyw43_arch_init()) {
    	printf("WiFi init failed!\n");
    	return -1;
	}
	
    printf("WIFI initialized\n");
    	
	char ssid[] = "Ledziaki";
	char pass[] = "12345678";
	
	printf("Connecting to WiFi\n");
	
	cyw43_arch_enable_sta_mode();
	
	if (cyw43_arch_wifi_connect_timeout_ms(ssid, pass, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
		printf("Failed to connect\n");
		return -1;
	}
	
	printf("WIFI connected\n");
	printf("GPIO init in progress...\n");
	
	for(int i=0; i < 12; i++){
		gpio_init(i);
		gpio_set_dir(i, GPIO_OUT);
	}
	
	printf("GPIO init successful\n");
	
	printf("Multicore queue init\n");
	
	queue_init(&queue, sizeof(bool), 10);

	/* Get a default configuration for the http server */
	cfg = http_default_cfg();
	cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
	
	printf("Registering PUT|DELETE/circle...\n");
	
	if ((err = register_hndlr_methods(&cfg, "/circle", circle_handler,
					  HTTP_METHODS_PUT_DELETE, &queue))
	    != ERR_OK) {
		HTTP_LOG_ERROR("Register /circle: %d", err);
		return -1;
		}
		
	printf("Launching led controller on core1\n");
	sleep_ms(5);
	multicore_reset_core1();
	sleep_ms(5);
	multicore_launch_core1(core1_led_controller);
	
	printf("Starting the server...\n");
	/* Start the server */
	if (http_srv_init(&srv, &cfg) != ERR_OK) {
		// Error handling
		printf("Starting server failed!\n");
	}
	
	for (;;) {
		#if PICO_CYW43_ARCH_POLL
			cyw43_arch_poll();
			sleep_ms(1);
		#else
			__wfi();
		#endif
	}

	
	printf("This should be unreachable!\n");
	return 0;
}
