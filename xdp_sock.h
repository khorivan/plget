/*
 * Copyright (C) 2019
 * Authors:	Ivan Khoronzhuk <ivan.khoronzhuk@linaro.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef PLGET_XDP_SOCK_H
#define PLGET_XDP_SOCK_H

#include "plget.h"

#define FRAME_SHIFT	11
#define FRAME_SIZE	(1 << FRAME_SHIFT)	/* 2 frames per page */
#define FRAME_NUM	256	/* number of frames to operate on */
#define FRAME_HEADROOM	0

typedef __u64 umem_desc;
typedef struct xdp_desc sock_desc;

struct queue {
	__u32 cached_prod;
	__u32 cached_cons;
	__u32 mask;
	__u32 size;
	__u32 *producer;
	__u32 *consumer;
	void *ring;
	void *map;
};

struct sock_umem {
	char *frames;
	struct queue fq;
	struct queue cq;
	int fd;
};

struct xsock {
	struct queue rq;
	struct queue tq;
	struct sock_umem *umem;
	struct xdp_desc desc; /* desc for rolling in echo-lat mode */
	int sfd;
};

#ifdef CONF_AFXDP

int xdp_socket(void);
int xsk_sendto(void);
int xsk_recvmsg_start(struct timespec *ts);
void xsk_recvmsg_fail(void);
void xsk_recvmsg_complete(struct msghdr *msg);

#else
inline static int xdp_socket(void)
{
	return 1;
}

inline static int xsk_sendto(void)
{
	return 1;
}

inline static int xsk_recvmsg_start(struct timespec *ts)
{
	return 1;
}

inline static void xsk_recvmsg_fail(void)
{
}

inline static void xsk_recvmsg_complete(struct msghdr *msg)
{
}

#endif

#endif
