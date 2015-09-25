#include <c_types.h>
#include <math.h>

#include "time.h"
#include "osapi.h"

#include "max7219.h"

int digits[11][15] = {
  {
    1, 1, 1,
    1, 0, 1,
    1, 0, 1,
    1, 0, 1,
    1, 1, 1
  },
  {
    0, 1, 0,
    1, 1, 0,
    0, 1, 0,
    0, 1, 0,
    1, 1, 1
  },
  {
    1, 1, 1,
    0, 0, 1,
    1, 1, 1,
    1, 0, 0,
    1, 1, 1
  },
  {
    1, 1, 1,
    0, 0, 1,
    1, 1, 1,
    0, 0, 1,
    1, 1, 1
  },
  {
    1, 0, 1,
    1, 0, 1,
    1, 1, 1,
    0, 0, 1,
    0, 0, 1
  },
  {
    1, 1, 1,
    1, 0, 0,
    1, 1, 1,
    0, 0, 1,
    1, 1, 1
  },
  {
    1, 1, 1,
    1, 0, 0,
    1, 1, 1,
    1, 0, 1,
    1, 1, 1
  },
  {
    1, 1, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1
  },
  {
    1, 1, 1,
    1, 0, 1,
    1, 1, 1,
    1, 0, 1,
    1, 1, 1
  },
  {
    1, 1, 1,
    1, 0, 1,
    1, 1, 1,
    0, 0, 1,
    0, 0, 1
  },
  {
    0, 0, 0,
    0, 1, 0,
    0, 0, 0,
    0, 1, 0,
    0, 0, 0
  }
};

void draw_digit(uint8_t r, uint8_t c, int* digit) {
  int i;
  int dev = 0;

  int rr, cc;

  for(i = 0; i <= 14; i++) {
    cc = c + ((i % 3));
    rr = r + (4 - floor(i / 3));

    dev = floor(cc / 8);
    cc -= dev * 8;
    setLed(dev, rr, cc, digit[i]);
  }
}

void time_init(uint32_t offset) {
  int i = 2;
  draw_digit(1, i, digits[2]);
  draw_digit(1, i += 4, digits[3]);
  draw_digit(1, i += 3, digits[10]);
  draw_digit(1, i += 3, digits[3]);
  draw_digit(1, i += 4, digits[6]);
  draw_digit(1, i += 3, digits[10]);
  draw_digit(1, i += 3, digits[4]);
  draw_digit(1, i += 4, digits[2]);
}
