#include <stdint.h>
#include <time.h>

#include "lights.h"
#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"

uint32_t led_line1, led_line2, led_line_sum = 0;

/* vybarvi rgb ledku (if ledNum==0 -> RGB1 else RGB2) barvou v parametru */
void color_rgb(int ledNum, int color) {
  if (!ledNum) {
    *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB1_o) = color;
  } else {
    *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB2_o) = color;
  }
}

/* prechod lcd dole na pase... udela se vzdy kdyz jsem se jde do menu nebo se da exit */
void transition_led() {
  uint32_t val_line=5;
  struct timespec loop_delay = {.tv_sec = 0, .tv_nsec = 20 * 1000 * 1000};
  for (int k=0; k<33; k++) {
    *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = val_line;
    val_line<<=1;
    clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
  }
}

/* updatuje skore v LED pasku*/
void update_score(int player) {
  if (!player) {
    *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = 0;
  } else if (player == 1) {
    led_line_sum += led_line1;
    *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = led_line_sum;
    led_line1 >>= 1;
  } else {
    led_line_sum += led_line2;
    *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = led_line_sum;
    led_line2 <<= 1;
  }
}