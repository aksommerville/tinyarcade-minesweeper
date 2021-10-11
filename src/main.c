#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "tinyc.h"
#include "softarcade.h"

/* Audio.
 *************************************************************/

#if SOFTARCADE_AUDIO_ENABLE

struct softarcade_synth synth={0};

int16_t tinyc_client_update_synthesizer() {
  return softarcade_synth_update(&synth);
}

static const int16_t *get_wave(uint8_t waveid,void *userdata) {
  return 0;
}

static uint16_t get_pcm(void *pcmpp,uint8_t waveid,void *userdata) {
  return 0;
}

#else
int16_t tinyc_client_update_synthesizer() { return 0; }
#endif

/* Video.
 ***********************************************************************/
 
SOFTARCADE_IMAGE_DECLARE(fb,96,64,0)
SOFTARCADE_IMAGE_DECLARE(bgbits,96,64,0)

SOFTARCADE_IMAGE_DECLARE(tiles,20,20,
  146,146,146,146,146,182,182,182,182,146,246,246,246,246,232,147,147,147,147,2,
  146,146,146,146,146,182,146,146,146,73,246,232,232,232,132,147,2,2,2,1,
  146,146,146,146,146,182,146,146,146,73,246,232,232,232,132,147,2,2,2,1,
  146,146,146,146,146,182,146,146,146,73,246,232,232,232,132,147,2,2,2,1,
  146,146,146,146,146,146,73,73,73,73,232,132,132,132,132,2,1,1,1,1,
  146,146,146,146,146,146,146,146,146,146,146,146,146,146,146,146,146,146,146,146,
  146,146,146,146,146,146,146,146,4,146,146,4,146,4,146,146,4,146,4,146,
  146,146,4,146,146,146,146,146,146,146,146,146,146,146,146,146,146,146,146,146,
  146,146,146,146,146,146,4,146,146,146,146,146,4,146,146,146,4,146,4,146,
  146,146,146,146,146,146,146,146,146,146,146,146,146,146,146,146,146,146,146,146,
  146,146,146,146,146,146,4,146,4,146,4,146,146,146,4,4,146,146,146,4,
  146,4,146,4,146,146,146,146,146,146,146,146,146,146,146,146,146,4,146,146,
  146,146,4,146,146,146,4,146,4,146,4,146,4,146,4,4,146,146,146,4,
  146,4,146,4,146,146,146,146,146,146,146,146,146,146,146,146,146,4,146,146,
  146,146,146,146,146,146,4,146,4,146,4,146,146,146,4,4,146,146,146,4,
  4,146,4,146,4,232,182,182,182,232,182,182,182,182,146,182,182,182,182,146,
  146,146,146,146,146,182,232,146,232,73,182,146,146,146,73,182,146,146,146,73,
  4,146,4,146,4,182,146,232,146,73,182,146,146,146,73,182,146,146,146,73,
  146,146,146,146,146,182,232,146,232,73,182,146,146,146,73,182,146,146,146,73,
  4,146,4,146,4,232,73,73,73,232,146,73,73,73,73,146,73,73,73,73,
)
static struct softarcade_image tilev[16]={0};
#define TILESIZE 5

SOFTARCADE_IMAGE_DECLARE(status_ok,16,16,
  28,28,28,28,28,31,31,31,31,31,31,28,28,28,28,28,
  28,28,28,31,31,31,31,31,31,31,31,31,31,28,28,28,
  28,28,31,31,31,31,31,31,31,31,31,31,31,31,28,28,
  28,31,31,31,31,31,31,31,31,31,31,31,31,31,31,28,
  28,31,31,31,31,0,31,31,31,31,0,31,31,31,31,28,
  31,31,31,31,0,0,0,31,31,0,0,0,31,31,31,31,
  31,31,31,31,0,31,0,31,31,0,31,0,31,31,31,31,
  31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,
  31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,
  31,31,31,0,31,31,31,31,31,31,31,31,0,31,31,31,
  31,31,31,31,0,31,31,31,31,31,31,0,31,31,31,31,
  28,31,31,31,31,0,0,0,0,0,0,31,31,31,31,28,
  28,31,31,31,31,31,31,31,31,31,31,31,31,31,31,28,
  28,28,31,31,31,31,31,31,31,31,31,31,31,31,28,28,
  28,28,28,31,31,31,31,31,31,31,31,31,31,28,28,28,
  28,28,28,28,28,31,31,31,31,31,31,28,28,28,28,28,
)

SOFTARCADE_IMAGE_DECLARE(status_lose,16,16,
  28,28,28,28,28,21,21,21,21,21,21,28,28,28,28,28,
  28,28,28,21,21,21,21,21,21,21,21,21,21,28,28,28,
  28,28,21,21,21,21,21,21,21,21,21,21,21,21,28,28,
  28,21,21,21,21,21,21,21,21,21,21,21,21,21,21,28,
  28,21,21,21,21,0,0,21,21,0,0,21,21,21,21,28,
  21,21,21,21,0,0,0,21,21,0,0,0,21,21,21,21,
  21,21,21,21,0,21,21,21,21,21,21,0,21,21,21,21,
  21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,
  21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,
  21,21,21,21,21,0,0,0,0,0,0,21,21,21,21,21,
  21,21,21,21,0,21,21,21,21,21,21,0,21,21,21,21,
  28,21,21,21,0,21,21,21,21,21,21,0,21,21,21,28,
  28,21,21,21,21,21,21,21,21,21,21,21,21,21,21,28,
  28,28,21,21,21,21,21,21,21,21,21,21,21,21,28,28,
  28,28,28,21,21,21,21,21,21,21,21,21,21,28,28,28,
  28,28,28,28,28,21,21,21,21,21,21,28,28,28,28,28,
)

static struct softarcade_font font={
  .metrics={
    96,53,105,181,124,181,184,41,85,85,181,181,83,167,47,181,149,117,149,117,149,149,149,149,149,
    149,46,83,117,142,117,153,149,149,149,117,149,149,149,149,149,117,149,149,117,181,181,181,149,
    149,149,149,181,149,181,181,181,181,117,85,181,85,173,175,108,149,149,114,149,149,117,150,149,
    50,89,117,85,175,143,146,150,150,146,149,117,143,175,175,111,150,114,117,53,117,174,0
  },
  .bits={
    0,3892314112,3019898880,1473639680,1584648192,2290649216,1158764852,3221225472,1782579200,
    2508193792,2881415808,557728256,218103808,4160749568,536870912,143165440,1805475840,3375235072,
    3781750784,3306946560,2582712320,4175552512,1760124928,4045684736,1768513536,1769017344,
    2684354560,1291845632,706871296,4042260480,2292711424,3781428224,1773694976,1777963008,
    3924418560,1917190144,3919175680,4176015360,4175986688,1756983296,2583269376,3912105984,
    286875648,2596966400,2454585344,2397771904,2389391488,1952651008,3924328448,1771794432,
    3924398080,2019680256,4178067968,2576965632,2354356736,2355844352,2324211840,2354856448,
    3847094272,3938451456,2181570688,3586129920,581042176,4063232,2290089984,1635348480,
    2297028608,1915748352,293171200,1776836608,732168192,1769037824,2297008128,2952790016,
    1163919360,2464808960,2856321024,3580493824,3918528512,1771438080,1776844800,1769017344,
    3918004224,2019680256,1268908032,2574254080,2324168704,2371092480,2860515328,2574344192,
    4017094656,1780875264,4160749568,3366715392,1162084352,0
  },
};

/* Game logic.
 **************************************************************/
 
#define COLC 12
#define ROWC 12
#define INITIAL_BOMB_COUNT 16

// Low 4 bits are tileid. 0x80=mine. 0x70=reserved.
static uint8_t grid[COLC*ROWC];

static uint8_t pvinput=0;
static int8_t selcol=COLC>>1,selrow=ROWC>>1;
static uint8_t bombc=0;
static uint8_t flagc=0;
static uint8_t touchc=0;
static uint8_t gameover=0;
static uint8_t status=0; // if (gameover), 0=lose 1=win
static uint8_t blackout=0;
static uint32_t starttime=0;
static uint32_t endtime=0;

static void place_random_bomb() {
  while (1) {
    uint8_t col=rand()%COLC;
    uint8_t row=rand()%ROWC;
    uint8_t p=row*COLC+col;
    if (!(grid[p]&0x80)) {
      grid[p]|=0x80;
      return;
    }
  }
}

static void start_game() {
  starttime=millis();
  srand(starttime);
  memset(grid,0x01,COLC*ROWC); // all hidden
  bombc=INITIAL_BOMB_COUNT;
  flagc=0;
  uint8_t i=0;
  for (;i<bombc;i++) {
    place_random_bomb();
  }
  selcol=COLC>>1;
  selrow=ROWC>>1;
  gameover=0;
  touchc=0;
}

static void end_game(uint8_t victory) {
  endtime=millis();
  gameover=1;
  blackout=30;
  status=victory;
}

// Call when the last flag is placed -- game ends.
static void check_flags() {
  uint8_t victory=1;
  uint8_t *v=grid;
  uint8_t i=COLC*ROWC;
  for (;i-->0;v++) {
    if ((*v)&0x80) {
      if (((*v)&0x0f)==0x02) ; // flagged bomb -- good
      else { // Unflagged bomb!
        (*v)=((*v)&0xf0)|0x03;
        victory=0;
      }
    } else if (((*v)&0x0f)==0x02) { // flagged safe
      (*v)=((*v)&0xf0)|0x0d;
      victory=0;
    }
  }
  end_game(victory);
}

static uint8_t count_neighbor_bombs(uint8_t col,uint8_t row,uint8_t p) {
  uint8_t c=0;
  if (col>0) {
    if (row>0) {
      if (grid[p-COLC-1]&0x80) c++;
    }
    if (grid[p-1]&0x80) c++;
    if (row<ROWC-1) {
      if (grid[p+COLC-1]&0x80) c++;
    }
  }
  if (row>0) {
    if (grid[p-COLC]&0x80) c++;
  }
  if (row<ROWC-1) {
    if (grid[p+COLC]&0x80) c++;
  }
  if (col<COLC-1) {
    if (row>0) {
      if (grid[p-COLC+1]&0x80) c++;
    }
    if (grid[p+1]&0x80) c++;
    if (row<ROWC-1) {
      if (grid[p+COLC+1]&0x80) c++;
    }
  }
  return c;
}

static void cascade_count(int8_t col,int8_t row,uint8_t p) {
  if ((col<0)||(row<0)||(col>=COLC)||(row>=ROWC)) return;
  if ((grid[p]&0x0f)!=0x01) return;
  uint8_t bombc=count_neighbor_bombs(col,row,p);
  if (bombc) {
    grid[p]=(grid[p]&0xf0)|(3+bombc);
  } else {
    grid[p]=(grid[p]&0xf0)|0x00;
    cascade_count(col-1,row-1,p-COLC-1);
    cascade_count(col  ,row-1,p-COLC  );
    cascade_count(col+1,row-1,p-COLC+1);
    cascade_count(col-1,row  ,p     -1);
    cascade_count(col+1,row  ,p     +1);
    cascade_count(col-1,row+1,p+COLC-1);
    cascade_count(col  ,row+1,p+COLC  );
    cascade_count(col+1,row+1,p+COLC+1);
  }
}

static void expose_cell(uint8_t col,uint8_t row) {
  uint8_t p=row*COLC+col;
  if ((grid[p]&0x0f)!=0x01) return;
  if (grid[p]&0x80) {
    if (!touchc) {
      // As a courtesy, if the very first thing they expose is a bomb, quietly move it it elsewhere.
      place_random_bomb();
      grid[p]&=~0x80;
    } else {
      grid[p]=0x83;
      end_game(0);
      return;
    }
  }
  touchc++;
  cascade_count(col,row,p);
}

static void flag_cell(uint8_t col,uint8_t row) {
  uint8_t p=row*COLC+col;
  if ((grid[p]&0x0f)==0x02) {
    // allow taking flags back
    grid[p]=(grid[p]&0xf0)|0x01;
    flagc--;
    return;
  }
  if ((grid[p]&0x0f)!=0x01) return;
  grid[p]=(grid[p]&0xf0)|0x02;
  flagc++;
  if (flagc>=bombc) {
    check_flags();
  }
}

static void handle_buttons(uint8_t pressed) {
  if (pressed&TINYC_BUTTON_LEFT) {
    selcol--;
    if (selcol<0) selcol=COLC-1;
  } else if (pressed&TINYC_BUTTON_RIGHT) {
    selcol++;
    if (selcol>=COLC) selcol=0;
  } else if (pressed&TINYC_BUTTON_UP) {
    selrow--;
    if (selrow<0) selrow=ROWC-1;
  } else if (pressed&TINYC_BUTTON_DOWN) {
    selrow++;
    if (selrow>=ROWC) selrow=0;
  }
  if (pressed&TINYC_BUTTON_A) {
    expose_cell(selcol,selrow);
  }
  if (pressed&TINYC_BUTTON_B) {
    flag_cell(selcol,selrow);
  }
}
 
static void update_game(uint8_t input) {
  if (blackout>0) {
    pvinput=0;
    blackout--;
    return;
  }
  if (gameover) {
    if (input&(TINYC_BUTTON_A|TINYC_BUTTON_B)) {
      start_game();
      pvinput=input;
    }
    return;
  }
  if (input!=pvinput) {
    handle_buttons(input&~pvinput);
    pvinput=input;
  }
}

/* Rendering.
 *****************************************************************/
 
static uint8_t hiliteframe=0;
static uint8_t hiliteclock=0;
 
static void slice_tilesheet() {
  uint8_t tileid=0;
  for (;tileid<16;tileid++) {
    uint8_t col=tileid&3;
    uint8_t row=tileid>>2;
    softarcade_image_crop(tilev+tileid,&image_tiles,col*TILESIZE,row*TILESIZE,TILESIZE,TILESIZE);
  }
}

static void draw_hilite(uint8_t addx,uint8_t addy,uint8_t col,uint8_t row,uint8_t frame) {
  int8_t x=addx+col*TILESIZE;
  int8_t y=addy+row*TILESIZE;
  uint8_t *dst=image_fb.v+image_fb.stride*y+x;
  int8_t i;
  // Walk around the perimeter, setting every fourth pixel. (frame) establishes phase.
  #define CHECK { frame++; if (frame>=4) { frame=0; *dst=0x1f; } else if (frame==2) *dst=0x00; }
  for (i=TILESIZE-1;i-->0;x++,dst++) CHECK
  for (i=TILESIZE-1;i-->0;y++,dst+=image_fb.stride) CHECK
  for (i=TILESIZE-1;i-->0;x--,dst--) CHECK
  for (i=TILESIZE-1;i-->0;y--,dst-=image_fb.stride) CHECK
  #undef CHECK
}
 
static void draw_scene() {
  softarcade_image_clear(&image_fb,0);
  
  const uint8_t fieldx=2,fieldy=2;
  
  const uint8_t *src=grid;
  uint8_t row=0,dsty=fieldy;
  for (;row<ROWC;row++,dsty+=TILESIZE) {
    uint8_t col=0,dstx=fieldx;
    for (;col<COLC;col++,src++,dstx+=TILESIZE) {
      softarcade_blit_unchecked(&image_fb,dstx,dsty,tilev+((*src)&15));
    }
  }
  
  uint32_t elapsedms=endtime-starttime;
  if (!gameover) {
    elapsedms=millis()-starttime;
    
    hiliteclock++;
    if (hiliteclock>=4) {
      hiliteclock=0;
      hiliteframe++;
      if (hiliteframe>=4) {
        hiliteframe=0;
      }
    }
    draw_hilite(fieldx,fieldy,selcol,selrow,hiliteframe);
  }
  
  uint32_t seconds=elapsedms/1000;
  uint32_t minutes=seconds/60;
  seconds%=60;
  char clock[16];
  int clockc=snprintf(clock,sizeof(clock),"%d:%02d",minutes,seconds);
  if ((clockc>0)&&(clockc<sizeof(clock))) {
    softarcade_font_render(&image_fb,fieldx+TILESIZE*COLC+1,1,&font,clock,clockc,0xff);
  }
  
  clockc=snprintf(clock,sizeof(clock),"%d/%d",flagc,bombc);
  if ((clockc>0)&&(clockc<sizeof(clock))) {
    softarcade_font_render(&image_fb,fieldx+TILESIZE*COLC+1,9,&font,clock,clockc,0xff);
  }
  
  if (gameover) {
    if (status) {
      softarcade_blit_unchecked(&image_fb,fieldx+TILESIZE*COLC+9,18,&image_status_ok);
    } else {
      softarcade_blit_unchecked(&image_fb,fieldx+TILESIZE*COLC+9,18,&image_status_lose);
    }
  }
}

/* Main loop.
 ******************************************************************/

static double next_time=0.0;
static uint8_t noteid=0x40;
static uint8_t voiceid=0;
static uint8_t waveid=0;
static uint16_t songp=0;

void loop() {

  unsigned long now=millis();
  if (now<next_time) {
    delay(1);
    return;
  }
  next_time+=16.66666;
  if (next_time<now) {
    tinyc_usb_log("dropped frame\n");
    next_time=now+16.66666;
  }

  uint8_t input_state=tinyc_read_input();
  update_game(input_state);

  draw_scene();
  tinyc_send_framebuffer(image_fb.v);
}

/* Initialize.
 */

void setup() {

  #if SOFTARCADE_AUDIO_ENABLE
  softarcade_synth_init(&synth,22050,get_wave,get_pcm,0);
  #endif
  
  image_status_ok.colorkey=0x1c;
  image_status_lose.colorkey=0x1c;

  tinyc_init();
  slice_tilesheet();
  start_game();
  
  tinyc_init_usb_log();
}
