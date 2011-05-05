/*
 * cc2520-arch.h
 *
 *  Created on: 5 mai 2011
 *      Author: tony
 */

#ifndef __CC2520_ARCH_H__
#define __CC2520_ARCH_H_

#include "spi-arch.h"

/** Input: FIFOP from CC2520 (Port). */
#define CC2520_FIFOP_PORT(type) P1##type
/** Input: FIFOP from CC2520 (Pin). */
#define CC2520_FIFOP_PIN        BIT6
/** Input: FIFO from CC2520 (Port). */
#define CC2520_FIFO_PORT(type)  P1##type
/** Input: FIFO from CC2520 (Pin). */
#define CC2520_FIFO_PIN         BIT5
/** Input: CCA from CC2520 (Port). */
#define CC2520_CCA_PORT(type)   P1##type
/** Input: CCA from CC2520 (Pin). */
#define CC2520_CCA_PIN          BIT7
/** Input: SFD from CC2520 (Port). */
#define CC2520_SFD_PORT(type)   P2##type
/** Input: SFD from CC2520 (Pin). */
#define CC2520_SFD_PIN          BIT0
/** Output: SPI Chip Select (CS_N) (Port). */
#define CC2520_CSN_PORT(type)   P3##type
/** Output: SPI Chip Select (CS_N) (Pin). */
#define CC2520_CSN_PIN          BIT0
/** Output: VREG_EN to CC2520 (Port). */
#define CC2520_VREG_PORT(type)  P4##type
/** Output: VREG_EN to CC2520 (Pin). */
#define CC2520_VREG_PIN         BIT3
/** Output: RESET_N to CC2520 (Port). */
#define CC2520_RESET_PORT(type) P4##type
/** Output: RESET_N to CC2520 (Pin). */
#define CC2520_RESET_PIN        BIT4

/** CC2520 IRQ Vector. */
#define CC2520_IRQ_VECTOR PORT1_VECTOR

#define CC2520_CONF_SYMBOL_LOOP_COUNT 800

/* Pin status.CC2520 */
#define CC2520_FIFOP_IS_1 (!!(CC2520_FIFOP_PORT(IN) & CC2520_FIFOP_PIN))
#define CC2520_FIFO_IS_1  (!!(CC2520_FIFO_PORT(IN)  & CC2520_FIFO_PIN))
#define CC2520_CCA_IS_1   (!!(CC2520_CCA_PORT(IN)   & CC2520_CCA_PIN))
#define CC2520_SFD_IS_1   (!!(CC2520_SFD_PORT(IN)   & CC2520_SFD_PIN))

/* The CC2520 reset pin. */
#define SET_RESET_INACTIVE()   (CC2520_RESET_PORT(OUT) |= CC2520_RESET_PIN)
#define SET_RESET_ACTIVE()     (CC2520_RESET_PORT(OUT) &= ~CC2520_RESET_PIN)

/* CC2520 voltage regulator enable pin. */
#define SET_VREG_ACTIVE()       (CC2520_VREG_PORT(OUT) |=  CC2520_VREG_PIN)
#define SET_VREG_INACTIVE()     (CC2520_VREG_PORT(OUT) &= ~CC2520_VREG_PIN)

/** ENABLE CSn (active low). */
#define CC2520_SPI_ENABLE() do {\
                              UCB0CTL1 &= ~UCSWRST;\
                              clock_delay(5);\
                              CC2520_CSN_PORT(OUT) &= ~CC2520_CSN_PIN;\
                              clock_delay(5);\
                            } while(0)

/** DISABLE CSn (active low). */
#define CC2520_SPI_DISABLE() do {\
                                clock_delay(5);\
                                UCB0CTL1 |= UCSWRST;\
                                clock_delay(1);\
                                CC2520_CSN_PORT(OUT) |= CC2520_CSN_PIN;\
                                clock_delay(5);\
                             } while(0)

/** Test if CSn is enabled. */
#define CC2520_SPI_IS_ENABLED() (!(CC2520_CSN_PORT(OUT) & CC2520_CSN_PIN))

/** CC2520 rising edge trigger for external interrupt 0 (FIFOP). */
#define CC2520_FIFOP_INT_INIT() do { \
    CC2520_FIFOP_PORT(IES) &= CC2520_FIFOP_PIN;  \
    CC2520_CLEAR_FIFOP_INT();                         \
  } while(0)

/* FIFOP on external interrupt 0. */
#define CC2520_ENABLE_FIFOP_INT()  do {\
                                     CC2520_FIFOP_PORT(IE) |= CC2520_FIFOP_PIN;\
                                   }  while(0)
#define CC2520_DISABLE_FIFOP_INT() do {\
                                     CC2520_FIFOP_PORT(IE) &= ~CC2520_FIFOP_PIN;\
                                   } while(0)
#define CC2520_CLEAR_FIFOP_INT()   do {\
                                     CC2520_FIFOP_PORT(IFG) &= ~CC2520_FIFOP_PIN;\
                                   } while (0)

#endif /* __CC2520_ARCH_H__ */
