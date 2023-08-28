/**************************************************************
 * main.c
 * rev 1.0 28-Aug-2023 Hunter Kruger-Ilingworth
 * Lab5sinewave
 * extention topic to mimic the following graph:
 * https://www.desmos.com/calculator/mhtmuoljgf 
 * ***********************************************************/
#include <stdio.h>
#include "pico/stdlib.h"
#include <stdbool.h>
#include "hardware/pwm.h"
#include <math.h>

#define RED_LED 11
#define GREEN_LED 12
#define BLUE_LED 13

int main(void)
{
  // ENABLE USE OF SDK
  stdio_init_all();

  // INITIALISE LED PINS
  gpio_init(RED_LED);
  gpio_set_dir(RED_LED, true);               // set direction to recieve input
  gpio_set_function(RED_LED, GPIO_FUNC_PWM); // set function to PWM
  uint Rslice_num = pwm_gpio_to_slice_num(RED_LED);
  pwm_set_wrap(Rslice_num, 255);          // set the current PWM counter wrap value
  pwm_set_enabled(Rslice_num, true);      // enable PWM
  pwm_set_gpio_level(RED_LED, 255); // assign initial PWM value

  gpio_init(GREEN_LED);
  gpio_set_dir(GREEN_LED, true);               // set direction to recieve input
  gpio_set_function(GREEN_LED, GPIO_FUNC_PWM); // set function to PWM
  uint Gslice_num = pwm_gpio_to_slice_num(GREEN_LED);
  pwm_set_wrap(Gslice_num, 255);              // Set the current PWM counter wrap value
  pwm_set_enabled(Gslice_num, true);          // enable PWM
  pwm_set_gpio_level(GREEN_LED, 255); // assign initial PWM value

  gpio_init(BLUE_LED);
  gpio_set_dir(BLUE_LED, true);               // set direction to recieve input
  gpio_set_function(BLUE_LED, GPIO_FUNC_PWM); // set function to PWM
  uint Bslice_num = pwm_gpio_to_slice_num(BLUE_LED);
  pwm_set_wrap(Bslice_num, 255);              // Set the current PWM counter wrap value
  pwm_set_enabled(Bslice_num, true);          // enable PWM
  pwm_set_gpio_level(GREEN_LED, 255); // assign initial PWM value

  //create variables for graph function
  double PI = 3.1416;
  double Amp = PI/255;
  double R_phase = 0;
  double G_phase = PI/3;
  double B_phase = (2*PI)/3;

  while (true)
  {
    // Adjust the duty cycle of the green LED to create blinking effect
    for (int duty_cycle = 0; duty_cycle <= 255; duty_cycle += 1)
    {
      double red_value = pow(sin(Amp * duty_cycle + R_phase), 2) * 255.0;
      double green_value = pow(sin(Amp * duty_cycle + G_phase), 2) * 255.0;
      double blue_value = pow(sin(Amp * duty_cycle + B_phase), 2) * 255.0;

      pwm_set_chan_level(Rslice_num, PWM_CHAN_A, (int)red_value);
      pwm_set_chan_level(Gslice_num, PWM_CHAN_A, (int)green_value);
      pwm_set_chan_level(Bslice_num, PWM_CHAN_A, (int)blue_value);

      sleep_ms(20); // Delay to observe changes
    }   
  }
}
