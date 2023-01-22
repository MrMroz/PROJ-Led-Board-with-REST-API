#include <httpserver.hpp>
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

using namespace httpserver;

std::shared_ptr<http_response> not_found_custom(const http_request& req) {
  return std::shared_ptr<string_response>(new string_response("Not found custom", 404, "text/plain"));
}

std::shared_ptr<http_response> not_allowed_custom(const http_request& req) {
  return std::shared_ptr<string_response>(new string_response("Not allowed custom", 405, "text/plain"));
}

class hello_world_resource : public http_resource {
public:
  std::shared_ptr<http_response> render(const http_request&) {
      return std::shared_ptr<http_response>(new string_response("Hello, World!"));
  }
};

int main(int argc, char** argv) {

	char ssid[] = "Abgassonderuntersuchung;
	char pass[] = "pudzian123";
	stdio_init_all();

	if (cyw43_arch_init_with_country(CYW43_COUNTRY_UK)) {
		printf("failed to initialise\n");
		return 1;
	}
	printf("initialised\n");

	cyw43_arch_enable_sta_mode();

	if (cyw43_arch_wifi_connect_timeout_ms(ssid, pass, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
		printf("failed to connect\n");
		return 1;
	}
	printf("connected\n");

	webserver ws = create_webserver(8080)
	      .not_found_resource(not_found_custom)
	      .method_not_allowed_resource(not_allowed_custom);

	hello_world_resource hwr;
	hwr.disallow_all();
	hwr.set_allowing("GET", true);
	ws.register_resource("/hello", &hwr);
	ws.start(true);

	return 0;
}
