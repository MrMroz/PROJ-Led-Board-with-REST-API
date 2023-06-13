#include "lib/matrix.h"
#include "lib/controller.h"
#include "lib/handlers.h"

int main() {
	// -- FAZA INICJALIZACJI ---
	stdio_init_all();
	set_sys_clock_khz(200000, false);
	
	matrix_gpio_init();
	multicore_init();
	wifi_init();
	rest_api_init();

	for (;;) {
		#if PICO_CYW43_ARCH_POLL
			cyw43_arch_poll();
			sleep_ms(1);
		#else
			__wfi();
		#endif
	}

	return 0;
}
