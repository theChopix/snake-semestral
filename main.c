/*******************************************************************
  Project main function template for MicroZed based MZ_APO board
  designed by Petr Porazil at PiKRON

  main.c      - main file
  draw.h      - drawing functions
  lights.h    - functions for managing LEDs
  knobs.h     - reading and initialising knobs
  snake.h     - snake initialisation and managing
  game.h      - functions to draw the game and manage input
  logic.h     - game logic function

  Jan Kratky and Dmytro Kadar

 *******************************************************************/

#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <termios.h>

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "font_types.h"
#include "logic.h"
#include "draw.h"
#include "snake.h"
#include "lights.h"
#include "game.h"

extern unsigned char *parlcd_mem_base, *mem_base;
unsigned short *fb;
// font_descriptor_t *fdes = &font_rom8x16;
knobs *kn;

extern int flag1, flag2;
/*rychlost je 0 - nejrychlejsi, 4 - nejpomalejsi*/
int speed_multiplier = 2;
extern unsigned char rk_record, bk_record;
extern bool game_over, Exit, level_up;
bool snake_and_fruit_is_initialised = false;
int game_type;
extern uint32_t led_line1, led_line2, led_lineSum;

snake_body *snake1, *snake2;
point* fruit;
knobs *kn;

time_t current_time;
time_t start_time;

/*function for choosing speed*/
void choose_speed(){
  bool set_speed = false;

  // refreshing screen and drawing speed choose menu
  display_black(fb);
  draw_speed_menu(fb);

  // waiting for button reset
  struct timespec loop_delay = {.tv_sec = 0, .tv_nsec = 300000000L};
  nanosleep(&loop_delay, &loop_delay);

  while(!set_speed){
    // changing slider position and seting speed multiplier
    read_knobs(kn, mem_base);
    if (kn->gk % 20 == 16) {
      draw_slider(0, fb);
      speed_multiplier = 4;
    } else if (kn->gk % 20 == 12) {
      draw_slider(1, fb);
      speed_multiplier = 3;
    } else if (kn->gk % 20 == 8) {
      draw_slider(2, fb);
      speed_multiplier = 2;
    } else if (kn->gk % 20 == 4) {
      draw_slider(3, fb);
      speed_multiplier = 1;
    } else if (kn->gk % 20 == 0) {
      draw_slider(4, fb);
      speed_multiplier = 0;
    } 

    update_view(fb);

    if (kn->gb) {
      set_speed = true;
    }
  }
}

/*function for game initialising*/
void set_new_game(int level) {
  setup(level, fb, kn);
  snake_and_fruit_is_initialised = true;
  while(!game_over) {
    draw_game(snake1, snake2, fruit, fb, start_time, current_time);
    input(snake1, snake2, fruit, kn, mem_base, &rk_record, &bk_record);
    logic(level, speed_multiplier, flag1, flag2, snake1, snake2, fruit, fb);
  }
}

/*function for drawing main menu and choosing game mode*/
void main_menu() {
  bool game_set = false;
  color_rgb(0, BLACK_COLOR);
  color_rgb(1, BLACK_COLOR);
  while(!game_set) {
    read_knobs(kn, mem_base);
    if (kn->gk % 16 == 0) {
      color_game_type(0, fb);
    } else if (kn->gk % 16 == 4) {
      color_game_type(1, fb);
    } else if (kn->gk % 16 == 8) {
      color_game_type(2, fb);
    } else{
      color_game_type(3, fb);
    } 
    
    update_view(fb);

    if (kn->gb) {
      if (kn->gk % 16 == 0) {
        game_type = SINGLEPLAYER;
      } else if (kn->gk % 16 == 4) {
        game_type = MULTIPLAYER;
      } else if (kn->gk % 16 == 8) {
        game_type = DEMO;
      } else {
        Exit = true;
      }
      game_set = true;
    }
  }
  level_up = false;
  if (!Exit) {
    choose_speed();

    for (int i = 1; i < 4; i++) {
      display_black(fb);
      display_level(i, fb);

      time(&start_time);

      set_new_game(i);
      // if level_up false, game is over and go back to menu
      if (!level_up) 
        break;
    }
  }
  display_black(fb);
}

int main(){
    // initializing global variables
    Exit = false;
    mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
    parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
    fb  = (unsigned short *)malloc(DISPLAY_HEIGHT*DISPLAY_WIDTH*2);
    kn = (knobs *)malloc(sizeof(knobs));
    srand(time(NULL));
    led_line1 = 1;
    led_line2 = 1;
    led_line1 <<= 31;

    while(!Exit) {
      main_menu();
      transition_led();
    }

    // refreshing display
    display_black(fb);
    update_view(fb);

    // free all allocated memory
    if (snake_and_fruit_is_initialised){
      free_snake(snake1);
      free_fruit(fruit);
    }
    if(game_type == MULTIPLAYER)
      free_snake(snake2);
    free(fb);
    free(kn);
    return 0;
}