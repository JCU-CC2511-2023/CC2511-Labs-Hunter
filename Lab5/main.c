/**************************************************************
 * main.c
 * rev 1.0 21-Aug-2023 Hunter Kruger-Ilingworth
 * Lab5
 * ***********************************************************/
#include <stdio.h>
#include "pico/stdlib.h"
#include <stdbool.h>
#include "hardware/pwm.h"

#define FLOATING_PIN 15
#define BLUE_LED 13

#define CONTENT_OF(addr) (*(volatile uint32_t *)addr)
#define PAD_CONTROL_BASE 0x4001c000
#define REG_PAD_CONTROL_GPIO15 (PAD_CONTROL_BASE + 0x40)
#define CTRL_PDE 0x4 // Bit 2 REG_PAD_CONTROL_GPIO15 register pg300 rp2040 datasheet
#define CTRL_PUE 0x8 // Bit 3 REG_PAD_CONTROL_GPIO15 register pg300 rp2040 datasheet

int main(void)
{
  // ENABLE USE OF SDK
  stdio_init_all();

  // INITIALISE BLUE_LED
  gpio_init(BLUE_LED);
  gpio_set_dir(BLUE_LED, true); // recieving input
  // INITIALISE FLOATING PIN
  gpio_init(FLOATING_PIN);
  gpio_set_dir(FLOATING_PIN, false); // outputting signal

  // add code to clear the PUE and PDE registers
  CONTENT_OF(REG_PAD_CONTROL_GPIO15) &= ~(CTRL_PDE + CTRL_PUE); // resets CTRL_PUE and CTRL_PDE pins
  while (true)
  {
    // use blue led to display the status of the chosen pin
    gpio_put(BLUE_LED, gpio_get(FLOATING_PIN));

    int ch = getchar_timeout_us(0);
    if (ch != PICO_ERROR_TIMEOUT) // code to detect character input
    {
      // Handle the various interesting values of ch here...
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
      if (ch == 'h') //using an else statment was buggy for some reason
      {
        //intruction print for incorrect keypresses (h for help)
        printf("Pin %d Options:\r\n u - Pullup \r\n d - Pulldown \r\n o - Pull none \r\n", FLOATING_PIN);
      }
    }
    sleep_ms(20); //buffer for CPU
  }
}
