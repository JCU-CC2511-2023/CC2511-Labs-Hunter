/**************************************************************
 * main.c
 * rev 1.0 14-Aug-2023 Hunter Kruger-Ilingworth
 * Lab4
 * objective: control onboard LED without using the SDK.
 * ***********************************************************/
#include <stdbool.h>

//CONSTANT AND DATA TYPE DEFINITION
typedef unsigned long uint32_t;
# define CONTENT_OF( addr ) (* ( volatile uint32_t *) addr )
# define LED_PIN 25

//PAD CONTROL REGISTER DEFINITION
# define PAD_BASE 0x4001c000
# define PADS_BANK0_GPIO25 ( SIO_BASE+0x68 )

//FUNCTION ALLOCATION REGISTER DEFINITION
# define IO_BANK0_BASE 0x40014000
# define GPIO25_CTRL (0x40014000+0x0cc)

//DATA DIRECTION REGISTER DEFINITION
# define SIO_BASE 0xd0000000
# define REG_GPIO_OE_SET ( SIO_BASE+0x024 )

//LED TOGGLE REGISTER DEFINITION
# define GPIO_OUT_SET ( SIO_BASE+0x014 )
# define GPIO_OUT_CLR ( SIO_BASE+0x018 )

int main(void) {

  // ENABLE PADS
  CONTENT_OF(PADS_BANK0_GPIO25) = CONTENT_OF(PADS_BANK0_GPIO25) & ~(0x1 << 7); 
  //output disable, value = 0, bit = 7
  //moves the first bit seven steps and set it to zero
  CONTENT_OF(PADS_BANK0_GPIO25) = CONTENT_OF(PADS_BANK0_GPIO25) | (0x1 << 6); 
  //inupt enable, value = 1, bit = 6
  //alternatively, CONTENT_OF(PADS_BANK0_GPIO25) = CONTENT_OF(PADS_BANK0_GPIO25) & (0x01 << 6); assuming the rest are 0

  // SELECT GPIO FUNCTION
  CONTENT_OF(GPIO25_CTRL) = (CONTENT_OF(GPIO25_CTRL) & ~(0x1f) | (0x5)); 
  //clears the first 4 bits, and sets them to function 5

  // SET DATA DIRECTION
  CONTENT_OF(REG_GPIO_OE_SET) = CONTENT_OF(REG_GPIO_OE_SET) | (0x1<<LED_PIN); 
  //sets the 25th bit (LED_PIN) to 1

  while (true) {
    
    CONTENT_OF(GPIO_OUT_SET) = CONTENT_OF(GPIO_OUT_SET) | (0x1 << LED_PIN); 

    for(uint32_t i = 0 ; i < 8000000; i ++) {
    __asm volatile( " nop " );} //wait

    CONTENT_OF(GPIO_OUT_CLR) = CONTENT_OF(GPIO_OUT_SET) | (0x1 << LED_PIN);

    for(uint32_t i = 0 ; i < 8000000; i ++) {
    __asm volatile( " nop " );} //wait

  }
  
}
