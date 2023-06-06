#include "controller.h"
#include "matrix.h"

queue_t queue;

void multicore_init(){
    printf("--- Multicore init ---\n");

    printf("Multicore queue init\n");
	queue_init(&queue, sizeof(struct led_request), 100);

    printf("Launching led controller on core1\n");
	sleep_ms(5);
	multicore_reset_core1();
	sleep_ms(5);
	multicore_launch_core1(core1_led_controller);
    
    printf("--- Multicore init successful---\n");
}

void core1_led_controller() {
    bool did_receive = false;
    struct led_request payload;
    for(;;) {
        if(!queue_is_empty(&queue)) {
            did_receive = queue_try_remove(&queue, &payload);

            if(!did_receive) {
                sleep_ms(1);
                continue;
            }

            switch(payload.type) {
                case 1:
                    scrolling_text(payload.text, payload.speed);
                    break;
                case 2:
                    spectrum_anim(500, payload.speed);
                    break;
                default:
                    printf("Core1 :: Invalid request type\n");
            }

        } else {
            sleep_ms(1);
        }
    }
}