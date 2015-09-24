#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"

#include "eagle_soc.h"

#include "max7219.h"

char spidata[16];
char status[64];
int maxDevices;

void digitalWrite(uint8 pin, uint8 value) {
  WRITE_PERI_REG(GPIO_OUT,
      (READ_PERI_REG(GPIO_OUT) & (uint32) (~(0x01 << pin)))
      | (value << pin));
}

void max7219_shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val)
{
	uint8_t i;

	for (i = 0; i < 8; i++)  {
		if (bitOrder == LSBFIRST)
			digitalWrite(dataPin, !!(val & (1 << i)));
		else	
			digitalWrite(dataPin, !!(val & (1 << (7 - i))));
			
		digitalWrite(clockPin, 1);
		digitalWrite(clockPin, 0);		
	}
}

void spiTransfer(int addr, volatile char opcode, volatile char data) {
    //Create an array with the data to shift out
    int offset = addr * 2;
    int maxbytes = maxDevices * 2;
    int i;

    for(i = 0;i < maxbytes;i ++ )
        spidata[i] = 0;
    //put our device data into the array
    spidata[offset + 1] = opcode;
    spidata[offset] = data;
    //enable the line 
    digitalWrite(CS, 0);
    //Now shift out the data 
    for(i = maxbytes; i > 0; i -- )
        max7219_shiftOut(DIN, CLK, MSBFIRST, spidata[i - 1]);
    //latch the data onto the display
    digitalWrite(CS, 1);
}    

void setLed(int addr, int column, int row, bool state) {
    int offset;
    char val=0x00;

    if(addr<0 || addr>=maxDevices)
        return;
    if(row<0 || row>7 || column<0 || column>7)
        return;
    offset=addr*8;
    val=0b10000000 >> column;
    if(state)
        status[offset+row]=status[offset+row]|val;
    else {
        val=~val;
        status[offset+row]=status[offset+row]&val;
    }
    spiTransfer(addr, row+1,status[offset+row]);
}

void setScanLimit(int addr, int limit) {
    if(addr<0 || addr>=maxDevices)
        return;
    if(limit>=0 && limit<8)
        spiTransfer(addr, OP_SCANLIMIT,limit);
}

void shutdown(int addr, bool b) {
    if(addr<0 || addr>=maxDevices)
        return;
    if(b)
        spiTransfer(addr, OP_SHUTDOWN,0);
    else
        spiTransfer(addr, OP_SHUTDOWN,1);
}

void setIntensity(int addr, int intensity) {
    if(addr<0 || addr>=maxDevices)
        return;
    if(intensity>=0 && intensity<16)	
        spiTransfer(addr, OP_INTENSITY,intensity);
}

void max7219_lightemup()
{
  int d;
  int x;
  int y;

  for(d = 0; d < maxDevices; d++) {
    for(x = 0; x < 8; x++) {
      for(y = 0; y < 8; y++) {
        setLed(d, x, y, true);
        os_delay_us(9000);
      }
    }
  }
}
void clearDisplay(int addr) {
    int offset;
    int i;

    if(addr<0 || addr>=maxDevices)
        return;
    offset=addr*8;

    for(i=0;i<8;i++) {
        status[offset+i]=0;
        spiTransfer(addr, i+1,status[offset+i]);
    }
}

void max7219_init() {
  PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO12);
  PIN_PULLUP_DIS(PERIPHS_IO_MUX_MTDI_U);
  WRITE_PERI_REG(GPIO_ENABLE_W1TS, 0x1 << CS);

  PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, FUNC_GPIO13);
  PIN_PULLUP_DIS(PERIPHS_IO_MUX_MTCK_U);
  WRITE_PERI_REG(GPIO_ENABLE_W1TS, 0x1 << DIN);

  PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U, FUNC_GPIO14);
  PIN_PULLUP_DIS(PERIPHS_IO_MUX_MTMS_U);
  WRITE_PERI_REG(GPIO_ENABLE_W1TS, 0x1 << CLK);

  digitalWrite(CS, 1);

  int i;
  int numDevices = 4;

  maxDevices = numDevices;

  for(i=0;i<64;i++) 
    status[i]=0x00;

  for(i=0;i<maxDevices;i++) {
    spiTransfer(i, OP_DISPLAYTEST, 0);
    //scanlimit is set to max on startup
    setScanLimit(i, 7);
    //decode is done in source
    spiTransfer(i, OP_DECODEMODE, 0);
    
    clearDisplay(i);
    setIntensity(i, 7);
    //we go into shutdown-mode on startup
    shutdown(i, true);
    shutdown(i, false);
  }
}
