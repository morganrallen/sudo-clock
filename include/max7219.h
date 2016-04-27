#define OP_NOOP   0
#define OP_DIGIT0 1
#define OP_DIGIT1 2
#define OP_DIGIT2 3
#define OP_DIGIT3 4
#define OP_DIGIT4 5
#define OP_DIGIT5 6
#define OP_DIGIT6 7
#define OP_DIGIT7 8
#define OP_DECODEMODE  9
#define OP_INTENSITY   10
#define OP_SCANLIMIT   11
#define OP_SHUTDOWN    12
#define OP_DISPLAYTEST 15

#define DIN 13	// 13 - data in pin
#define CS 12	// 12 - load (CS) pin
#define CLK 14	// 14 - clock pin

typedef enum
{
    MSBFIRST = 0, LSBFIRST
} spiOrder_t;

#define HIGH 1
#define LOW 0

#define GPIO_OUT      (PERIPHS_GPIO_BASEADDR + 0x00)
#define GPIO_OUT_W1TS   (PERIPHS_GPIO_BASEADDR + 0x04)
#define GPIO_OUT_W1TC   (PERIPHS_GPIO_BASEADDR + 0x08)

#define GPIO_ENABLE     (PERIPHS_GPIO_BASEADDR + 0x0C)
#define GPIO_ENABLE_W1TS  (PERIPHS_GPIO_BASEADDR + 0x10)
#define GPIO_ENABLE_W1TC  (PERIPHS_GPIO_BASEADDR + 0x14)

#define GPIO_IN       (PERIPHS_GPIO_BASEADDR + 0x18)

void max7219_lightemup();
void max7219_init();
