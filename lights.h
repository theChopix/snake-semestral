#ifndef LIGHTS_H
#define LIGHTS_H

unsigned char *parlcd_mem_base, *mem_base;

/*function for lighting specific LED with specific color*/
void color_rgb(int ledNum, int color);

/*function for making cool effect on LED strip*/
void transition_led();

/*function for updating score in LED strip*/
void update_score(int player);

#endif  /*LIIGHTS_H*/