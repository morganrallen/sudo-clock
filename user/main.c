/*
 * sudo-clock
 * Author: Morgan 'ARR!' Allen <morganrallen@gmail.com>
 * Description: Clock built for sudoroom.org
 */

#include <c_types.h>
#include <osapi.h>
#include <user_interface.h>
#include <time.h>
#include <mem.h>

#include "main.h"
#include "user_config.h"
#include "uart.h"

#include "max7219.h"
#include "display.h"

#include "udp.h"

static os_timer_t network_timer;

void ICACHE_FLASH_ATTR network_wait_for_ip() {
	struct ip_info ipconfig;

	os_timer_disarm(&network_timer);
	wifi_get_ip_info(STATION_IF, &ipconfig);

	if (wifi_station_get_connect_status() == STATION_GOT_IP && ipconfig.ip.addr != 0) {
		char page_buffer[40];
		os_sprintf(page_buffer,"ip: %d.%d.%d.%d\r\n",IP2STR(&ipconfig.ip));
		uart0_send(page_buffer);
	} else {
		char page_buffer[40];
		os_sprintf(page_buffer,"network retry, status: %d\r\n",wifi_station_get_connect_status());
		if(wifi_station_get_connect_status() == 3) wifi_station_connect();
		uart0_send(page_buffer);
		os_timer_setfn(&network_timer, (os_timer_func_t *)network_wait_for_ip, NULL);
		os_timer_arm(&network_timer, 2000, 0);
	}
}

void ICACHE_FLASH_ATTR wifi_config_station() {
	struct station_config stationConf;

	wifi_set_opmode(0x1);
	stationConf.bssid_set = 0;
	os_strcpy(&stationConf.ssid, WIFI_SSID, os_strlen(WIFI_SSID));
  if(os_strlen(WIFI_PWD) > 0) {
    os_strcpy(&stationConf.password, WIFI_PWD, os_strlen(WIFI_PWD));
  }
	wifi_station_set_config(&stationConf);
	uart0_send("wifi connecting...\r\n");
	wifi_station_connect();
	os_timer_disarm(&network_timer);
	os_timer_setfn(&network_timer, (os_timer_func_t *)network_wait_for_ip, NULL);
	os_timer_arm(&network_timer, 2000, 0);
}

void ICACHE_FLASH_ATTR ShowIP() {
	struct ip_info ipconfig;
	char msg[50];
	wifi_get_ip_info(STATION_IF, &ipconfig);
	if (wifi_station_get_connect_status() == STATION_GOT_IP && ipconfig.ip.addr != 0) {
		os_sprintf(msg, "ip: %d.%d.%d.%d, mask: %d.%d.%d.%d, gw: %d.%d.%d.%d\r\n",
			IP2STR(&ipconfig.ip), IP2STR(&ipconfig.netmask), IP2STR(&ipconfig.gw));
	} else {
		os_sprintf(msg, "network status: %d\r\n", wifi_station_get_connect_status());
	}
	uart0_send(msg);
}

/*
void ICACHE_FLASH_ATTR ShowInfo() {
	char msg[50];

    os_sprintf(msg, "\r\nSDK: v%s\r\n", system_get_sdk_version());
    uart0_send(msg);

    os_sprintf(msg, "Free Heap: %d\r\n", system_get_free_heap_size());
    uart0_send(msg);

    os_sprintf(msg, "CPU Frequency: %d MHz\r\n", system_get_cpu_freq());
    uart0_send(msg);

    os_sprintf(msg, "System Chip ID: 0x%x\r\n", system_get_chip_id());
    uart0_send(msg);

    os_sprintf(msg, "SPI Flash ID: 0x%x\r\n", spi_flash_get_id());
    uart0_send(msg);

    os_sprintf(msg, "SPI Flash Size: %d\r\n", (1 << ((spi_flash_get_id() >> 16) & 0xff)));
    uart0_send(msg);
}
*/

void ICACHE_FLASH_ATTR ProcessCommand(char* str) {
	if (!strcmp(str, "help")) {
		uart0_send("available commands\r\n");
		uart0_send("  help - display this message\r\n");
		uart0_send("  ip - show current ip address\r\n");
		uart0_send("  connect - connect to wifi\r\n");
		uart0_send("  restart - restart the esp8266\r\n");
		uart0_send("  info - show esp8266 info\r\n");
		uart0_send("  time - get time from ds1307\r\n");
		uart0_send("\r\n");
	} else if (!strcmp(str, "connect")) {
		wifi_config_station();
	} else if (!strcmp(str, "restart")) {
		uart0_send("Restarting...\r\n\r\n");
		system_restart();
	} else if (!strcmp(str, "ip")) {
		ShowIP();
	} else if (!strcmp(str, "info")) {
		//ShowInfo();
	} else if (!strcmp(str, "arm")) {
    toggle_animate();
	}
}

static os_timer_t update_time_timer;

void ICACHE_FLASH_ATTR user_init(void) {
	char msg[50];

	uart_init(BIT_RATE_115200,BIT_RATE_115200);

  gpio_init();
  max7219_init();
  uart0_send("max7219 init done\n");

  time_init();

  uart0_send("time init done\n\n");

	uart0_send("type \"help\" and press <enter> for help...\r\n");
  udp_init();
}
