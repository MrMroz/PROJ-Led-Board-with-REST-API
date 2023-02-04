#include "picow_http/http.h"
#include <stdio.h>
#include <stdint.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/multicore.h"
#include "pico/sync.h"


#include "handlers.h"

static struct netinfo netinfo;

#define HTTP_METHODS_GET_PUT ((1U << HTTP_METHOD_GET) | (1U << HTTP_METHOD_PUT))

int main(void) {
	struct server *srv;
	struct server_cfg cfg;
	err_t err;
	
	stdio_init_all();
	
	gpio_init(1);
	gpio_set_dir(1, GPIO_IN);
	
	bool start_trigger = false;
	
	while(!start_trigger) {
		start_trigger = gpio_get(1);
		printf("Waiting for input...%s\n", start_trigger ? "true" : "false");
		sleep_ms(3000);
	}
	
	
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
	
	gpio_init(0);
	gpio_set_dir(0, GPIO_OUT);
	gpio_put(0, false);
	
	printf("GPIO init successful\n");

	/* Get a default configuration for the http server */
	cfg = http_default_cfg();
	cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
	
	printf("Registering GET|PUT /led/...\n");
	
	if ((err = register_hndlr_methods(&cfg, "/led", led_handler,
					  HTTP_METHODS_GET_PUT, NULL))
	    != ERR_OK) {
		HTTP_LOG_ERROR("Register /led: %d", err);
		return -1;
		}

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
