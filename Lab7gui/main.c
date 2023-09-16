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
#include <string.h>

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
volatile char save[100];   // temporary variable which buffer will offload to
volatile unsigned int i = 0;
volatile bool input_ready = false;

// put this in the LED AIO INIT code
int red_level = 0;
int green_level = 0;
int blue_level = 0;

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
        // uart_puts(UART_ID, "\n enter recieved \n");
        buffer[i] = '\0'; // add a trailing null!
        input_ready = true;
        break;
      case 127: // backspace (CHECK THAT YOU DONT BACKSPACE WHILE INDEX IS 0)
        if (i != 0)
        {
          // uart_puts(UART_ID, "\n backspace recieved \n");
          i--; // decrement index (clearing shouldnt be nessesary)
          buffer[i] = '\0';
          uart_putc(UART_ID, '\177');
        }
        break;
      default: // normal character recieved

        buffer[i] = ch; // save the character
        i++;            // increment the index
        uart_putc(UART_ID, ch);
        break;
      }

      // buffer overflow correction logic
      // TODO echo the char back to the user

      term_erase_line();
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
  // uart_puts(UART_ID, "\nHello, uart interrupts\n");
}

void gui_update()
{
  // UI CODE
  term_cls();
  char colour_title_string[100];
  char red_status_string[100];
  char green_status_string[100];
  char blue_status_string[100];

  char tutorial_title_string[100];
  char red_info_string[100];
  char green_info_string[100];
  char blue_info_string[100];
  sprintf(colour_title_string, "+------[COLOURS]------+");
  sprintf(red_status_string, "Red: %hu ", red_level);
  sprintf(green_status_string, "Green: %hu", green_level);
  sprintf(blue_status_string, "Blue: %hu", blue_level);

  sprintf(tutorial_title_string, "+------[HOW TO USE]------+");
  sprintf(red_info_string, ">> type: red n");
  sprintf(green_info_string, ">> type: green n");
  sprintf(blue_info_string, ">> type: blue n");

  //width is equal to the width of each boxes header
  int width_box1 = strlen(colour_title_string); 
  int width_box2 = strlen(tutorial_title_string);

  // Height of both rectangles are equal
  int height = 7;                  

  // Clear the terminal
  term_cls();
  term_move_to(0, 0);

  // Output to terminal
  term_set_color(clrWhite, clrBlack);
  for (int y_cnt = 1; y_cnt <= height; y_cnt++)
  {
    for (int x_cnt = 1; x_cnt <= width_box1; x_cnt++)
    {
      // Print an empty but coloured character for the border
      if (y_cnt == 1 || y_cnt == height || x_cnt == 1 || x_cnt == width_box1)
      {
        if (y_cnt == 1 && x_cnt == 1)
        {
          term_set_color(clrBlack, clrCyan);
          uart_puts(UART_ID, colour_title_string);
          term_move_to(width_box1, y_cnt);
          x_cnt = width_box1;
        }
        else
        {
          term_move_to(x_cnt, y_cnt);
          term_set_color(clrCyan, clrCyan);
          uart_putc(UART_ID, ' ');
          term_set_color(clrWhite, clrBlack);
        }
      }
      else if (y_cnt == 3 && x_cnt == 3)
      {
        term_move_to(x_cnt, y_cnt);
        uart_puts(UART_ID, red_status_string);
        x_cnt == width_box1;
        term_move_to(width_box1, y_cnt);
      }
      else if (y_cnt == 4 && x_cnt == 3)
      {
        term_move_to(x_cnt, y_cnt);
        uart_puts(UART_ID, green_status_string);
        x_cnt == width_box1;
        term_move_to(width_box1, y_cnt);
      }
      else if (y_cnt == 5 && x_cnt == 3)
      {
        term_move_to(x_cnt, y_cnt);
        uart_puts(UART_ID, blue_status_string);
        x_cnt == width_box1;
        term_move_to(width_box1, y_cnt);
      }
    }
    
    for (int x_cnt = 1; x_cnt <= width_box2; x_cnt++)
    {
      // Print an empty but coloured character for the border
      if (y_cnt == 1 || y_cnt == height || x_cnt == 1 || x_cnt == width_box2)
      {
        if (y_cnt == 1 && x_cnt == 1)
        {
          term_move_to(x_cnt + width_box1 + 1, y_cnt);
          term_set_color(clrBlack, clrCyan);
          uart_puts(UART_ID, tutorial_title_string);
          term_move_to(width_box1 + width_box2 + 1, y_cnt);
          x_cnt = width_box2;
        }
        else
        {
          term_move_to(x_cnt + width_box1 + 1, y_cnt);
          term_set_color(clrCyan, clrCyan);
          uart_putc(UART_ID, ' ');
          term_set_color(clrWhite, clrBlack);
        }
      }
      else if (y_cnt == 3 && x_cnt == 3)
      {
        term_move_to(x_cnt + width_box1 + 1, y_cnt);
        uart_puts(UART_ID, red_info_string);
        x_cnt == width_box2;
        term_move_to(width_box1 + width_box2 + 1, y_cnt);
      }
      else if (y_cnt == 4 && x_cnt == 3)
      {
        term_move_to(x_cnt + width_box1 + 1, y_cnt);
        uart_puts(UART_ID, green_info_string);
        x_cnt == width_box2;
        term_move_to(width_box1 + width_box2 + 1, y_cnt);
      }
      else if (y_cnt == 5 && x_cnt == 3)
      {
        term_move_to(x_cnt + width_box1 + 1, y_cnt);
        uart_puts(UART_ID, blue_info_string);
        x_cnt == width_box2;
        term_move_to(width_box1 + width_box2 + 1, y_cnt);
      }
    }
    
    putchar('\n'); // Move to the next row
  }
}

int main(void)
{
  stdio_init_all();
  aio_init_uart();
  aio_init_rgb_pwm();

  // UI SETUP
  term_set_color(clrCyan, clrBlack); // set background and forground colours
  gui_update();

  while (1)
  {
    tight_loop_contents(); // so the cpu can do other stuff
    // INTERRUPT HANDLING CODE
    if (input_ready == true)
    {
      // process inputs
      uint16_t val;
      // if ((val >= 255) || (val <= 0)) {
      //   uart_puts(UART_ID, "Invalid Input");
      // }
      if (sscanf(buffer, "red %hu", &val) > 0)
      {
        // check if its legal 0<val<255
        pwm_set_gpio_level(RED_LED, val * val);
        red_level = val;
      }
      else if (sscanf(buffer, "green %hu", &val) > 0)
      {
        // check if its legal 0<val<255
        pwm_set_gpio_level(GREEN_LED, val * val);
        green_level = val;
      }
      else if (sscanf(buffer, "blue %hu", &val) > 0)
      {
        // check if its legal 0<val<255
        pwm_set_gpio_level(BLUE_LED, val * val);
        blue_level = val;
      }

      // clear buffer
      for (size_t x = 0; x < i; x++)
      {
        buffer[x] = '\0';
      }
      i = 0;
      input_ready = false;

      // update GUI
      gui_update();
    }
  }
}
