# sudo-clock
It's a clock, for #sudoroom

# requirements
This requires the ESP8266 IoT SDK, after following it's build directions you
should have the paths needed for the next build step.

# wiring

| ESP | MATRIX |
|-----|--------|
| 13  |   DIN  |
| 12  |   CS   |
| 14  |   CLK  |

# build
```
  SDK_DIR=/path/to/esp_iot_sdk_v1.x/ ESPTOOL2=/path/to/esptool/esptool2 make
```

You can also edit the Makefile setting the paths to the SDK and esptool2.

# flash
  esptool.py --port COM2 write_flash -fs 8m 0x00000 firmware/0x00000.bin 0x40000 firmware/0x40000.bin

Tested with SDK v1.4 on an ESP12 (if using a board with less than 1mb of flash
a change to the second linker script will be required).

```
  miniterm.py /dev/ttyUSB0 115200 --rts=0 --dtr=0
```

NOTE: `--rts/--dtr` are only needed if you have these lines connected from your USB-Serial to
the ESP8266 for buttonless flashing.

# ITS A CLOCK!!!
![It's a clock!](https://github.com/morganrallen/sudo-clock/raw/master/extra/clock_v2.jpg)
