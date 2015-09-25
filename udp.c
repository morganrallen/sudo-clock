#include "c_types.h"
#include "user_interface.h"
#include "espconn.h"
#include "mem.h"
#include "gpio.h"
#include "at_custom.h"
#include "osapi.h"
#include "time.h"

#include "ntp.h"
#include "display.h"
/**
 * Minimal example: Setup wifi for station mode, setup connection, wait for an IP.
 * If an IP has been assigned (we poll for that every 3s), start a udp socket. No remote
 * IP set, only local port set.
 * Expected: Broadcast UDP works.
 * Current behaviour: Only unicast traffic is accepted, no broadcast packets.
 */

uint8 ntp_server[] = {131, 107, 13, 100}; // microsoft
//uint8 ntp_server[] = {192, 168, 4, 229}; // microsoft

static struct espconn* pUdpServer;
static os_timer_t timer_check_connection;

static void ICACHE_FLASH_ATTR network_received(void *arg, char *data, unsigned short len) ;
static void ICACHE_FLASH_ATTR network_udp_start(void) ;
static void ICACHE_FLASH_ATTR timeout_func(void *arg);
static void ICACHE_FLASH_ATTR init_check_timer();

void udp_init(void)
{  
  init_check_timer();
}

static void ICACHE_FLASH_ATTR init_check_timer() {
  //Disarm timer
  os_timer_disarm(&timer_check_connection);

  //Setup and arm timer
  os_timer_setfn(&timer_check_connection, (os_timer_func_t *)timeout_func, 0);
  os_timer_arm(&timer_check_connection, 3000, 1);

  timeout_func(0);
}

static void ICACHE_FLASH_ATTR timeout_func(void *arg)
{
  // We execute this timer function as long as we do not have an IP assigned
  struct ip_info info;
  wifi_get_ip_info(STATION_IF, &info);

  uart0_send("...\n\r");

  if (wifi_station_get_connect_status() != STATION_GOT_IP || info.ip.addr == 0)
    return;

  // IP assigned, disarm timer
  os_timer_disarm(&timer_check_connection);

  network_udp_start();
}

static void ICACHE_FLASH_ATTR network_udp_start(void) 
{   
  ntp_t ntp;

  pUdpServer = (struct espconn *)os_zalloc(sizeof(struct espconn));
  pUdpServer->type=ESPCONN_UDP;
  pUdpServer->state=ESPCONN_NONE;
  pUdpServer->proto.udp= (esp_udp *)os_zalloc(sizeof(esp_udp));
  pUdpServer->proto.udp->local_port = espconn_port();                          // Set local port to 2222
  pUdpServer->proto.udp->remote_port = 123;                         // Set remote port
  os_memcpy(pUdpServer->proto.udp->remote_ip, ntp_server, 4);

  os_memset(&ntp, 0, sizeof(ntp_t));
	ntp.options = 0b00100011; // leap = 0, version = 4, mode = 3 (client)

  if(espconn_create(pUdpServer) == 0)
  {
    espconn_regist_recvcb(pUdpServer, network_received);
    uart0_send("UDP OK\n\r");
    espconn_sent(pUdpServer, (uint8*)&ntp, sizeof(ntp_t));
  }
}

static void ICACHE_FLASH_ATTR network_received(void *arg, char *data, unsigned short len) 
{
  uart0_send("derp");
  ntp_t *ntp;
  time_t timestamp;
	ntp = (ntp_t*)data;

	timestamp = ntp->trans_time[0] << 24 | ntp->trans_time[1] << 16 |ntp->trans_time[2] << 8 | ntp->trans_time[3];
	// convert to unix time
	timestamp -= 2208988800UL;
  timestamp -= 60 * 60 * 7;
  char msg[200];
  os_sprintf(msg, "time: %d", timestamp);
  uart0_send(msg);
  uart0_send(timestamp);
  display_set_time(timestamp);
}
