#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "lights.h"
#include "mzapo_regs.h"
#include "knobs.h"

unsigned char rk_record, bk_record;

void read_knobs(knobs *k, unsigned char * mem_base) {
    uint32_t rgb_knobs_value;

    rgb_knobs_value = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o);

    k->rk = (rgb_knobs_value>>16) & 0xFF; // red knob position
    k->gk = (rgb_knobs_value>>8) & 0xFF;  // green knob position
    k->bk = (rgb_knobs_value>>0) & 0xFF;  // blue knob position
    k->rb = (rgb_knobs_value>>26) & 1;    // red button
    k->gb = (rgb_knobs_value>>25) & 1;    // green button
    k->bb = (rgb_knobs_value>>24) & 1;    // blue button
}

void record_flags(knobs *kn) {
  read_knobs(kn, mem_base);
  rk_record = kn->rk;
  bk_record = kn->bk;
}