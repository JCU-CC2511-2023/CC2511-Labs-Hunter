/**************************************************************
 * main.c
 * rev 1.0 20-Sep-2023 hkrug
 * Lab9
 * ***********************************************************/

#include "pico/stdlib.h"
#include <stdbool.h>
#include <stdio.h>
#include "hardware/pwm.h"
#include "hardware/uart.h"

#define RED_LED 11
#define GREEN_LED 12
#define BLUE_LED 13

bool light_is_on = 0;
volatile bool val = 0;


/*! \brief all in one function which enables LEDS*/
void rgb_init_aio()
{
  //INITIALISE LED PINS
  gpio_init(RED_LED);
  gpio_set_dir(RED_LED, true); //recieving input

  gpio_init(GREEN_LED);
  gpio_set_dir(GREEN_LED, true); //recieving input

  gpio_init(BLUE_LED);
  gpio_set_dir(BLUE_LED, true); //recieving input
}

void update_LED(val){
  light_is_on = val;
  gpio_put(RED_LED, val);
  gpio_put(GREEN_LED, val);
  gpio_put(BLUE_LED, val);

  sleep_ms(1); // buffer time
}

int main(void) {
  stdio_init_all();
  rgb_init_aio();
  
  while (true) {
    // TODO - Repeated code here
    update_LED(false);
    sleep_ms(51);
    update_LED(true);
    sleep_ms(51);
    u_int8_t message = "hello";
    printf("Message: %u", message);
  }
}
