#include <c_types.h>
#include <osapi.h>
#include <user_interface.h>
#include <math.h>
#include <time.h>
#include <mem.h>

#include "display.h"
#include "max7219.h"

bool animate = false;
int pause = 0;
int doClear = 0;

uint32 last;
time_t epochish = 1443180702;

os_timer_t ticker;

void clear() {
  int d, r, c;

  for(d = 0; d < 4; d++) {
    for(r = 0; r < 8; r++) {
      for(c = 0; c < 8; c++) {
        setLed(d, r, c, 0);
      }
    }
  }
}

int draw_digit(uint8_t r, uint8_t c, int *digit) {
  int i;
  int dev = 0;
  int len;
  int mod;
  int rtn = 4;

  if(digit[24] == NULL && digit[19] != NULL) {
    len = 19;
    mod = 4;
    rtn = 5;
  } else if(digit[24] == NULL) {
    len = 14;
    mod = 3;
  } else {
    len = 24;
    mod = 5;
    rtn = 6;
  }

  int rr, cc;

  for(i = 0; i <= len; i++) {
    cc = c + ((i % mod));
    rr = r + (4 - floor(i / mod));

    dev = floor(cc / 8);
    cc -= dev * 8;
    setLed(dev, rr, cc, digit[i]);
  }

  return rtn;
}

int gen_row(int row) {
  if(animate) {
    return (row == 0 ? 1 : 0) + (row) % 2;
  } else {
    return 1;
  }
}

int ICACHE_FLASH_ATTR draw_char(int r, int c, char *s) {
  char msg[64];

  int ls = (int)toupper((int)*s) - 54;

  return draw_digit(gen_row(r), c, digits[ls]);
}

void ICACHE_FLASH_ATTR draw_string(char *str) {
  char *ptr;
  int i = 1;

  while(*str != '\0') {
    if(*str == ' ') {
      i += 1;
    } else {
      i += draw_char(1, i, str);
    }

    *str++;
  }
}

void display_time(time_t time) {
  //time = time / 1000;

  uint32 sec = time % 60;
  uint32 min = time / 60 % 60;
  uint32 hour = time / 60 / 60 % 24;

  int i = 2;

  int s1 = floor(sec / 10);
  int s2 = sec % 10;

  int m1 = floor(min / 10);
  int m2 = min % 10;

  int h1 = floor(hour / 10);
  int h2 = hour % 10;

  int r = 1;

  if(animate) {
    clear();
  }

  draw_digit(gen_row(1 + time), i,      digits[h1]);
  draw_digit(gen_row(2 + time), i += 4, digits[h2]);
  draw_digit(gen_row(3 + time), i += 3, digits[10]);
  draw_digit(gen_row(4 + time), i += 3, digits[m1]);
  draw_digit(gen_row(5 + time), i += 4, digits[m2]);
  draw_digit(gen_row(6 + time), i += 3, digits[10]);
  draw_digit(gen_row(7 + time), i += 3, digits[s1]);
  draw_digit(gen_row(8 + time), i += 4, digits[s2]);
}

uint32 last;

void tick() {
  uint32 now;
  now = system_get_time();
  /*
  char msg[22];
  os_sprintf(msg, "%d - %d = %d mod %d div %d\n", now, last, now - last, ((now - last) % 100000), ((now - last) / 1000000));
  uart0_send(msg);
  */

  epochish += ((now - last) / 1000000);

  if(pause > 0) {
    pause--;
    doClear = 1;
  } else {
    if(doClear) {
      clear();
      doClear = 0;
    }

    display_time(epochish);
  }

  last = now;
}

void toggle_animate() {
  animate = !animate;
  clear();
}

void arm_timer() {
  last = system_get_time();

  os_timer_disarm(&ticker);
  os_timer_setfn(&ticker, tick, NULL);
  os_timer_arm(&ticker, 1000, 1);
}

void display_set_time(time_t time) {
  char msg[22];
  os_sprintf(msg, "epochish - time = %d\n", epochish - time);
  uart0_send(msg);
  os_free(msg);

  epochish = time;
  clear();
  display_time(time);
  arm_timer();
}

void time_init(uint32_t offset) {
  int i = 2;
  draw_digit(1, i, digits[0]);
  draw_digit(1, i += 4, digits[0]);
  draw_digit(1, i += 3, digits[10]);
  draw_digit(1, i += 3, digits[0]);
  draw_digit(1, i += 4, digits[0]);
  draw_digit(1, i += 3, digits[10]);
  draw_digit(1, i += 3, digits[0]);
  draw_digit(1, i += 4, digits[0]);
}
