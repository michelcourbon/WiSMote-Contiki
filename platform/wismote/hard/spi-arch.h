/**
 * \addtogroup wismote
 * @{
 */

/**
 * \file
 *         SPI platform-dependent routines.
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

#ifndef __SPI_ARCH_H__
#define __SPI_ARCH_H__

/** Macro to wait for TX ready. */
#define SPI_WAITFORTxREADY()    while(!(UCB0IFG & UCTXIFG))
/** Macro to wait for TX done.  */
#define SPI_WAITFOREOTx()       while(!(UCB0IFG & UCTXIFG))
/** Macro to wait for RX done.  */
#define SPI_WAITFOREORx()       while(!(UCB0IFG & UCRXIFG))
/** Macro for "busy wait".      */
#define SPI_BUSY_WAIT()         while((UCB0STAT & UCBUSY))
/** TX Buffer.                  */
#define SPI_TXBUF UCB0TXBUF
/** RX Buffer.                  */
#define SPI_RXBUF UCB0RXBUF

/** SPI Port. */
#define SPI_PORT(type) P3##type
/** CS1N pin. */
#define SPI_CS1N BIT0
/** MOSI pin. */
#define SPI_MOSI BIT1
/** MISO pin. */
#define SPI_MISO BIT2
/** CLK pin. */
#define SPI_CLK  BIT3

#define SPI_FLASH_ENABLE()      //( P4OUT &= ~BV(FLASH_CS) )
#define SPI_FLASH_DISABLE()     //( P4OUT |=  BV(FLASH_CS) )

#define SPI_FLASH_HOLD()        //( P4OUT &= ~BV(FLASH_HOLD) )
#define SPI_FLASH_UNHOLD()      //( P4OUT |=  BV(FLASH_HOLD) )

#endif

/** @} */
