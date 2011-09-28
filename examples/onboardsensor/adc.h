/*******************************************************************************
* @fn          ADC 
*
* @brief      This functions configure the ADC to sample on channel 6 or 8 
*             at 1MHz in 13 cycle.
*             The result of the conversion is the value of the luminosity sensor APDS9007.
*
* @param       none
* @return      none
*/
#include <msp430f5437.h>

// On board sensor
#define LUMINOSITY_SENSOR_CHANNEL  ADC12INCH3               //  Select channel 8
#define THERMORESISTANCE_CHANNEL   ADC12INCH2 | ADC12INCH1  //  Select channel 6

// APDS9007 shutdown PIN P4.2
#define ENABLE_LUMINOSITY_SENSOR() P4DIR |= BIT2;   P4OUT &= ~BIT2  
#define SHUTDOWN_LUMINOSITY_SENSOR() P4DIR |= BIT2;   P4OUT |= BIT2

// CTN power PIN P2.6/ACLK
#define ENABLE_TEMPERATURE_SENSOR() P2DIR |= BIT6;   P2OUT |= BIT6
#define SHUTDOWN_TEMPERATURE_SENSOR() P2DIR |= BIT6;   P2OUT &= ~BIT6

void  	TEMP_ADC_INITIALIZATION();
void 	LUM_ADC_INITIALIZATION();
void  	ADC_START_CONVERSION();
void 	TEMP_ADC_SHUTDOWNPOWER();
void 	LUM_ADC_SHUTDOWNPOWER();
