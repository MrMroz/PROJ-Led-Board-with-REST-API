#include "picow_http/http.h"

int
main(void)
{
	struct server *srv;
	struct server_cfg cfg;

	/*
	 * Connection to WLAN, and any other initialization for your app
	 * here ...
	 */

	/* Get a default configuration for the http server */
	cfg = http_default_cfg();

	/* Start the server */
	if (http_srv_init(&srv, &cfg) != ERR_OK) {
		// Error handling
	}
}
