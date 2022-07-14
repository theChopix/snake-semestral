#ifndef KNOBS_H
#define KNOBS_H

/*structure that stores knobs values*/
typedef struct{
  int rk, gk, bk, rb, gb, bb;
} knobs;

/*function for reading knobs values*/
void read_knobs(knobs *k, unsigned char * mem_base);

/*function for reading red and blue knob values for future comparations*/
void record_flags(knobs *kn);

#endif  /*DRAW_H*/