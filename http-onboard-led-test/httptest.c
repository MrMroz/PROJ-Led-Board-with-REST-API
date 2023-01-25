#include "picow_http/http.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

int main(void) {
	struct server *srv;
	struct server_cfg cfg;
	
	stdio_init_all();
    	if (cyw43_arch_init()) {
        	printf("WiFi init failed");
        	return -1;
    	}
    	
    	
	char ssid[] = "Ledziaki";
	char pass[] = "12345678";
	
	if (cyw43_arch_init()) {
		printf("WIFI init failed, exiting\n");
		return -1;
	}
	
	print("WIFI initialised\n");
	
	cyw43_arch_enable_sta_mode();
	
	if (cyw43_arch_wifi_connect_timeout_ms(ssid, pass, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
		printf("Failed to connect\n");
		return -1;
	}
	
	print("WIFI connected")

	/* Get a default configuration for the http server */
	cfg = http_default_cfg();

	/* Start the server */
	if (http_srv_init(&srv, &cfg) != ERR_OK) {
		// Error handling
	}
}
