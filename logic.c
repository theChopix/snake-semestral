#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "draw.h"
#include "snake.h"
#include "lights.h"

void logic(int lev, int speed_multiplier, int flag1, int flag2, snake_body * snake1, snake_body * snake2, point * fruit, unsigned short * fb) {
  color_rgb(0, GREEN_COLOR);
  if (flag1) {
    snake1->axis *= -1;
    snake1->direction *= snake1->axis * flag1;
  }  

  move_snake(snake1, fb);
  
  if (snake1->points[0].x == 0 || snake1->points[0].x == DISPLAY_WIDTH-BASIC_SCALE-lev*LEVEL_CONSTANT
   || snake1->points[0].y == 0 || snake1->points[0].y == DISPLAY_HEIGHT-BASIC_SCALE) {
     color_rgb(0, RED_COLOR);
     game_over = true;
     level_up = false;
  }

  if (snake1->size > 4) {
    for (int i = 4; i < snake1->size-1; i++) {
      if (snake1->points[0].x == snake1->points[i].x && snake1->points[0].y == snake1->points[i].y) {
        color_rgb(0, RED_COLOR);
        game_over = true;
        level_up = false;
      }
    }
  }

  if (snake1->points[0].x == fruit->x && snake1->points[0].y == fruit->y) {
    extend_snake(snake1);
    update_score(1);
    color_rgb(0, BLUE_COLOR);
    fruit->x = (rand() % ((DISPLAY_WIDTH-(lev*LEVEL_CONSTANT))/BASIC_SCALE -2) + 1)*BASIC_SCALE;
    fruit->y = (rand() % (DISPLAY_HEIGHT/BASIC_SCALE -2) + 1)*BASIC_SCALE;
    // there are 3 levels, if player gets 17 points in multiplayer or 32 in multiplayer
    // go to next level
    int max_size = game_type == MULTIPLAYER ? 17 : 33;
    if (snake1->size == max_size) {
      game_over = true;
      level_up = true;
    }
  }

  if (game_type == MULTIPLAYER) {
    color_rgb(1, GREEN_COLOR);
    for (int i = 0; i < snake2->size-1; i++) {
      if (snake1->points[0].x == snake2->points[i].x && snake1->points[0].y == snake2->points[i].y) {
        color_rgb(0, RED_COLOR);
        game_over = true;
        level_up = false;
      }
    }

    if (flag2) {
      snake2->axis *= -1;
      snake2->direction *= snake2->axis * flag2;
    }

    move_snake(snake2, fb);

    if (snake2->points[0].x == 0 || snake2->points[0].x == DISPLAY_WIDTH-BASIC_SCALE - lev*LEVEL_CONSTANT
    || snake2->points[0].y == 0 || snake2->points[0].y == DISPLAY_HEIGHT-BASIC_SCALE) {
      color_rgb(1, RED_COLOR);
      game_over = true;
      level_up = false;
    }

    if (snake2->points[0].x == fruit->x && snake2->points[0].y == fruit->y) {
      extend_snake(snake2);
      update_score(2);
      color_rgb(1, BLUE_COLOR);
      fruit->x = (rand() % ((DISPLAY_WIDTH-(lev*LEVEL_CONSTANT))/BASIC_SCALE -2) + 1)*BASIC_SCALE;
      fruit->y = (rand() % (DISPLAY_HEIGHT/BASIC_SCALE -2) + 1)*BASIC_SCALE;
      if (snake1->size == 17) {
        game_over = true;
        level_up = true;
      }
    }
    if (snake2->size > 4) {
      for (int i = 0; i < snake1->size-1; i++) {
        if (snake2->points[0].x == snake1->points[i].x && snake2->points[0].y == snake1->points[i].y) {
          color_rgb(1, RED_COLOR);
          game_over = true;
          level_up = false;
        }
      }
    }

    for (int i = 0; i < snake1->size-1; i++) {
      if (snake2->points[0].x == snake1->points[i].x && snake2->points[0].y == snake1->points[i].y) {
        color_rgb(1, RED_COLOR);
        game_over = true;
        level_up = false;
      }
    }
  }
  /* zdrzeni casu - ta 150 by mohla byt parametr ktery by se dal v nastaveni menit */
  struct timespec loop_delay = {.tv_sec = 0, .tv_nsec = (speed_multiplier*75) * 1000 * 1000};
  clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
}