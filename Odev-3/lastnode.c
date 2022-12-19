#include "contiki.h"
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"

#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define WITH_SERVER_REPLY  1
#define UDP_CLIENT_PORT	8765
#define UDP_SERVER_PORT	5678

static struct simple_udp_connection udp_conn;

PROCESS(udp_server_process, "UDP server");
AUTOSTART_PROCESSES(&udp_server_process);
/*---------------------------------------------------------------------------*/
static void
udp_rx_callback(struct simple_udp_connection *c,
         const uip_ipaddr_t *sender_addr,
         uint16_t sender_port,
         const uip_ipaddr_t *receiver_addr,
         uint16_t receiver_port,
         const uint8_t *data,
         uint16_t datalen)
{
  LOG_INFO("Received response '%.*s' from ", datalen, (char *) data);
  // E�er gelen paket TURN_On ise A�a tekrar Turned_On paketi g�nderilir. 
  if(strcmp((char*)data, "LIGHT6: TURN_ON") == 0){
  	simple_udp_sendto(&udp_conn, (uint8_t*)"LIGHT6: TURNED_ON", sizeof("LIGHT6: TURNED_ON"), sender_addr);
  }
  // aksi halde wrong req paketi g�nderilir.
  else{
	simple_udp_sendto(&udp_conn, (uint8_t*)"WRONG REQ.", sizeof("WRONG REQ."), sender_addr);  	
  }
  
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_server_process, ev, data)
{
  PROCESS_BEGIN();

  /* Initialize DAG root */
  NETSTACK_ROUTING.root_start();
	
  /* Udp ba�lant�s� ger�eklenir.  */
  simple_udp_register(&udp_conn, UDP_SERVER_PORT, NULL,
                      UDP_CLIENT_PORT, udp_rx_callback);
                      
  
  PROCESS_END();
}
