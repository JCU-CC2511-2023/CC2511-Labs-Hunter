/**************************************************************
 * main.c
 * rev 1.0 21-Aug-2023 Hunter Kruger-Ilingworth
 * Lab5
 * ***********************************************************/

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
    gpio_put(BLUE_LED, !gpio_get(FLOATING_PIN));
  }
}
