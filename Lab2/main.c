/**************************************************************
 * main.c
 * rev 1.0 6-Apr-2021 bruce
 * Blinky for Pico - used to test connectivity
 * ***********************************************************/

#include "pico/stdlib.h"
#include <string.h>

int main(void)
{
    // Initialise the GPIO pin that controls the on-board LED 
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    // Loop for ever
    while (true) {
        gpio_put(LED_PIN, 1); // Switch on the LED 
        sleep_ms(500);        // Wait for a while
        gpio_put(LED_PIN, 0); // Switch off the LED 
        sleep_ms(500);        // Wait for a while
    }
}
