/* SPDX-License-Identifier: GPL-2.0 */
#ifndef UAPI_GOLDFISH_ADDRESS_SPACE_H
#define UAPI_GOLDFISH_ADDRESS_SPACE_H

#include <linux/types.h>

#define GOLDFISH_ADDRESS_SPACE_DEVICE_NAME	"goldfish_address_space"

struct goldfish_address_space_allocate_block {
	__u64 size;
	__u64 offset;
	__u64 phys_addr;
};

struct goldfish_address_space_ping {
	__u64 offset;
	__u64 size;
	__u64 metadata;
	__u64 wait_offset;
	__u32 wait_flags;
	__u32 direction;
};

#define GOLDFISH_ADDRESS_SPACE_IOCTL_MAGIC	'G'

#define GOLDFISH_ADDRESS_SPACE_IOCTL_OP(OP, T)	\
	_IOWR(GOLDFISH_ADDRESS_SPACE_IOCTL_MAGIC, OP, T)

#define GOLDFISH_ADDRESS_SPACE_IOCTL_ALLOCATE_BLOCK \
	GOLDFISH_ADDRESS_SPACE_IOCTL_OP(10, \
		struct goldfish_address_space_allocate_block)

#define GOLDFISH_ADDRESS_SPACE_IOCTL_DEALLOCATE_BLOCK \
	GOLDFISH_ADDRESS_SPACE_IOCTL_OP(11, __u64)

#define GOLDFISH_ADDRESS_SPACE_IOCTL_PING \
	GOLDFISH_ADDRESS_SPACE_IOCTL_OP(12, \
		struct goldfish_address_space_ping)

#endif /* UAPI_GOLDFISH_ADDRESS_SPACE_H */
