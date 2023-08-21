/**************************************************************
 * main.c
 * rev 1.0 21-Aug-2023 Hunter Kruger-Ilingworth
 * Lab5
 * ***********************************************************/

#include "pico/stdlib.h"
#include <stdbool.h>
#include "hardware/pwm.h"

#define FLOATING_PIN 15

#define CONTENT_OF(addr) (*(volatile uint32_t*)addr)
#define PAD_CONTROL_BASE 0x4001c000
#define REG_PAD_CONTROL_GPIO15 (PAD_CONTROL_BASE+0x40) 
#define CTRL_PDE 0x4//is this in use?
#define CTRL_PUE 0x8//is this in use?

#define PUE 3 //within the REG_PAD_CONTROL_GPIO15 register
#define PDE 2 //within the REG_PAD_CONTROL_GPIO15 register




int main(void) {
  // TODO - Initialise components and variables
  while (true) {
    // TODO - Repeated code here
  }
}
