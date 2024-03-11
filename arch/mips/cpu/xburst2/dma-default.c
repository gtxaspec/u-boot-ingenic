/*
 * Copyright (c) 2013 Ingenic Semiconductor Co.,Ltd
 *	qipengzhen <aric.pzqi@ingenic.com>
 *
 *	<porting from kernel arch/mips/mm/dma-default.c>
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <command.h>
#include <netdev.h>
#include <asm/mipsregs.h>
#include <asm/cacheops.h>
#include <asm/reboot.h>
#include <asm/io.h>
#include <asm/arch/wdt.h>

#include <asm/dma-default.h>
#include <asm/jz_cache.h>
#include <asm/bcache.h>

/*
 * Dummy cache handling routines for machines without boardcaches
 */
__weak void bc_inv(unsigned long page, unsigned long size)
{
}
__weak void bc_wback_inv(unsigned long page, unsigned long size)
{
}


static void r4k_dma_cache_wback_inv(unsigned long addr, unsigned long size)
{

	blast_dcache_range(addr, addr + size);

	bc_wback_inv(addr, size);
	__sync();
}



static void r4k_dma_cache_inv(unsigned long addr, unsigned long size)
{

	unsigned long lsize = cpu_dcache_line_size();
	unsigned long almask = ~(lsize - 1);
	cache_op(Hit_Writeback_Inv_D, addr & almask);
	cache_op(Hit_Writeback_Inv_D, (addr + size - 1)  & almask);
	blast_inv_dcache_range(addr, addr + size);

	bc_inv(addr, size);
	__sync();
}


#define dma_cache_wback		r4k_dma_cache_wback_inv
#define dma_cache_inv		r4k_dma_cache_inv
#define dma_cache_wback_inv	r4k_dma_cache_wback_inv



static inline void __dma_sync_virtual(void *addr, size_t size,
	enum dma_data_direction direction)
{
	switch (direction) {
	case DMA_TO_DEVICE:
		dma_cache_wback((unsigned long)addr, size);
		break;

	case DMA_FROM_DEVICE:
		dma_cache_inv((unsigned long)addr, size);
		break;

	case DMA_BIDIRECTIONAL:
		dma_cache_wback_inv((unsigned long)addr, size);
		break;

	default:
		printf("Bug with unkown direction for dma sync!!\n");
	}
}

void dma_cache_sync(void *dev, void *vaddr, size_t size,
		enum dma_data_direction direction)
{
	__dma_sync_virtual(vaddr, size, direction);
}

