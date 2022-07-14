#include<stdio.h>
#include <time.h>
#include <stdbool.h>

#include "draw.h"
#include "snake.h"

#define LEFT -1
#define RIGHT 1
#define STRAIGHT 0

int flag1, flag2;

/* vykresli na displej situaci hry */
void draw_game(snake_body * snake1, snake_body * snake2, point * fruit, unsigned short * fb, time_t start_time, time_t current_time) {
  for (int i = 0; i < snake1->size; i++) 
    draw_pixel(snake1->points[i].x, snake1->points[i].y, WHITE_COLOR, BASIC_SCALE, fb);
  if (game_type == MULTIPLAYER) {
    for (int i = 0; i < snake2->size; i++) {
      draw_pixel(snake2->points[i].x, snake2->points[i].y, RED_COLOR, BASIC_SCALE, fb);
    }
  }
  draw_pixel(fruit->x, fruit->y, YELLOW_COLOR, BASIC_SCALE, fb);
  
  /*timer drawing*/
  time(&current_time);

  char time_text[14], score_text[4];
  int sec = (current_time-start_time);
  int min = sec/60;
  sec = sec%60;

  sprintf(time_text, "%d:%d", min,sec);

  draw_rectangle(405, 50, 470, 90, BLACK_COLOR, fb);

  draw_text(405, 20, "TIME", WHITE_COLOR, TIMER_SCALE, fb);
  draw_text(405, 50, time_text, WHITE_COLOR, TIMER_SCALE, fb);

  draw_rectangle(415, 250, 470, 290, BLACK_COLOR, fb);
  sprintf(score_text, "%d", snake1->size-1);
  draw_text(395, 220, "SCORE", WHITE_COLOR, TIMER_SCALE, fb);
  draw_text(440, 250, score_text, WHITE_COLOR, TIMER_SCALE, fb);

  update_view(fb);
}

void input(snake_body * snake1, snake_body * snake2, point * fruit, knobs * kn, unsigned char * mem_base, unsigned char * rk_record, unsigned char * bk_record) {
  if (game_type == DEMO) {
    /* AI snake */
    flag1 = STRAIGHT;
    if (snake1->axis == Y_AXIS && snake1->direction == POS_DIRECTION && fruit->y <= snake1->points[0].y) {
      flag1 = fruit->x < snake1->points[0].x ? RIGHT : LEFT;
    } else if (snake1->axis == Y_AXIS && snake1->direction == NEG_DIRECTION && fruit->y >= snake1->points[0].y) {
      flag1 = fruit->x < snake1->points[0].x ? LEFT : RIGHT;
    } else if (snake1->axis == X_AXIS && snake1->direction == POS_DIRECTION && fruit->x <= snake1->points[0].x) {
      flag1 = fruit->y < snake1->points[0].y ? LEFT : RIGHT; 
    } else if (snake1->axis == X_AXIS && snake1->direction == NEG_DIRECTION && fruit->x >= snake1->points[0].x) {
      flag1 = fruit->y < snake1->points[0].y ? RIGHT : LEFT;
    }
  } else {
    read_knobs(kn, mem_base);

    if (kn->rk != *rk_record && (kn->rk + 2 != *rk_record && kn->rk - 2 != *rk_record)) {
      flag1 = kn->rk > *rk_record ? LEFT : RIGHT;
    } else {
      flag1 = STRAIGHT;
    }
    *rk_record = kn->rk;
    if (game_type == MULTIPLAYER) {
      if (kn->bk != *bk_record && (kn->bk + 2 != *rk_record && kn->bk - 2 != *rk_record)) {
        flag2 = kn->bk > *bk_record ? LEFT : RIGHT;
      } else {
        flag2 = STRAIGHT;
      }
      *bk_record = kn->bk;
    }
  }
}

