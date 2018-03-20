/*
 * Copyright (C) 2016 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef GOLDFISH_DMA_H
#define GOLDFISH_DMA_H

#include <linux/dma-mapping.h>
#include <linux/platform_device.h>
#include <linux/types.h>

/* GOLDFISH DMA
 *
 * Goldfish DMA is an extension to the pipe device
 * and is designed to facilitate high-speed RAM->RAM
 * transfers from guest to host.
 *
 * Interface (guest side):
 *
 * The guest user calls goldfish_dma_alloc (ioctls)
 * and then mmap() on a goldfish pipe fd,
 * which means that it wants high-speed access to
 * host-visible memory.
 *
 * The guest can then write into the pointer
 * returned by mmap(), and these writes
 * become immediately visible on the host without BQL
 * or otherweise context switching.
 *
 * The main data structure tracking state is
 * struct goldfish_dma_context, which is included
 * as an extra pointer field in struct goldfish_pipe.
 * Each such context is associated with possibly
 * one physical address and size describing the
 * allocated DMA region, and only one allocation
 * is allowed for each pipe fd. Further allocations
 * require more open()'s of pipe fd's.
 *
 * dma_alloc_coherent() is used to obtain contiguous
 * physical memory regions, and we allocate and interact
 * with this region on both guest and host through
 * the following ioctls:
 *
 * - LOCK: lock the region for data access.
 * - UNLOCK: unlock the region. This may also be done from the host
 *   through the WAKE_ON_UNLOCK_DMA procedure.
 * - CREATE_REGION: initialize size info for a dma region.
 * - GETOFF: send physical address to guest drivers.
 * - (UN)MAPHOST: uses goldfish_pipe_cmd to tell the host to
 * (un)map to the guest physical address associated
 * with the current dma context. This makes the physically
 * contiguous memory (in)visible to the host.
 *
 * Guest userspace obtains a pointer to the DMA memory
 * through mmap(), which also lazily allocates the memory
 * with dma_alloc_coherent. (On last pipe close(), the region is freed).
 * The mmaped() region can handle very high bandwidth
 * transfers, and pipe operations can be used at the same
 * time to handle synchronization and command communication.
 *
 * This is the kernel header defining Goldfish DMA state.
 * It consists mainly of a physical contiguous memory
 * along with a mutex to coordinate access.
 *
 * The ioctls() and mmap() are implemented as ioctl
 * and mmap handlers in the goldfish_pipe_v2.c.
 */

#define GOLDFISH_DMA_BUFFER_SIZE (32 * 1024 * 1024)

struct goldfish_pipe_command;

struct goldfish_dma_context {
	struct device *pdev_dev;	/* pointer to feed to dma_***_coherent */
	void *dma_vaddr;		/* kernel vaddr of dma region */
	size_t dma_size;		/* size of dma region */
	dma_addr_t phys_begin;		/* paddr of dma region */
	dma_addr_t phys_end;		/* paddr of dma region + dma_size */
};

struct goldfish_dma_ioctl_info {
	dma_addr_t phys_begin;
	size_t size;
};

/* There is an ioctl associated with goldfish dma driver.
 * Make it conflict with ioctls that are not likely to be used
 * in the emulator.
 * 'G'	00-3F	drivers/misc/sgi-gru/grulib.h	conflict!
 * 'G'	00-0F	linux/gigaset_dev.h	conflict!
 */
#define GOLDFISH_DMA_IOC_MAGIC	'G'
#define GOLDFISH_DMA_IOC_OP(OP)	_IOWR(GOLDFISH_DMA_IOC_MAGIC, OP, \
				struct goldfish_dma_ioctl_info)

#define GOLDFISH_DMA_IOC_LOCK		GOLDFISH_DMA_IOC_OP(0)
#define GOLDFISH_DMA_IOC_UNLOCK		GOLDFISH_DMA_IOC_OP(1)
#define GOLDFISH_DMA_IOC_GETOFF		GOLDFISH_DMA_IOC_OP(2)
#define GOLDFISH_DMA_IOC_CREATE_REGION	GOLDFISH_DMA_IOC_OP(3)

#endif /* GOLDFISH_DMA_H */
