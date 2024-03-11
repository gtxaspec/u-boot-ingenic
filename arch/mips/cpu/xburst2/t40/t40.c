/*
 * T40 common routines
 *
 * Copyright (c) 2015 Ingenic Semiconductor Co.,Ltd
 * Author: Elvis <huan.wang@ingenic.com>
 * Based on: arch/mips/cpu/xburst/jz4775/jz4775.c
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

/* #define DEBUG */
#include <config.h>
#include <common.h>
#include <asm/io.h>
#include <asm/arch/clk.h>
#include <asm/arch/cpm.h>
#include <asm/arch/tcu.h>
#include <spl.h>
#include <asm/cacheops.h>
#include <asm/dma-default.h>

#ifdef CONFIG_BIG_SPL
#include "testcode.c"
#endif


#ifdef CONFIG_SPL_BUILD

/* Pointer to as well as the global data structure for SPL */
DECLARE_GLOBAL_DATA_PTR;
gd_t gdata __attribute__ ((section(".data")));

#ifndef CONFIG_BURNER
struct global_info ginfo __attribute__ ((section(".data"))) = {
	.extal		= CONFIG_SYS_EXTAL,
	.cpufreq	= CONFIG_SYS_CPU_FREQ,
	.ddrfreq	= CONFIG_SYS_MEM_FREQ,
	.uart_idx	= CONFIG_SYS_UART_INDEX,
	.baud_rate	= CONFIG_BAUDRATE,
};
#endif

extern void pll_init(void);
extern void sdram_init(void);
extern void validate_cache(void);

#ifdef CONFIG_SIMULATION
volatile noinline void hello_word(void)
{
	while(1)
		asm volatile ("wait");
}
#endif

#define cache_op(op, addr)		\
	__asm__ __volatile__(		\
		".set	push\n"		\
		".set	noreorder\n"	\
		".set	mips3\n"	\
		"cache	%0, %1\n"	\
		".set	pop\n"		\
		:			\
		: "i" (op), "R" (*(unsigned char *)(addr)))


static void flush_dcache_range1(u32 addr, u32 size)
{
	u32 start = addr;
	for (; start < addr+size; start += CONFIG_SYS_CACHELINE_SIZE) {
		cache_op(INDEX_WRITEBACK_INV_D, start);
	}
	__asm__ __volatile__("sync");
}

static void init_bootuptime_timer(void)
{
       writel(TCU_TCSR_PRESCALE16 | TCU_TCSR_RTC_EN, TCU_TCSR(3));
       writel(0xffff, TCU_TDFR(3));
       writel(0, TCU_TCNT(3));
       writel(TCU_TER_TCEN3, TCU_TESR);
}

void board_init_f(ulong dummy)
{
	unsigned int ccu_value=0;
	/*init bootup time timer as early as possible. */
	init_bootuptime_timer();

	/* Set global data pointer */
	gd = &gdata;

    /* close ccu IFU simple Loop */
	ccu_value = *((volatile unsigned int *)(0xb2200fe0));
	*((volatile unsigned int *)(0xb2200fe0)) = (ccu_value | 0x00000018);

	/* Setup global info */
#ifndef CONFIG_CMD_BURN
	gd->arch.gi = &ginfo;
#else
	gd->arch.gi = (struct global_info *)CONFIG_SPL_GINFO_BASE;
#endif

#ifdef CONFIG_BURNER
	gd->arch.gi->ddr_div = ((gd->arch.gi->cpufreq % gd->arch.gi->ddrfreq) == 0)
		               ? (gd->arch.gi->cpufreq / gd->arch.gi->ddrfreq)
		               : (gd->arch.gi->cpufreq / gd->arch.gi->ddrfreq + 1);
#endif

	gpio_init();

#ifndef CONFIG_FPGA
	/* Init uart first */
	enable_uart_clk();
#endif

#ifdef CONFIG_SPL_SERIAL_SUPPORT
	preloader_console_init();
#endif

#ifndef CONFIG_FPGA
	debug("Timer init\n");
	timer_init();

#ifdef CONFIG_SPL_REGULATOR_SUPPORT
	debug("regulator set\n");
	spl_regulator_set();
#endif
#ifdef CONFIG_SIMULATION
	debug("CLK stop\n");
#endif
	clk_prepare();
#ifdef CONFIG_SIMULATION
	debug("PLL init\n");
#endif
	pll_init();
#ifdef CONFIG_SIMULATION
	debug("CLK init\n");
#endif
	clk_init();
#endif
#ifdef CONFIG_SIMULATION
	debug("SDRAM init\n");
#endif
	sdram_init();
#ifdef CONFIG_SIMULATION
	printf("SDRAM init ok\n");
#endif

#ifdef CONFIG_SIMULATION
	{
		hello_word();
	}
#endif

#ifdef CONFIG_DDR_TEST
	ddr_basic_tests();
#endif

#ifndef CONFIG_BURNER
	/* Clear the BSS */
	memset(__bss_start, 0, (char *)&__bss_end - __bss_start);
#ifdef CONFIG_PALLADIUM
	{
		debug("Going to palladium kernel......\n");
		void (*kernel)(void);
		kernel = (void (*)(void))0x80010000;
		(*kernel)();
	}
#endif

	debug("board_init_r\n");
	board_init_r(NULL, 0);
#endif
}

extern void flush_cache_all(void);

void __noreturn jump_to_image_no_args(struct spl_image_info *spl_image)
{
	typedef void __noreturn (*image_entry_noargs_t)(void);

	image_entry_noargs_t image_entry =
		(image_entry_noargs_t) spl_image->entry_point;

	flush_cache_all();

	/* allcate L2 cache size */
	/***********************************
	  L2 cache size
	  reg addr: 0x12200060
	  bit   12 11 10
			0   0  0   L2C=0KB
			0   0  1   L2C=128KB
			0   1  0   L2C=256KB
			0   1  1   L2C=512KB
			1   0  0   L2C=1024KB
	 ***********************************/
	/* wait l2cache alloc ok */
	__asm__ volatile(
			".set push     \n\t"
			".set mips32r2 \n\t"
			"sync          \n\t"
			"lw $0,0(%0)   \n\t"
			".set pop      \n\t"
			::"r" (0xa0000000));
	*((volatile unsigned int *)(0xb2200060)) = 0x00000400;
	__asm__ volatile(
			".set push     \n\t"
			".set mips32r2 \n\t"
			"sync          \n\t"
			"lw $0,0(%0)   \n\t"
			".set pop      \n\t"
			::"r" (0xa0000000));
	printf("image entry point: 0x%X\n", spl_image->entry_point);
	//printf("t40.c jump_to_image_no_args image entry time: %d\n", get_time(0));
	image_entry();
}

#endif /* CONFIG_SPL_BUILD */

/*
 * U-Boot common functions
 */

void enable_interrupts(void)
{
}

int disable_interrupts(void)
{
	return 0;
}
