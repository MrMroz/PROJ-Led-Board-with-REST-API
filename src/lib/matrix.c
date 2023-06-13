#include "matrix.h"

uint8_t mx[MATRIX_HEIGHT][MATRIX_WIDTH] = {0};

// Inicjalizacja pinów GPIO
void matrix_gpio_init() {
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

void set_row(uint8_t r) {
    gpio_put(A_PIN, (r & 0x0F) & (1 << 0)); 
    gpio_put(B_PIN, (r & 0x0F) & (1 << 1));
    gpio_put(C_PIN, (r & 0x0F) & (1 << 2));
    gpio_put(D_PIN, (r & 0x0F) & (1 << 3));
}

void row_shift(uint8_t *arr, int n, bool rev, bool wrap) {
	uint8_t tmp;
	switch (rev) {
		case false:
			tmp = arr[0];

			for(int i = 1; i < n; i++) {
				arr[i - 1] = arr[i];
			}

			if(wrap) {
				arr[n - 1] = tmp;
			} else{
				arr[n - 1] = 0;
			}

			break;
			
		case true:
			tmp = arr[n-1];

			for(int i = (n-2); i >= 0; i--) {
				arr[i + 1] = arr[i];
			}

			if(wrap) {
				arr[0] = tmp;
			} else {
				arr[0] = 0;
			}

			
			break;
	}
}

// (r r r g g g b b)
// threshold 0-7
void redraw_128(uint8_t row_num, uint8_t threshold) {
	set_row(row_num);
	
	for(int i = 0; i < MATRIX_WIDTH; i++) {
		gpio_put(CLK_PIN, 0);

		gpio_put(R1_PIN, (mx[row_num][i] >> 5) > threshold);
		gpio_put(G1_PIN, ((mx[row_num][i] >> 2) & 0x7) > threshold);
		gpio_put(B1_PIN, (mx[row_num][i] & 0x3) > threshold);
		
		gpio_put(R2_PIN, (mx[row_num+16][i] >> 5) > threshold);
		gpio_put(G2_PIN, ((mx[row_num+16][i] >> 2) & 0x7) > threshold);
		gpio_put(B2_PIN, (mx[row_num+16][i] & 0x3) > threshold);

		gpio_put(CLK_PIN, 1);
		}
	
	gpio_put(LAT_PIN, 1);
	gpio_put(OE_PIN, 0);
	sleep_us(50);
	gpio_put(LAT_PIN, 0);
	gpio_put(OE_PIN, 1);
}

void redraw_full() {
	for(uint8_t k = 0; k < 7; k++) {
		for(uint8_t i = 0; i < (MATRIX_HEIGHT/2); i++) {
			redraw_128(i, k);
		}
	}
}

void swap_mx_color(uint8_t c) {
	for(int i = 0; i < 32; i++) {
		for(int k = 0; k < 64; k++) {
			mx[i][k] = c;
		}
	}
}

// rgb -> r - 0, g - 1, b - 0;
uint8_t cex(uint8_t color, uint8_t rgb) {
	switch(rgb) {
		case 0: //r
			return (color >> 5) & 0x7;
			break;
		case 1: //g
			return (color >> 2) & 0x7;
			break;
		case 2: //b
			return (color & 0x3);
			break;
		default:
			return(0x0);
			break;
	}
}

void full_shift(bool rev, bool wrap) {
	for(int i = 0; i < MATRIX_HEIGHT; i++) {
		row_shift(mx[i], MATRIX_WIDTH, rev, wrap);
	}
}


void load_right(uint8_t img[32][19], uint8_t col, uint8_t color) {
	if(color == 0x00) {
		for(int i = 0; i < MATRIX_HEIGHT; i++) {
			mx[i][63] = img[i][col];
		}
	} else {
		for(int i = 0; i < MATRIX_HEIGHT; i++) {
			mx[i][63] = (img[i][col] & color);
		}
	}

}



uint8_t spectrum_cycle(uint8_t c) {
	uint8_t state;

	if((cex(c, 0) == 7) && (cex(c, 1) < 7)) {state = 1;}
	else if((cex(c, 0) > 0) && (cex(c, 1) == 7)) {state = 2;}
	else if((cex(c, 1) == 7) && (cex(c, 2) < 3)) {state = 3;}
	else if((cex(c, 1) > 0) && (cex(c, 2) == 3)) {state = 4;}
	else if((cex(c, 0) < 7) && (cex(c, 2) == 3)) {state = 5;}
	else if((cex(c, 0) == 7) && (cex(c, 2) > 0)) {state = 6;}

	switch (state) {
		case 1:
			return c + 0b00000100;
			break;
		case 2:
			return c - 0b00100000;
			break;
		case 3:
			return c + 0b00000001;
			break;
		case 4:
			return c - 0b00000100;
			break;
		case 5:
			return c + 0b00100000;
			break;
		case 6:
			return c - 0b00000001;
			break;
		default:
			return 0b11100000;
	}
}

void vertical_spectrum() {
	uint8_t vc;
	for(int i = 0; i < 32; i++) {
		vc = spectrum_cycle(mx[i][0]);
		for(int k = 0; k < 64; k++) {
			mx[i][k] = vc;
		}
	}
}

void spectrum_anim(uint16_t loops, uint8_t speed) {
	uint8_t cnt = 0;
	uint8_t cnt_max = UINT8_T_MAX - speed;

	uint8_t init_c = 0b11100000;
	for(int i = 0; i < 32; i++) {
		for(int k = 0; k < 64; k++) {
			mx[i][k] = init_c;
		}
		init_c = spectrum_cycle(init_c);
	}

	if(loops == 0U) {
		for(;;) {
			redraw_full();
			cnt++;

			if(cnt >= cnt_max) {
				cnt = 0;
				vertical_spectrum();
			}
		}
	} else {
		for(int i = 0; i < loops; i++) {
			redraw_full();
			cnt++;

			if(cnt >= cnt_max) {
				cnt = 0;
				vertical_spectrum();
			}
		}
	}

	swap_mx_color(0); // Wyczyszczenie tablicy
}

void load_right_char(char *text, uint8_t *colcnt, short *steps_to_end, uint8_t color) {
	switch(*text) {
		case 'A':
			load_right(A, *colcnt, color);
			break;
		case 'B':
			load_right(B, *colcnt, color);
			break;
		case 'C':
			load_right(C, *colcnt, color);
			break;
		case 'D':
			load_right(D, *colcnt, color);
			break;
		case 'E':
			load_right(E, *colcnt, color);
			break;
		case 'F':
			load_right(F, *colcnt, color);
			break;
		case 'G':
			load_right(G, *colcnt, color);
			break;
		case 'H':
			load_right(H, *colcnt, color);
			break;
		case 'I':
			load_right(I, *colcnt, color);
			break;
		case 'J':
			load_right(J, *colcnt, color);
			break;
		case 'K':
			load_right(K, *colcnt, color);
			break;
		case 'L':
			load_right(L, *colcnt, color);
			break;
		case 'M':
			load_right(M, *colcnt, color);
			break;
		case 'N':
			load_right(N, *colcnt, color);
			break;
		case 'O':
			load_right(O, *colcnt, color);
			break;
		case 'P':
			load_right(P, *colcnt, color);
			break;
		case 'Q':
			load_right(Q, *colcnt, color);
			break;
		case 'R':
			load_right(R, *colcnt, color);
			break;
		case 'S':
			load_right(S, *colcnt, color);
			break;
		case 'T':
			load_right(T, *colcnt, color);
			break;
		case 'U':
			load_right(U, *colcnt, color);
			break;
		case 'V':
			load_right(V, *colcnt, color);
			break;
		case 'W':
			load_right(W, *colcnt, color);
			break;
		case 'X':
			load_right(X, *colcnt, color);
			break;
		case 'Y':
			load_right(Y, *colcnt, color);
			break;
		case 'Z':
			load_right(Z, *colcnt, color);
			break;
		case '0':
			load_right(_0, *colcnt, color);
			break;
		case '1':
			load_right(_1, *colcnt, color);
			break;
		case '2':
			load_right(_2, *colcnt, color);
			break;
		case '3':
			load_right(_3, *colcnt, color);
			break;
		case '4':
			load_right(_4, *colcnt, color);
			break;
		case '5':
			load_right(_5, *colcnt, color);
			break;
		case '6':
			load_right(_6, *colcnt, color);
			break;
		case '7':
			load_right(_7, *colcnt, color);
			break;
		case '8':
			load_right(_8, *colcnt, color);
			break;
		case '9':
			load_right(_9, *colcnt, color);
			break;
		case ' ':
			break; // Nie wyswietlamy nic 'colcnt' razy
		case '_':
			break; // Nie wyswietlamy nic 'colcnt' razy
		case '\0':
			*steps_to_end = 69;
			break;
		default: // Niezaimplementowane znaki pomijamy
			*colcnt = 19;
			*steps_to_end -= 19;
			}
}

void scrolling_text(char *text, uint8_t speed, uint8_t color) {
	swap_mx_color(0); // Wyczyszczenie tablicy
	uint8_t cnt = 0;
	uint8_t cnt_max = UINT8_T_MAX - speed;
	short steps_to_end = strlen(text)*19 + 64 + 6;
	uint8_t colcnt = 0;
	while(steps_to_end > 0) {
		redraw_full();
		cnt++;
		
		if(cnt >= cnt_max) {
			full_shift(false, false);
			steps_to_end--;
			
			if(steps_to_end >= 70) {
				load_right_char(text, &colcnt, &steps_to_end, color);
			}

			colcnt++;
			if(colcnt >= 19) {
				colcnt = 0;

				if(steps_to_end >= 70) {
					text++;
				}
			}
			cnt = 0;
		}
	}
	swap_mx_color(0); // Wyczyszczenie tablicy
}