/**************************************************************
 * main.c
 * rev 1.0 17-Aug-2023 hkrug
 * Assignment1
 * ***********************************************************/

#include "pico/stdlib.h"
#include <stdbool.h>
#include <stdio.h>

//DEFINE GPIO PINS
#define RED_LED 11
#define GREEN_LED 12
#define BLUE_LED 13
#define SW_1 2
#define SW_2 3
#define SW_3 4

int main(void)
{
  //ENABLE USE OF SDK
  stdio_init_all();

  //INITIALISE LED PINS
  gpio_init(RED_LED);
  gpio_set_dir(RED_LED, true); //recieving input

  gpio_init(GREEN_LED);
  gpio_set_dir(GREEN_LED, true); //recieving input

  gpio_init(BLUE_LED);
  gpio_set_dir(BLUE_LED, true); //recieving input

  //INITIALISE SWITCH PINS

  gpio_init(SW_1);
  gpio_set_dir(SW_1, false); //outputting signal

  gpio_init(SW_2);
  gpio_set_dir(SW_2, false); //outputting signal

  gpio_init(SW_3);
  gpio_set_dir(SW_3, false); //outputting signal


  while (true)
  { // Repeated code here within the while statement

    gpio_put(RED_LED, !gpio_get(SW_1));
    gpio_put(GREEN_LED, !gpio_get(SW_2));
    gpio_put(BLUE_LED, !gpio_get(SW_3));

    sleep_ms(1); // program buffer
  }
}
