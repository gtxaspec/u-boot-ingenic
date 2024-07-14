/*
 * DDR driver for inno DDR PHY.
 * Used by t21
 *
 * Copyright (C) 2017 Ingenic Semiconductor Co.,Ltd
 * Author: Zoro <ykli@ingenic.cn>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/* #define DEBUG */
#include <config.h>
#include <common.h>
#include <ddr/ddr_common.h>
#include <asm/io.h>
#include <asm/ddr_dwc.h>
#include <asm/arch/cpm.h>

#ifndef CONFIG_FPGA

void reset_dll(void)
{
/*
 * WARNING: 2015-01-08
 * 	DDR CLK GATE(CPM_DRCG 0xB00000D0), BIT6 must set to 1 (or 0x40).
 * 	If clear BIT6, chip memory will not be stable, gpu hang occur.
 */
#if 0
	writel(3 | (1<<6), CPM_DRCG);
	mdelay(5);
	writel(0x7d | (1<<6), CPM_DRCG);
	mdelay(5);
#else
	cpm_writel(0x73 | (1 << 6) , CPM_DRCG);
	mdelay(1);
	cpm_writel(0x71 | (1 << 6), CPM_DRCG);
	mdelay(1);
#endif
}

#endif
