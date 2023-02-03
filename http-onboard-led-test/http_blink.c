#include "picow_http/http.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "handlers.h"

static struct netinfo netinfo;

int main(void) {
	struct server *srv;
	struct server_cfg cfg;
	err_t err;
	
	stdio_init_all();
    if (cyw43_arch_init()) {
    	printf("WiFi init failed");
    	return -1;
	}
	
    printf("WIFI initialized\n");
    	
	char ssid[] = "Ledziaki";
	char pass[] = "12345678";
	
	cyw43_arch_enable_sta_mode();
	
	if (cyw43_arch_wifi_connect_timeout_ms(ssid, pass, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
		printf("Failed to connect\n");
		return -1;
	}
	
	printf("WIFI connected");
	
	gpio_init(0);
	gpio_set_dir(0, GPIO_OUT);
	gpio_put(0, false);

	/* Get a default configuration for the http server */
	cfg = http_default_cfg();
	cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
	
	if ((err = register_hndlr_methods(&cfg, "/led", get_led_handler,
					  HTTP_METHOD_GET, NULL))
	    != ERR_OK) {
		HTTP_LOG_ERROR("Register /led: %d", err);
		return -1;
		}
		
	if ((err = register_hndlr_methods(&cfg, "/led", put_led_handler,
					  HTTP_METHOD_PUT, NULL))
	    != ERR_OK) {
		HTTP_LOG_ERROR("Register /led: %d", err);
		return -1;
		}


	/* Start the server */
	if (http_srv_init(&srv, &cfg) != ERR_OK) {
		// Error handling
		printf("Starting server failed!");
	}
	
	return 0;
}
