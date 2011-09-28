#include "adc.h"

/*******************************************************************************
* @fn          ADC 
*
* @brief      This functions configure the ADC to sample on channel 6 or 8 
*             at 1MHz in 13 cycle.
*             The result of the conversion is the value of the thermo-resistore 
*             R18 on the board or of the luminosity sensor APDS9007.
*
* @param       none
* @return      none
*/
void  TEMP_ADC_INITIALIZATION()
{
	  ENABLE_TEMPERATURE_SENSOR(); //  shutdown off
	  ADC12CTL0 = ADC12SHT02 + ADC12ON;         // Sampling time, ADC12 on
	  ADC12CTL1 = ADC12SHP;                     // Use sampling timer
	  ADC12MCTL0 |= THERMORESISTANCE_CHANNEL;
	  P6SEL |= BIT6;      // P6.6 ADC option select CTN
	  P6DIR &= BIT6;		// P6.6 as Input
	  ADC12CTL0 |= ADC12ENC;  // Enable Conversion (ADC12_A enabled)
}

void LUM_ADC_INITIALIZATION()
{
	  ENABLE_LUMINOSITY_SENSOR(); //  shutdown off
	  ADC12CTL0 = ADC12SHT02 + ADC12ON;         // Sampling time, ADC12 on
	  ADC12CTL1 = ADC12SHP;                     // Use sampling timer
	  ADC12MCTL0 |= LUMINOSITY_SENSOR_CHANNEL;
	  P5SEL |= BIT0;      // P5.0 ADC option select LUMINOSITY sensor
	  P5DIR &= ~BIT0;     // P5.0 as INPUT
	  ADC12CTL0 |= ADC12ENC;  // Enable Conversion (ADC12_A enabled)
}

void  ADC_START_CONVERSION()
{
    ADC12IFG &= ~ADC12IFG0;        // Clear "conversion finished" Interrupt Flag
    ADC12CTL0 |= ADC12SC;          // Start sampling/conversion    
}

void TEMP_ADC_SHUTDOWNPOWER()
{
	SHUTDOWN_TEMPERATURE_SENSOR();
}

