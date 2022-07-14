#ifndef SNAKE_H
#define SNAKE_H

#include "knobs.h"

#define X_AXIS -1
#define Y_AXIS 1
#define POS_DIRECTION 1
#define NEG_DIRECTION -1

bool game_over, Exit, level_up;

int game_type;

enum gameTyp{SINGLEPLAYER, MULTIPLAYER, DEMO};

/*structure for storing pixel position*/
typedef struct {
  int x, y;
} point;

/*structure for storing all information about snake*/
typedef struct{
  int axis;
  int direction;
  int size;
  point* points;
  point extender;
} snake_body;

/*function for initialising snake(if multiplayer, then snakes)*/
snake_body* initialize_snake(int constant, int level);

/*function for extending snake if it eat fruit*/
void extend_snake(snake_body *snake);

/*function for moving snake*/
void move_snake(snake_body *snake, unsigned short * fb);

/*function for setting up enviroment for game*/
void setup(int lev, unsigned short * fb, knobs* kn);

/*function for free fruit*/
void free_fruit(point* my_fruit);

/*function for free snake*/
void free_snake(snake_body* my_snake);

#endif /*SNAKE_H*/