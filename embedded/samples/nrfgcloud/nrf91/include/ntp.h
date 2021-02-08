#ifndef _NTP_H_
#define _NTP_H_

#include <zephyr.h>
#include <net/socket.h>
#include <stdio.h>
#include <drivers/uart.h>
#include <string.h>
#include <time.h>
#include "ntp.h"

struct ntp_format {
	u8_t flags;
	u8_t stratum; /* stratum */
	u8_t poll; /* poll interval */
	s8_t precision; /* precision */
	u32_t rootdelay; /* root delay */
	u32_t rootdisp; /* root dispersion */
	u32_t refid; /* reference ID */
	u32_t reftime_sec; /* reference time */
	u32_t reftime_frac; /* reference time */
	u32_t org_sec; /* origin timestamp */
	u32_t org_frac; /* origin timestamp */
	u32_t rec_sec; /* receive timestamp */
	u32_t rec_frac; /* receive timestamp */
	u32_t xmt_sec; /* transmit timestamp */
	u32_t xmt_frac; /* transmit timestamp */
};

int blocking_recv(int fd, u8_t *buf, u32_t size, u32_t flags);

int blocking_recvfrom(int fd, void *buf, u32_t size, u32_t flags,
		      struct sockaddr *src_addr, socklen_t *addrlen);

int blocking_send(int fd, u8_t *buf, u32_t size, u32_t flags);

int blocking_connect(int fd, struct sockaddr *local_addr, socklen_t len);

void ntp_swap_endianess(struct ntp_format *ntp);

//Function for getting unix time
u32_t ntp_get_unix_time(void);

#endif //_NTP_