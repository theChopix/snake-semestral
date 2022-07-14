#ifndef GAME_H
#define GAME_H

#include <time.h>

#include "draw.h"
#include "snake.h"

/*function for drawing actual game*/
void draw_game(snake_body * snake1, snake_body * snake2, point * fruit, unsigned short * fb, time_t start_time, time_t current_time);

/*function for reading input and change snake direction*/
void input(snake_body * snake1, snake_body * snake2, point * fruit, knobs * kn, unsigned char * mem_base, unsigned char * rk_record, unsigned char * bk_record);

#endif /*GAME_H*/