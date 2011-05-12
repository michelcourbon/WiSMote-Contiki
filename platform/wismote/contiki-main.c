/**
 * \addtogroup wismote
 * @{
 */

/**
 * \file
 *         Contiki main.
 * \author
 *         Anthony Gelibert <anthony.gelibert@lcis.grenoble-inp.fr>
 * \date
 *         March 21, 2011
 */

/*
 * Copyright (c) 2011, LCIS/CTSYS.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/* From MSP430-GCC */
#include <stdio.h>
#include <string.h>
#include <signal.h>

/* From CONTIKI */
#include "contiki.h"
#include "dev/leds.h"
#include "dev/watchdog.h"
#include "dev/spi.h"
#include "dev/button-sensor.h"
#include "dev/sht11-sensor.h"
#include "lib/sensors.h"

/* From MSP430x5xx */
#include "uart0.h"
#include "uart1.h"
#include "spl.h"
#include "msp430.h"

/* From platform */
#include "contiki-conf.h"
#include "diag.h"
#include "cc2520.h"
#include "parallax_pir-555-28027.h"

/* If the macro aren't defined, we consider them like disabled. */
#ifndef WITH_UIP
/** By default uIP is disabled. */
#define WITH_UIP 0
#endif

#ifndef WITH_UIP6
/** By default uIP6 is disabled. */
#define WITH_UIP6 0
#endif

#ifndef CONTIKI_NO_NET
/** By default don't use network. */
#define CONTIKI_NO_NET 0
#endif

#if CONTIKI_NO_NET || (!WITH_UIP && !WITH_UIP6)
#include "dev/serial-line.h"
#endif

#if !CONTIKI_NO_NET && (WITH_UIP || WITH_UIP6)
#include "dev/slip.h"
#include "net/tcpip.h"
#include "net/netstack.h"
#ifdef IEEE802154_CONF_PANID
#define IEEE802154_PANID (IEEE802154_CONF_PANID)
#else
#define IEEE802154_PANID (0xBABE)
#endif

#endif /* !CONTIKI_NO_NET && (WITH_UIP || WITH_UIP6) */

#if !CONTIKI_NO_NET && WITH_UIP
#include "net/uip.h"
#include "net/uip-fw.h"
#include "net/uip-fw-drv.h"
#include "net/uip-over-mesh.h"
static struct uip_fw_netif slipif =
  {UIP_FW_NETIF(192,168,1,2, 255,255,255,0, slip_send)};
static struct uip_fw_netif meshif =
  {UIP_FW_NETIF(172,16,0,0, 255,255,0,0, uip_over_mesh_send)};
#endif /* !CONTIKI_NO_NET && WITH_UIP */

#if !CONTIKI_NO_NET && WITH_UIP6
#include "net/uip6.h"
#include "net/uip-ds6.h"
#endif /* !CONTIKI_NO_NET && WITH_UIP6 */

SENSORS(&PIR_555_28027_sensor, &button_sensor, &sht11_sensor);

/** Doesn't display the list of auto-processes before executing them. */
#define DEBUG_PROCESS 0
/** Doesn't display the list of sensors before starting them. */
#define DEBUG_SENSORS 0
/** Display the MAC address. */
#define DEBUG_MAC 1
/** Enable the diagnostic port. */
#define DEBUG_DIAGNOSTIC 1
/** Enable radio. */
#define DEBUG_RADIO 1

#if DEBUG_PROCESS
/*---------------------------------------------------------------------------*/
static void
print_processes(struct process * const processes[])
{
  printf("Starting");
  while(*processes != NULL) {
    printf(" '%s'", PROCESS_NAME_STRING(*processes));
    processes++;
  }
  putchar('\n');
}
/*---------------------------------------------------------------------------*/
#endif /* DEBUG_PROCESS */

#if DEBUG_SENSORS
/*---------------------------------------------------------------------------*/
static void
print_sensors(void)
{
  struct sensors_sensor * sensor = sensors_first();

  printf("Sensors (%d): ", SENSORS_NUM - 1);
  while (sensor != NULL) {
    printf(" '%s' ", sensor->type);
    sensor = sensors_next(sensor);
  }
  putchar('\n');
}
/*---------------------------------------------------------------------------*/
#endif /* DEBUG_SENSORS */

/*---------------------------------------------------------------------------*/

#if !CONTIKI_NO_NET && (WITH_UIP || WITH_UIP6)

static void
set_rime_addr(void)
{
  rimeaddr_t n_addr;

  memset(&n_addr, 0, sizeof(rimeaddr_t));
#if WITH_UIP6
  n_addr.u8[7] = 0x02;
  n_addr.u8[6] = 0x00;
#else
  n_addr.u8[0] = 0x00;
  n_addr.u8[1] = 0x02;
#endif

  rimeaddr_set_node_addr(&n_addr);
}

/*---------------------------------------------------------------------------*/

static uint8_t is_gateway;

static void
set_gateway(void)
{
  if(!is_gateway) {
    leds_on(LEDS_RED);
    printf("%d.%d: making myself the IP network gateway.\n\n",
           rimeaddr_node_addr.u8[0], rimeaddr_node_addr.u8[1]);
    printf("IPv4 address of the gateway: %d.%d.%d.%d\n\n",
           uip_ipaddr_to_quad(&uip_hostaddr));
    uip_over_mesh_set_gateway(&rimeaddr_node_addr);
    uip_over_mesh_make_announced_gateway();
    is_gateway = 1;
  }
}

#endif /* !CONTIKI_NO_NET && (WITH_UIP || WITH_UIP6) */

/*---------------------------------------------------------------------------*/

int
main(void)
{
  /* Initialize the msp430 */
  cpu_init();
  /* Initialize the clock of the card and the clock module */
  clock_init();
  /* Initialize the LEDs */
  leds_init();

  leds_on(LEDS_RED);
  /* Initialize the UART */
  /* See MSP430x5xx/6xx Family User's Guide p. 588 */
  // 9600   -> uart0_init(416,UCBRS_6,UCBRF_0);   OK
  // 38400  -> uart0_init(104, UCBRS_1, UCBRF_0); OK
  // 57600  -> uart0_init(69, UCBRS_4,UCBRF_0);   OK
  // 115200 -> uart0_init(34,UCBRS_6,UCBRF_0);    NOK
  uart0_init(69, UCBRS_4,UCBRF_0);
  uart1_init(69, UCBRS_4,UCBRF_0);

  leds_off(LEDS_RED);

  leds_on(LEDS_BLUE);
  /* Initialize the SPI bus */
  spi_init();
  leds_off(LEDS_BLUE);

#if !CONTIKI_NO_NET && (WITH_UIP || WITH_UIP6)
  slip_arch_init(0);
#endif
  leds_on(LEDS_GREEN);

  /* Initialize the RTimer */
  rtimer_init();
  /* Initialize the "process system" (core/sys/process.h)     */
  process_init();
  /* Initialize the ETimer module */
  process_start(&etimer_process, NULL);
  /* Initialize the CTimer module */
  ctimer_init();

#if !CONTIKI_NO_NET && (WITH_UIP || WITH_UIP6)
  set_rime_addr();
  cc2520_init();
  {
    uint8_t longaddr[8];
    uint16_t shortaddr;

    shortaddr = (rimeaddr_node_addr.u8[0] << 8) | rimeaddr_node_addr.u8[1];
    memset(longaddr, 0, sizeof(longaddr));
    rimeaddr_copy((rimeaddr_t *)&longaddr, &rimeaddr_node_addr);

#if DEBUG_MAC
    printf("MAC %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x\n",
           longaddr[0], longaddr[1], longaddr[2], longaddr[3],
           longaddr[4], longaddr[5], longaddr[6], longaddr[7]);
#endif /* !DEBUG_MAC */
    cc2520_set_pan_addr(IEEE802154_PANID, shortaddr, longaddr);
  }
  cc2520_set_channel(RF_CHANNEL);

#if WITH_UIP6
  {
    int i;
    for(i =0;i<RIMEADDR_SIZE;i++)
      uip_lladdr.addr[i] = rimeaddr_node_addr.u8[i];
  }
#endif /* WITH_UIP6 */
  queuebuf_init();
  NETSTACK_RDC.init();
  NETSTACK_MAC.init();
  NETSTACK_NETWORK.init();
#if DEBUG_RADIO
  printf("%s %s, channel check rate %lu Hz, radio channel %u\n",
         NETSTACK_MAC.name,
         NETSTACK_RDC.name,
         CLOCK_SECOND / (NETSTACK_RDC.channel_check_interval() == 0 ? 1 : NETSTACK_RDC.channel_check_interval()),
         RF_CHANNEL);
#endif /* DEBUG_RADIO */

#endif /* !CONTIKI_NO_NET */

  /* Initialize the Serial Line module */
#if CONTIKI_NO_NET || (!WITH_UIP && !WITH_UIP6)
#if !SL_USE_UART1
  uart0_set_input(serial_line_input_byte);
#else /* !SL_USE_UART1 */
  uart1_set_input(serial_line_input_byte);
#endif /* !SL_USE_UART1 */
  serial_line_init();
#endif


#if TIMESYNCH_CONF_ENABLED
  timesynch_init();
  timesynch_set_authority_level((rimeaddr_node_addr.u8[0] << 4) + 16);
#endif /* TIMESYNCH_CONF_ENABLED */

#if !CONTIKI_NO_NET && WITH_UIP
  uip_init();
  process_start(&tcpip_process, NULL);
  uip_fw_init();
  process_start(&uip_fw_process, NULL);
  process_start(&slip_process, NULL);
  slip_set_input_callback(set_gateway);
  {
      uip_ipaddr_t addr;

      uip_ipaddr(&addr, 192,168,1,2);
      uip_sethostaddr(&addr);
#if UIP_LOGGING
      printf("IP Address: %d.%d.%d.%d\n", uip_ipaddr_to_quad(&addr));
#endif

      uip_ipaddr(&addr,192,168,1,1);
      uip_setdraddr(&addr);
#if UIP_LOGGING
      printf("Def Router Address: %d.%d.%d.%d\n", uip_ipaddr_to_quad(&addr));
#endif

      uip_ipaddr(&addr, 255,255,255,0);
      uip_setnetmask(&addr);
#if UIP_LOGGING
      printf("Netmask Address: %d.%d.%d.%d\n", uip_ipaddr_to_quad(&addr));
#endif

  }
  uip_fw_default(&slipif);
#endif

  /* Initialize the EnerGest module */
  energest_init();
  /* SETUP : END */

  ENERGEST_ON(ENERGEST_TYPE_CPU);
  watchdog_start();

  printf(CONTIKI_VERSION_STRING " started.\n");

  /* Initialize the sensors */
  process_start(&sensors_process, NULL);
#if DEBUG_SENSORS
  print_sensors();
#endif

#if DEBUG_DIAGNOSTIC && (WITH_UIP || WITH_UIP6)
  process_start(&diagnostic_process, NULL);
  printf("Diagnostic :\n - local port: 7890\n - remote port: 7891\n");
#else
  printf("No diagnostic.\n");
#endif

  /* Start the processes */
#if DEBUG_PROCESS
  print_processes(autostart_processes);
#endif /* DEBUG_PROCESS */
  autostart_start(autostart_processes);

  while (1) {
    int r;

    do {
      watchdog_periodic();
      r = process_run();
    }
    while (r > 0);

    /* Idle processing */
    int s = splhigh(); /* Disable interrupts. */
    /* uart1_active is for avoiding LPM3 when still sending or receiving */
    if (process_nevents() != 0 || uart0_active() || uart1_active()) {
      /* Re-enable interrupts. */
      splx(s);
    } else {
      static unsigned long irq_energest = 0;
      /* Re-enable interrupts and go to sleep atomically. */
      ENERGEST_OFF(ENERGEST_TYPE_CPU);
      ENERGEST_ON(ENERGEST_TYPE_LPM);
      /* We only want to measure the processing done in IRQs when we
       are asleep, so we discard the processing time done when we
       were awake. */
      energest_type_set(ENERGEST_TYPE_IRQ, irq_energest);
      watchdog_stop();
      /* XXX_PTV Ici, il faut ajouter une mise en veille dans certain cas.
       * Voir la plateforme Sky.
       * _BIS_SR(LPM3_bits);
       */
      dint();
      irq_energest = energest_type_time(ENERGEST_TYPE_IRQ);
      eint();
      watchdog_start();
      ENERGEST_OFF(ENERGEST_TYPE_LPM);
      ENERGEST_ON(ENERGEST_TYPE_CPU);
    }
  }
  return 0;
}

/*---------------------------------------------------------------------------*/

#if UIP_LOGGING
void uip_log(char *msg)
{
  printf("uIP: %s\n",msg);
}
#endif

/*---------------------------------------------------------------------------*/

#if LOG_CONF_ENABLED
void log_message(const char *part1, const char *part2)
{
  printf("log_message: %s / %s\n",part1,part2);
}
#endif

/*---------------------------------------------------------------------------*/

/** @} */
