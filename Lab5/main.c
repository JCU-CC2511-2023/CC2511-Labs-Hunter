/**************************************************************
 * main.c
 * rev 1.0 21-Aug-2023 Hunter Kruger-Ilingworth
 * Lab5
 * ***********************************************************/
#include <stdio.h>
#include "pico/stdlib.h"
#include <stdbool.h>
#include "hardware/pwm.h"

// LED GPIO PIN DEFINITIONS
#define RED_LED 11
#define GREEN_LED 12
#define BLUE_LED 13

// PART 1 DEFINITIONS
#define FLOATING_PIN 15
#define CONTENT_OF(addr) (*(volatile uint32_t *)addr)
#define PAD_CONTROL_BASE 0x4001c000
#define REG_PAD_CONTROL_GPIO15 (PAD_CONTROL_BASE + 0x40)
#define CTRL_PDE 0x4 // Bit 2 REG_PAD_CONTROL_GPIO15 register pg300 rp2040 datasheet
#define CTRL_PUE 0x8 // Bit 3 REG_PAD_CONTROL_GPIO15 register pg300 rp2040 datasheet

int main(void)
{
  // ENABLE USE OF SDK
  stdio_init_all();

  // CREATE VARIABLES THAT WILL HOLD THE PWN LEVELS
  int red_level = 0;
  int green_level = 0;
  int blue_level = 0;

  // INITIALISE FLOATING PIN
  gpio_init(FLOATING_PIN);
  gpio_set_dir(FLOATING_PIN, false);                            // set direction to output signal
  CONTENT_OF(REG_PAD_CONTROL_GPIO15) &= ~(CTRL_PDE + CTRL_PUE); // resets CTRL_PUE and CTRL_PDE pins

  // INITIALISE LED PINS
  gpio_init(RED_LED);
  gpio_set_dir(RED_LED, true);               // set direction to recieve input
  gpio_set_function(RED_LED, GPIO_FUNC_PWM); // set function to PWM
  uint Rslice_num = pwm_gpio_to_slice_num(RED_LED);
  pwm_set_wrap(Rslice_num, 255);          // set the current PWM counter wrap value
  pwm_set_enabled(Rslice_num, true);      // enable PWM
  pwm_set_gpio_level(RED_LED, red_level); // assign PWM value

  gpio_init(GREEN_LED);
  gpio_set_dir(GREEN_LED, true);               // set direction to recieve input
  gpio_set_function(GREEN_LED, GPIO_FUNC_PWM); // set function to PWM
  uint Gslice_num = pwm_gpio_to_slice_num(GREEN_LED);
  pwm_set_wrap(Gslice_num, 255);              // Set the current PWM counter wrap value
  pwm_set_enabled(Gslice_num, true);          // enable PWM
  pwm_set_gpio_level(GREEN_LED, green_level); // assign PWM value

  gpio_init(BLUE_LED);
  gpio_set_dir(BLUE_LED, true); // set direction to recieve input

  while (true)
  {
    // use blue LED as a visual indicator of GPIO15 value
    gpio_put(BLUE_LED, gpio_get(FLOATING_PIN));

    /**************************************************************
     * Key for control of Red and Green LEDs:
     * lowercase -> decrease
     * uppercase -> increase
     *
     * Key for control of PUE and PDE Resistors
     * u -> pullup on, pulldown off
     * d -> pulldown on, pullup off
     * o -> both off
     * ***********************************************************/

    int ch = getchar_timeout_us(0);
    if (ch != PICO_ERROR_TIMEOUT) // code to detect character input
    {
      // PART 1 CODE
      if (ch == 'u')
      {
        // If you receive ’u’ turn the pullup on and the pulldown off.
        CONTENT_OF(REG_PAD_CONTROL_GPIO15) &= ~(CTRL_PDE + CTRL_PUE); // clear both
        CONTENT_OF(REG_PAD_CONTROL_GPIO15) |= CTRL_PUE;               // set PUE to 1
        printf("Pin %d to Pullup\r\n", FLOATING_PIN);
      }
      if (ch == 'd')
      {
        // If you receive ’d’ turn the pullup off and the pulldown on.
        CONTENT_OF(REG_PAD_CONTROL_GPIO15) &= ~(CTRL_PDE + CTRL_PUE); // clear both
        CONTENT_OF(REG_PAD_CONTROL_GPIO15) |= CTRL_PDE;               // set PDE to 1
        printf("Pin %d to Pulldown\r\n", FLOATING_PIN);
      }
      if (ch == 'o')
      {
        // If you receive ’o’ turn both pullup and pulldown off.
        CONTENT_OF(REG_PAD_CONTROL_GPIO15) &= ~(CTRL_PDE + CTRL_PUE); // clear both
        printf("Pin %d to Pull none\r\n", FLOATING_PIN);
      }
      if (ch == 'h') // using an else statment was buggy for some reason
      {
        // intruction print for incorrect keypresses (h for help)
        printf("Pin %d Options:\r\n u - Pullup \r\n d - Pulldown \r\n o - Pull none \r\n", FLOATING_PIN);
      }

      // PART 2 CODE
      if (ch == 'r')
      {
        if ((red_level - 17) >= 0)
        {
          red_level -= 17;
          printf("Red decrease to %d \r\n", red_level);
        }
      }
      if (ch == 'R')
      {
        if ((red_level + 17) <= 255)
        {
          red_level += 17;
          printf("Red increase to %d \r\n", red_level);
        }
      }
      if (ch == 'g')
      {
        if ((green_level - 17) >= 0)
        {
          green_level -= 17;
          printf("Green decrease to %d \r\n", green_level);
        }
      }
      if (ch == 'G')
      {
        if ((green_level + 17) <= 255)
        {
          green_level += 17;
          printf("Green increase to %d \r\n", green_level);
        }
      }
    }
    pwm_set_gpio_level(RED_LED, red_level);
    pwm_set_gpio_level(GREEN_LED, green_level);
    sleep_ms(100);
  }
}
