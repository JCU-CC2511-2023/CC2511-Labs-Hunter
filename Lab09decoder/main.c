/**************************************************************
 * main.c
 * rev 1.0 20-Sep-2023 hkrug
 * Lab9decoder
 * ***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"

#define LDR_PIN 26

// UART
#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 0 // We are using pins 0 and 1, but see the GPIO function select table in the
#define UART_RX_PIN 1 // datasheet for information on which other pins can be used.
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY UART_PARITY_NONE

// ADC
float constant = 3.3 / ((2 << 12) - 1);

// MESSAGE BUFFER DEFINITIONS
volatile char message[100];
volatile unsigned int i = 0;


int round(x){
  float rounded_x = x/10;
  if (x%10 >= 5)
  {
    //round up by adding 1 and making it an integer
    return (rounded_x + 1) * 10;
  }
  else {
    //round down by making it an ineger, thus chopping the decimal
    return rounded_x * 10;
  }
  
}

void decode_input(time_elapsed)
{
  switch (time_elapsed)
  {
  case 40:
    //add 0 to message
    message[i] = 0;
    i++;
    break;
  case 50:
    message[i] = '\n';
    printf("end of message!");
    i = 0;
    //decode message
    break;
  case 60:
    //add 1 to message
    message[i] = 1;
    i++;
    break;
  default: //other
    break;
  }
  printf("message: %s", message );
}

int main(void)
{
  stdio_init_all();
  adc_init();

  adc_gpio_init(LDR_PIN);
  adc_select_input(0);

  int light_lvl_gain = 500; // give arbitrary gain so that changes are easier to detect
  volatile int light_lvl_prev = constant * adc_read() * (light_lvl_gain / 3.3); // store previous reading
  volatile bool logic_high = false;
  int time_increment_ms = 4; //time increment in msiliseconds
  volatile int time_elapsed = 0;
  signed int light_level_threshold = 4;
  while (true)
  {
    uint16_t adc = adc_read();
    uint16_t light_lvl = constant * adc * (light_lvl_gain / 3.3);
    int16_t delta_light_lvl = light_lvl - light_lvl_prev;

    if (delta_light_lvl >= light_level_threshold && logic_high == false)
    {
      printf("R");
      logic_high = true;
      time_elapsed = 0;
    }
    else if (delta_light_lvl <= -(light_level_threshold) && logic_high == true)
    {
      printf("F");
      logic_high = false;
      decode_input(time_elapsed);
      time_elapsed = round(time_elapsed);
      printf("  Pulse width: %d ms \n", time_elapsed);
    }

    light_lvl_prev = light_lvl; 
    time_elapsed += time_increment_ms;
    sleep_ms(time_increment_ms);
  }
}