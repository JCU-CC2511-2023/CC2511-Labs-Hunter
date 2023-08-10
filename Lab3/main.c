/**************************************************************
 * main.c
 * rev 1.0 10-Aug-2023 hkrug
 * Lab3
 * ***********************************************************/

#include "pico/stdlib.h"
#include <stdbool.h>
#include <stdio.h>


int main(void) {
  // TODO - Initialise components and variables
  stdio_init_all(); //Bruce put this here, so im fairly sure its useful
  printf("Hello, World!\r\n"); //test print
  //*#define RED_LED 11 // these are hopefully the correct numbers
  //*#define GREEN_LED 12
  //*#define BLUE_LED 13

  //*gpio_init(RED_LED);
  //*gpio_init(GREEN_LED);
  //*gpio_init(BLUE_LED);

  while (true) {
    // TODO - Repeated code here

    //respond to user input by setting the GPIO pin high or low
    //*gpio_set_dir(RED_LED, true);
    //*gpio_set_dir(BLUE_LED, true);
    //*gpio_set_dir(RED_LED, true);
    printf("Some text\r\n");
  }
}
