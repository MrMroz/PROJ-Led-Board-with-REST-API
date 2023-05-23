#ifndef MATRIX_H
#define MATRIX_H

#include "pico/time.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <limits.h>

#include <stdio.h>
#include <stdint.h>



#include "symbols.h"


#define uint8_t unsigned char
#define uint16_t unsigned short

#define UINT8_MAX UCHAR_MAX
#define UINT16_MAX USHRT_MAX

// Ustawienia pinów GPIO do połączenia z interfejsem HUB75
#define R1_PIN 22 
#define G1_PIN 15
#define B1_PIN 18
#define R2_PIN 26
#define G2_PIN 14
#define B2_PIN 19
#define A_PIN 16
#define B_PIN 13
#define C_PIN 17
#define D_PIN 12
#define CLK_PIN 21
#define LAT_PIN 8
#define OE_PIN 20

// Wymiary matrycy
#define MATRIX_HEIGHT 32
#define MATRIX_WIDTH 64


// Zmienna globalna - reprezentacja aktualnego stanu matrycy
uint8_t mx[MATRIX_HEIGHT][MATRIX_WIDTH];

// Inicjalizacja wszystkich uzywanych pinow GPIO
// Według definicji powyzej
static inline void matrix_gpio_init();

// Ustawia input na konkretny wiersz
// r - z zakresu (0-15)
inline static void set_row(uint8_t r);

// Aktualizacja stanu 2x64 pixeli na wierszach row_num oraz row_num+16
// row_num - z zakresu (0-15)
// threshold - z zakresu (0-6) - okresla jaka wartosc musza przewyzszyc
//  kolory pixeli w kodzie (rrrgggbb) aby byc zaswietlone, przejscie od 0 do 6 daje wyswietlenie pelnego koloru 8bit
inline static void redraw_128(uint8_t row_num, uint8_t threshold);

// Uzycie redraw_128 w odpowiednich iteracjach aby zaktualizowac cala matryce o stan wskazany w tablicy mx
inline static void redraw_full();

// Przesuwa podany wiersz o jeden element
// rev -> true - w prawo; false - w lewo
// wrap -> czy ma "zawijac" krancowe elementy, ktore normalnie byly by utracone
inline static void row_shift(uint8_t *arr, int n, bool rev, bool wrap);

// Pelne przesuniecie o jedna pozycje w kierunku poziomym, jak w row_shift ale dla calej tablicy mx
// Uzywane do animacji i przesuwania obrazu
inline static void full_shift(bool rev, bool wrap);

// Shorthand do ustawienia calej tablicy na jeden wskazany kolor
inline static void swap_mx_color(uint8_t c);

// Shorthand do wydobycia pojedynczej skladowej rgb (0-7) z wartosci koloru w kodzie (rrrgggbb)
// rgb -> 0 - r, 1 - g, 2 - b
inline static uint8_t cex(uint8_t color, uint8_t rgb);

// Laduje wskazana kolumne obrazu w tablicy na skrajna prawa kolumne tablicy mx
inline static void load_right(uint8_t img[32][19], uint8_t col);



// Zwraca nastepny kolor w plynnej animacji spektrum kolorow wg schematu
// uint8  (r r r g g g b b)
// 7, 0, 0 -> state 1
// 7, 7, 0 -> state 2
// 0, 7, 0 -> state 3
// 0, 7, 3 -> state 4
// 0, 0, 3 -> state 5
// 7, 0, 3 -> state 6
// 7, 0, 0 -> state 1
inline static uint8_t spectrum_cycle(uint8_t c);

// Generuje kolejna klatke animacji spektrum kolorow w poziomie dla tablicy mx
// Nie nadaje sie do skomplikowanych obrazow
inline static vertical_spectrum();

// Kompletna animacja kolorystyczna, blokujaca
// loops - ilosc petli, okresla dlugosc animacji
// speed - predkosc animacji, rekomendowane 10
void spectrum_anim(uint16_t loops, uint8_t speed);

// Funkcja pomocnicza dla scrolling_text(), zawiera instrukcje case wybierajaca odpowiednia tablice symbolu
inline static void load_right_char(char *text, uint8_t *colcnt, short *steps_to_end)

// Funkcja wyswietlajaca dowolny tekst A-Z, 0-9 lub spacje
// W teorii maksymalna dlugosc tekstu to 1721, ograniczona przez short steps_to_end
void scrolling_text(char *text, uint8_t speed)

#endif