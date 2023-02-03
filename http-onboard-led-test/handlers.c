#include "pico/cyw43_arch.h"
#include "picow_http/http.h"
#include "handlers.h"

#define MAX_INT_LEN (STRLEN_LTRL("−2147483648"))

#define ON_LEN (STRLEN_LTRL("on"))
#define OFF_LEN (STRLEN_LTRL("off"))


err_t get_led_handler(struct http *http, void *p) {
	struct req *req = http_req(http);
	struct resp *resp = http_resp(http);
	err_t err;
	
	bool led_state = gpio_get(0);
	
	char * led_state_char = led_state ? "ON" : "OFF";
	
	
	return http_resp_send_buf(http, led_state_char, sizeof(led_state), false);
}

err_t put_led_handler(struct http *http, void *P) {
	struct req *req = http_req(http);
	struct resp *resp = http_resp(http);
	err_t err;
	char body[MAX_INT_LEN];
	size_t body_len = MAX_INT_LEN;
	size_t query_len;
	const char *query;
	
	query = http_req_query(req, &query_len);
	
	if (query != NULL) {
		const char *val;
		size_t val_len;

		val = http_req_query_val(query, query_len, "state", 5, &val_len);
		
		switch (val_len) {
			case ON_LEN:
				if (memcmp(val, "on", ON_LEN) != 0)
					return http_resp_err(http,
							 HTTP_STATUS_UNPROCESSABLE_CONTENT);
				gpio_put(0, true);
				break;
			case OFF_LEN:
				if (memcmp(val, "off", OFF_LEN) != 0)
					return http_resp_err(http,
					     HTTP_STATUS_UNPROCESSABLE_CONTENT);
				gpio_put(0, false);
				break;
			}
	}

	err = http_resp_set_status(resp, HTTP_STATUS_OK);
	if (err != ERR_OK) {
		HTTP_LOG_ERROR("Set status 200 failed: %d", err);
		return http_resp_err(http,
				     HTTP_STATUS_INTERNAL_SERVER_ERROR);
	}
	
	body_len = snprintf(body, MAX_INT_LEN, "TEST");

	return http_resp_send_buf(http, body, body_len, false);
}
