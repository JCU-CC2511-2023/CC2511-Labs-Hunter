#include "pico/stdlib.h"

uint32_t c_average(uint32_t Q, uint32_t* values) {
  int n = 1 << Q; // Calculate 2^Q using bit shifting
  long long int sum_x = 0;
  for (int i = 0; i < n; i++)
  {
    sum_x += values[i];
  }
  uint32_t avg_x = sum_x/n;
  return avg_x;
}
