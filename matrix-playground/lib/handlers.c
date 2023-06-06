#include "handlers.h"
#include "controller.h"

#define LED_REQUEST_JSON_LEN (STRLEN_LTRL("{type:\"\", text:\"\", speed:\"\"}"))

struct server *srv;
struct server_cfg cfg;
err_t err;

void wifi_init() {
	printf("--- WiFi init ---\n");
    
	if (cyw43_arch_init()) {
		printf("WiFi init failed!\n");
		return;
	}
    printf("WiFi module initialized\n");
	
	printf("Connecting to WiFi network\n");

	cyw43_arch_enable_sta_mode();
	if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
		printf("Failed to connect\n");
		return;
	}
	
	printf("--- WiFi init successful ---\n");
}

void rest_api_init() {
	printf("--- HTTP Server init ---\n");
	cfg = http_default_cfg();

	printf("Registering PUT /api/text...\n");
	if ((err = register_hndlr(&cfg, "/api/text", text_put_handler,
			HTTP_METHOD_PUT, &queue)) != ERR_OK) {
		HTTP_LOG_ERROR("Register /api/text: %d", err);
		return;
	}

	printf("Registering PUT /api/spectrum...\n");
	if ((err = register_hndlr(&cfg, "/api/spectrum", spectrum_put_handler,
			HTTP_METHOD_PUT, &queue)) != ERR_OK) {
		HTTP_LOG_ERROR("Register /api/spectrum: %d", err);
		return;
	}
	printf("Registering GET /api/next...\n");
	if ((err = register_hndlr(&cfg, "/api/next", next_get_handler,
			HTTP_METHOD_GET, &queue)) != ERR_OK) {
		HTTP_LOG_ERROR("Register /api/next: %d", err);
		return;
	}
	
	printf("Starting the server...\n");
	/* Start the server */
	if (http_srv_init(&srv, &cfg) != ERR_OK) {
		// Error handling
		printf("Starting server failed!\n");
	}
	
	// for (;;) {
	// 	#if PICO_CYW43_ARCH_POLL
	// 		cyw43_arch_poll();
	// 		sleep_ms(1);
	// 	#else
	// 		__wfi();
	// 	#endif
	// }

	printf("--- HTTP Server init successful ---\n");
}

err_t text_put_handler(struct http *http, void *q) {
	struct req *req = http_req(http);
	struct resp *resp = http_resp(http);
	err_t err;
	size_t query_len;
	char *query;

	char param_data[MAX_TEXT_LEN];
	size_t param_len;

	struct led_request payload;
	payload.type = 1;
	query = http_req_query(req, &query_len);
	http_req_query_val(query, query_len, "t", 1, &param_len);
	strncpy(param_data, http_req_query_val(query, query_len, "t", 1, &param_len), param_len);
	//param_data = http_req_query_val(query, query_len, "t", 1, &param_len);
	param_data[(int) param_len] = '\0';
	strncpy(payload.text, param_data, param_len);
	payload.text[(int) param_len] = '\0';

	http_req_query_val(query, query_len, "speed", 5, &param_len);
	strncpy(param_data, http_req_query_val(query, query_len, "speed", 5, &param_len), param_len);
	//param_data = http_req_query_val(query, query_len, "speed", 5, &param_len);

	err = sscanf(param_data, "%u", &payload.speed);
	if(err < 0) {
		printf("Error parsing speed param data");
		
		err = http_resp_set_status(resp, HTTP_STATUS_UNPROCESSABLE_CONTENT);
		if (err != ERR_OK) {
			HTTP_LOG_ERROR("Set status 422 failed: %d", err);
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
		
		return http_resp_send_buf(http, "422", 3, false);
	}

	queue_add_blocking(q, &payload);

	err = http_resp_set_status(resp, HTTP_STATUS_CREATED);
	if (err != ERR_OK) {
		HTTP_LOG_ERROR("Set status 201 failed: %d", err);
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
	
	return http_resp_send_buf(http, "201", 3, false);
}

err_t spectrum_put_handler(struct http *http, void *q) {
	struct req *req = http_req(http);
	struct resp *resp = http_resp(http);
	err_t err;
	size_t query_len;
	char *query;

	char param_data[MAX_TEXT_LEN];
	size_t param_len;

	struct led_request payload;
	payload.type = 2;
	
	query = http_req_query(req, &query_len);

	strcpy(param_data, http_req_query_val(query, query_len, "speed", 5, &param_len));
	//param_data = http_req_query_val(query, query_len, "speed", 5, &param_len);

	err = sscanf(param_data, "%u", &payload.speed);
	if(err < 0) {
		printf("Error parsing speed param data");
		
		err = http_resp_set_status(resp, HTTP_STATUS_UNPROCESSABLE_CONTENT);
		if (err != ERR_OK) {
			HTTP_LOG_ERROR("Set status 422 failed: %d", err);
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
		
		return http_resp_send_buf(http, "422", 3, false);
	}

	queue_add_blocking(q, &payload);

	err = http_resp_set_status(resp, HTTP_STATUS_CREATED);
	if (err != ERR_OK) {
		HTTP_LOG_ERROR("Set status 201 failed: %d", err);
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
	
	return http_resp_send_buf(http, "201", 3, false);
}

err_t next_get_handler(struct http *http, void *q) {
	struct req *req = http_req(http);
	struct resp *resp = http_resp(http);
	err_t err;
	struct led_request payload;

	if(queue_is_empty(q)) {
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
		
		return http_resp_send_buf(http, "NULL", 4, false);
	} else {
		queue_peek_blocking(q, &payload);
		char* output[MAX_TEXT_LEN + LED_REQUEST_JSON_LEN];

		snprintf(output, MAX_TEXT_LEN + LED_REQUEST_JSON_LEN, "{type:%d, text:\"%s\", speed:\"%d\"}", payload.type, payload.text, payload.speed);

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
		return http_resp_send_buf(http, output, MAX_TEXT_LEN + LED_REQUEST_JSON_LEN, false);
	}
}