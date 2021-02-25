/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#ifndef _NTP_H_
#define _NTP_H_

#include <zephyr.h>
#include <net/socket.h>
#include <stdio.h>
#include <drivers/uart.h>
#include <string.h>
#include <time.h>
#include "ntp.h"

struct ntp_format
{
	uint8_t flags;
	uint8_t stratum;		/* stratum */
	uint8_t poll;			/* poll interval */
	uint8_t precision;		/* precision */
	uint32_t rootdelay;	/* root delay */
	uint32_t rootdisp;		/* root dispersion */
	uint32_t refid;		/* reference ID */
	uint32_t reftime_sec;  /* reference time */
	uint32_t reftime_frac; /* reference time */
	uint32_t org_sec;		/* origin timestamp */
	uint32_t org_frac;		/* origin timestamp */
	uint32_t rec_sec;		/* receive timestamp */
	uint32_t rec_frac;		/* receive timestamp */
	uint32_t xmt_sec;		/* transmit timestamp */
	uint32_t xmt_frac;		/* transmit timestamp */
};

/**
 * @brief Receive data from a connected peer while blocking.
 * 
 * @note The recv() function is equivalent to recvfrom() with null pointer
 *       src_addr and addrlen arguments, and to read() if the socket
 *       argument refers to a socket and the flags argument is 0.
 * @note The select() and poll() functions can be used to determine when data
 *       is available to be received.
 * @note For a description of the flags parameter, see the full normative
 * 		 description at `POSIX.1-2017 article
 * 		 <http://pubs.opengroup.org/onlinepubs/9699919799/functions/recv.html>`
 * 
 * @param[in] fd Specifies the socket file descriptor.
 * @param[in] buf Points to a buffer where the message should be stored.
 * @param[in] size Specifies the length in bytes of the buffer pointed to by
 *            the buffer argument.
 * @param[in] flags Specifies the type of message reception.
 * 
 * @retval Upon successful completion, recv() shall return the length of the
 *         message in bytes. If no messages are available to be received and
 *         the peer has performed an orderly shutdown, recv() shall return 0.
 *         Otherwise, -1 shall be returned and errno set to indicate the error.
 */
int blocking_recv(int fd, uint8_t *buf, uint32_t size, uint32_t flags);

/**
 * @brief Receive data from an arbitrary network address while blocking.
 * 
 * @note The select() and poll() functions can be used to determine when data
 *       is available to be received.
 * @note For a description of the flags parameter, see the full normative
 * 		 description at `POSIX.1-2017 article
 * 		 <http://pubs.opengroup.org/onlinepubs/9699919799/functions/recvfrom.html>`
 * 
 * @param[in] fd Specifies the socket file descriptor.
 * @param[in] buf Points to the buffer where the message should be stored.
 * @param[in] size Specifies the length in bytes of the buffer pointed to by
 *            the buffer argument.
 * @param[in] flags Specifies the type of message reception.
 * @param[in] src_addr A null pointer, or points to a sockaddr structure in
 *            which the sending address is to be stored. The length and format
 *            of the address depend on the address family of the socket.
 * @param[in] addrlen Either a null pointer, if address is a null pointer, or a
 *            pointer to a socklen_t object which on input specifies the length
 *            of the supplied sockaddr structure, and on output specifies the
 *            length of the stored address.
 * 
 * @retval Upon successful completion, recvfrom() shall return the length of
 *         the message in bytes. If no messages are available to be received
 *         and the peer has performed an orderly shutdown, recvfrom() shall
 *         return 0. Otherwise, the function shall return -1 and set errno to
 *         indicate the error.
 */
int blocking_recvfrom(int fd, void *buf, uint32_t size, uint32_t flags,
					  struct sockaddr *src_addr, socklen_t *addrlen);

/**
 * @brief Send data to a connected peer while blocking.
 * 
 * @note If the socket argument refers to a connection-mode socket, the send()
 *       function is equivalent to sendto() (with any value for the dest_addr
 *       and dest_len arguments, as they are ignored in this case). If the
 *       socket argument refers to a socket and the flags argument is 0, the
 *       send() function is equivalent to write().
 * @note For a description of the flags parameter, see the full normative
 * 		 description at `POSIX.1-2017 article
 * 		 <http://pubs.opengroup.org/onlinepubs/9699919799/functions/send.html>`
 * 
 * @param[in] fd Specifies the socket file descriptor.
 * @param[in] buf Points to the buffer containing the message to send.
 * @param[in] size Specifies the length of the message in bytes.
 * @param[in] flags Specifies the type of message transmission.
 * 
 * @retval Upon successful completion, send() shall return the number of bytes
 *         sent. Otherwise, -1 shall be returned and errno set to indicate the
 *         error.
 */
int blocking_send(int fd, uint8_t *buf, uint32_t size, uint32_t flags);

/**
 * @brief Connect a socket to a peer network address while blocking.
 * 
 * @note If connect() fails, the state of the socket is unspecified. Conforming
 *       applications should close the file descriptor and create a new socket
 *       before attempting to reconnect.
 * @note For a description of the flags parameter, see the full normative
 * 		 description at `POSIX.1-2017 article
 * 		 <http://pubs.opengroup.org/onlinepubs/9699919799/functions/connect.html>`
 * 
 * @param[in] fd Specifies the file descriptor associated with the socket.
 * @param[in] local_addr Points to a sockaddr structure containing the peer
 *            address. The length and format of the address depend on the
 *            address family of the socket.
 * @param[in] len Specifies the length of the sockaddr structure pointed to by
 *            the address argument.
 * 
 * @retval Upon successful completion, connect() shall return 0; otherwise, -1
 *         shall be returned and errno set to indicate the error.
 */
int blocking_connect(int fd, struct sockaddr *local_addr, socklen_t len);

/**
 * @brief Convert NTP time values from host to network byte order.
 * 
 * @param[in] ntp Points to the ntp_format object to be converted.
 */
void ntp_swap_endianess(struct ntp_format *ntp);

/**
 * @brief Get Unix time from NTP server.
 * 
 * @note Only supports IPv4.
 * 
 * @retval Upon successful completion, returns Unix timestamp at the time that
 *         the server replied, in seconds. Otherwise, returns a negative error
 *         code indicating reason for failure.
 */
uint32_t ntp_get_unix_time(void);

#endif /* _NTP_H_ */
