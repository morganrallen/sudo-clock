# sudo-clock
It's a clock, for #sudoroom

# requirements
This uses [raburton/esp8266 rboot](https://github.com/raburton/esp8266/tree/master/rboot) as it's base,
this handle Wifi and OTA updates automagically. It also requires the ESP8266 IoT SDK, after following
it's build directions you should have the paths needed for the next build step.


# build
```
  SDK_DIR=/path/to/esp_iot_sdk_v1.x/ ESPTOOL2=/path/to/esptool/esptool2 make
```

You can also edit the Makefile setting the paths to the SDK and esptool2.

# flash
  esptool.py --port COM2 write_flash -fs 8m 0x00000 rboot.bin 0x02000 rom0.bin 0x82000 rom1.bin

Tested with SDK v1.4 on an ESP12 (if using a board with less than 1mb of flash
a change to the second linker script will be required).

# OTA Update
Connect to the device over you serial port and issue the `ota` command once connected to Wifi.
Wifi connenection can be checked with `ip` or initialed for the first time with `connect`.
This is MUCH faster than flashing. It is recommended to use it during develeopment.

```
  miniterm.py /dev/ttyUSB0 115200 --rts=0 --dtr=0
```

NOTE: `--rts/--dtr` are only needed if you have these lines connected from your USB-Serial to
the ESP8266 for buttonless flashing.
