
#include <stdint.h>

#include "lwip/ip_addr.h"
#include "picow_http/http.h"

#define MAC_ADDR_LEN (sizeof("01:02:03:04:05:06"))

struct netinfo {
	unsigned	magic;
#define NETINFO_MAGIC (0x4f5dde9f)
	char		ip[IPADDR_STRLEN_MAX];
	char		mac[MAC_ADDR_LEN];
};


err_t get_led_handler(struct http *http, void *p);
err_t put_led_handler(struct http *http, void *P);
