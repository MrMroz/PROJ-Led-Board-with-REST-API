#pragma once

#include "pico/cyw43_arch.h"
#include "pico/multicore.h"
#include "pico/sync.h"
#include "pico/util/queue.h"

#include "matrix.h"
#include "handlers.h"

struct led_request {
    // 1 - text, 2 - spectrum
    uint8_t type;
    char text[MAX_TEXT_LEN];
    uint8_t speed;
};

extern queue_t queue;

void multicore_init();
void core1_led_controller();