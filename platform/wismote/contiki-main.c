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
#include <signal.h>

/* From CONTIKI */
#include "contiki.h"
#include "dev/leds.h"
#include "dev/watchdog.h"
#include "dev/button-sensor.h"
#include "dev/sht11-sensor.h"
#if UIP_USE_DS2411_FOR_MAC_ADDRESS
#include "dev/ds2411.h"
#endif
#include "lib/sensors.h"

/* From MSP430x5xx */
#include "uart0.h"
#include "uart1.h"
#include "spi.h"
#include "spl.h"
#include "msp430.h"

/* From platform */
#include "diag.h"
#include "parallax_pir-555-28027.h"

/* If the macro aren't defined, we consider them like disabled. */
#ifndef WITH_UIP
#define WITH_UIP 0
#endif

#ifndef WITH_UIP6
#define WITH_UIP6 0
#endif

#ifndef CONTIKI_NO_NET
#define CONTIKI_NO_NET 0
#endif

#if CONTIKI_NO_NET || (!WITH_UIP && !WITH_UIP6)
#include "dev/serial-line.h"
#endif

#if !CONTIKI_NO_NET && (WITH_UIP || WITH_UIP6)
#include "dev/slip.h"
#include "net/tcpip.h"
#endif

#if !CONTIKI_NO_NET && WITH_UIP
#include "net/uip.h"
#include "net/uip-fw.h"
#include "net/uip-fw-drv.h"
static struct uip_fw_netif slipif =
  {UIP_FW_NETIF(192,168,1,2, 255,255,255,0, slip_send)};
#endif /* !CONTIKI_NO_NET && WITH_UIP */


#include "multsht15-sensors.h"
SENSORS(&button_sensor, &upper_sensor, &lower_sensor);
//SENSORS(&button_sensor);

/** Don't display the list of auto-processes before executing them. */
#define DEBUG_PROCESS 0
/** Don't display the list of sensors before starting them. */
#define DEBUG_SENSORS 0
/** Don't display the diagnostic port. */
#define DEBUG_DIAGNOSTIC 0
/** Display the DS2411 address. */
#define DEBUG_DS2411 1

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
#if UIP_USE_DS2411_FOR_MAC_ADDRESS
  /* Initialize the DS2411 */
  ds2411_init();
  ds2411_id[0] = 0x00;
  ds2411_id[1] = 0x12;
  ds2411_id[2] = 0x75;
#if DEBUG_DS2411
  printf("I'm %02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\n",
         ds2411_id[0], ds2411_id[1], ds2411_id[2], ds2411_id[3],
         ds2411_id[4], ds2411_id[5], ds2411_id[6], ds2411_id[7]);
#endif
#endif
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
  /* Initialize the Serial Line module */
#if CONTIKI_NO_NET || (!WITH_UIP && !WITH_UIP6)
#if !SL_USE_UART1
  uart0_set_input(serial_line_input_byte);
#else
  uart1_set_input(serial_line_input_byte);
#endif
  serial_line_init();
#endif

#if !CONTIKI_NO_NET && WITH_UIP
  uip_init();
  process_start(&tcpip_process, NULL);
  uip_fw_init();
  process_start(&uip_fw_process, NULL);
  process_start(&slip_process, NULL);
  {
      uip_ipaddr_t addr;

#if UIP_USE_DS2411_FOR_MAC_ADDRESS
      uip_ipaddr(&addr, 192,168,1, ds2411_id[7]);
#else
      uip_ipaddr(&addr, 192,168,1, 2);
#endif
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

#if !CONTIKI_NO_NET && (WITH_UIP || WITH_UIP6)
  process_start(&diagnostic_process, NULL);
#endif

#if DEBUG_DIAGNOSTIC && !CONTIKI_NO_NET && (WITH_UIP || WITH_UIP6)
  printf("Diagnostic :\n - local port: 7890\n - remote port: 7891\n");
#elif DEBUG_DIAGNOSTIC && (CONTIKI_NO_NET || (!WITH_UIP && !WITH_UIP6))
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
