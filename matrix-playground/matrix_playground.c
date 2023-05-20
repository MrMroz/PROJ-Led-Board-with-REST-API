#include <stdio.h>
#include <stdint.h>

#include "pico/stdlib.h"
#include "pico/sync.h"
#include "pico/cyw43_arch.h"

#include "lib/led_matrix.h"

#define uint8_t unsigned char



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

void test() {
	for (int i = 0; i < MATRIX_WIDTH; i++) {
		gpio_put(CLK_PIN, 0);
		gpio_put(G1_PIN, 1);
		gpio_put(CLK_PIN, 1);
	}
	
	gpio_put(LAT_PIN, 1);
        gpio_put(OE_PIN, 0);
        sleep_us(30);
        gpio_put(LAT_PIN, 0);
        gpio_put(OE_PIN, 1);
}

void load_row(uint8_t *row, uint8_t row_num) {
	set_row(row_num);
	bool second_part = (row_num > 15);
	
	switch (second_part) {
		case false:
			for(int i = 0; i < MATRIX_WIDTH; i++) {
				gpio_put(CLK_PIN, 0);
				gpio_put(R1_PIN, row[i] & 0x1);
				gpio_put(G1_PIN, (row[i] >> 1) & 0x1);
				gpio_put(B1_PIN, (row[i] >> 2) & 0x1);
				gpio_put(R2_PIN, 0);
				gpio_put(G2_PIN, 0);
				gpio_put(B2_PIN, 0);
				gpio_put(CLK_PIN, 1);
			}
			break;
		case true:
			for(int i = 0; i < MATRIX_WIDTH; i++) {
				gpio_put(CLK_PIN, 0);
				gpio_put(R2_PIN, row[i] & 0x1);
				gpio_put(G2_PIN, (row[i] >> 1) & 0x1);
				gpio_put(B2_PIN, (row[i] >> 2) & 0x1);
				gpio_put(R1_PIN, 0);
				gpio_put(G1_PIN, 0);
				gpio_put(B1_PIN, 0);
				gpio_put(CLK_PIN, 1);
			}
			break;
	}
	
	
	gpio_put(LAT_PIN, 1);
        gpio_put(OE_PIN, 0);
        sleep_us(50);
        gpio_put(LAT_PIN, 0);
        gpio_put(OE_PIN, 1);
}

void rotate(uint8_t *arr, int n, bool rev) {
	uint8_t tmp;
	switch (rev) {
		case false:
			tmp = arr[0];

			for(int i = 1; i < n; i++) {
			   arr[i - 1] = arr[i];
			}

			arr[n - 1] = tmp;
			
			break;
			
		case true:
			tmp = arr[n-1];

			for(int i = (n-2); i >= 0; i--) {
			   arr[i + 1] = arr[i];
			}

			arr[0] = tmp;
			
			break;
	}
}



int main() {
	stdio_init_all();
	set_sys_clock_khz(200000, false);
	
	set_row(0);
	
	sleep_ms(5000);
	
	printf("WiFi init\n");
    
    if (cyw43_arch_init()) {
    	printf("WiFi init failed!\n");
    	return -1;
	}

	printf("GPIO init\n");

	led_matrix_init();
	
	cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
	set_row(0);
	
	uint8_t  c[64] = 	{0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
				0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2,
				0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3,
				0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4,
				0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5,
				0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6,
				0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7,
				0x7};
				
	uint8_t full[32][64];
	
	for(int i = 0; i < 32; i++) {
		for(int k = 0; k < 64; k++) {
			full[i][k] = c[k];
		}
	}
	
	int cnt = 0;
	int cnt_max = 20;
	for(;;) {
		for(int i = 0; i < MATRIX_HEIGHT; i++) {
			load_row(full[i], i);
		}
		cnt++;
		if(cnt >= cnt_max) {
			for(int l = 0; l < 8; l++){
				for(int n = 0; n < 4; n++){
					rotate(full[4*l + n], 64, ((l%2)==1));	
				}

			}
			cnt = 0;

		}
	}


	return 0;
}
