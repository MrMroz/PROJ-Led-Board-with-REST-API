#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <stdio.h>
#include "pico/time.h"
#include "picture.h"


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

//wymiary matrycy
#define MATRIX_HEIGHT 32
#define MATRIX_WIDTH 64

static inline void led_matrix_init();
void set_row(uint8_t r);
static inline void set_pixel (uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b, int brightness);
void send_picture(const unsigned short *picture, uint8_t r, uint8_t g, uint8_t b, int brightness);

#endif
