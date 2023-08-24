/**************************************************************
 * main.c
 * rev 1.0 21-Aug-2023 Hunter Kruger-Ilingworth
 * Lab5
 * ***********************************************************/
#include <stdio.h>
#include "pico/stdlib.h"
#include <stdbool.h>
#include "hardware/pwm.h"

#define RED_LED 11
#define GREEN_LED 12
#define BLUE_LED 13

int main(void)
{
  // ENABLE USE OF SDK
  stdio_init_all();

  //CREATE VARIABLES THAT WILL HOLD THE PWN LEVELS
  int red_level = 0;
  int green_level = 0;
  int blue_level = 0;

  // INITIALISE LED PINS
  gpio_init(RED_LED);
  gpio_set_dir(RED_LED, true); // set direction to recieve input
  gpio_set_function(RED_LED, GPIO_FUNC_PWM); // set function to PWM
  uint Rslice_num = pwm_gpio_to_slice_num(RED_LED);
  pwm_set_wrap(Rslice_num, 255); // set the current PWM counter wrap value
  pwm_set_enabled(Rslice_num, true); // enable PWM
  pwm_set_gpio_level(RED_LED, red_level); //assign PWM value

  gpio_init(GREEN_LED); 
  gpio_set_dir(GREEN_LED, true);// set direction to recieve input
  gpio_set_function(GREEN_LED, GPIO_FUNC_PWM); // set function to PWM
  uint Gslice_num = pwm_gpio_to_slice_num(GREEN_LED);
  pwm_set_wrap(Gslice_num, 255); // Set the current PWM counter wrap value
  pwm_set_enabled(Gslice_num, true); //enable PWM
  pwm_set_gpio_level(GREEN_LED, green_level); //assign PWM value

  gpio_init(BLUE_LED);
  gpio_set_dir(BLUE_LED, true); // set direction to recieve input
  gpio_set_function(BLUE_LED, GPIO_FUNC_PWM); // set function to PWM
  uint Bslice_num = pwm_gpio_to_slice_num(BLUE_LED);
  pwm_set_wrap(Bslice_num, 255); // set the current PWM counter wrap value
  pwm_set_enabled(Bslice_num, true);//enable PWM
  pwm_set_gpio_level(BLUE_LED, blue_level);//assign PWM value

  while (true)
  {

    int ch = getchar_timeout_us(0);
    if (ch != PICO_ERROR_TIMEOUT) // code to detect character input

    /**************************************************************
     * Key:
     * lowercase -> decrease
     * uppercase -> increase
     * ***********************************************************/
    {
      if (ch == 'r') 
      {
        if ((red_level - 51) >= 0)
        {
          red_level -= 51;
          printf("Red decrease to %d \r\n", red_level);
        }
      }
      if (ch == 'R')
      {
        if ((red_level + 51) <= 255)
        {
          red_level += 51;
          printf("Red increase to %d \r\n", red_level);
        }
      }
      if (ch == 'g')
      {
        if ((green_level - 51) >= 0)
        {
          green_level -= 51;
          printf("Green decrease to %d \r\n", green_level);
        }
      }
      if (ch == 'G')
      {
        if ((green_level + 51) <= 255)
        {
          green_level += 51;
          printf("Green increase to %d \r\n", green_level);
        }
      }
      if (ch == 'b')
      {
        if ((blue_level - 51) >= 0)
        {
          blue_level -= 51;
          printf("Blue decrease to %d \r\n", blue_level);
        }
      }
      if (ch == 'B')
      {
        if ((blue_level + 51) <= 255)
        {
          blue_level += 51;
          printf("Blue increase to %d \r\n", blue_level);
        }
      }
    }
    pwm_set_gpio_level(RED_LED, red_level);
    pwm_set_gpio_level(GREEN_LED, green_level);
    pwm_set_gpio_level(BLUE_LED, blue_level);
    sleep_ms(100);
  }
}
