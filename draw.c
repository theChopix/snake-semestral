#include <time.h>
#include <unistd.h>

#include "draw.h"
#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "font_types.h"
#include "knobs.h"
#include "lights.h"

font_descriptor_t *fdes = &font_rom8x16;
unsigned char *parlcd_mem_base;

int SPEED_POSITIONS[] = {40, 145, 230, 325, 420};
const char* level = "Level ";
const char* single = "Singleplayer";
const char* multi = "Multiplayer";
const char* demo = "Demo";
const char* exit_string = "Exit";

void draw_pixel(int x_coord, int y_coord, unsigned short color, int scale, unsigned short * fb) {
  if (x_coord>=0 && x_coord<DISPLAY_WIDTH && y_coord>=0 && y_coord<DISPLAY_HEIGHT) {
    for( int j = 0; j < scale; j++){
    	for(int i = 0; i < scale; i++)
        	fb[(x_coord + i) + DISPLAY_WIDTH *(y_coord +j)] = color;
    }
  }
}

void draw_char(int x, int y, char ch, unsigned short color, int size, unsigned short * fb) {
  int w = fdes->maxwidth;
  const font_bits_t *ptr;
  if ((ch >= fdes->firstchar) && (ch-fdes->firstchar < fdes->size)) {
    if (fdes->offset) {
      ptr = &fdes->bits[fdes->offset[ch-fdes->firstchar]];
    } else {
      int bw = (fdes->maxwidth+15)/16;
      ptr = &fdes->bits[(ch-fdes->firstchar)*bw*fdes->height];
    }
    int i, j;
    for (i=0; i<fdes->height; i++) {
      font_bits_t val = *ptr;
      for (j=0; j<w; j++) {
        if ((val&0x8000)!=0) {
          draw_pixel(x+size*j, y+size*(i-1), color, size, fb);
        } else {
        }
        val<<=1;
      }
      ptr++;
    }
  }
}

/* napise text na displej do souradnic x, y */
void draw_text(int x, int y, const char* text, unsigned short color, int size, unsigned short * fb) {
  char cur_char = text[0];
  int counter = 0;
  while(cur_char != '\0') {
    draw_char(x + (counter*fdes->maxwidth)*size, y, cur_char, color, size, fb);
    cur_char = text[++counter];
  }
}

void draw_rectangle(int x_start, int y_start, int x_end, int y_end, unsigned short color, unsigned short * fb) {
  for (int i = x_start; i < x_end; i=i+BASIC_SCALE) {
    for (int j = y_start; j < y_end; j=j+BASIC_SCALE) {
      draw_pixel(i, j, color, BASIC_SCALE, fb);
    }
  }
}

/* vybarvi cely displej cerne - default; pred aktualizaci displeje */
void display_black(unsigned short * fb) {
  for (int i = 0; i < DISPLAY_WIDTH; i=i+BASIC_SCALE) {
    for (int j = 0; j < DISPLAY_HEIGHT; j=j+BASIC_SCALE) {
      draw_pixel(i, j, BLACK_COLOR, BASIC_SCALE, fb);
    }
  }
}

/* nakresli hranice pro jednotlive levely - jestli je level vyssi tim je herni plocha mensi */
void draw_boarders(int lev, unsigned short * fb) {
  for (int i=0; i<2; i++) {
      for (int j=0; j<(DISPLAY_WIDTH-lev*LEVEL_CONSTANT)/BASIC_SCALE; j++) {
        draw_pixel(j*BASIC_SCALE, i*(DISPLAY_HEIGHT-BASIC_SCALE), WHITE_COLOR, BASIC_SCALE, fb);
      }
    }
  for (int i=0; i<2; i++) {
      for (int j=0; j<DISPLAY_HEIGHT/BASIC_SCALE; j++) {
        draw_pixel(i*((DISPLAY_WIDTH-lev*LEVEL_CONSTANT)-BASIC_SCALE), j*BASIC_SCALE, WHITE_COLOR, BASIC_SCALE, fb);
    }
  }
}

/* updatuje displej podle toho co je v bufferu *fb */
void update_view(unsigned short * fb) {
  *(volatile uint16_t*)(parlcd_mem_base + PARLCD_REG_CMD_o) = 0x2c;
  int i, j, ptrtmp = 0;
  for (i = 0; i < DISPLAY_HEIGHT ; i++) {
    for (j = 0; j < DISPLAY_WIDTH ; j++) {
        parlcd_write_data(parlcd_mem_base, fb[ptrtmp++]); // draw buffer on the screen
    }
  }
}

/* zobrazi level a cislo jeste pred tim nez zacne hra */
void display_level(int i, unsigned short * fb) {
  draw_text(156, 136, level, BLUE_COLOR, LETTER_SCALE, fb);
  switch (i)
  {
    case LEVEL1:
      draw_char(300, 136, '1', BLUE_COLOR, LETTER_SCALE, fb);
      break;
    case LEVEL2:
      draw_char(300, 136, '2', BLUE_COLOR, LETTER_SCALE, fb);
      break;
    case LEVEL3:
      draw_char(300, 136, '3', BLUE_COLOR, LETTER_SCALE, fb);
      break;
  }
  update_view(fb);
  sleep(1);
}

void draw_speed_menu(unsigned short * fb){
  draw_text(20, 100, "SLOW", WHITE_COLOR, LETTER_SCALE, fb);
  draw_text(170, 100, "MEDIUM", WHITE_COLOR, LETTER_SCALE, fb);
  draw_text(360, 100, "FAST", WHITE_COLOR, LETTER_SCALE, fb);
}

void draw_slider(int selected, unsigned short * fb){
  draw_rectangle(40,170,440, 190, WHITE_COLOR, fb);
  draw_rectangle(SPEED_POSITIONS[selected], 170, SPEED_POSITIONS[selected]+20, 190, RED_COLOR, fb);
}

/* vybarvi typ hry v menu modre */
void color_game_type(int blue_type, unsigned short * fb) {
  int colors[] = {WHITE_COLOR, WHITE_COLOR, WHITE_COLOR, WHITE_COLOR};
  colors[blue_type] = BLUE_COLOR;
  draw_text(96, 72, single, colors[0], LETTER_SCALE, fb);
  draw_text(108, 136, multi, colors[1], LETTER_SCALE, fb);
  draw_text(192, 200, demo, colors[2], LETTER_SCALE, fb);
  draw_text(360, 264, exit_string, colors[3], LETTER_SCALE, fb);
}