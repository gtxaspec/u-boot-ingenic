/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, <wd@denx.de>
 * (C) Copyright 2011
 * Xiangfu Liu <xiangfu@openmobilefree.net>
 *
  * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <asm/arch/jz4740.h>

void _machine_restart(void)
{
	struct jz4740_wdt *wdt = (struct jz4740_wdt *)JZ4740_WDT_BASE;
	struct jz4740_tcu *tcu = (struct jz4740_tcu *)JZ4740_TCU_BASE;
	u16 tmp;

	/* wdt_select_extalclk() */
	tmp = readw(&wdt->tcsr);
	tmp &= ~(WDT_TCSR_EXT_EN | WDT_TCSR_RTC_EN | WDT_TCSR_PCK_EN);
	tmp |= WDT_TCSR_EXT_EN;
	writew(tmp, &wdt->tcsr);

	/* wdt_select_clk_div64() */
	tmp = readw(&wdt->tcsr);
	tmp &= ~WDT_TCSR_PRESCALE_MASK;
	tmp |= WDT_TCSR_PRESCALE64,
	writew(tmp, &wdt->tcsr);

	writew(100, &wdt->tdr); /* wdt_set_data(100) */
	writew(0, &wdt->tcnt); /* wdt_set_count(0); */
	writel(TCU_TSSR_WDTSC, &tcu->tscr); /* tcu_start_wdt_clock */
	writeb(readb(&wdt->tcer) | WDT_TCER_TCEN, &wdt->tcer); /* wdt start */

	while (1)
		;
}
