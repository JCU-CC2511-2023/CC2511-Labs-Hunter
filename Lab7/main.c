/**************************************************************
 * main.c
 * rev 1.0 05-Sep-2023 hkrug
 * Lab7
 * extention topic to mimic the following graph:
 * https://www.desmos.com/calculator/mhtmuoljgf
 * ***********************************************************/
#include <stdio.h>
#include "pico/stdlib.h"
#include <stdbool.h>
#include "hardware/pwm.h"
#include <math.h>
#include "hardware/uart.h"
#include "terminal.h"

#define RED_LED 11
#define GREEN_LED 12
#define BLUE_LED 13

// UART
#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 0 // We are using pins 0 and 1, but see the GPIO function select table in the
#define UART_RX_PIN 1 // datasheet for information on which other pins can be used.
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY UART_PARITY_NONE

// define global variables
volatile char buffer[100]; // this has probably got to change
volatile char save[100]; // temporary variable which buffer will offload to
volatile unsigned int i = 0;
volatile bool input_ready = false;

// RX interrupt handler
static int chars_rxed = 0;

/*! \brief All in one function which enables LEDS and PWM*/
void aio_init_rgb_pwm()
{
  /**************************************************************
   * All in one function which enables LEDS and PWM
   * ***********************************************************/

  // LED PIN PWM INITIALISATION CODE
  gpio_init(RED_LED);
  gpio_set_dir(RED_LED, true);               // set direction to recieve input
  gpio_set_function(RED_LED, GPIO_FUNC_PWM); // set function to PWM
  uint Rslice_num = pwm_gpio_to_slice_num(RED_LED);
  pwm_set_enabled(Rslice_num, true);    // enable PWM
  pwm_set_gpio_level(RED_LED, 255 ^ 2); // assign initial PWM value

  gpio_init(GREEN_LED);
  gpio_set_dir(GREEN_LED, true);               // set direction to recieve input
  gpio_set_function(GREEN_LED, GPIO_FUNC_PWM); // set function to PWM
  uint Gslice_num = pwm_gpio_to_slice_num(GREEN_LED);
  pwm_set_enabled(Gslice_num, true);      // enable PWM
  pwm_set_gpio_level(GREEN_LED, 255 ^ 2); // assign initial PWM value

  gpio_init(BLUE_LED);
  gpio_set_dir(BLUE_LED, true);               // set direction to recieve input
  gpio_set_function(BLUE_LED, GPIO_FUNC_PWM); // set function to PWM
  uint Bslice_num = pwm_gpio_to_slice_num(BLUE_LED);
  pwm_set_enabled(Bslice_num, true);     // enable PWM
  pwm_set_gpio_level(BLUE_LED, 255 ^ 2); // assign initial PWM value
}

/*! \brief Handles the UART interrupt code*/
void on_uart_rx()
{
  while (uart_is_readable(UART_ID))
  {
    uint8_t ch = uart_getc(UART_ID);
    if (uart_is_writable(UART_ID))
    {
      switch (ch)
      {
      case 13: // enter
        //uart_puts(UART_ID, "\n enter recieved \n");
        buffer[i] = '\0'; // add a trailing null!
        input_ready = true;
        break;
      case 127: // backspace (CHECK THAT YOU DONT BACKSPACE WHILE INDEX IS 0)
        if (i != 0)
        {
          //uart_puts(UART_ID, "\n backspace recieved \n");
          i--; // decrement index (clearing shouldnt be nessesary)
          buffer[i] = '\0';
        }
        else {
          //uart_puts(UART_ID, "\n illegal backspace \n");
        }
        break;
      default: // normal character recieved

        uart_putc(UART_ID, ch); // outputs uart to terminal
        //uart_puts(UART_ID, "\normal character recived\n");
        buffer[i] = ch; // save the character
        i++;            // increment the index
        break;

      }

      // buffer overflow correction logic
      // TODO echo the char back to the user
      // TODO handle the enter key
    }
    chars_rxed++; // characters read variable, kinda useless
  }
}

/*! \brief All in one function which enables UART*/
void aio_init_uart()
{
  /**************************************************************
   * All in one function which enables UART and sets up interrupt
   * ***********************************************************/

  // Set up our UART with a basic baud rate.
  uart_init(UART_ID, 2400);

  // Set the TX and RX pins by using the function select on the GPIO
  // Set datasheet for more information on function select
  gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
  gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

  // Actually, we want a different speed
  // The call will return the actual baud rate selected, which will be as close as
  // possible to that requested
  int __unused actual = uart_set_baudrate(UART_ID, BAUD_RATE);

  // Set UART flow control CTS/RTS, we don't want these, so turn them off
  uart_set_hw_flow(UART_ID, false, false);

  // Set our data format
  uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

  // Turn off FIFO's - we want to do this character by character
  uart_set_fifo_enabled(UART_ID, false);

  // Set up a RX interrupt
  // We need to set up the handler first
  // Select correct interrupt for the UART we are using
  int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;

  // And set up and enable the interrupt handlers
  irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
  irq_set_enabled(UART_IRQ, true);

  // Now enable the UART to send interrupts - RX only
  uart_set_irq_enables(UART_ID, true, false);

  // OK, all set up.
  // Lets send a basic string out, and then run a loop and wait for RX interrupts
  // The handler will count them, but also reflect the incoming data back with a slight change!
  uart_puts(UART_ID, "\nHello, uart interrupts\n");
}

int main(void)
{
  stdio_init_all();
  aio_init_uart();
  aio_init_rgb_pwm();

  while (1)
  {
    tight_loop_contents();
    if (input_ready == true)
    {
      // process inputs
      uart_puts(UART_ID, "\n BUFFER RECIEVED \n");
      uint16_t val;
      if (sscanf(buffer, "red %hu", &val) > 0) {
          // Command was red
          uart_puts(UART_ID, "\n red \n");
          pwm_set_gpio_level(RED_LED, val * val);
      } else if (sscanf(buffer, "green %hu", &val) > 0) {
          // Command was green
          uart_puts(UART_ID, "\n greeb \n");
          pwm_set_gpio_level(GREEN_LED, val * val);
      } else if (sscanf(buffer, "blue %hu", &val) > 0) {
          // Command was blue
          uart_puts(UART_ID, "\n blue \n");
          pwm_set_gpio_level(BLUE_LED, val * val);
      }


      uart_puts(UART_ID, "\n mepowoof \n");
      //clear buffer
      for (size_t x = 0; x < i; x++) 
      {
        buffer[x] = '\0';
      }
      i = 0;
      input_ready = false;

      // after processing buffer, clear it and set it to staring conditions.
    }

}

      
    
  
}
