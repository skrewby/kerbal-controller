#include "hal.h"
#include "simpit.h"
#include "simpit_message_types.h"
#include <stdint.h>

int main() {
    stm32_init();
    serial_init(115200);

    led_on();
    while (!simpit_init()) {
        delay(100);
    }
    led_off();
    simpit_print("Controller Connected", PRINT_TO_SCREEN);

    while (1) {
        if (button_pressed()) {
            simpit_print("Staging", PRINT_TO_SCREEN);
            simpit_activate_action(STAGE_ACTION);
        }
    }
}
