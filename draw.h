#ifndef DRAW_H
#define DRAW_H

#define BLACK_COLOR 0
#define WHITE_COLOR 65535
#define YELLOW_COLOR 65504
#define BLUE_COLOR 31
#define RED_COLOR 63488
#define GREEN_COLOR 2016

#define DISPLAY_WIDTH 480
#define DISPLAY_HEIGHT 320
#define BASIC_SCALE 10
#define LETTER_SCALE 3
#define TIMER_SCALE 2

#define LEVEL_CONSTANT 90

enum level{LEVEL0, LEVEL1, LEVEL2, LEVEL3};

/*function for drawing one pixel(of exact scale)*/
void draw_pixel(int x_coord, int y_coord, unsigned short color, int scale, unsigned short * fb);

/*function for drawing one character*/
void draw_char(int x, int y, char ch, unsigned short color, int size, unsigned short * fb);

/*function for drawing text(array of characters)*/
void draw_text(int x, int y, const char* text, unsigned short color, int size, unsigned short * fb);

/*function for drawing rectangle on specified coordinates*/
void draw_rectangle(int x_start, int y_start, int x_end, int y_end, unsigned short color, unsigned short * fb);

/*function for drawing boeders*/
void draw_boarders(int lev, unsigned short * fb);

/*function for clearing display(making it black)*/
void display_black(unsigned short * fb);

/*function for drawing buffer on screen*/
void update_view(unsigned short * fb);

/*function for drawing level*/
void display_level(int i, unsigned short * fb);

/*function for drawing speed choose menu*/
void draw_speed_menu(unsigned short * fb);

/*function for drawing slider in speed choose menu*/
void draw_slider(int selected, unsigned short * fb);

/*function for changing game type color(if selected)*/
void color_game_type(int blue_type, unsigned short * fb);

#endif  /*DRAW_H*/