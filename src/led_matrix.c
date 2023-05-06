#include "../lib/led_matrix.h"


// #define SET_ROW_PINS(row) PORTC = row | PORTC & 240

// Bufor ramki dla jednej kolumny pikseli
uint16_t pixel_buffer[32];


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
	gpio_put(OE_PIN, 0);
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



/*
At first, you select a line to draw to via the 4 line select pins. They are in binary, 
ordered A B C D where a High means 1 and a low means 0. So for example for row 11 
we would have A=1 B=1 C=0 D=1 (1+2+0+8 = 11).
*/
void set_row(uint8_t r) {
switch (r & 0x0F) // dla qwartości z przedziłąu 16-31 przypisuje 0-15
{
    case 0b0000: //0    
        gpio_put(A_PIN, 0);
        gpio_put(B_PIN, 0);
        gpio_put(C_PIN, 0);
        gpio_put(D_PIN, 0);
        break;

    case 0b0001: //1
        gpio_put(A_PIN, 1);
        gpio_put(B_PIN, 0);
        gpio_put(C_PIN, 0);
        gpio_put(D_PIN, 0);
        break;

    case 0b0010: //2
        gpio_put(A_PIN, 0);
        gpio_put(B_PIN, 1);
        gpio_put(C_PIN, 0);
        gpio_put(D_PIN, 0);
        break;

    case 0b0011: //3
        gpio_put(A_PIN, 1);
        gpio_put(B_PIN, 1);
        gpio_put(C_PIN, 0);
        gpio_put(D_PIN, 0);
        break;

    case 0b0100: //4
        gpio_put(A_PIN, 0);
        gpio_put(B_PIN, 0);
        gpio_put(C_PIN, 1);
        gpio_put(D_PIN, 0);
        break;

    case 0b0101: //5
        gpio_put(A_PIN, 1);
        gpio_put(B_PIN, 0);
        gpio_put(C_PIN, 1);
        gpio_put(D_PIN, 0);
        break;

    case 0b0110: //6
        gpio_put(A_PIN, 0);
        gpio_put(B_PIN, 1);
        gpio_put(C_PIN, 1);
        gpio_put(D_PIN, 0);
        break;

    case 0b0111: //7
        gpio_put(A_PIN, 1);
        gpio_put(B_PIN, 1);
        gpio_put(C_PIN, 1);
        gpio_put(D_PIN, 0);
        break;

    case 0b1000: //8
        gpio_put(A_PIN, 0);
        gpio_put(B_PIN, 0);
        gpio_put(C_PIN, 0);
        gpio_put(D_PIN, 1);
        break;

    case 0b1001: //9
        gpio_put(A_PIN, 1);
        gpio_put(B_PIN, 0);
        gpio_put(C_PIN, 0);
        gpio_put(D_PIN, 1);
        break;

    case 0b1010: //10
        gpio_put(A_PIN, 0);
        gpio_put(B_PIN, 1);
        gpio_put(C_PIN, 0);
        gpio_put(D_PIN, 1);
        break;

    case 0b1011: //11
        gpio_put(A_PIN, 1);
        gpio_put(B_PIN, 1);
        gpio_put(C_PIN, 0);
        gpio_put(D_PIN, 1);
        break;

    case 0b1100: //12
        gpio_put(A_PIN, 0);
        gpio_put(B_PIN, 0);
        gpio_put(C_PIN, 1);
        gpio_put(D_PIN, 1);
        break;

    case 0b1101: //13
        gpio_put(A_PIN, 1);
        gpio_put(B_PIN, 0);
        gpio_put(C_PIN, 1);
        gpio_put(D_PIN, 1);
        break;

    case 0b1110: //14
        gpio_put(A_PIN, 0);
        gpio_put(B_PIN, 1);
        gpio_put(C_PIN, 1);
        gpio_put(D_PIN, 1);
        break;

    case 0b1111: //15
        gpio_put(A_PIN, 1);
        gpio_put(B_PIN, 1);
        gpio_put(C_PIN, 1);
        gpio_put(D_PIN, 1);
        break;

    default:
        break;
    }
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
        // sleep_us(1);
        if (i == x) {  
            gpio_put(R1_PIN, (y < 16) & r); //first part of matrix
            gpio_put(G1_PIN, (y < 16) & g); //first part of matrix
            gpio_put(B1_PIN, (y < 16) & b); //first part of matrix
            gpio_put(R2_PIN, (y > 15) & r); //second part of matrix
            gpio_put(G2_PIN, (y > 15) & g); //second part of matrix
            gpio_put(B2_PIN, (y > 15) & b); //second part of matrix
            pixel = 1;
            // break;
        } else {
            gpio_put(R1_PIN, 0);
            gpio_put(G1_PIN, 0);
            gpio_put(B1_PIN, 0);
            gpio_put(R2_PIN, 0);
            gpio_put(G2_PIN, 0);
            gpio_put(B2_PIN, 0);
        }
        
        gpio_put(CLK_PIN, 1);
        // sleep_us(1);
    }
    gpio_put(CLK_PIN, 0);
    // sleep_us(1);
    
    if (pixel) {
        gpio_put(LAT_PIN, 1);
        sleep_us(1);
        gpio_put(LAT_PIN, 0);
        // sleep_us(1);
        gpio_put(OE_PIN, 0);
        sleep_us(brightness);
        gpio_put(OE_PIN, 1);
        // sleep_us(1);
    pixel = 0;
    }   
}

/*  funkcja wypełnia dany wiersz  */
void fill_row(int y, uint8_t r, uint8_t g, uint8_t b, int brightness) {

    set_row(y);

    gpio_put(OE_PIN, 1);
    gpio_put(LAT_PIN, 0);

    // Wysłanie danych pikseli do rejestru FM6124
    for (int j = 0; j < MATRIX_WIDTH; j++) {
        gpio_put(CLK_PIN, 0);
        gpio_put(R1_PIN, (y < 16) & r);
        gpio_put(G1_PIN, (y < 16) & g);
        gpio_put(B1_PIN, (y < 16) & b);
        gpio_put(R2_PIN, (y > 15) & r);
        gpio_put(G2_PIN, (y > 15) & g);
        gpio_put(B2_PIN, (y > 15) & b);

        // sleep_us(1);
        gpio_put(CLK_PIN, 1);
        // sleep_us(1);
    }
    gpio_put(CLK_PIN, 0);
    gpio_put(LAT_PIN, 1);
    sleep_us(1);
    gpio_put(LAT_PIN, 0);
    // sleep_us(1);
    gpio_put(OE_PIN, 0);
    sleep_us(brightness);
    gpio_put(OE_PIN, 1);
    // sleep_us(1);
}

/*  funkcja wyświetla daną kolumne  */
void fill_column(int x, uint8_t r, uint8_t g, uint8_t b, int brightness) {
    for (int i = 0; i < MATRIX_HEIGHT; i++) {
        set_pixel(x, i, r, g, b, brightness);
    }
}


/*  funkcja wypełnia całą matrycę wybranym kolorem  */
void fill_matrix(uint8_t r, uint8_t g, uint8_t b, int brightness) {
    for (int i = 0; i < MATRIX_HEIGHT; i++) {
        fill_row(i, r, g, b, brightness);
    }
}


void fill_half_matrix(uint8_t r, uint8_t g, uint8_t b, int brightness) {
    for (int i = 0; i < MATRIX_HEIGHT; i++) {
        // Wyłączanie wyjścia matrycy LED
        gpio_put(OE_PIN, 1);

        // Ustawienie wiersza w interfejsie HUB75
        set_row(i);

        gpio_put(LAT_PIN, 0);

        // Wysłanie danych pikseli do rejestru FM6124
        for (int j = 0; j < MATRIX_WIDTH; j++) {
        gpio_put(CLK_PIN, 0);
        gpio_put(R1_PIN, (i < 16) & r);
        gpio_put(G1_PIN, (i < 16) & g);
        gpio_put(B1_PIN, (i < 16) & b);
        gpio_put(R2_PIN, (i > 15) & r);
        gpio_put(G2_PIN, (i > 15) & g);
        gpio_put(B2_PIN, (i > 15) & b);

        // sleep_us(1);
        gpio_put(CLK_PIN, 1);
        // sleep_us(1);
        }


        // Przesłanie danych z rejestru FM6124 do matrycy LED
        gpio_put(CLK_PIN, 0);
        gpio_put(LAT_PIN, 1);
        sleep_us(1);
        gpio_put(LAT_PIN, 0);
        // sleep_us(1);
        gpio_put(OE_PIN, 0);
        sleep_us(brightness);
        gpio_put(OE_PIN, 1);
        // sleep_us(1);
    }
}


/* funkcja wyświetla matrycę podzieloną na trójkąty [\/] */
void fill_rgb_matrix(int brightness) {     

    for (int i = 0; i < MATRIX_HEIGHT; i++) {
        
        for (int j = 0; j < (i + 1); j++) {
            set_pixel(j,i,1,0,0, brightness);
            set_pixel(63 - j,i,0,0,1, brightness);
        }
        
        for (int j = i; j < (63 - i); j++) {
            set_pixel(j,i,0,1,0, brightness);
        }
    }

    // Przesłanie danych z rejestru FM6124 do matrycy LED
    gpio_put(CLK_PIN, 0);
    gpio_put(LAT_PIN, 1);
    sleep_us(1);
    gpio_put(LAT_PIN, 0);
    // sleep_us(1);
    gpio_put(OE_PIN, 0);
    sleep_us(brightness);
    gpio_put(OE_PIN, 1);
    // sleep_us(1);
}


/*  funkcja wyświetla animację z przesuwającym się wierszem i kolumną*/
void animation(int speed, int brightness) {
    for (int i = 0; i < MATRIX_HEIGHT; i++) {
        fill_row(i, 1, 0, 0, brightness);
        // fill_row(i, (i%3)&1, (i+1%3), (i+2%3)&1, brightness);
        fill_column(i, 0, 0, 1, brightness);
        sleep_ms(speed);
    }
    for (int i = MATRIX_HEIGHT; i > 0; i--) {
        fill_row(i - 1, 1, 0, 0, brightness);
        fill_column( 64 - i, 0, 0, 1, brightness);
        sleep_ms(speed);
    }
    for (int i = 0; i < MATRIX_HEIGHT; i++) {
        fill_row(i, 1, 0, 0, brightness);
        fill_column(63 - i, 0, 0, 1, brightness);
        sleep_ms(speed);
    }
    for (int i = MATRIX_HEIGHT; i > 0; i--) {
        fill_row(i - 1, 1, 0, 0, brightness);
        fill_column(i - 1, 0, 0, 1, brightness);
        sleep_ms(speed);
    }
}


// Funkcja do wysyłania bufora ramki do matrycy LED __ raczej nieużywana, tutaj wyświetla wypełnioną matrycę takim wzorkiem
void send_frame() {   

    for (int i = 0; i < MATRIX_HEIGHT / 2; i++) {
        
        // Wyłączanie wyjścia matrycy LED
        gpio_put(OE_PIN, 1);
        
        // Ustawienie wiersza w interfejsie HUB75
        set_row(i);

        // gpio_put(A_PIN, (i >> 0) & 1);
        // gpio_put(B_PIN, (i >> 1) & 1);
        // gpio_put(C_PIN, (i >> 2) & 1);
        // gpio_put(D_PIN, (i >> 3) & 1);

        gpio_put(LAT_PIN, 0);

        // Wysłanie danych pikseli do rejestru FM6124
        uint16_t pixels = pixel_buffer[i];
        for (int j = 0; j < 64; j++) {
        gpio_put(CLK_PIN, 0);
        gpio_put(R1_PIN, (j+1)%2);
        gpio_put(G1_PIN, 0);
        gpio_put(B1_PIN, j % 2);
        gpio_put(R2_PIN, (i+1)%2);
        gpio_put(G2_PIN, i%2);
        gpio_put(B2_PIN, 0);
        
        sleep_us(1);
        gpio_put(CLK_PIN, 1);
        sleep_us(1);
        }


    // Przesłanie danych z rejestru FM6124 do matrycy LED
    gpio_put(CLK_PIN, 0);
    gpio_put(LAT_PIN, 1);
    sleep_us(1);

    // Włączenie wyjścia matrycy LED
    gpio_put(OE_PIN, 0);
    sleep_us(1);
    gpio_put(CLK_PIN, 1);
    sleep_us(0);


    gpio_put(LAT_PIN, 0);
    gpio_put(OE_PIN, 1);
    gpio_put(CLK_PIN, 0);
    sleep_us(10);
    }
}

/*  funkcja wyśweitla obrazek na matrycy przesłany w postaci tablicy rgb pikseli    */
void send_picture(const unsigned short *picture, uint8_t r, uint8_t g, uint8_t b, int brightness) {
	const unsigned short *p = picture;
	for (int i = 0; i < MATRIX_HEIGHT; i++) {
		for (int j = 0; j < MATRIX_WIDTH; j++) {
			if (*p != 0) {
				set_pixel(j, i, r, g, b, brightness);
			}
			// set_pixel(i, j, r & *p, g & *p, b & *p, brightness);
			*p++;
		}
	}
}

/*  funkcja wyśweitla kilka obrazków w odstępie ki;lku sekund   */
void picture_animation(uint8_t r, uint8_t g, uint8_t b, int brightness) {
	for (int i = 0; i < 500; i++) {
		send_picture(RedHeart, 1, 0, 0, brightness);
	}
	for (int i = 0; i < 800; i++) {
		send_picture(Moon, 1, 0, 0, brightness);
	}
	for (int i = 0; i < 500; i++) {
		send_picture(YellowStar, 1, 0, 0, brightness);
	}
	for (int i = 0; i < 200; i++) {
		send_picture(surface, 1, 0, 0, 10);
	}
}







int main() {

    stdio_init_all();
    led_matrix_init();

    // Przykładowy kod do generowania obrazu na matrycy LED
    while (true) {
        

        // set_pixel(24, 12, 0, 0, 1, 100);         // wyświetlanie piksela
        // fill_row(3, 1, 0, 0, 100);               // wyświetlanie wiersza
        // fill_column(30, 1, 0, 0, 100);           // wyświetlanie kolumny
        // fill_matrix(1,0,0,200);                  // wypełnianie matrycy
        // fill_half_matrix(1,0,0,300);             // to jeszcze nie działa
        // fill_rgb_matrix(5);                     // to wyświetla trójkątny wzorek
        // animation(30, 200);                      // to animacja taka z przesuwającą się kolumną i wierszem
        // send_frame();                            // to taki wzorek
        picture_animation(0, 0, 1, 50);          // a to obrazki wyświetlane co chwile

    }
    return 0;
}








// Ustawienie wartości pikseli w buforze ramki
	// for (int x = 0; x < 64; x++) {
	//   for (int y = 0; y < 32; y++) {
	//     uint8_t r = x * 4;
	//     uint8_t g = y * 8;
	//     uint8_t b = (x + y) * 2;
	//     set_pixel_color(x, y, r, g, b);
	//   }
	// }

	// Wysłanie bufora ramki do matrycy LED
	// send_frame();


	/* dla 16 bitów w rejestrze */
// void set_pixel (uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
//   set_row(y);
//   int counter = 0, pixel = 0;
//   for (int i = 0; i < MATRIX_WIDTH/16; i++) {
//     for (int j = 0; j < 16; j++) {

//       gpio_put(CLK_PIN, 0);
//       sleep_us(5);
//       if (counter == x) {  
//         gpio_put(R1_PIN, r);
//         gpio_put(G1_PIN, g);
//         gpio_put(B1_PIN, b);
//         gpio_put(R2_PIN, r);
//         gpio_put(G2_PIN, g);
//         gpio_put(B2_PIN, b);
//         pixel = 1;
//       } else {
//         gpio_put(R1_PIN, 0);
//         gpio_put(G1_PIN, 0);
//         gpio_put(B1_PIN, 0);
//         gpio_put(R2_PIN, 0);
//         gpio_put(G2_PIN, 0);
//         gpio_put(B2_PIN, 0);
//       }
//       counter++;
	
//       gpio_put(CLK_PIN, 1);
//       sleep_us(5);
	
//     }
//     gpio_put(CLK_PIN, 0);
//     sleep_us(10);
//     if (pixel) {
//       gpio_put(LAT_PIN, 1);
//       sleep_us(5);
//       gpio_put(LAT_PIN, 0);
//       sleep_us(2);
//       gpio_put(OE_PIN, 0);
//       sleep_us(5);
//       gpio_put(OE_PIN, 1);
//       sleep_us(5);
//       pixel = 0;
//     }
	
//   }
// }

// Funkcja do ustawiania wartości koloru piksela w buforze ramki
// void set_pixel_color(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
//   uint16_t color = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
//   pixel_buffer[y] |= (color << (x * 16));
// }


//// do sendframe
// Wysyłanie danych pikseli do rejestru FM6124
// for (int i = 0; i < 32; i++) {
//   // Ustawienie wiersza w interfejsie HUB75
//   gpio_put(A_PIN, (i >> 0) & 1);
//   gpio_put(B_PIN, (i >> 1) & 1);
//   gpio_put(C_PIN, (i >> 2) & 1);
//   gpio_put(D_PIN, (i >> 3) & 1);

//   // Wysłanie danych pikseli do rejestru FM6124
//   uint16_t pixels = pixel_buffer[i];
//   for (int j = 0; j < 16; j++) {
//     gpio_put(CLK_PIN, 0);
//     gpio_put(R1_PIN, (pixels >> (15 - j)) & 1);
//     gpio_put(G1_PIN, (pixels >> (31 - j)) & 1);
//     gpio_put(B1_PIN, (pixels >> (47 - j)) & 1);
//     gpio_put(R2_PIN, (pixels >> (63 - j)) & 1);
//     gpio_put(G2_PIN, (pixels >> (79 - j)) & 1);
//     gpio_put(B2_PIN, (pixels >> (95 - j)) & 1);
//     sleep_us(10);
//     gpio_put(CLK_PIN, 1);
//     sleep_us(10);
//   }
// }
// gpio_put(R1_PIN, (pixels >> (15 - j)) & 1);
	// gpio_put(G1_PIN, (pixels >> (31 - j)) & 1);
	// gpio_put(B1_PIN, (pixels >> (47 - j)) & 1);
	// gpio_put(R2_PIN, (pixels >> (63 - j)) & 1);
	// gpio_put(G2_PIN, (pixels >> (79 - j)) & 1);
	// gpio_put(B2_PIN, (pixels >> (95 - j)) & 1);