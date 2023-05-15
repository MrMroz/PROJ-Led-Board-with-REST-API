#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <stdio.h>
#include "pico/time.h"
#include "picture.h"


// Ustawienia pinów GPIO do połączenia z interfejsem HUB75
#define R1_PIN 8
#define G1_PIN 0
#define B1_PIN 1
#define R2_PIN 9
#define G2_PIN 2
#define B2_PIN 4
#define A_PIN 10
#define B_PIN 3
#define C_PIN 11
#define D_PIN 5
#define CLK_PIN 12
#define LAT_PIN 6
#define OE_PIN 7

//wymiary matrycy
#define MATRIX_HEIGHT 32
#define MATRIX_WIDTH 64

static inline void led_matrix_init();
static inline void clock();
void set_row(uint8_t r);
static inline void set_pixel (uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b, int brightness);
void send_picture(const unsigned short *picture, uint8_t r, uint8_t g, uint8_t b, int brightness);

#endif