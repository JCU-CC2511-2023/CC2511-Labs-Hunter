/**************************************************************
 * main.c
 * rev 1.0 14-Sep-2023 Hunter Kruger-Ilingworth
 * Lab8
 * ***********************************************************/

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"

#define LDR_PIN 26

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
int red_level = 255;
int green_level = 255;
int blue_level = 255;

//ADC
float constant = 3.3/((2<<12)-1);


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
  pwm_set_gpio_level(RED_LED, red_level); // assign initial PWM value

  gpio_init(GREEN_LED);
  gpio_set_dir(GREEN_LED, true);               // set direction to recieve input
  gpio_set_function(GREEN_LED, GPIO_FUNC_PWM); // set function to PWM
  uint Gslice_num = pwm_gpio_to_slice_num(GREEN_LED);
  pwm_set_enabled(Gslice_num, true);      // enable PWM
  pwm_set_gpio_level(GREEN_LED, green_level); // assign initial PWM value

  gpio_init(BLUE_LED);
  gpio_set_dir(BLUE_LED, true);               // set direction to recieve input
  gpio_set_function(BLUE_LED, GPIO_FUNC_PWM); // set function to PWM
  uint Bslice_num = pwm_gpio_to_slice_num(BLUE_LED);
  pwm_set_enabled(Bslice_num, true);     // enable PWM
  pwm_set_gpio_level(BLUE_LED, blue_level); // assign initial PWM value
}


int main(void) {
  stdio_init_all();
  adc_init();
  aio_init_rgb_pwm();

  adc_gpio_init(LDR_PIN);
  adc_select_input(0);
  
  while (true) {
    uint16_t adc = adc_read();
    uint16_t pwm = constant*adc*(255/3.3);
    float voltage = constant*adc;
        
    pwm_set_gpio_level(RED_LED, pwm * pwm);
    pwm_set_gpio_level(GREEN_LED, pwm * pwm);
    pwm_set_gpio_level(BLUE_LED, pwm * pwm);

    printf("ADC: %u Voltage: %f  PWM: %u \r\n ", adc, voltage, pwm);
    sleep_ms(10);
  }
}
