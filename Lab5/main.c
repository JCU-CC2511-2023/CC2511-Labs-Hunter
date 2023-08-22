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

  // INITIALISE LED PINS
  gpio_init(RED_LED);
  gpio_set_dir(RED_LED, true);               // recieving input
  gpio_set_function(RED_LED, GPIO_FUNC_PWM); // set function to PWM
  uint Rslice_num = pwm_gpio_to_slice_num(RED_LED);
  pwm_set_wrap(Rslice_num, 255); // may have to adjust
  pwm_set_chan_level(Rslice_num, PWM_CHAN_A, 128);
  pwm_set_enabled(Rslice_num, true);
  //pwm_set_gpio_level(RED_LED, 255);

  gpio_init(GREEN_LED);
  gpio_set_dir(GREEN_LED, true);               // recieving input
  gpio_set_function(GREEN_LED, GPIO_FUNC_PWM); // set function to PWM
  uint Gslice_num = pwm_gpio_to_slice_num(GREEN_LED);
  pwm_set_wrap(Gslice_num, 255); // may have to adjust
  pwm_set_chan_level(Gslice_num, PWM_CHAN_A, 128);
  pwm_set_enabled(Gslice_num, true);
  pwm_set_gpio_level(GREEN_LED, 255);

  gpio_init(BLUE_LED);
  gpio_set_dir(BLUE_LED, true);               // recieving input
  gpio_set_function(BLUE_LED, GPIO_FUNC_PWM); // set function to PWM
  uint Bslice_num = pwm_gpio_to_slice_num(BLUE_LED);
  pwm_set_wrap(Bslice_num, 255); // may have to adjust
  pwm_set_chan_level(Bslice_num, PWM_CHAN_A, 128);
  pwm_set_enabled(Bslice_num, true);
  pwm_set_gpio_level(BLUE_LED, 255);

  while (true)
  {
    // Adjust the duty cycle of the green LED to create blinking effect
    for (int duty_cycle = 0; duty_cycle <= 255; duty_cycle += 1)
    {
        pwm_set_gpio_level(RED_LED, duty_cycle);
        pwm_set_gpio_level(GREEN_LED, duty_cycle);
        pwm_set_gpio_level(BLUE_LED, duty_cycle);
        sleep_ms(20); // Delay to observe changes
    }   
  }
}
