/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, <wd@denx.de>
 * (C) Copyright 2011
 * Xiangfu Liu <xiangfu@openmobilefree.net>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <netdev.h>
#include <asm/mipsregs.h>
#include <asm/cacheops.h>
#include <asm/reboot.h>
#include <asm/io.h>
#include <asm/arch/wdt.h>

#define cache_op(op, addr)		\
	__asm__ __volatile__(		\
		".set	push\n"		\
		".set	noreorder\n"	\
		".set	mips3\n"	\
		"cache	%0, %1\n"	\
		".set	pop\n"		\
		:			\
		: "i" (op), "R" (*(unsigned char *)(addr)))

void __attribute__((weak)) _machine_restart(void)
{
	int time = RTC_FREQ / WDT_DIV * RESET_DELAY_MS / 1000;

	if(time > 65535)
		time = 65535;

	writel(TSCR_WDTSC, TCU_BASE + TCU_TSCR);

	writel(0, WDT_BASE + WDT_TCNT);
	writel(time, WDT_BASE + WDT_TDR);
	writel(TCSR_PRESCALE | TCSR_RTC_EN, WDT_BASE + WDT_TCSR);
	writel(0,WDT_BASE + WDT_TCER);

	printf("reset in %dms", RESET_DELAY_MS);
	writel(TCER_TCEN,WDT_BASE + WDT_TCER);
	mdelay(1000);
}

int do_reset(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	_machine_restart();

	fprintf(stderr, "*** reset failed ***\n");
	return 0;
}

void flush_cache(ulong start_addr, ulong size)
{
	unsigned long lsize = CONFIG_SYS_CACHELINE_SIZE;
	unsigned long addr = start_addr & ~(lsize - 1);
	unsigned long aend = (start_addr + size - 1) & ~(lsize - 1);

	for (; addr <= aend; addr += lsize) {
		cache_op(HIT_WRITEBACK_INV_D, addr);
		cache_op(HIT_INVALIDATE_I, addr);
	}
}

void flush_dcache_range(ulong start_addr, ulong stop)
{
	unsigned long lsize = CONFIG_SYS_CACHELINE_SIZE;
	unsigned long addr = start_addr & ~(lsize - 1);
	unsigned long aend = (stop - 1) & ~(lsize - 1);
	unsigned int writebuffer;

	for (; addr <= aend; addr += lsize)
		cache_op(HIT_WRITEBACK_INV_D, addr);
	__asm__ __volatile__("sync");
	writebuffer = *(volatile unsigned int *)0xa0000000;
}

void invalidate_dcache_range(ulong start_addr, ulong stop)
{
	unsigned long lsize = CONFIG_SYS_CACHELINE_SIZE;
	unsigned long addr = start_addr & ~(lsize - 1);
	unsigned long aend = (stop - 1) & ~(lsize - 1);

	for (; addr <= aend; addr += lsize)
		cache_op(HIT_INVALIDATE_D, addr);
}

void flush_icache_all(void)
{
	u32 addr, t = 0;

	for (addr = CKSEG0; addr < CKSEG0 + CONFIG_SYS_ICACHE_SIZE;
	     addr += CONFIG_SYS_CACHELINE_SIZE) {
		cache_op(INDEX_INVALIDATE_I, addr);
	}

	/* invalidate btb */
	__asm__ __volatile__(
		".set mips32\n\t"
		"mfc0 %0, $16, 7\n\t"
		"nop\n\t"
		"ori %0,2\n\t"
		"mtc0 %0, $16, 7\n\t"
		".set mips2\n\t"
		:
		: "r" (t));
}

void flush_dcache_all(void)
{
	u32 addr;

	for (addr = CKSEG0; addr < CKSEG0 + CONFIG_SYS_DCACHE_SIZE;
	     addr += CONFIG_SYS_CACHELINE_SIZE) {
		cache_op(INDEX_WRITEBACK_INV_D, addr);
	}

	__asm__ __volatile__("sync");
}

void flush_cache_all(void)
{
	flush_dcache_all();
	flush_icache_all();
}
