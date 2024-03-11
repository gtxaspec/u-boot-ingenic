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
#define DEBUG   /*Print functions using debug*/
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
extern void sdram_init(int pll_sel);
extern void epc_debug(void);
extern void validate_cache(void);

#ifdef CONFIG_SIMULATION
volatile noinline void hello_word(void)
{
	while(1)
		asm volatile ("wait");
}
#endif

#if 0
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
#endif

static void init_bootuptime_timer(void)
{
       writel(TCU_TCSR_PRESCALE16 | TCU_TCSR_RTC_EN, TCU_TCSR(3));
       writel(0xffff, TCU_TDFR(3));
       writel(0, TCU_TCNT(3));
       writel(TCU_TER_TCEN3, TCU_TESR);
}

static int ddr_mem_test12(void)
{
	int i = 0;
	int j = 0;
	int ret = 8;
	unsigned int addr;
	unsigned int value_get;
	unsigned int data;
	unsigned int test_size =  64;
	unsigned int test_data =  0xffffffff;
	//printf("mem test12 start!\n");
	for(i = 0; i < 1; i++) {


		addr = 0x81000000;
		for (j = 0; j < test_size; j+=4) {
			data = (j/4)%2?test_data:0;
			*(volatile unsigned int *)(addr+j) = data;
		}

		//flush cache
		flush_dcache_range(addr, addr+test_size);
		flush_l2cache_range(addr, addr+test_size);

		//read cached
		addr = 0x81000000;
		for (j = 0; j < test_size; j+=4) {
			value_get = *(volatile unsigned int *)(addr+j);
		}
#if 0
		//write uncached
		addr = 0xa1000000;
		for (j = 0; j < test_size; j+=4) {
			data = (j/4)%2?test_data:0;
			*(volatile unsigned int *)(addr+j) = data;
		}
#endif
		//invalid cache
		addr = 0x81000000;
		invalid_dcache_range(addr, addr+test_size);
		invalid_l2cache_range(addr, addr+test_size);
		//read cached
		addr = 0x81000000;
		printf("mem data:\n");
		for (j = 0; j < test_size; j+=4) {
			value_get = *(volatile unsigned int *)(addr+j);
			if(j/4%4%2)
				if(test_data == value_get)
					ret -= 1;
			if (0 == j/4%4)
				printf("%x: ", addr+j);
			printf("  %x", value_get);
			if (3 == j/4%4)
				printf("\n");
		}
	}
	//printf("mem test12 end!\n");
	return ret;
}

static int ddr_mem_test4(void)
{
	volatile u32 tmp = 0;
	u32 tmp_data = 0;

#ifdef CONFIG_DDR_TYPE_DDR2
	for (tmp = 0xa0000000; tmp < 0xa4000000; tmp+=0x104) {
		u32 td = 0x5a5a5a5a;
		*(u32*)tmp = td;
		tmp_data = *(u32*)tmp;
		if (tmp_data != td) {
			printf("\n##### addr = %p, want = %x, get = %x\n", tmp, td, tmp_data);
			return -1;
		}
	}
#else
	for (tmp = 0xa0000000; tmp < 0xa8000000; tmp+=0x104) {
		u32 td = 0x5a5a5a5a;
		*(u32*)tmp = td;
		tmp_data = *(u32*)tmp;
		if (tmp_data != td) {
			printf("\n##### addr = %p, want = %x, get = %x\n", tmp, td, tmp_data);
			return -1;
		}
	}
#endif
	return 0;
}

void board_init_f(ulong dummy)
{
	unsigned int ccu_value = 0;
	int pll_sel = 0;

	/*init bootup time timer as early as possible. */
	init_bootuptime_timer();

	/* Set global data pointer */
	gd = &gdata;

    /* close ccu IFU simple Loop */
	ccu_value = *((volatile unsigned int *)(0xb2200fe0));
	*((volatile unsigned int *)(0xb2200fe0)) = (ccu_value | 0x00000078);

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
	/* debug("Timer init\n"); */
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
#endif/*CONFIG_FPGA*/

#ifdef CONFIG_SIMULATION
	debug("SDRAM init\n");
#endif

	sdram_init(pll_sel);
	while(ddr_mem_test4())
	{
		pll_sel++;
		sdram_init(pll_sel);
	}

	epc_debug();
#ifdef CONFIG_DDR_SELF_REFRESH
    mdelay(10);
    sumalte_ddr_self_refresh();
    mdelay(10);
    sumalte_ddr_exit_self_refresh();
#endif

#ifdef CONFIG_BIG_SPL
     debug("before test codes\n");
     TEST_CODES
     debug("after test codes\n");
#endif


#ifdef CONFIG_SIMULATION
	{
		hello_word();
	}
#endif

	if(ddr_mem_test12())
	{
		printf("ddr error!\n");
		do_reset(NULL, 0, 0, NULL);
	}
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

	/* debug("board_init_r\n"); */
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
	/* set L2cache 128K */
	*((volatile unsigned int *)(0xb2200060)) = (*(volatile unsigned int *)(0xb2200060) & ~(0x7<<10)) | (0x1<<10);
	__asm__ volatile(
			".set push     \n\t"
			".set mips32r2 \n\t"
			"sync          \n\t"
			"lw $0,0(%0)   \n\t"
			".set pop      \n\t"
			::"r" (0xa0000000));
	debug("image entry point: 0x%X\n", spl_image->entry_point);
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
