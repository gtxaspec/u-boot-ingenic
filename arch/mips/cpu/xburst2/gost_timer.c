/*
 * Timer for T40
 *
 * Copyright (c) 2016 Ingenic Semiconductor Co.,Ltd
 * Author: qipengzhen <aric.pzqi@ingenic.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <config.h>
#include <common.h>
#include <div64.h>
#include <asm/io.h>
#include <asm/mipsregs.h>
#include <asm/arch/g_ost.h>

DECLARE_GLOBAL_DATA_PTR;

unsigned int multiple __attribute__ ((section(".data")));

static uint32_t gost_readl(uint32_t off)
{
	return readl((void __iomem *)G_OST_BASE + off);
}

static void gost_writel(uint32_t val, uint32_t off)
{
	writel(val, (void __iomem *)G_OST_BASE + off);
}

#define USEC_IN_1SEC 1000000
int timer_init(void)
{
#ifdef CONFIG_FPGA
	multiple = CONFIG_SYS_OST_FREQ / USEC_IN_1SEC / G_OST_DIV;
#else
	multiple = CONFIG_SYS_EXTAL / USEC_IN_1SEC / G_OST_DIV;
#endif
	/* Disable OST */
	gost_writel(0, G_OSTER);

	/* Configure OST */
	gost_writel(G_OSTCCR_PRESCALE, G_OSTCCR);

	/* Reset OST */
	reset_timer();

	/* Start OST */
	gost_writel(1, G_OSTER);

	return 0;
}

void reset_timer(void)
{
	gost_writel(1, G_OSTCR);
}

static uint64_t get_timer64(void)
{
	uint32_t low = gost_readl(G_OSTCNTL);
	uint32_t high = gost_readl(G_OSTCNTB);
	return ((uint64_t)high << 32) | low;
}


ulong get_timer(ulong base)
{
	return lldiv(get_timer64(), (USEC_IN_1SEC/CONFIG_SYS_HZ) * multiple) - base;
}

void __udelay(unsigned long usec)
{
	/* OST count increments at 3MHz */
	uint64_t end = get_timer64() + ((uint64_t)usec * multiple);
	while (get_timer64() < end);
}

unsigned long long get_ticks(void)
{
	return get_timer64();
}

ulong get_tbclk(void)
{
	return CONFIG_SYS_HZ;
}
