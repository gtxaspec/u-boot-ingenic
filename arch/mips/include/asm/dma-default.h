/*
 * arch/mips/include/asm/jz_cache.h
 *
 * Copyright (c) 2016 Ingenic Semiconductor Co.,Ltd
 * 	qipengzhen <aric.pzqi@ingenic.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#ifndef __DMA_DEFAULT_H__
#define __DMA_DEFAULT_H__

#include <asm/jz_cache.h>


enum dma_data_direction {
	DMA_BIDIRECTIONAL = 0,
	DMA_TO_DEVICE = 1,
	DMA_FROM_DEVICE = 2,
	DMA_NONE = 3,
};

void dma_cache_sync(void *dev, void *vaddr, size_t size,
		enum dma_data_direction direction);


#endif

