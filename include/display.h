#ifndef DISPLAY_H
#define DISPLAY_H

extern int pause;
void clear();

static int digits[47][25] = {
  { // 0
    1, 1, 1,
    1, 0, 1,
    1, 0, 1,
    1, 0, 1,
    1, 1, 1
  },
  { // 1
    0, 1, 0,
    1, 1, 0,
    0, 1, 0,
    0, 1, 0,
    1, 1, 1
  },
  { // 2
    1, 1, 1,
    0, 0, 1,
    1, 1, 1,
    1, 0, 0,
    1, 1, 1
  },
  { // 3
    1, 1, 1,
    0, 0, 1,
    1, 1, 1,
    0, 0, 1,
    1, 1, 1
  },
  { // 4
    1, 0, 1,
    1, 0, 1,
    1, 1, 1,
    0, 0, 1,
    0, 0, 1
  },
  { // 5
    1, 1, 1,
    1, 0, 0,
    1, 1, 1,
    0, 0, 1,
    1, 1, 1
  },
  { // 6
    1, 1, 1,
    1, 0, 0,
    1, 1, 1,
    1, 0, 1,
    1, 1, 1
  },
  { // 7
    1, 1, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1
  },
  { // 8
    1, 1, 1,
    1, 0, 1,
    1, 1, 1,
    1, 0, 1,
    1, 1, 1
  },
  { // 9
    1, 1, 1,
    1, 0, 1,
    1, 1, 1,
    0, 0, 1,
    0, 0, 1
  },
  { // :
    0, 0, 0,
    0, 1, 0,
    0, 0, 0,
    0, 1, 0,
    0, 0, 0
  },
  { // A
    1, 1, 0,
    1, 0, 1,
    1, 1, 1,
    1, 0, 1,
    1, 0, 1
  },
  { // B
    1, 1, 1,
    1, 0, 1,
    1, 1, 0,
    1, 0, 1,
    1, 1, 1
  },
  { // C
    1, 1, 1,
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    1, 1, 1
  },
  { // D
    1, 1, 0,
    1, 0, 1,
    1, 0, 1,
    1, 0, 1,
    1, 1, 0
  },
  { // E
    1, 1, 1,
    1, 0, 0,
    1, 1, 1,
    1, 0, 0,
    1, 1, 1
  },
  { // F
    1, 1, 1,
    1, 0, 0,
    1, 1, 1,
    1, 0, 0,
    1, 0, 0
  },
  { // G
    1, 1, 1,
    1, 0, 0,
    1, 0, 0,
    1, 0, 1,
    1, 1, 1
  },
  { // H
    1, 0, 1,
    1, 0, 1,
    1, 1, 1,
    1, 0, 1,
    1, 0, 1
  },
  { // I
    1, 1, 1,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    1, 1, 1
  },
  { // J
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    1, 0, 1,
    1, 1, 1
  },
  { // K
    1, 0, 1,
    1, 0, 1,
    1, 1, 0,
    1, 0, 1,
    1, 0, 1
  },
  { // L
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    1, 1, 1
  },
  { // M
    1, 0, 0, 0, 1,
    1, 1, 0, 1, 1,
    1, 0, 1, 0, 1,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1
  },
  { // N
    1, 0, 0, 1,
    1, 0, 0, 1,
    1, 1, 0, 1,
    1, 0, 1, 1,
    1, 0, 0, 1
  },
  { // O
    1, 1, 1,
    1, 0, 1,
    1, 0, 1,
    1, 0, 1,
    1, 1, 1
  },
  { // P
    1, 1, 1,
    1, 0, 1,
    1, 1, 1,
    1, 0, 0,
    1, 0, 0
  },
  { // Q
    1, 1, 1, 0,
    1, 0, 1, 0,
    1, 0, 1, 0,
    1, 0, 1, 0,
    1, 1, 1, 1
  },
  { // R
    1, 1, 1,
    1, 0, 1,
    1, 1, 0,
    1, 0, 1,
    1, 0, 1
  },
  { // S
    1, 1, 1,
    1, 0, 0,
    1, 1, 1,
    0, 0, 1,
    1, 1, 1
  },
  { // T
    1, 1, 1,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0
  },
  { // U
    1, 0, 1,
    1, 0, 1,
    1, 0, 1,
    1, 0, 1,
    1, 1, 1
  },
  { // V
    1, 0, 1,
    1, 0, 1,
    1, 0, 1,
    1, 0, 1,
    0, 1, 0
  },
  { // W
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    1, 0, 1, 0, 1,
    1, 1, 0, 1, 1,
    1, 0, 0, 0, 1
  },
  { // X
    1, 0, 1,
    1, 0, 1,
    0, 1, 0,
    1, 0, 1,
    1, 0, 1
  },
  { // Y
    1, 0, 1,
    1, 0, 1,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0
  },
  { // Z
    1, 1, 1,
    0, 0, 1,
    1, 1, 1,
    0, 0, 0,
    1, 1, 1
  }
};

#endif
