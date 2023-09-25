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

int main(void)
{
  stdio_init_all();
  adc_init();

  adc_gpio_init(LDR_PIN);
  adc_select_input(0);

  int light_lvl_gain = 220;        // give arbitrary gain so that changes are easier to detect
  volatile int light_lvl_prev = 0; // store previous reading
  volatile bool logic_high = false;
  while (true)
  {
    uint16_t adc = adc_read();
    uint16_t light_lvl = constant * adc * (light_lvl_gain / 3.3);
    int16_t delta_light_lvl = light_lvl - light_lvl_prev;
    // printf("ADC: %u _____ Light Level: %u _____ Delta: %d \r\n ", adc, light_lvl, delta_light_lvl);

    if (delta_light_lvl >= 10 && logic_high == false)
    {
      printf("Rising Edge Detected \n");
      logic_high = true;
    }
    else if (delta_light_lvl <= -10 && logic_high == true)
    {
      printf("Falling Edge Detected \n");
      logic_high = false;
    }

    light_lvl_prev = light_lvl; // get ready for next data reading
    sleep_ms(10);
  }
}