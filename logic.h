#ifndef LOGIC_H
#define LOGIC_H

#include "draw.h"
#include "snake.h"

/*function for game logic*/
int logic(int lev, int speed_multiplier, int flag1, int flag2, snake_body * snake1, snake_body * snake2, point * fruit, unsigned short * fb);

#endif /*LOGIC_H*/