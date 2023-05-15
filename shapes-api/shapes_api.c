#include "picow_http/http.h"
#include <stdio.h>
#include <stdint.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/multicore.h"
#include "pico/sync.h"
#include "pico/util/queue.h"

#include "lib/led_matrix.h"
#include "handlers.h"

#define uint8_t unsigned char

static struct netinfo netinfo;
queue_t queue;

// Inicjalizacja pinów GPIO
static inline void led_matrix_init() {
	gpio_init(R1_PIN);
	gpio_set_dir(R1_PIN, GPIO_OUT);
	gpio_init(G1_PIN);
	gpio_set_dir(G1_PIN, GPIO_OUT);
	gpio_init(B1_PIN);
	gpio_set_dir(B1_PIN, GPIO_OUT);
	gpio_init(R2_PIN);
	gpio_set_dir(R2_PIN, GPIO_OUT);
	gpio_init(G2_PIN);
	gpio_set_dir(G2_PIN, GPIO_OUT);
	gpio_init(B2_PIN);
	gpio_set_dir(B2_PIN, GPIO_OUT);
	gpio_init(A_PIN);
	gpio_set_dir(A_PIN, GPIO_OUT);
	gpio_init(B_PIN);
	gpio_set_dir(B_PIN, GPIO_OUT);
	gpio_init(C_PIN);
	gpio_set_dir(C_PIN, GPIO_OUT);
	gpio_init(D_PIN);
	gpio_set_dir(D_PIN, GPIO_OUT);
	gpio_init(LAT_PIN);
	gpio_set_dir(LAT_PIN, GPIO_OUT);
	gpio_init(OE_PIN);
	gpio_set_dir(OE_PIN, GPIO_OUT);
	gpio_init(CLK_PIN);
	gpio_set_dir(CLK_PIN, GPIO_OUT);

	// Ustawienie stanu początkowego wyjść
	gpio_put(OE_PIN, 1);
	gpio_put(LAT_PIN, 0);
	gpio_put(R1_PIN, 0);
	gpio_put(G1_PIN, 0);
	gpio_put(B1_PIN, 0);
	gpio_put(R2_PIN, 0);
	gpio_put(G2_PIN, 0);
	gpio_put(B2_PIN, 0);
	gpio_put(A_PIN, 0);
	gpio_put(B_PIN, 0);
	gpio_put(C_PIN, 0);
	gpio_put(D_PIN, 0);
	gpio_put(CLK_PIN, 0);
}

/* dla 64 bitów w rejestrze */
//funkcja wyświetla pojedynczy pixel o współrzędnych x, y na matrycy w wybranym kolorze i jasności
static inline void set_pixel (uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b, int brightness) {
    // wybranie wiersza
    set_row(y);
    int pixel = 0; //jeśli wykryjemy pixel to pixel = 1

    //wybieranie kolumny
    for (int i = 0; i < MATRIX_WIDTH; i++) {
        
        gpio_put(CLK_PIN, 0);


        if (i == x) {  
            gpio_put(R1_PIN, (y < 16) & r); //first part of matrix
            gpio_put(G1_PIN, (y < 16) & g); //first part of matrix
            gpio_put(B1_PIN, (y < 16) & b); //first part of matrix
            gpio_put(R2_PIN, (y > 15) & r); //second part of matrix
            gpio_put(G2_PIN, (y > 15) & g); //second part of matrix
            gpio_put(B2_PIN, (y > 15) & b); //second part of matrix
            pixel = 1;
        } else {
            gpio_put(R1_PIN, 0);
            gpio_put(G1_PIN, 0);
            gpio_put(B1_PIN, 0);
            gpio_put(R2_PIN, 0);
            gpio_put(G2_PIN, 0);
            gpio_put(B2_PIN, 0);
        }
        
        gpio_put(CLK_PIN, 1);
    }
    // gpio_put(CLK_PIN, 0);
    
    if (pixel) {
        gpio_put(LAT_PIN, 1);
        gpio_put(OE_PIN, 0);
        sleep_us(brightness);
        gpio_put(LAT_PIN, 0);
        gpio_put(OE_PIN, 1);
    pixel = 0;
    }   
}

void set_row(uint8_t r) {
    gpio_put(A_PIN, (r & 0x0F) & (1 << 0)); 
    gpio_put(B_PIN, (r & 0x0F) & (1 << 1));
    gpio_put(C_PIN, (r & 0x0F) & (1 << 2));
    gpio_put(D_PIN, (r & 0x0F) & (1 << 3));
}

/*  funkcja wyśweitla obrazek na matrycy przesłany w postaci tablicy rgb pikseli    */
void send_picture(const unsigned short *picture, uint8_t r, uint8_t g, uint8_t b, int brightness) {
	const unsigned short *p = picture;
	for (int i = 0; i < MATRIX_HEIGHT; i++) {
		for (int j = 0; j < MATRIX_WIDTH; j++) {
			if (*p != 0) {
				set_pixel(j, i, r, g, b, brightness);
			}
			*p++;
		}
	}
}

/* funkcja wyświetla matrycę podzieloną na trójkąty [\/] */
void send_triangles(int brightness) {     
    for (int i = 0; i < MATRIX_HEIGHT; i++) {
        for (int j = 0; j < MATRIX_WIDTH; j++) {
        	set_pixel(j, i,(j < i + 1), ((j >= i + 1) && (j <= (MATRIX_WIDTH - i - 2))), (j > (MATRIX_WIDTH - i - 2)), brightness);
        }
    }
}

void core1_led_controller() {
	bool did_receive = false;
	uint8_t request = 0;
	// 0 - brak
	// 1 - heart
	// 2 - moon
	// 3 - star
	// 4 - surface
	// 5 - triangles

	while(true) {
		did_receive = queue_try_remove(&queue, &request);

		switch(request) {
			case 0:
				sleep_us(1);
			case 1:
				send_picture(RedHeart, 1, 0, 0, 50);
			case 2:
				send_picture(Moon, 0, 0, 1, 50);
			case 3:
				send_picture(YellowStar, 1, 1, 0, 50);
			case 4:
				send_picture(surface, 1, 0, 0, 10);
			case 5:
				send_triangles(15);
		}
	}
}

int main() {
	struct server *srv;
	struct server_cfg cfg;
	err_t err;
	
	stdio_init_all();
	set_sys_clock_khz(200000, false);
	
	sleep_ms(5000);
	
	printf("WiFi init\n");
    if (cyw43_arch_init()) {
    	printf("WiFi init failed!\n");
    	return -1;
	}
    	
	char ssid[] = "Ledziaki";
	char pass[] = "12345678";
	
	printf("Connecting to WiFi\n");
	
	cyw43_arch_enable_sta_mode();
	
	if (cyw43_arch_wifi_connect_timeout_ms(ssid, pass, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
		printf("Failed to connect\n");
		return -1;
	}

	printf("GPIO init\n");

	led_matrix_init();
	
	printf("Multicore queue init\n");
	
	queue_init(&queue, sizeof(uint8_t), 10);

	/* Get a default configuration for the http server */
	cfg = http_default_cfg();
	cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);

	printf("Registering PUT /blank\n");
	
	if ((err = register_hndlr_methods(&cfg, "/blank", blank_handler,
					  HTTP_METHOD_PUT, &queue))
	    != ERR_OK) {
		HTTP_LOG_ERROR("Register /blank: %d", err);
		return -1;
		}

	
	printf("Registering PUT /heart\n");
	
	if ((err = register_hndlr_methods(&cfg, "/heart", heart_handler,
					  HTTP_METHOD_PUT, &queue))
	    != ERR_OK) {
		HTTP_LOG_ERROR("Register /heart: %d", err);
		return -1;
		}

	printf("Registering PUT /moon\n");
	
	if ((err = register_hndlr_methods(&cfg, "/moon", moon_handler,
					  HTTP_METHOD_PUT, &queue))
	    != ERR_OK) {
		HTTP_LOG_ERROR("Register /moon: %d", err);
		return -1;
		}
	
	printf("Registering PUT /star\n");
	
	if ((err = register_hndlr_methods(&cfg, "/star", star_handler,
					  HTTP_METHOD_PUT, &queue))
	    != ERR_OK) {
		HTTP_LOG_ERROR("Register /star: %d", err);
		return -1;
		}

	printf("Registering PUT /surface\n");
	
	if ((err = register_hndlr_methods(&cfg, "/surface", surface_handler,
					  HTTP_METHOD_PUT, &queue))
	    != ERR_OK) {
		HTTP_LOG_ERROR("Register /surface: %d", err);
		return -1;
		}

	printf("Registering PUT /triangles\n");
	
	if ((err = register_hndlr_methods(&cfg, "/triangles", triangles_handler,
					  HTTP_METHOD_PUT, &queue))
	    != ERR_OK) {
		HTTP_LOG_ERROR("Register /triangles: %d", err);
		return -1;
		}
		
	printf("Launching led controller on core1\n");
	sleep_ms(5);
	multicore_reset_core1();
	sleep_ms(5);
	multicore_launch_core1(core1_led_controller);
	
	printf("Starting the server...\n");
	/* Start the server */
	if (http_srv_init(&srv, &cfg) != ERR_OK) {
		// Error handling
		printf("Starting server failed!\n");
	}
	
	for (;;) {
		#if PICO_CYW43_ARCH_POLL
			cyw43_arch_poll();
			sleep_ms(1);
		#else
			__wfi();
		#endif
	}
	
	printf("This should be unreachable!\n");
	return 0;
}
