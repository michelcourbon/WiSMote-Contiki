/*
 * tempinosity.c
 *
 *  Created on: 19 sept. 2011
 *      Author: michel
 */

#include "contiki.h"
//#include "lib/sensors.h"
#include "iohandlers.h"

#include "adc.h"

#include <stdint.h>
#include <stdio.h>

#define INTERVAL (CLOCK_CONF_SECOND * 3)

/*---------------------------------------------------------------------------*/
PROCESS(onboardsensor_process, "onboardsensor_process);");
AUTOSTART_PROCESSES(&onboardsensor_process);

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(onboardsensor_process, ev, data)
{
  static struct etimer et;
  static unsigned int value;

  PROCESS_BEGIN();
  TEMP_ADC_INITIALIZATION();
  //LUM_ADC_INITIALIZATION();
  ADC_START_CONVERSION(); // Start a new conversion

  while (1)
  {

	  /* Check temperature every INTERVAL */
    etimer_set(&et, INTERVAL);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    if(ADC12IFG & ADC12IFG0) // When sampling is over
    {
    	value = ADC12MEM0; // store result
       ADC_START_CONVERSION();      // Start a new conversion
    }


    /* Read the acquisition. */
    if (value == -1)
    {
      printf("value: N/A\n\r");
    }
    else
    {
      printf("value: %d\n\r", value);
    }

  }
  PROCESS_END();
}

/*---------------------------------------------------------------------------*/

