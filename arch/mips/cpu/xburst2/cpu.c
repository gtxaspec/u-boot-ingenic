/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, <wd@denx.de>
 * (C) Copyright 2011
 * Xiangfu Liu <xiangfu@openmobilefree.net>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
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

#include <asm/jz_cache.h>

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

//	fprintf(stderr, "*** reset failed ***\n");
	return 0;
}

void flush_cache(ulong start_addr, ulong size)
{
	if(size == 0) {
		return;
	}

	flush_dcache_range(start_addr, start_addr + size);
	flush_icache_range(start_addr, start_addr + size);
}

void flush_cache_all(void)
{
	flush_icache_all(); /* invalid icache */

	flush_dcache_all(); /* writeback invalid dcache,  */
	__asm__ volatile(
		".set push     \n\t"
		".set mips32r2 \n\t"
		"sync          \n\t"
		".set pop      \n\t"
		);


	flush_l2cache_all(); /* writeback invalid l2cache */
	__asm__ volatile(
		".set push     \n\t"
		".set mips32r2 \n\t"
		"lw $0,0(%0)   \n\t"
		".set pop      \n\t"
		::"r" (0xa0000000));
}



#define read_c0_desave()	__read_32bit_c0_register($31,0)
#define read_c0_kscracth1()	__read_32bit_c0_register($31,2)
#define read_c0_kscracth2()	__read_32bit_c0_register($31,3)
#define read_c0_kscracth3()	__read_32bit_c0_register($31,4)
#define read_c0_kscracth4()	__read_32bit_c0_register($31,5)
#define read_c0_kscracth5()	__read_32bit_c0_register($31,6)
#define read_c0_kscracth6()	__read_32bit_c0_register($31,7)


void dump_c0_regs(void)
{
	printf("read_c0_status()   :%x\n", read_c0_status());
	printf("read_c0_cause()	   :%x\n", read_c0_cause());
	printf("read_c0_desave()   :%x\n", read_c0_desave()   );
	printf("read_c0_kscracth1():%x\n", read_c0_kscracth1());
	printf("read_c0_kscracth2():%x\n", read_c0_kscracth2());
	printf("read_c0_kscracth3():%x\n", read_c0_kscracth3());
	printf("read_c0_kscracth4():%x\n", read_c0_kscracth4());
	printf("read_c0_kscracth5():%x\n", read_c0_kscracth5());
	printf("read_c0_kscracth6():%x\n", read_c0_kscracth6());
}


#define DCACHE_WAYS 8
#define DCACHE_SET_PER_WAY 128
#define DCACHE_CACHE_LINESIZE   32
#define DCACHE_SIZE	(DCACHE_WAYS * DCACHE_SET_PER_WAY * DCACHE_CACHE_LINESIZE)

#define ICACHE_WAYS 8
#define ICACHE_SET_PER_WAY 128
#define ICACHE_CACHE_LINESIZE   32
#define ICACHE_SIZE	(ICACHE_WAYS * ICACHE_SET_PER_WAY * ICACHE_CACHE_LINESIZE)


#define Index_Invalidate_I	0x00
#define Index_WriteBack_Inv_D	0x01
#define Index_Load_Tag_I	0x04
#define Index_Load_Tag_D	0x05
#define Index_Store_Tag_I	0x08
#define Index_Store_Tag_D	0x09
#define Index_Store_Data_I	0x0c

#if 0
/*  4 * 128 * 32 =  */
void dump_dcache_tag(void)
{
	int way = 0,set = 0,byte = 0;
	unsigned int taglo = 0, datalo = 0;
	unsigned int addr;

	unsigned int data[8];
	int i;
	unsigned int start = 0x80000000;
	//for(addr = start; addr < start + DCACHE_SIZE; addr += DCACHE_CACHE_LINESIZE) {
	for(addr = start; addr < start + 0x1000; addr += DCACHE_CACHE_LINESIZE) {
		for(byte = 0; byte < DCACHE_CACHE_LINESIZE; byte+=4) {

			cache_op(Index_Load_Tag_D, addr | byte);

			taglo = read_c0_taglo();
			datalo = read_c0_datalo();

			data[byte/4] = datalo;
		}

		printf("%x: ",  addr);
		for(i = 0; i < 8; i++) {
			printf("%x ", data[i]);
		}
		printf("\n");
	}
}

int dump_icache_tag(unsigned int start, unsigned int len)
{
	unsigned int addr;
	unsigned int byte;
	unsigned int taglo;
	unsigned int data[8];
	int i;
	for(addr = start; addr < start + len; addr += ICACHE_CACHE_LINESIZE) {
		for(byte = 0; byte < ICACHE_CACHE_LINESIZE; byte+=4) {
			cache_op(Index_Load_Tag_I, addr | byte);

			taglo = read_c0_taglo();
			data[byte/4] =  read_c0_datalo();
		}
		printf("[icache][V:%d] %x: ", (taglo & 1), addr);
		for(i = 0; i < 8; i++) {
			printf("%x ", data[i]);
		}
		printf("\n");
	}

}
#endif

void debug_dcache_store(void)
{
	unsigned int start = 0x80001000;
	unsigned int addr;
	unsigned int val = 0;

	for(addr = start; addr < (start + 16384); addr += DCACHE_CACHE_LINESIZE) {

		val = (addr & 0x1ffff000);
		val |= 1;
		write_c0_taglo(val);

		cache_op(Index_Store_Tag_D, addr);
	}
}

void debug_icache_store(unsigned int start, unsigned int len)
{
	unsigned int addr = 0;
	unsigned int val;
	int i;
	/* Index Store Tag */
	for(addr = start; addr < (start + 32 *1024); addr += ICACHE_CACHE_LINESIZE) {
		val = addr & 0x1ffff000;
		val |= 1;
		write_c0_taglo(val);
		cache_op(Index_Store_Tag_I, addr);
	}
	/* Index Store Data */
	for(i = 0; i < 1024; i++) {
		write_c0_datalo(0);
		cache_op(Index_Store_Data_I, (start + i*4));
	}
}


void debug_cache_2(void)
{
	unsigned int addr = 0x80000000;
	unsigned int val = 0;
	unsigned int data = 0;

	val = addr & 0x1ffff000;
	val |= 1;
	write_c0_taglo(val);
	cache_op(Index_Store_Tag_I, addr);


	write_c0_datalo(0x12345678);
	cache_op(Index_Store_Data_I, addr);


	cache_op(Index_Load_Tag_I, addr);
	data = read_c0_datalo();

	printf("data: %x\n", data);
}

void debug_cache(void)
{
        unsigned int addr = 0x80000000;
        unsigned int val = 0;
        unsigned int data = 0;

        val = addr & 0x1ffff000;
        val |= 1;
        __asm__ __volatile__( "mtc0\t%z0, " "$28" "\n\t"
                        :
                        : "Jr" ((unsigned int)(val)));

        __asm__ __volatile__( ".set	push\n"
                        ".set	noreorder\n"
                        ".set	mips3\n"
                        "cache	%0, %1\n"
                        ".set	pop\n"
                        :
                        : "i" (0x08), "R" (*(unsigned char *)(addr)));

        __asm__ __volatile__( ".set\tmips32\n\t"
                        "mtc0\t%z0, ""$28"", ""1" "\n\t"
                        ".set\tmips0"
                        :
                        : "Jr" ((unsigned int)(0x12345678)));


        __asm__ __volatile__( ".set	push\n"
                        ".set	noreorder\n"
                        ".set	mips3\n"
                        "cache	%0, %1\n"
                        ".set	pop\n"
                        :
                        : "i" (0x0c), "R" (*(unsigned char *)(addr)));


        __asm__ __volatile__( ".set	push\n"
                        ".set	noreorder\n"
                        ".set	mips3\n"
                        "cache	%0, %1\n"
                        ".set	pop\n"
                        :
                        : "i" (0x04), "R" (*(unsigned char *)(addr)));

        data = ({ int __res;
                        __asm__ __volatile__( ".set\tmips32\n\t"
                                "mfc0\t%0, " "$28" ", " "1" "\n\t"
                                ".set\tmips0\n\t"
                                : "=r" (__res));
                        __res; });

        printf("data: %x\n", data);
}
