/**************************************************************
 * main.c
 * rev 1.0 06-Nov-2023 hkrug
 * Examination
 * ***********************************************************/

#include "pico/stdlib.h"
#include <stdbool.h>
#include <stdlib.h>
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"

// ONBOARD PIN MACRO DEFINITIONS
#define LDR_PIN 26
#define RED_LED 11
#define GREEN_LED 12
#define BLUE_LED 13
#define SW_1 2
#define SW_2 3
#define SW_3 4

// TASK RELATED MACRO DEFINITIONS
#define ADC 0
#define GAME 1

// ONBOARD BUTTONS SETUP
void init_buttons()
{
  gpio_init(SW_1);
  gpio_set_dir(SW_1, false); //outputting signal

  gpio_init(SW_2);
  gpio_set_dir(SW_2, false); //outputting signal

  gpio_init(SW_3);
  gpio_set_dir(SW_3, false); //outputting signal
}

// RGB LED GPIO SETUP
/*! \brief All in one function which enables LEDS (not PWM!)
 * \param None
 */
void init_rgb_led()
{
  gpio_init(RED_LED);
  gpio_set_dir(RED_LED, true); // recieving input

  gpio_init(GREEN_LED);
  gpio_set_dir(GREEN_LED, true); // recieving input

  gpio_init(BLUE_LED);
  gpio_set_dir(BLUE_LED, true); // recieving input
}

// PWM RGB LED GPIO SETUP
int red_level = 0;
int green_level = 0;
int blue_level = 0;
/*! \brief All in one function which enables LEDS and PWM
 * \param None
 */
void init_rgb_led_pwm()
{
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

// ADC SETUP
/*! \brief All in one function which enables ADC
 * \param None
 */
void init_adc()
{
  adc_init();
  adc_gpio_init(LDR_PIN);
  adc_select_input(0);
}
float adc_constant = 3.3 / ((2 << 12) - 1);

void turn_off_leds(){
  gpio_put(RED_LED, false);
  gpio_put(GREEN_LED, false);
  gpio_put(BLUE_LED, false);
}

// TASK RELATED GLOBAL VARIABLES
volatile int adc_sample_counter = 0;
int rand(void); 
void srand(unsigned int seed);
volatile int current_mode = ADC;
volatile bool input_recieved = false;

int main(void)
{
  stdio_init_all();
  init_buttons();
  init_rgb_led();
  //init_rgb_led_pwm(); only one can be chosen
  init_adc();

  printf("CC2511 Exam 2022");
  printf("Hunter Kruger-Ilingworth");
  while (true)
  { 
    if (adc_sample_counter == 500)
    {
      adc_sample_counter = 0;
      uint16_t adc = adc_read();
      uint16_t pwm = adc_constant*adc*(255/3.3);
      float voltage = adc_constant*adc;
      printf("\r\nADC: %u Voltage: %f \r\n ", adc, voltage);
    }
    sleep_ms(1); // buffer time
    adc_sample_counter++;

    if (gpio_get(SW_2) == 0) //if green is pressed
    {
      printf("\r\nEntering Game Mode");
      current_mode = GAME;
      
      long int led_off_counter = 0;
      input_recieved = false;
      signed int score = 0;
      int r = rand() % 3 + 1; // compiler wants me to generate a redundant random number despite making one at the start of the game
      while (current_mode == GAME)
      {
        if (led_off_counter == 0) //if round has just started
        {
          r = rand() % 3 + 1;
          printf("\n%i", r);
          switch (r)
          {
          case 1:
            gpio_put(RED_LED, true);
            break;
          case 2:
            gpio_put(GREEN_LED, true);  
            break;
          case 3:
            gpio_put(BLUE_LED, true);
            break;
          default:
            break;
          }
        }
        led_off_counter++;
        if (led_off_counter >= 1000)
        {
          input_recieved = (!gpio_get(SW_1)||!gpio_get(SW_2)||!gpio_get(SW_3));
          turn_off_leds();
        }
        

        
        if (input_recieved)
        {
          if (!gpio_get(SW_1) && r == 1)
          {
            printf("Correct");
            score++;
          }
          else if (!gpio_get(SW_2) && r == 2)
          {
            printf("Correct");
            score++;
          }
          else if (!gpio_get(SW_3) && r == 3)
          {
            printf("Correct");
            score++;
          }
          else{
            printf("Incorrect");
            score--;
          }
          
          led_off_counter = 0;
          input_recieved = false;

          if (score == 5 || score == -5)
          {
            if (score == 5)
            {
              printf("Congratulations! You won!");
            }
            else{
              printf("Hard luck, you lost!");
            }
            
            current_mode = ADC;
            sleep_ms(100);
          }
          
          
        }
           
        
        sleep_ms(1);
        
        
        
      }
      
    }
    
  }
}
