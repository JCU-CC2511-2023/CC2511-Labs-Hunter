/**************************************************************
 * main.c
 * rev 1.0 10-Aug-2023 Hunter Kruger-Ilingworth
 * Lab3
 * ***********************************************************/
#include "pico/stdlib.h"
#include <stdbool.h>
#include <stdio.h>

int main(void) {
  // TODO - Initialise components and variables
  stdio_init_all(); //Bruce put this here, so im fairly sure its useful

  #define RED_LED 11
  gpio_init(RED_LED);
  gpio_set_dir(RED_LED, true);

  #define GREEN_LED 12
  gpio_init(GREEN_LED);
  gpio_set_dir(GREEN_LED, true);

  #define BLUE_LED 13
  gpio_init(BLUE_LED);
  gpio_set_dir(BLUE_LED, true);

  while (true) {// Repeated code here within the while statement
    
    int ch = getchar_timeout_us(0);//respond to user input by setting the GPIO pin high or low
    if (ch != PICO_ERROR_TIMEOUT) {
    // Handle the various interesting values of ch here...
      if (ch == 'r'){
        gpio_put(RED_LED, !gpio_get(RED_LED));//inverses current value of the GPIO pin
        printf("RED TOGGLED\r\n");
        }
      if (ch == 'g'){
        gpio_put(GREEN_LED, !gpio_get(GREEN_LED));//inverses current value of the GPIO pin
        printf("GREEN TOGGLED\r\n");
        }
      if (ch == 'b'){
        gpio_put(BLUE_LED, !gpio_get(BLUE_LED));//inverses current value of the GPIO pin
        printf("BLUE TOGGLED\r\n");
        }  
    }
    sleep_ms(20);//wait for another key press
  }
}
