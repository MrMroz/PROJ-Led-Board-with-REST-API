#include "pico/cyw43_arch.h"
#include "picow_http/http.h"
#include "handlers.h"


#include <stdio.h>
#include <stdint.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/multicore.h"
#include "pico/sync.h"
#include "pico/util/queue.h"

#define MAX_INT_LEN (STRLEN_LTRL("−2147483648"))

#define ON_LEN (STRLEN_LTRL("on"))
#define OFF_LEN (STRLEN_LTRL("off"))


err_t circle_handler(struct http *http, void *q) {
	struct req *req = http_req(http);
	struct resp *resp = http_resp(http);
	err_t err;
	bool success;
	bool payload;
	
	enum http_method_t method = http_req_method(req);
	
	switch (method) {
		case HTTP_METHOD_PUT:
			payload = true;
			success = queue_try_add(q, &payload);
			break;
		
		case HTTP_METHOD_DELETE:
			payload = false;
			success = queue_try_add(q, &payload);
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
	
	if(success) {
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
}
