/**
 * \defgroup wismote Wismote platform
 *
 * This is the module implementing Contiki on the Wismote platform.
 *
 * @{
 */

/**
 * \file
 *         Contiki configuration.
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

#ifndef __CONTIKI_CONF_H__
#define __CONTIKI_CONF_H__

/* ----- Compiler macros ----- */
/* For the CC_CONF_* macros, see : /core/sys/cc.h */
#define CC_CONF_REGISTER_ARGS 1
#define CC_CONF_INLINE  __inline__
#define CCIF
#define CLIF

/* ----- Various stuffs ----- */
/* See ./core/lib/petsciiconv.h */
#define WITH_ASCII 1

/* ----- Clock module ----- */
#define CLOCK_CONF_SECOND 128UL
/* XXX_PTV Change to long */
typedef unsigned short clock_time_t;

/* ----- Energest module ----- */
/** Enable energest. */
#define ENERGEST_CONF_ON 1

/* ----- Profile module ----- */
/** Disable profile module. */
#define PROFILE_CONF_ON 0

/* ----- Process module ----- */
#define PROCESS_CONF_NUMEVENTS 8
#define PROCESS_CONF_STATS 1

#ifndef CONTIKI_NO_NET
/* ----- UIP module ----- */
/* ---- General ---- */
typedef unsigned short uip_stats_t;
/** Globally enable logging. */
#define PLATFORM_LOGGING           1

#if PLATFORM_LOGGING
/** Enable UIP logging. */
#define UIP_CONF_LOGGING           1
/** Enable configuration logging. */
#define LOG_CONF_ENABLED           1
/** Enable UIP statistics. */
#define UIP_CONF_STATISTICS        1
#else
/** Disable UIP logging. */
#define UIP_CONF_LOGGING           0
/** Disable configuration logging. */
#define LOG_CONF_ENABLED           0
/** Disable UIP statistics. */
#define UIP_CONF_STATISTICS        0
#endif

/** Disable PING ADDR. CONF. */
#define UIP_CONF_PINGADDRCONF      0
/** Disable IP packet reassembly. */
#define UIP_CONF_REASSEMBLY        0

/* ---- DHCP ----- */
#define UIP_CONF_DHCP_LIGHT

/* ---- ICMP ---- */
/** Enable destination unreachable. */
#define UIP_CONF_ICMP_DEST_UNREACH 1

/* ---- UDP ---- */
/** Enable UDP compilation. */
#define UIP_CONF_UDP               1
/** Disable UDP checksum. */
#define UIP_CONF_UDP_CHECKSUMS     0

#if UIP_CONF_UDP
/** Number of simultaneous connection. */
#define UIP_CONF_UDP_CONNS         5
/** Enable broadcast. */
#define UIP_CONF_BROADCAST         1
#else
#define UIP_CONF_UDP_CONNS         0
/** Disable UDP broadcast. */
#define UIP_CONF_BROADCAST         0
#endif

/* ---- TCP ---- */
/** Enable TCP compilation. */
#define UIP_CONF_TCP               1

#if UIP_CONF_TCP
#ifndef UIP_CONF_RECEIVE_WINDOW
/** Default TCP Receive window. */
#define UIP_CONF_RECEIVE_WINDOW  48
#endif /* UIP_CONF_RECEIVE_WINDOW */
#ifndef UIP_CONF_TCP_MSS
/** Default TCP MSS. */
#define UIP_CONF_TCP_MSS         48
#endif /* UIP_CONF_TCP_MSS */
/** Enable open connection. */
#define UIP_CONF_ACTIVE_OPEN       1
/** Number of open connection. */
#define UIP_CONF_MAX_CONNECTIONS   5
/** Number of open ports. */
#define UIP_CONF_MAX_LISTENPORTS   5
/** Disable TCP split. */
#define UIP_CONF_TCP_SPLIT         0
#else /* UIP_CONF_TCP */
/** Disable open connex. */
#define UIP_CONF_ACTIVE_OPEN       0
/** No open connex. */
#define UIP_CONF_MAX_CONNECTIONS   0
/** No port available. */
#define UIP_CONF_MAX_LISTENPORTS   0
#endif /* UIP_CONF_TCP */

/* ---- Stacks ---- */
#ifndef NETSTACK_CONF_MAC
/** Default MAC Layer: CSMA. */
#define NETSTACK_CONF_MAC     csma_driver
#endif /* NETSTACK_CONF_MAC */

#ifndef NETSTACK_CONF_RDC
/** Default RDC Layer: Contiki MAC. */
#define NETSTACK_CONF_RDC     contikimac_driver
#endif /* NETSTACK_CONF_RDC */

#ifndef NETSTACK_CONF_RADIO
/** Default Radio Layer: CC2520. */
#define NETSTACK_CONF_RADIO   cc2520_driver
#endif /* NETSTACK_CONF_RADIO */

#ifndef NETSTACK_CONF_FRAMER
/** Default Framer Layer : Framer 802.15.4. */
#define NETSTACK_CONF_FRAMER  framer_802154
#endif /* NETSTACK_CONF_FRAMER */

/* ----- RDC Layer ----- */
#ifndef NETSTACK_CONF_RDC_CHANNEL_CHECK_RATE
#define NETSTACK_CONF_RDC_CHANNEL_CHECK_RATE 8
#endif /* NETSTACK_CONF_RDC_CHANNEL_CHECK_RATE */

/* ----- CC2520 ----- */
#ifndef CC2520_CONF_AUTOACK
/** Enable Auto-ACK by default */
#define CC2520_CONF_AUTOACK              1
#endif /* CC2520_CONF_AUTOACK */

/* ----- Contiki MAC Layer ----- */
#define CONTIKIMAC_CONF_BROADCAST_RATE_LIMIT 0

/* ----- Packet buffer ----- */
#define PACKETBUF_CONF_ATTRS_INLINE 1

/* ----- Radio ----- */
#ifndef RF_CHANNEL
/** RF Channel. */
#define RF_CHANNEL 26
#endif /* RF_CHANNEL */
/** Default IEEE 802.15.4 PAN ID: 0xBABE. */
#define IEEE802154_CONF_PANID                    0xBABE


#if WITH_UIP6
#ifndef NETSTACK_CONF_NETWORK
/** Default Network Layer: SICSLowPan. */
#define NETSTACK_CONF_NETWORK           sicslowpan_driver
#endif /* NETSTACK_CONF_NETWORK */

/* ----- SICSLOWPAN Layer ----- */
#define SICSLOWPAN_CONF_COMPRESSION_IPV6        0
#define SICSLOWPAN_CONF_COMPRESSION_HC1         1
#define SICSLOWPAN_CONF_COMPRESSION_HC01        2
#define SICSLOWPAN_CONF_COMPRESSION             SICSLOWPAN_COMPRESSION_HC06
#define SICSLOWPAN_CONF_CONVENTIONAL_MAC        1
#define SICSLOWPAN_CONF_FRAG                    1
#define SICSLOWPAN_CONF_MAXAGE                  8
#define SICSLOWPAN_CONF_MAX_ADDR_CONTEXTS       2
#define SICSLOWPAN_CONF_MAX_MAC_TRANSMISSIONS   5

/* ----- CXMAC Layer ----- */
#define CXMAC_CONF_ANNOUNCEMENTS         0

/* ----- XMAC Layer ----- */
#define XMAC_CONF_ANNOUNCEMENTS          0

#ifndef QUEUEBUF_CONF_NUM
#define QUEUEBUF_CONF_NUM                8
#endif

#define UIP_CONF_LL_802154              1
#define UIP_CONF_LLH_LEN                0
#define UIP_CONF_ROUTER                 1

/* ----- DS6 ----- */
#ifndef UIP_CONF_DS6_NBR_NBU
#define UIP_CONF_DS6_NBR_NBU     30
#endif /* UIP_CONF_DS6_NBR_NBU */
#ifndef UIP_CONF_DS6_ROUTE_NBU
#define UIP_CONF_DS6_ROUTE_NBU   30
#endif /* UIP_CONF_DS6_ROUTE_NBU */

/* ----- ND6 ----- */
#define UIP_CONF_ND6_SEND_RA            0
#define UIP_CONF_ND6_REACHABLE_TIME     600000
#define UIP_CONF_ND6_RETRANS_TIMER      10000
#define UIP_CONF_ND6_MAX_PREFIXES       3
#define UIP_CONF_ND6_MAX_NEIGHBORS      4
#define UIP_CONF_ND6_MAX_DEFROUTERS     2

#define UIP_CONF_NETIF_MAX_ADDRESSES    3

/* ----- IPv6 ----- */
#define UIP_CONF_IPV6                   1
#define UIP_CONF_IPV6_CHECKS            0
#define UIP_CONF_IPV6_REASSEMBLY        0
#define UIP_CONF_IPV6_QUEUE_PKT         0
#define UIP_CONF_IPV6_RPL               1

/* ----- IP ----- */
/** Disable IP Forward */
#define UIP_CONF_IP_FORWARD      1

#else /* WITH_UIP6 */
#ifndef NETSTACK_CONF_NETWORK
/** Default Network Layer: RIME. */
#define NETSTACK_CONF_NETWORK rime_driver
#endif /* NETSTACK_CONF_NETWORK */

#define COLLECT_CONF_ANNOUNCEMENTS       1

/* ----- RIME Layer ----- */
#define RIMEADDR_CONF_SIZE               8

/* ----- CXMAC Layer ----- */
#define CXMAC_CONF_ANNOUNCEMENTS         0
#define CXMAC_CONF_COMPOWER              1

/* ----- XMAC Layer ----- */
#define XMAC_CONF_ANNOUNCEMENTS          0
#define XMAC_CONF_COMPOWER               1

/* ----- Contiki MAC Layer ----- */
#define CONTIKIMAC_CONF_ANNOUNCEMENTS    0
#define CONTIKIMAC_CONF_COMPOWER         1

#ifndef COLLECT_NEIGHBOR_CONF_MAX_COLLECT_NEIGHBORS
#define COLLECT_NEIGHBOR_CONF_MAX_COLLECT_NEIGHBORS     32
#endif /* COLLECT_NEIGHBOR_CONF_MAX_COLLECT_NEIGHBORS */

#ifndef QUEUEBUF_CONF_NUM
#define QUEUEBUF_CONF_NUM                16
#endif /* QUEUEBUF_CONF_NUM */

#ifndef TIMESYNCH_CONF_ENABLED
#define TIMESYNCH_CONF_ENABLED           0
#endif /* TIMESYNCH_CONF_ENABLED */

#if TIMESYNCH_CONF_ENABLED
/* CC2520 SDF timestamps must be on if timesynch is enabled. */
#undef CC2520_CONF_SFD_TIMESTAMPS
#define CC2520_CONF_SFD_TIMESTAMPS       1
#endif /* TIMESYNCH_CONF_ENABLED */

/** Enable IP Forward */
#define UIP_CONF_IP_FORWARD             1

#endif /* WITH_UIP6 */

#else /* CONTIKI_NO_NET */

/** Disable UDP. */
#define UIP_CONF_UDP 0
/** Disable TCP. */
#define UIP_CONF_TCP 0
#endif

/* ----- CLOCK module ----- */
/** CLOCK: this platform uses XT1 clock. */
#define XT1_CONF_CLOCK 1
/** CLOCK: disable debug. */
#define SMCLOCK_CONF_DEBUG 0

/* ----- IOHANDLERS module ----- */
/** IOHANDLERS: disable debug. */
#define IOHANDLERS_CONF_DEBUG 0

/* ----- UART choice ----- */
/** UART1: SLIP doesn't use the MAIN_UART */
#define SLIP_USE_UART1  0
/** UART1: "stdio.h" uses the MAIN_UART */
#define STDIO_USE_UART1 0
/** Serial Line module uses the MAIN_UART */
#define SL_USE_UART1    0

/* ----- UART module ----- */
/** UART0: uses interrupt for TX. */
#define UART0_CONF_TX_WITH_INTERRUPT 1
/** UART0: size of the TX buffer. */
#define UART0_CONF_TX_BUFSIZE      128
/** UART1: uses interrupt for TX. */
#define UART1_CONF_TX_WITH_INTERRUPT 1
/** UART1: size of the TX buffer. */
#define UART1_CONF_TX_BUFSIZE      128

/* ----- Serial Line module ----- */
/** Buffer for the serial line reception buffer. */
#define SERIAL_LINE_CONF_BUFSIZE 64

/* ----- Diagnostic ----- */
/** Diagnostic use a JSON output. */
#define DIAGNOSTIC_CONF_OUTPUT JSON_OUTPUT

/* ----- Includes ----- */
#include "types.h"
#include <msp430f5437.h>

#endif /* __CONTIKI_CONF_H__ */

/** @} */

