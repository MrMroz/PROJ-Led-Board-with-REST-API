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

#define HTTP_STATUS_TOO_MANY_REQUESTS 429

err_t blank_handler(struct http *http, void *q) {
	struct req *req = http_req(http);
	struct resp *resp = http_resp(http);
	err_t err;
	uint8_t payload = 0;

	bool success = queue_try_add(q, &payload);

	if(success) {
		err = http_resp_set_status(resp, HTTP_STATUS_OK);
		if (err != ERR_OK) {
			HTTP_LOG_ERROR("Set status 200 failed: %d", err);
			return http_resp_err(http,
						HTTP_STATUS_INTERNAL_SERVER_ERROR);
		}
		
		if ((err = http_resp_set_hdr_ltrl(resp, "Cache-Control", "no-store")) != ERR_OK) {
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

		return http_resp_send_buf(http, "Successfully displayed blank", 29, false);
	}

	err = http_resp_set_status(resp, HTTP_STATUS_TOO_MANY_REQUESTS);
	if (err != ERR_OK) {
		HTTP_LOG_ERROR("Set status 429 failed: %d", err);
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
	
	return http_resp_send_buf(http, "Too many requests are being made, try again later", 50, false);

}

err_t heart_handler(struct http *http, void *q) {
	struct req *req = http_req(http);
	struct resp *resp = http_resp(http);
	err_t err;
	uint8_t payload = 1;

	bool success = queue_try_add(q, &payload);

	if(success) {
		err = http_resp_set_status(resp, HTTP_STATUS_OK);
		if (err != ERR_OK) {
			HTTP_LOG_ERROR("Set status 200 failed: %d", err);
			return http_resp_err(http,
						HTTP_STATUS_INTERNAL_SERVER_ERROR);
		}
		
		if ((err = http_resp_set_hdr_ltrl(resp, "Cache-Control", "no-store")) != ERR_OK) {
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

		return http_resp_send_buf(http, "Successfully displayed heart", 29, false);
	}

	err = http_resp_set_status(resp, HTTP_STATUS_TOO_MANY_REQUESTS);
	if (err != ERR_OK) {
		HTTP_LOG_ERROR("Set status 429 failed: %d", err);
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
	
	return http_resp_send_buf(http, "Too many requests are being made, try again later", 50, false);
}

err_t moon_handler(struct http *http, void *q) {
	struct req *req = http_req(http);
	struct resp *resp = http_resp(http);
	err_t err;
	uint8_t payload = 2;

	bool success = queue_try_add(q, &payload);

	if(success) {
		err = http_resp_set_status(resp, HTTP_STATUS_OK);
		if (err != ERR_OK) {
			HTTP_LOG_ERROR("Set status 200 failed: %d", err);
			return http_resp_err(http,
						HTTP_STATUS_INTERNAL_SERVER_ERROR);
		}
		
		if ((err = http_resp_set_hdr_ltrl(resp, "Cache-Control", "no-store")) != ERR_OK) {
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

		return http_resp_send_buf(http, "Successfully displayed moon", 28, false);
	}

	err = http_resp_set_status(resp, HTTP_STATUS_TOO_MANY_REQUESTS);
	if (err != ERR_OK) {
		HTTP_LOG_ERROR("Set status 429 failed: %d", err);
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
	
	return http_resp_send_buf(http, "Too many requests are being made, try again later", 50, false);
}

err_t star_handler(struct http *http, void *q) {
	struct req *req = http_req(http);
	struct resp *resp = http_resp(http);
	err_t err;
	uint8_t payload = 3;

	bool success = queue_try_add(q, &payload);

	if(success) {
		err = http_resp_set_status(resp, HTTP_STATUS_OK);
		if (err != ERR_OK) {
			HTTP_LOG_ERROR("Set status 200 failed: %d", err);
			return http_resp_err(http,
						HTTP_STATUS_INTERNAL_SERVER_ERROR);
		}
		
		if ((err = http_resp_set_hdr_ltrl(resp, "Cache-Control", "no-store")) != ERR_OK) {
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

		return http_resp_send_buf(http, "Successfully displayed star", 28, false);
	}

	err = http_resp_set_status(resp, HTTP_STATUS_TOO_MANY_REQUESTS);
	if (err != ERR_OK) {
		HTTP_LOG_ERROR("Set status 429 failed: %d", err);
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
	
	return http_resp_send_buf(http, "Too many requests are being made, try again later", 50, false);
}

err_t surface_handler(struct http *http, void *q) {
	struct req *req = http_req(http);
	struct resp *resp = http_resp(http);
	err_t err;
	uint8_t payload = 4;

	bool success = queue_try_add(q, &payload);

	if(success) {
		err = http_resp_set_status(resp, HTTP_STATUS_OK);
		if (err != ERR_OK) {
			HTTP_LOG_ERROR("Set status 200 failed: %d", err);
			return http_resp_err(http,
						HTTP_STATUS_INTERNAL_SERVER_ERROR);
		}
		
		if ((err = http_resp_set_hdr_ltrl(resp, "Cache-Control", "no-store")) != ERR_OK) {
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

		return http_resp_send_buf(http, "Successfully displayed surface", 31, false);
	}

	err = http_resp_set_status(resp, HTTP_STATUS_TOO_MANY_REQUESTS);
	if (err != ERR_OK) {
		HTTP_LOG_ERROR("Set status 429 failed: %d", err);
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
	
	return http_resp_send_buf(http, "Too many requests are being made, try again later", 50, false);
}

err_t triangles_handler(struct http *http, void *q) {
	struct req *req = http_req(http);
	struct resp *resp = http_resp(http);
	err_t err;
	uint8_t payload = 5;

	bool success = queue_try_add(q, &payload);

	if(success) {
		err = http_resp_set_status(resp, HTTP_STATUS_OK);
		if (err != ERR_OK) {
			HTTP_LOG_ERROR("Set status 200 failed: %d", err);
			return http_resp_err(http,
						HTTP_STATUS_INTERNAL_SERVER_ERROR);
		}
		
		if ((err = http_resp_set_hdr_ltrl(resp, "Cache-Control", "no-store")) != ERR_OK) {
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

		return http_resp_send_buf(http, "Successfully displayed triangles", 33, false);
	}

	err = http_resp_set_status(resp, HTTP_STATUS_TOO_MANY_REQUESTS);
	if (err != ERR_OK) {
		HTTP_LOG_ERROR("Set status 429 failed: %d", err);
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
	
	return http_resp_send_buf(http, "Too many requests are being made, try again later", 50, false);
}