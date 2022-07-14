#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "snake.h"
#include "draw.h"
#include "lights.h"

snake_body *snake1, *snake2;
point* fruit;
bool game_over, Exit, level_up;
extern uint32_t led_line1, led_line2, led_line_sum;

/* inicializuje hada - defaultni smer kladny po ose X */
snake_body* initialize_snake(int constant, int level) {
  snake_body* ret = (snake_body*) malloc(sizeof(snake_body));
  ret->axis = X_AXIS;
  ret->direction = POS_DIRECTION;
  ret->size = 1;
  ret->points = (point*)malloc(sizeof(point));
  ret->points[0].x = constant * ((DISPLAY_WIDTH-level*LEVEL_CONSTANT)/3);
  ret->points[0].y = DISPLAY_HEIGHT/2;
  return ret;
}

/* prodluzovani snaka */
void extend_snake(snake_body *snake) {
  snake->size++;
  snake->points = (point*)realloc(snake->points, snake->size*sizeof(point));
  snake->points[snake->size-1] = snake->extender;
}

void move_snake(snake_body *snake, unsigned short * fb) {
  /* extender - hodnotu posledniho clanku hada si ulozi snake ve strukture a v pripade 
  ze sni fruit se tahle  hodnota znovu pricte k hadovi */
  snake->extender = snake->points[snake->size-1];
  draw_pixel(snake->extender.x, snake->extender.y, BLACK_COLOR, BASIC_SCALE, fb);
  for (int i=0; i < snake->size-1; i++) {
    snake->points[snake->size-1-i] = snake->points[snake->size-2-i];
  }
  if (snake->axis == 1) {
    snake->points[0].y += BASIC_SCALE*snake->direction;
  } else {
    snake->points[0].x += BASIC_SCALE*snake->direction;
  }
}

void setup(int lev, unsigned short * fb, knobs *kn) {
  led_line1 = 1;
  led_line1 <<= 31;
  led_line2 = 1;
  led_line_sum = 0;
  update_score(0);
  display_black(fb);
  draw_boarders(lev, fb);
  game_over = false;
  snake1 = initialize_snake(1, lev);
  if (game_type == MULTIPLAYER) 
    snake2 = initialize_snake(2, lev);
  fruit = (point*)malloc(sizeof(point));
  fruit->x = (rand() % ((DISPLAY_WIDTH-(lev*LEVEL_CONSTANT))/BASIC_SCALE -2) + 1)*BASIC_SCALE;
  fruit->y = (rand() % (DISPLAY_HEIGHT/BASIC_SCALE -2) + 1)*BASIC_SCALE;
  for(int i = 0; i == snake1->size; i++){
    if (snake1->points[i].x == fruit->x && snake1->points[i].y == fruit->y){
      fruit->x = (rand() % ((DISPLAY_WIDTH-(lev*LEVEL_CONSTANT))/BASIC_SCALE -2) + 1)*BASIC_SCALE;
      fruit->y = (rand() % (DISPLAY_HEIGHT/BASIC_SCALE -2) + 1)*BASIC_SCALE;
      break;
    }
  }
  record_flags(kn);
}

void free_fruit(point* my_fruit){
  free(my_fruit);
}

void free_snake(snake_body* my_snake){
  free(my_snake->points);
  free(my_snake);
}