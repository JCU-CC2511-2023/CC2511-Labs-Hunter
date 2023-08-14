/**************************************************************
 * main.c
 * rev 1.0 14-Aug-2023 Hunter Kruger-Ilingworth
 * Lab4
 * objective: control onboard LED without using the SDK.
 * ***********************************************************/
#include <stdbool.h>

typedef unsigned long uint32_t;

//define constants

# define LED_PIN 25

//PAD CONTROL
//you must set Output disable to off (0) and Input enable to on (1) in main
//output disable, value = 0, bits = 7
//inupt enable, value = 1, bits = 6
# define PAD_BASE 0x4001c000
# define PADS_BANK0_GPIO25 ( SIO_BASE+0x68 )


//FUNCTION ALLOCATION - SIGNAL MULTIPLEXING
//GPIOS control reigster - function 5 "all the others can be safely set to 0"
# define IO_BANK0_BASE 0x40014000
# define GPIO25_CTRL (0x40014000+0x0cc)

//DATA DIRECTION
# define SIO_BASE 0xd0000000
# define REG_GPIO_OE_SET ( SIO_BASE+0x024 )

int main(void) {
  // 1. enable the pads
  


  // 2. select the GPIO function



  // 3. set the data direction.



  while (true) {
    //  code to turn the LED on and off, with a timed waiting period between each action.
  }
}
