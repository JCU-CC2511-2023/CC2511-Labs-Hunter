/**************************************************************
 * main.c
 * rev 1.0 06-Nov-2023 hkrug
 * Examination
 * ***********************************************************/

#include "pico/stdlib.h"
#include <stdbool.h>
#include <stdlib.h>
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"

// ONBOARD PIN MACRO DEFINITIONS
#define LDR_PIN 26
#define RED_LED 11
#define GREEN_LED 12
#define BLUE_LED 13
#define SW_1 2
#define SW_2 3
#define SW_3 4

// TASK RELATED MACRO DEFINITIONS

// ONBOARD BUTTONS SETUP
void init_buttons()
{
  gpio_init(SW_1);
  gpio_set_dir(SW_1, false); //outputting signal

  gpio_init(SW_2);
  gpio_set_dir(SW_2, false); //outputting signal

  gpio_init(SW_3);
  gpio_set_dir(SW_3, false); //outputting signal
}

// RGB LED GPIO SETUP
/*! \brief All in one function which enables LEDS (not PWM!)
 * \param None
 */
void init_rgb_led()
{
  gpio_init(RED_LED);
  gpio_set_dir(RED_LED, true); // recieving input

  gpio_init(GREEN_LED);
  gpio_set_dir(GREEN_LED, true); // recieving input

  gpio_init(BLUE_LED);
  gpio_set_dir(BLUE_LED, true); // recieving input
}

// PWM RGB LED GPIO SETUP
int red_level = 0;
int green_level = 0;
int blue_level = 0;
/*! \brief All in one function which enables LEDS and PWM
 * \param None
 */
void init_rgb_led_pwm()
{
  // LED PIN PWM INITIALISATION CODE
  gpio_init(RED_LED);
  gpio_set_dir(RED_LED, true);               // set direction to recieve input
  gpio_set_function(RED_LED, GPIO_FUNC_PWM); // set function to PWM
  uint Rslice_num = pwm_gpio_to_slice_num(RED_LED);
  pwm_set_enabled(Rslice_num, true);      // enable PWM
  pwm_set_gpio_level(RED_LED, red_level); // assign initial PWM value

  gpio_init(GREEN_LED);
  gpio_set_dir(GREEN_LED, true);               // set direction to recieve input
  gpio_set_function(GREEN_LED, GPIO_FUNC_PWM); // set function to PWM
  uint Gslice_num = pwm_gpio_to_slice_num(GREEN_LED);
  pwm_set_enabled(Gslice_num, true);          // enable PWM
  pwm_set_gpio_level(GREEN_LED, green_level); // assign initial PWM value

  gpio_init(BLUE_LED);
  gpio_set_dir(BLUE_LED, true);               // set direction to recieve input
  gpio_set_function(BLUE_LED, GPIO_FUNC_PWM); // set function to PWM
  uint Bslice_num = pwm_gpio_to_slice_num(BLUE_LED);
  pwm_set_enabled(Bslice_num, true);        // enable PWM
  pwm_set_gpio_level(BLUE_LED, blue_level); // assign initial PWM value
}

// ADC SETUP
/*! \brief All in one function which enables ADC
 * \param None
 */
void init_adc()
{
  adc_init();
  adc_gpio_init(LDR_PIN);
  adc_select_input(0);
}
float adc_constant = 3.3 / ((2 << 12) - 1);

int main(void)
{
  stdio_init_all();
  init_buttons();
  init_rgb_led();
  //init_rgb_led_pwm(); only one can be chosen
  init_adc();
  printf("\nCC2511 Exam 2022");
  printf("\nHunter Kruger-Ilingworth");
  while (true)
  {
    gpio_put(RED_LED, !gpio_get(SW_1));
    gpio_put(GREEN_LED, !gpio_get(SW_2));
    gpio_put(BLUE_LED, !gpio_get(SW_3));
    sleep_ms(1); // buffer time
  }
}
