#include "pico/stdlib.h"
#include "hardware/gpio.h"

// Ustawienia pinów GPIO do połączenia z interfejsem HUB75
#define R1_PIN 0
#define G1_PIN 1
#define B1_PIN 2
#define R2_PIN 3
#define G2_PIN 4
#define B2_PIN 5
#define A_PIN 6
#define B_PIN 7
#define C_PIN 8
#define D_PIN 9
#define LAT_PIN 10
#define OE_PIN 11
#define CLK_PIN 12

static inline void led_matrix_init() {
    // Inicjalizacja pinów GPIO
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

// Bufor ramki dla jednej kolumny pikseli
uint16_t pixel_buffer[32];

// Funkcja do ustawiania wartości koloru piksela w buforze ramki
void set_pixel_color(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
  uint16_t color = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
  pixel_buffer[y] |= (color << (x * 16));
}

// Funkcja do wysyłania bufora ramki do matrycy LED
void send_frame() {
  // Wyłączanie wyjścia matrycy LED
  gpio_put(OE_PIN, 1);

  // Wysyłanie danych pikseli do rejestru FM6124
  for (int i = 0; i < 32; i++) {
    // Ustawienie wiersza w interfejsie HUB75
    gpio_put(A_PIN, (i >> 0) & 1);
    gpio_put(B_PIN, (i >> 1) & 1);
    gpio_put(C_PIN, (i >> 2) & 1);
    gpio_put(D_PIN, (i >> 3) & 1);

    // Wysłanie danych pikseli do rejestru FM6124
    uint16_t pixels = pixel_buffer[i];
    for (int j = 0; j < 16; j++) {
      gpio_put(CLK_PIN, 0);
      gpio_put(R1_PIN, (pixels >> (15 - j)) & 1);
      gpio_put(G1_PIN, (pixels >> (31 - j)) & 1);
      gpio_put(B1_PIN, (pixels >> (47 - j)) & 1);
      gpio_put(R2_PIN, (pixels >> (63 - j)) & 1);
      gpio_put(G2_PIN, (pixels >> (79 - j)) & 1);
      gpio_put(B2_PIN, (pixels >> (95 - j)) & 1);
      gpio_put(CLK_PIN, 1);
    }
  }

  // Przesłanie danych z rejestru FM6124 do matrycy LED
  gpio_put(LAT_PIN, 0);
  gpio_put(LAT_PIN, 1);

  // Włączenie wyjścia matrycy LED
  gpio_put(OE_PIN, 0);
}

int main() {
  
  stdio_init_all();
  led_matrix_init();

  // Przykładowy kod do generowania obrazu na matrycy LED
  while (true) {
    // Ustawienie wartości pikseli w buforze ramki
    for (int x = 0; x < 64; x++) {
      for (int y = 0; y < 32; y++) {
        uint8_t r = x * 4;
        uint8_t g = y * 8;
        uint8_t b = (x + y) * 2;
        set_pixel_color(x, y, r, g, b);
      }
    }

    // Wysłanie bufora ramki do matrycy LED
    send_frame();
  }

  return 0;
}