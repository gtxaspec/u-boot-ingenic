/*
 * DDR driver for Synopsys DWC DDR PHY.
 * Used by Jz4775, JZ4780...
 *
 * Copyright (C) 2013 Ingenic Semiconductor Co.,Ltd
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
void reset_dllA(int bypass,enum ddr_type type)
{
/*
 * WARNING: 2015-01-08
 * 	DDR CLK GATE(CPM_DRCG 0xB00000D0), BIT6 must set to 1 (or 0x40).
 * 	If clear BIT6, chip memory will not stable, gpu hang occur.
 */
	cpm_writel(0x73 | (1 << 6) , CPM_DRCG);
	mdelay(1);
	cpm_writel(0x71 | (1 << 6), CPM_DRCG);
	mdelay(1);
}

void dynamic_clk_gate_enable(int bypass,enum ddr_type type)
{
	unsigned int val;
	if(bypass) {
		val = cpm_inl(CPM_DDRCDR);
		val |= 1 << 26;
		cpm_outl(val, CPM_DDRCDR);
	}
}

static struct jzsoc_ddr_hook ddr_hook={
	.prev_ddr_init = reset_dllA,
	.post_ddr_init = NULL,
//	.post_ddr_init = dynamic_clk_gate_enable,
};

void soc_ddr_init(void)
{
	register_ddr_hook(&ddr_hook);
}
#endif
