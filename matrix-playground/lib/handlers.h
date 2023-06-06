#pragma once

#include <stdio.h>
#include <stdint.h>

#include "lwip/ip_addr.h"

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/multicore.h"
#include "pico/sync.h"
#include "pico/util/queue.h"

#include "picow_http/http.h"

#include "matrix.h"

#define MAX_INT_LEN (STRLEN_LTRL("-2147483648"))
#define MAC_ADDR_LEN (sizeof("01:02:03:04:05:06"))
#define MAX_TEXT_LEN (UINT8_MAX)

#define WIFI_SSID ("Ledziaki")
#define WIFI_PASS ("12345678")
// #define NETINFO_MAGIC (0x4f5dde9f)

// struct netinfo {
// 	unsigned	magic;
// 	char		ip[IPADDR_STRLEN_MAX];
// 	char		mac[MAC_ADDR_LEN];
// };

extern struct server *srv;
extern struct server_cfg cfg;
extern err_t err;

void rest_api_init();
void wifi_init();

err_t text_put_handler(struct http *http, void *q);
err_t spectrum_put_handler(struct http *http, void *q);
err_t next_get_handler(struct http *http, void *q);
