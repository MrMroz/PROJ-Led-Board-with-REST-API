#include "pico/cyw43_arch.h"
#include "pico/sync.h"
#include "pico/time.h"
#include "hardware/gpio.h"

#include "lib/matrix.h"


void demo() {
	spectrum_anim(500, 250);
	
	sleep_ms(1000);
	
	scrolling_text("PROJEKT GRUPOWY LEDZIAKI ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789", 240);

	sleep_ms(1000);

	spectrum_anim(500, 250);
}

int main() {
	// -- FAZA INICJALIZACJI ---
	stdio_init_all();

	set_sys_clock_khz(200000, false);
	
	// Czas na ewentualne podlaczenie pod COM dla debugowania
	sleep_ms(5000);
	
	printf("WiFi init\n");
    
    if (cyw43_arch_init()) {
		printf("WiFi init failed!\n");
		return -1;
	}

	printf("GPIO init\n");

	matrix_gpio_init();

	// Dioda - koniec inicjalizacji
	cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);

	// -- KONIEC FAZY INICJALIZACJI ---
	
	for(;;) {
		demo();
	}

	return 0;
}
