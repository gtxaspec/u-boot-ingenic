/*
 * Startup Code for MIPS32 XBURST2 T40 CPU-core
 *
 * Copyright (c) 2013 Ingenic Semiconductor Co.,Ltd
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

#include <config.h>
#include <version.h>
#include <asm/regdef.h>
#include <asm/mipsregs.h>
#include <asm/addrspace.h>
#include <asm/cacheops.h>
#include <asm/arch/base.h>

#define RESERVED_FOR_SC(x) .space 1536, x

	.set noreorder
	.globl _start
	.section .start_section

_start:
#ifdef CONFIG_SPL_MMC_SUPPORT
	/* magic value ("MSPL") */
	.word 0x4d53504c
	.space 508, 0
	RESERVED_FOR_SC(0)
#endif

#if defined(CONFIG_SPL_NAND_SUPPORT) || defined(CONFIG_JZ_NAND_MGR)
	/*
	 * NAND parameters are stored with plenty of redundancy in the
	 * first 192 bytes of the first page of the SPL image.
	 */
	.space 512, 0x00
	RESERVED_FOR_SC(0)
#endif /* CONFIG_SPL_NAND_SUPPORT */

#ifdef CONFIG_SPL_SFC_SUPPORT
	.word 0x03040506
	.word 0x55aa5502
#ifdef CONFIG_SPL_SFC_NOR
	.word 0x000000aa
#elif CONFIG_SPL_SFC_NAND
#define SSI_PPB	(CONFIG_SPI_NAND_PPB / 32)
#define SSI_BPP (CONFIG_SPI_NAND_BPP / 1024)
	.word (0x00000000 | (SSI_PPB<<16) | (SSI_BPP<<24))
#endif
	.space 500, 0
	RESERVED_FOR_SC(0)
#endif /* CONFIG_SPL_SFC_SUPPORT */

#ifdef CONFIG_SPL_SPI_SUPPORT
#define SSI_GR_BOOT (CONFIG_SYS_EXTAL / (CONFIG_SYS_SPI_BOOT_FREQ * 2) - 1)
#ifdef CONFIG_SPI_NOR
	.word 0x55020304
	.word (0x00aa55aa | (SSI_GR_BOOT << 24))
#else
	.word 0x55020304
	.word (0x000055aa | (SSI_GR_BOOT << 24))
#endif
	.space 504, 0
	RESERVED_FOR_SC(0xff)
#endif /* CONFIG_SPL_SPI_SUPPORT */

    /* force riscv core soft reset */
    la t8, 0xb2200fe0
    lw t0, 0(t8)
    lui t1, 0x8000
    or t0, t0, t1
    sw t0, 0(t8)

	/*
	 * CU0=UM=EXL=IE=0, BEV=ERL=1, IP2~7=1
	 */
	li	t0, 0x0040FC04
	mtc0	t0, CP0_STATUS

	/* CAUSE register */
	/*
	 * DC=1, Disable Count Register.
	 * IV=1, use the specical interrupt vector (0x200)
	 * */
	mfc0	t0, CP0_CAUSE
	li	t1, 0x00800000
	or	t0, t0, t1
	mtc0	t0, CP0_CAUSE

    /* PA06~PA19 */
    la t8, 0xf7fc0
    la t9, 0xb0010134
    sw t8, 0(t9)

    la t8, 0xf7fc0
    la t9, 0xb0010144
    sw t8, 0(t9)

    /* PA15 */
    la t8, 0x8000
    la t9, 0xb0010138
    sw t8, 0(t9)

    la t8, 0x8000
    la t9, 0xb00101a4
    sw t8, 0(t9)

    /* PB00~PB31 */
    la t8, 0xffffffff
    la t9, 0xb0011134
    sw t8, 0(t9)

    la t8, 0xffffffff
    la t9, 0xb0011148
    sw t8, 0(t9)

    /* PC00~PC31 */
    la t8, 0xffffffff
    la t9, 0xb0012134
    sw t8, 0(t9)

    la t8, 0xffffffff
    la t9, 0xb0012148
    sw t8, 0(t9)

#ifdef CONFIG_PALLADIUM
	.set	mips32
init_caches:
	la	$25, CONFIG_K0BASE
	li      $2, 3                   // cacheable for kseg0 access
	mtc0    $2, $16                 // CP0_CONFIG
	nop

	mfc0    $3, $4,7		//enable rdhwr
	ori     $3, $3,1
	mtc0    $3, $4,7

	ori     $2, $25, 0              // start address
	ori     $3, $2, (CONFIG_L1CACHE_SIZE - CONFIG_L1CACHELINE_SIZE)   // end address, total 32KB
    mtc0    $0, $28, 0              // CP0_TAGLO
    mtc0    $0, $28, 1              // CP0_DATALO
cache_clear_a_line:
    cache   0x8, 0($2)              // Index_Store_Tag_I
    cache   0x9, 0($2)              // Index_Store_Tag_D
    bne     $2, $3, cache_clear_a_line
    addiu   $2, $2, 32              // increment CACHE_LINE_SIZE
    ori     $2, $25, 0              // start address
    ori     $3, $2, (CONFIG_L1CACHE_SIZE - CONFIG_L1CACHELINE_SIZE)       // end address, total 32KB spl stack space
    la      $4, CONFIG_L1CACHE_TAG_MASK // physical address and 2KB page mask
cache_alloc_a_line:
    and     $5, $2, $4
    ori     $5, $5, 1               // V bit of the physical tag
    mtc0    $5, $28, 0              // CP0_TAGLO
    cache   0x8, 0($2)              // Index_Store_Tag_I
    cache   0x9, 0($2)              // Index_Store_Tag_D
    bne     $2, $3, cache_alloc_a_line
    addiu   $2, $2, 32              // increment CONFIG_L1CACHELINE_SIZE

	li	sp, (CONFIG_K0BASE + CONFIG_STACK_SIZE)
#endif

	/* Set up stack */
#ifdef CONFIG_SPL_STACK
	li	sp, CONFIG_SPL_STACK
#endif
#ifdef CONFIG_SPL_NOR_SUPPORT
	.extern __data_start
	.extern __data_end
	/*Init data section, nor spl*/
	la	t0, __data_start
	la	t1, __data_end
	la	t2, CONFIG_K0BASE
1:
	lw	t3, 0(t0)
	addiu	t0, t0, 4
	sw	t3, 0(t2)
	addiu	t2, t2, 4
	bne	t0, t1, 1b
	nop
#endif
	j	board_init_f
	nop
