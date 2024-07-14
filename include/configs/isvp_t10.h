/*
 * Ingenic isvp configuration
 *
 * Copyright (c) 2013 Ingenic Semiconductor Co.,Ltd
 * Author: Zoro <ykli@ingenic.cn>
 * Based on: include/configs/urboard.h
 *           Written by Paul Burton <paul.burton@imgtec.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_ISVP_T10_H__
#define __CONFIG_ISVP_T10_H__

/**
 * Basic configuration(SOC, Cache, UART, DDR).
 */
#if defined(CONFIG_LITE_VERSION)
#define CONFIG_SYS_APLL_FREQ		712704000	/*If APLL is not used, it must be set to 0*/
#define CONFIG_SYS_APLL_MNOD		((59 << 20) | (2 << 14) | (1 << 11) | (1 << 8))
#define CONFIG_SYS_APLL_FRAC		0x645a1c
#else
#define CONFIG_SYS_APLL_FREQ		860160000	/*If APLL is not used, it must be set to 0*/
#define CONFIG_SYS_APLL_MNOD		((71 << 20) | (2 << 14) | (1 << 11) | (1 << 8))
#define CONFIG_SYS_APLL_FRAC		0xae147a
#endif

#define CONFIG_SYS_MPLL_FREQ		1200000000	/*If MPLL is not used, it must be set to 0*/

#define SEL_SCLKA			2
#define SEL_CPU				1
#define SEL_H0				2
#define SEL_H2				2

#define DIV_PCLK			12
#define DIV_H2				6
#define DIV_H0				6

#define DIV_L2				2
#define DIV_CPU				1
#define CONFIG_SYS_CPCCR_SEL		(((SEL_SCLKA & 3) << 30)		\
					| ((SEL_CPU & 3) << 28)			\
					| ((SEL_H0 & 3) << 26)			\
					| ((SEL_H2 & 3) << 24)			\
					| (((DIV_PCLK - 1) & 0xf) << 16)	\
					| (((DIV_H2 - 1) & 0xf) << 12)		\
					| (((DIV_H0 - 1) & 0xf) << 8)		\
					| (((DIV_L2 - 1) & 0xf) << 4)		\
					| (((DIV_CPU - 1) & 0xf) << 0))

#define CONFIG_CPU_SEL_PLL		APLL
#define CONFIG_DDR_SEL_PLL		MPLL
#define CONFIG_SYS_CPU_FREQ		CONFIG_SYS_APLL_FREQ

#define CONFIG_SYS_MEM_FREQ		(CONFIG_SYS_MPLL_FREQ / 3)

#define CONFIG_SYS_EXTAL		24000000	/* EXTAL freq: 24 MHz */
#define CONFIG_SYS_HZ			1000		/* incrementer freq */

#define CONFIG_SYS_DCACHE_SIZE		32768
#define CONFIG_SYS_ICACHE_SIZE		32768
#define CONFIG_SYS_CACHELINE_SIZE	32

/*
#define CONFIG_DDR_TEST_CPU
#define CONFIG_DDR_TEST
*/
#define CONFIG_DDR_PARAMS_CREATOR
#define CONFIG_DDR_HOST_CC
#define CONFIG_DDR_FORCE_SELECT_CS1
#define CONFIG_DDR_CS0			1	/* 1-connected, 0-disconnected */
#define CONFIG_DDR_CS1			0	/* 1-connected, 0-disconnected */
#define CONFIG_DDR_DW32			0	/* 1-32bit-width, 0-16bit-width */
#define CONFIG_DDRC_CTRL_PDT		DDRC_CTRL_PDT_128

#define CONFIG_DDR2_M14D5121632A
#define CONFIG_DDR_TYPE_DDR2

#define CONFIG_DDR_PHY_IMPEDANCE	40000
#define CONFIG_DDR_PHY_ODT_IMPEDANCE	50000 /* 75000 */
/*
#define CONFIG_DDR_PHY_IMPED_PULLUP	0xf
#define CONFIG_DDR_PHY_IMPED_PULLDOWN	0xf
*/

/*
#define CONFIG_DDR_DLL_OFF
*/

/*
#define CONFIG_DDR_CHIP_ODT
#define CONFIG_DDR_PHY_ODT
#define CONFIG_DDR_PHY_DQ_ODT
#define CONFIG_DDR_PHY_DQS_ODT
*/

/**
 * Boot arguments definitions.
 */
#define BOOTARGS_COMMON "mem=\\${osmem} rmem=\\${rmem}"
#define CONFIG_EXTRA_SETTINGS \
"osmem=39M@0x0\0" \
"ispmem=5M@0x2700000\0" \
"rmem=20M@0x2c00000\0"

/*
#if defined(CONFIG_SPL_MMC_SUPPORT)
#define CONFIG_BOOTARGS BOOTARGS_COMMON " init=/linuxrc root=/dev/mmcblk0p2 rw rootdelay=1"
#elif defined(CONFIG_SFC_NOR)
#define CONFIG_BOOTARGS BOOTARGS_COMMON " init=/linuxrc rootfstype=squashfs root=/dev/mtdblock2 rw mtdparts=jz_sfc:256k(boot),2560k(kernel),2048k(root),-(appfs)"
#endif
*/

/**
 * Boot command definitions.
 */

/*
#if defined(CONFIG_SPL_MMC_SUPPORT)
#define CONFIG_BOOTCOMMAND "mmc read 0x80600000 0x1800 0x3000; bootm 0x80600000"
#endif
*/

/*
#if defined(CONFIG_SFC_NOR)
#define CONFIG_BOOTCOMMAND "sf probe;sf read 0x80600000 0x40000 0x280000; bootm 0x80600000"
#endif
*/

/**
 * Drivers configuration.
 */
/* MMC */
#if defined(CONFIG_JZ_MMC_MSC0) || defined(CONFIG_JZ_MMC_MSC1)
#define CONFIG_GENERIC_MMC		1
#define CONFIG_MMC			1
#define CONFIG_JZ_MMC			1
#endif  /* JZ_MMC_MSC0 || JZ_MMC_MSC1 */

#if defined(CONFIG_JZ_MMC_MSC0)
#define CONFIG_JZ_MMC_SPLMSC		0
#define CONFIG_JZ_MMC_MSC0_PB		1
#endif

#if defined(CONFIG_SFC_COMMAND)
#define CONFIG_CMD_SF
#define CONFIG_SPI_FLASH
#define CONFIG_JZ_SFC_PA
#define CONFIG_JZ_SFC
#define CONFIG_SFC_NOR
#define CONFIG_SPI_FLASH_INGENIC
#define CONFIG_SPI_DUAL
/* #define CONFIG_SPI_QUAD */
#endif

/* SFC */
#if defined(CONFIG_SPL_SFC_SUPPORT)
#define CONFIG_SPL_SERIAL_SUPPORT
#define CONFIG_SPI_SPL_CHECK
#define CONFIG_JZ_SFC_PA
#if defined(CONFIG_SPI_NAND)
#define CONFIG_UBOOT_OFFSET		CONFIG_SPL_MAX_SIZE
#define CONFIG_SPI_NAND_BPP		(2048 + 64)	/*Bytes Per Page*/
#define CONFIG_SPI_NAND_PPB		(64)		/*Page Per Block*/
#define CONFIG_SPL_SFC_NAND
#define CONFIG_CMD_SFC_NAND
#define CONFIG_SYS_MAX_NAND_DEVICE	1
#define CONFIG_SYS_NAND_BASE		0xb3441000
#else /* CONFIG_SPI_NAND */
#define CONFIG_JZ_SFC
#define CONFIG_CMD_SF
#define CONFIG_SPI_FLASH_INGENIC
#define CONFIG_SPI_FLASH
#define CONFIG_UBOOT_OFFSET		CONFIG_SPL_MAX_SIZE
#define CONFIG_SPL_SFC_NOR
#define CONFIG_SPI_DUAL
/* #define CONFIG_SPI_QUAD */
#endif
#endif /* CONFIG_SPL_SFC_SUPPORT */

#if defined(CONFIG_JZ_SFC)
#define CONFIG_SPIFLASH_PART_OFFSET	0x3c00
#define CONFIG_SPI_NORFLASH_PART_OFFSET	0x3c74
#define CONFIG_NOR_MAJOR_VERSION_NUMBER	1
#define CONFIG_NOR_MINOR_VERSION_NUMBER	0
#define CONFIG_NOR_REVERSION_NUMBER	0
#define CONFIG_NOR_VERSION		(CONFIG_NOR_MAJOR_VERSION_NUMBER | (CONFIG_NOR_MINOR_VERSION_NUMBER << 8) | (CONFIG_NOR_REVERSION_NUMBER <<16))
#endif
/* END SFC */

/* MTD */
#define CONFIG_MTD_PARTITIONS
#define CONFIG_MTD_DEVICE

/* GMAC */
#define GMAC_PHY_MII			1
#define GMAC_PHY_RMII			2
#define GMAC_PHY_GMII			3
#define GMAC_PHY_RGMII			4

#define CONFIG_NET_GMAC_PHY_MODE	GMAC_PHY_RMII

#define PHY_TYPE_DM9161			1
#define PHY_TYPE_88E1111		2
#define PHY_TYPE_8710A			3
#define PHY_TYPE_IP101G			4

#define CONFIG_NET_PHY_TYPE		PHY_TYPE_IP101G

#define CONFIG_NET_GMAC

/* GPIO */
#define CONFIG_JZ_GPIO

/************************ USB CONFIG ***************************/
#define CONFIG_CMD_USB
#if defined(CONFIG_CMD_USB)
#define CONFIG_USB_DWC2
#define CONFIG_USB_DWC2_REG_ADDR	0x13500000
#define CONFIG_USB_HOST_ETHER
#define CONFIG_USB_ETHER_ASIX
#define CONFIG_USB_STORAGE
#endif

/**
 * Serial download configuration
 */
#define CONFIG_LOADS_ECHO		1	/* echo on for serial download */

/**
 * Miscellaneous configurable options
 */
#define CONFIG_DOS_PARTITION

#define CONFIG_LZO
#define CONFIG_RBTREE
#define CONFIG_LZMA

#define CONFIG_SKIP_LOWLEVEL_INIT
#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_SYS_NO_FLASH
#define CONFIG_SYS_FLASH_BASE		0 /* init flash_base as 0 */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_MISC_INIT_R		1

#define CONFIG_BOOTP_MASK		(CONFIG_BOOTP_DEFAUL)

#define CONFIG_SYS_MAXARGS		64
#define CONFIG_SYS_LONGHELP
#define CONFIG_SYS_PROMPT		CONFIG_SYS_BOARD "# "
/* #define CONFIG_SYS_CBSIZE		1024 */
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)

/*
#define CONFIG_SYS_MONITOR_LEN		(214 * 1024)
#define CONFIG_SYS_MALLOC_LEN		(32 * 1024 * 1024)
#define CONFIG_SYS_BOOTPARAMS_LEN	(128 * 1024)
*/

#define CONFIG_SYS_SDRAM_BASE		0x80000000 /* cached (KSEG0) address */
#define CONFIG_SYS_SDRAM_MAX_TOP	0x84000000 /* don't run into IO space */
#define CONFIG_SYS_INIT_SP_OFFSET	0x400000
#define CONFIG_SYS_LOAD_ADDR		0x82000000
#define CONFIG_SYS_MEMTEST_START	0x80000000
#define CONFIG_SYS_MEMTEST_END		0x84000000

#define CONFIG_SYS_TEXT_BASE		0x80100000
#define CONFIG_SYS_MONITOR_BASE		CONFIG_SYS_TEXT_BASE

/**
 * SPL configuration
 */
#define CONFIG_SPL
#define CONFIG_SPL_FRAMEWORK

#define CONFIG_SPL_NO_CPU_SUPPORT_CODE
#define CONFIG_SPL_START_S_PATH		"$(CPUDIR)/$(SOC)"

#if defined(CONFIG_SPL_NOR_SUPPORT)
#define CONFIG_SPL_LDSCRIPT		"$(CPUDIR)/$(SOC)/u-boot-nor-spl.lds"
#else /* CONFIG_SPL_NOR_SUPPORT */
#define CONFIG_SPL_LDSCRIPT		"$(CPUDIR)/$(SOC)/u-boot-spl.lds"
#endif /* CONFIG_SPL_NOR_SUPPORT */

#define CONFIG_SPL_PAD_TO		26624
#define CONFIG_SPL_MAX_SIZE		(26 * 1024)

#define CONFIG_UBOOT_OFFSET		CONFIG_SPL_MAX_SIZE
#define CONFIG_MMC_RAW_UBOOT_OFFSET	(CONFIG_UBOOT_OFFSET / 1024 + 17)
#define CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR	(CONFIG_MMC_RAW_UBOOT_OFFSET * 2)
#define CONFIG_SYS_U_BOOT_MAX_SIZE_SECTORS	0x400 /* 512 KB */

#define CONFIG_SPL_BOARD_INIT
#define CONFIG_SPL_LIBGENERIC_SUPPORT
#define CONFIG_SPL_GPIO_SUPPORT

#if defined(CONFIG_SPL_NOR_SUPPORT)
#define CONFIG_SPL_TEXT_BASE		0xba000000
#else
#define CONFIG_SPL_TEXT_BASE		0x80001000
#endif	/*CONFIG_SPL_NOR_SUPPORT*/

#define CONFIG_SPL_LZOP
#if defined(CONFIG_SPL_LZOP)
#define CONFIG_DECMP_BUFFER_ADRS	0x80200000
#endif

#if defined(CONFIG_SPL_MMC_SUPPORT)
#define CONFIG_SPL_SERIAL_SUPPORT
#endif /* CONFIG_SPL_MMC_SUPPORT */

#if defined(CONFIG_SPL_SPI_SUPPORT)
#define CONFIG_SPL_SERIAL_SUPPORT
#define CONFIG_SPI_SPL_CHECK
#define CONFIG_SYS_SPI_BOOT_FREQ	1000000
#endif /* CONFIG_SPL_SPI_SUPPORT */

#if defined(CONFIG_SPL_NOR_SUPPORT)
#define CONFIG_SPL_SERIAL_SUPPORT
#define CONFIG_SYS_UBOOT_BASE		(CONFIG_SPL_TEXT_BASE + CONFIG_SPL_PAD_TO - 0x40) //0x40 = sizeof (image_header)
#define CONFIG_SYS_OS_BASE		0
#define CONFIG_SYS_SPL_ARGS_ADDR	0
#define CONFIG_SYS_FDT_BASE		0
#endif /* CONFIG_SPL_NOR_SUPPORT */

/**
 * GPT configuration
 */
#if defined(CONFIG_GPT_CREATOR)
#define CONFIG_GPT_TABLE_PATH		"$(TOPDIR)/board/$(BOARDDIR)"
#else
/* USE MBR + zero-GPT-table instead if no gpt table defined*/
#define CONFIG_MBR_P0_OFF		64mb
#define CONFIG_MBR_P0_END		556mb
#define CONFIG_MBR_P0_TYPE		linux

#define CONFIG_MBR_P1_OFF		580mb
#define CONFIG_MBR_P1_END		1604mb
#define CONFIG_MBR_P1_TYPE		linux

#define CONFIG_MBR_P2_OFF		28mb
#define CONFIG_MBR_P2_END		58mb
#define CONFIG_MBR_P2_TYPE 		linux

#define CONFIG_MBR_P3_OFF		1609mb
#define CONFIG_MBR_P3_END		7800mb
#define CONFIG_MBR_P3_TYPE 		fat
#endif

/*
	Platform Default GPIOs
	These shall be specific to the SoC model
*/
#define CONFIG_GPIO_SETTINGS \
""

#include "isvp_common.h"

#endif /*__CONFIG_ISVP_T10_H__*/
