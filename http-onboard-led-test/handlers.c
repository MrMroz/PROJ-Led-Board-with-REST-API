#include "pico/cyw43_arch.h"
#include "picow_http/http.h"
#include "handlers.h"

#define MAX_INT_LEN (STRLEN_LTRL("−2147483648"))

#define ON_LEN (STRLEN_LTRL("on"))
#define OFF_LEN (STRLEN_LTRL("off"))


err_t led_handler(struct http *http, void *p) {
	struct req *req = http_req(http);
	struct resp *resp = http_resp(http);
	err_t err;
	
	enum http_method_t method = http_req_method(req);
	
	switch (method) {
		case HTTP_METHOD_GET:
			bool led_state = gpio_get(0);
	
			char * led_state_char = led_state ? "ON" : "OFF";
			size_t body_len = led_state ? 2 : 3;
			
			err = http_resp_set_status(resp, HTTP_STATUS_OK);
			if (err != ERR_OK) {
				HTTP_LOG_ERROR("Set status 200 failed: %d", err);
				return http_resp_err(http,
							 HTTP_STATUS_INTERNAL_SERVER_ERROR);
			}
			
			if ((err = http_resp_set_hdr_ltrl(resp, "Cache-Control", "no-store"))
				!= ERR_OK) {
				HTTP_LOG_ERROR("Set header Cache-Control failed: %d", err);
				return http_resp_err(http, HTTP_STATUS_INTERNAL_SERVER_ERROR);
			}
			
				if ((err = http_resp_set_type_ltrl(resp, "text/plain")) != ERR_OK) {
				HTTP_LOG_ERROR("http_resp_set_type_ltrl() failed: %d", err);
				return http_resp_err(http, HTTP_STATUS_INTERNAL_SERVER_ERROR);
			}
			
			if ((err = http_resp_set_len(resp, body_len)) != ERR_OK) {
				HTTP_LOG_ERROR("http_resp_set_len() failed: %d", err);
				return http_resp_err(http, HTTP_STATUS_INTERNAL_SERVER_ERROR);
			}
			
			return http_resp_send_buf(http, led_state_char, body_len, false);
			
			break;
		
		
		case HTTP_METHOD_PUT:
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
					default:
						err = http_resp_set_status(resp, HTTP_STATUS_BAD_REQUEST);
						if (err != ERR_OK) {
							HTTP_LOG_ERROR("Set status 400 failed: %d", err);
							return http_resp_err(http,
										 HTTP_STATUS_INTERNAL_SERVER_ERROR);
						}
						
							if ((err = http_resp_set_hdr_ltrl(resp, "Cache-Control", "no-store"))
							!= ERR_OK) {
							HTTP_LOG_ERROR("Set header Cache-Control failed: %d", err);
							return http_resp_err(http, HTTP_STATUS_INTERNAL_SERVER_ERROR);
						}
						
							if ((err = http_resp_set_type_ltrl(resp, "text/plain")) != ERR_OK) {
							HTTP_LOG_ERROR("http_resp_set_type_ltrl() failed: %d", err);
							return http_resp_err(http, HTTP_STATUS_INTERNAL_SERVER_ERROR);
						}
						
						if ((err = http_resp_set_len(resp, 3)) != ERR_OK) {
							HTTP_LOG_ERROR("http_resp_set_len() failed: %d", err);
							return http_resp_err(http, HTTP_STATUS_INTERNAL_SERVER_ERROR);
						}
						
						return http_resp_send_buf(http, "400", 3, false);
			}

			err = http_resp_set_status(resp, HTTP_STATUS_OK);
			if (err != ERR_OK) {
				HTTP_LOG_ERROR("Set status 200 failed: %d", err);
				return http_resp_err(http,
							 HTTP_STATUS_INTERNAL_SERVER_ERROR);
			}
			
				if ((err = http_resp_set_hdr_ltrl(resp, "Cache-Control", "no-store"))
				!= ERR_OK) {
				HTTP_LOG_ERROR("Set header Cache-Control failed: %d", err);
				return http_resp_err(http, HTTP_STATUS_INTERNAL_SERVER_ERROR);
			}
			
				if ((err = http_resp_set_type_ltrl(resp, "text/plain")) != ERR_OK) {
				HTTP_LOG_ERROR("http_resp_set_type_ltrl() failed: %d", err);
				return http_resp_err(http, HTTP_STATUS_INTERNAL_SERVER_ERROR);
			}
			
			if ((err = http_resp_set_len(resp, 3)) != ERR_OK) {
				HTTP_LOG_ERROR("http_resp_set_len() failed: %d", err);
				return http_resp_err(http, HTTP_STATUS_INTERNAL_SERVER_ERROR);
			}
			
			//body_len = snprintf(body, MAX_INT_LEN, "TEST");

			return http_resp_send_buf(http, "200", 3, false);
		}

		err = http_resp_set_status(resp, HTTP_STATUS_BAD_REQUEST);
		if (err != ERR_OK) {
			HTTP_LOG_ERROR("Set status 400 failed: %d", err);
			return http_resp_err(http,
						 HTTP_STATUS_INTERNAL_SERVER_ERROR);
		}
		
			if ((err = http_resp_set_hdr_ltrl(resp, "Cache-Control", "no-store"))
			!= ERR_OK) {
			HTTP_LOG_ERROR("Set header Cache-Control failed: %d", err);
			return http_resp_err(http, HTTP_STATUS_INTERNAL_SERVER_ERROR);
		}
		
			if ((err = http_resp_set_type_ltrl(resp, "text/plain")) != ERR_OK) {
			HTTP_LOG_ERROR("http_resp_set_type_ltrl() failed: %d", err);
			return http_resp_err(http, HTTP_STATUS_INTERNAL_SERVER_ERROR);
		}
		
		if ((err = http_resp_set_len(resp, 3)) != ERR_OK) {
			HTTP_LOG_ERROR("http_resp_set_len() failed: %d", err);
			return http_resp_err(http, HTTP_STATUS_INTERNAL_SERVER_ERROR);
		}
		
		return http_resp_send_buf(http, "400", 3, false);
		break;
		
		default:
			printf("Unrecognised http method!\n");
			
			err = http_resp_set_status(resp, HTTP_STATUS_METHOD_NOT_ALLOWED);
			if (err != ERR_OK) {
				HTTP_LOG_ERROR("Set status 405 failed: %d", err);
				return http_resp_err(http,
							 HTTP_STATUS_INTERNAL_SERVER_ERROR);
			}
			
			if ((err = http_resp_set_hdr_ltrl(resp, "Cache-Control", "no-store"))
				!= ERR_OK) {
				HTTP_LOG_ERROR("Set header Cache-Control failed: %d", err);
				return http_resp_err(http, HTTP_STATUS_INTERNAL_SERVER_ERROR);
			}
			
			if ((err = http_resp_set_type_ltrl(resp, "text/plain")) != ERR_OK) {
				HTTP_LOG_ERROR("http_resp_set_type_ltrl() failed: %d", err);
				return http_resp_err(http, HTTP_STATUS_INTERNAL_SERVER_ERROR);
			}
			
			if ((err = http_resp_set_len(resp, 3)) != ERR_OK) {
				HTTP_LOG_ERROR("http_resp_set_len() failed: %d", err);
				return http_resp_err(http, HTTP_STATUS_INTERNAL_SERVER_ERROR);
			}
			
			return http_resp_send_buf(http, "405", 3, false);
	}
}
