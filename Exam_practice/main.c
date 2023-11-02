/**************************************************************
 * main.c
 * rev 1.0 02-Nov-2023 hkrug
 * Exam_practice
 * ***********************************************************/

#include "pico/stdlib.h"
#include <stdbool.h>
#include <stdlib.h> 
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"

#define LDR_PIN 26
#define RED_LED 11
#define GREEN_LED 12
#define BLUE_LED 13

#define SW_1 2
#define SW_2 3
#define SW_3 4

#define IDLE 0
#define GAME 1

#define EASY 0
#define HARD 1

//Random 
int rand(void); 
void srand(unsigned int seed); 


volatile int current_mode = 0;
volatile int red_level = 0;
volatile int green_level = 0;
volatile int blue_level = 0;

float adc_constant = 3.3/((2<<12)-1);

/*! \brief All in one function which enables LEDS and PWM
* \param None
*/
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




/*! \brief All in one function which enables ADC
* \param None
*/
void aio_init_adc(){
  adc_init();
  adc_gpio_init(LDR_PIN);
  adc_select_input(0);
}

int main(void) {
  stdio_init_all();
  printf("CC2511 Exam 2022");
  printf("Hunter Kruger-Ilingworth");
  //INITIALISE LED PINS
  gpio_init(RED_LED);
  gpio_set_dir(RED_LED, true); //recieving input

  gpio_init(GREEN_LED);
  gpio_set_dir(GREEN_LED, true); //recieving input

  gpio_init(BLUE_LED);
  gpio_set_dir(BLUE_LED, true); //recieving input


  float voltage_memory[5] = {0}; // 5 elemet array of most recent 5 voltages ( order irrelevant)
  volatile int i = 0; // array index

  aio_init_adc();
  bool voltage_memory_full = false;
  while (true) {
    sleep_ms(500);
    uint16_t adc_reading = adc_read();
    float voltage = adc_constant*adc_reading;
    if (i != 4) // if the index is not about to overflow next increment
    {
      voltage_memory[i] = voltage;
      i++;
    }
    else {
      voltage_memory_full = true;
      voltage_memory[i] = voltage;
      i = 0; //reset the index
    }
    int array_length;
    //printf("\n arraylen: %f",array_length);   
    float sum = 0;
      if (!voltage_memory_full)
        array_length = i;
      else{
        array_length = 4;
      }
      for (size_t j = 0; j < array_length; j++)
      {
        sum += voltage_memory[j];
      }
      sum /= array_length;
    float average_voltage = sum;
    //printf("ADC: %u Voltage: %f  PWM: %u \r\n ", adc_reading, voltage, pwm);



    printf("\nVoltage: %f        Average Voltage: %f", voltage, average_voltage);
    if (gpio_get(SW_2) == 0)
    {

      printf("\nEntering Game Mode");
      current_mode = GAME;
      int score = 0;
      while (current_mode == GAME){
        int random_number = rand() % 3;
        switch (random_number)
        {
        case 0:
          gpio_put(RED_LED, 1);
          break;
        case 1:
          gpio_put(GREEN_LED, 1);
          break;
        case 2:
          gpio_put(BLUE_LED, 1);
          break;
        default:
          break;
        }
        bool user_not_ready = true;
        sleep_ms(500);
        bool game_mode = EASY;
        bool timeout = false;
        while (user_not_ready && game_mode == EASY)
        { 
          sleep_ms(20);
          bool SW_1_pressed = !gpio_get(SW_1);
          bool SW_2_pressed = !gpio_get(SW_2);
          bool SW_3_pressed = !gpio_get(SW_3);

          int ch = getchar_timeout_us(0); // respond to user input by setting the GPIO pin high or low
            if (ch != PICO_ERROR_TIMEOUT && ch == 'm')
            {
              game_mode = HARD;
              printf("\nswitched into 7 colour mode");
              user_not_ready = false;
            }
          if (SW_1_pressed && random_number == 0)
          {
            gpio_put(RED_LED, 0);
            printf("\nCorrect");
            score ++;
            printf("Score: %i", score);
            user_not_ready = false;
          }
          else if (SW_2_pressed && random_number == 1)
          {
            gpio_put(GREEN_LED, 0);
            printf("\nCorrect");
            score ++;
            printf("Score: %i", score);
            user_not_ready = false;
          }
          else if (SW_3_pressed && random_number == 2)
          {
            gpio_put(BLUE_LED, 0);
            printf("\nCorrect");
            score ++;
            printf("Score: %i", score);
            user_not_ready = false;
          }
          else if (SW_1_pressed || SW_2_pressed || SW_3_pressed || timeout)
          {
            printf("\nIncorrect");
            gpio_put(RED_LED, 0);
            gpio_put(GREEN_LED, 0);
            gpio_put(BLUE_LED, 0);
            score --;
            printf("Score: %i", score);
            user_not_ready = false;
          }
          //quit game logic
          if (score == 5 || score == -5){
            if (score == 5)
            {
              printf("\nCongratulations! You won!");
              current_mode = IDLE;
            }
            else if (score == -5)
            {
              printf("\nHard luck, you lost!");
              current_mode = IDLE;
            }
            else if (timeout){
              printf("\nOut of time! ");
            }
          }
          sleep_ms(3000);
          timeout = true;
        }
        //while (user_not_ready && game_mode == HARD)
      
      
      }
      
    }
    
  }
}

