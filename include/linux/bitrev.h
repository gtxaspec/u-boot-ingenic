/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_BITREV_H
#define _LINUX_BITREV_H

#include <linux/types.h>

extern u8 const byte_rev_table[256];

static inline u8 bitrev8(u8 byte)
{
	return byte_rev_table[byte];
}

u16 bitrev16(u16 in);
u32 bitrev32(u32 in);

#endif /* _LINUX_BITREV_H */
