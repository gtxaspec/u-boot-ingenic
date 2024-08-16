/*
 * Ingenic isvp T31 configuration
 *
 * Copyright (c) 2019  Ingenic Semiconductor Co.,Ltd
 * Author: Damon <jiansheng.zhang@ingenic.com>
 * Based on: include/configs/urboard.h
 *           Written by Paul Burton <paul.burton@imgtec.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_ISVP_T31_H__
#define __CONFIG_ISVP_T31_H__

#include "isvp_common.h"

/* Device Tree Configuration */
/*
#define CONFIG_OF_LIBFDT		1
*/
#if defined(CONFIG_OF_LIBFDT)
#define IMAGE_ENABLE_OF_LIBFDT		1
#define CONFIG_LMB
#endif

/**
 * Boot arguments definitions.
 */
#define BOOTARGS_COMMON "mem=\\${osmem} rmem=\\${rmem}"
#if defined(CONFIG_DDR2_128M) || defined(CONFIG_DDR3_128M)
#define CONFIG_EXTRA_SETTINGS \
"osmem=99M@0x0\0" \
"rmem=29M@0x6300000\0"
#else
#define CONFIG_EXTRA_SETTINGS \
"osmem=42M@0x0\0" \
"rmem=22M@0x2a00000\0"
#endif

#if defined(CONFIG_SPL_SFC_NOR) || defined(CONFIG_SPL_SFC_NAND)
/*#define CONFIG_SPL_SFC_SUPPORT*/
#define CONFIG_JZ_SFC
#define CONFIG_SPL_VERSION		1
#if defined(CONFIG_SPL_SFC_NOR)
#define CONFIG_SFC_NOR
#else
#define CONFIG_SFC_NAND
#endif
#define CONFIG_SPI_DUAL
/*#define CONFIG_SPI_QUAD*/
#endif /* defined(CONFIG_SPL_SFC_NOR) || defined(CONFIG_SPL_SFC_NAND) */

/*
#if defined(CONFIG_SPL_MMC_SUPPORT)
#define CONFIG_BOOTARGS BOOTARGS_COMMON " init=/linuxrc root=/dev/mmcblk0p2 rw rootdelay=1"
#elif defined(CONFIG_SFC_NOR)
#if defined(CONFIG_OF_LIBFDT)
#define CONFIG_BOOTARGS BOOTARGS_COMMON " init=/linuxrc rootfstype=squashfs root=/dev/mtdblock2 rw mtdparts=jz_sfc:320k(boot),2496k(kernel),2048k(root),64k(dtb),-(appfs)"
#else
#define CONFIG_BOOTARGS BOOTARGS_COMMON " init=/linuxrc rootfstype=squashfs root=/dev/mtdblock2 rw mtdparts=jz_sfc:256k(boot),2560k(kernel),2048k(root),-(appfs)"
#endif
#elif defined(CONFIG_SFC_NAND)
#define CONFIG_BOOTARGS BOOTARGS_COMMON " ip=off init=/linuxrc ubi.mtd=2 root=ubi0:rootfs ubi.mtd=3 rootfstype=ubifs rw"
#endif
*/

/**
 * Boot command definitions.
 */

/*
#if defined(CONFIG_SPL_MMC_SUPPORT)
#define CONFIG_BOOTCOMMAND "mmc read 0x80600000 0x1800 0x3000; bootm 0x80600000"
#endif

#if defined(CONFIG_SFC_NOR)
#if defined(CONFIG_OF_LIBFDT)
#define CONFIG_BOOTCOMMAND "sf probe;sf read 0x80600000 0x50000 0x270000;sf read 0x83000000 0x540000 0x10000;bootm 0x80600000 - 0x83000000"
#else
#define CONFIG_BOOTCOMMAND "sf probe;sf read 0x80600000 0x40000 0x280000; bootm 0x80600000"
#endif
#endif

#if defined(CONFIG_SFC_NAND)
#define CONFIG_BOOTCOMMAND "nand read 0x80600000 0x100000 0x800000;bootm 0x80600000"
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

#if defined(CONFIG_JZ_MMC_MSC0) && defined(CONFIG_JZ_MMC_MSC1)
#define CONFIG_JZ_MMC_SPLMSC		1
#define CONFIG_JZ_MMC_MSC0_PB		1
#define CONFIG_JZ_MMC_MSC1_PB		1
#else
#if defined(CONFIG_JZ_MMC_MSC0)
#define CONFIG_JZ_MMC_SPLMSC		0
#define CONFIG_JZ_MMC_MSC0_PB		1
#endif
#endif /* JZ_MMC_MSC0 && JZ_MMC_MSC1 */

/*
#if defined(CONFIG_SFC_COMMAND)
*/
#define CONFIG_SFC_NOR_COMMAND
/*
#else
#define CONFIG_SFC_NAND_COMMAND
#endif
*/

#if defined(CONFIG_SFC_NOR_COMMAND)
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
#if defined(CONFIG_SPL_SFC_SUPPORT) || defined(CONFIG_SFC_NAND_COMMAND)
#define CONFIG_SPL_SERIAL_SUPPORT
#define CONFIG_SPI_SPL_CHECK
#define CONFIG_JZ_SFC_PA
#if defined(CONFIG_SPL_SFC_NAND) || defined(CONFIG_SFC_NAND_COMMAND)
#define CONFIG_NAND_BURNER
#define CONFIG_SFC_NAND_RATE		100000000
#define CONFIG_SPIFLASH_PART_OFFSET	CONFIG_SPL_MAX_SIZE
#define CONFIG_SPI_NAND_BPP		(2048 + 64)	/*Bytes Per Page*/
#define CONFIG_SPI_NAND_PPB		(64)		/*Page Per Block*/
#define CONFIG_MTD_SFCNAND
#define CONFIG_CMD_SFCNAND
#define CONFIG_CMD_NAND
#define CONFIG_SPI_SPL_CHECK
#define CONFIG_SYS_MAX_NAND_DEVICE	1
#define CONFIG_SYS_NAND_BASE		0xb3441000
#define CONFIG_MTD_DEVICE
#define CONFIG_CMD_UBI
#define CONFIG_CMD_UBIFS
#define CONFIG_MTD_PARTITIONS
#define MTDIDS_DEFAULT			"nand0=nand"
#define MTDPARTS_DEFAULT		"mtdparts=nand:1M(boot),8M(kernel),40M(rootfs),-(data)"
#define CONFIG_SPI_STANDARD /* if the nand is QUAD mode, please annotate it. the default is one lan. */
#if defined(CONFIG_SPL_SFC_SUPPORT)
/*SFCNAND env*/
/* spi nand environment */
#define CONFIG_SYS_REDUNDAND_ENVIRONMENT
/* #define CONFIG_ENV_SECT_SIZE		0x20000 */
#define SPI_NAND_BLK			0x20000
/* #define CONFIG_ENV_SIZE		SPI_NAND_BLK */
/* #define CONFIG_ENV_OFFSET		0xC0000 */
#define CONFIG_ENV_OFFSET_REDUND	(CONFIG_ENV_OFFSET + CONFIG_ENV_SIZE)
#define CONFIG_ENV_IS_IN_SFC_NAND
#endif
/* MTD support */
/* #define CONFIG_SYS_NAND_SELF_INIT */
#define CONFIG_UBOOT_OFFSET		CONFIG_SPL_MAX_SIZE

#elif defined(CONFIG_SPL_SFC_NOR)
#define CONFIG_CMD_SF
#define CONFIG_SPI_FLASH_INGENIC
#define CONFIG_SPI_FLASH
#define CONFIG_UBOOT_OFFSET		CONFIG_SPL_MAX_SIZE
#endif
#endif /* CONFIG_SPL_SFC_SUPPORT */

#if defined(CONFIG_NORFLASH_32M)
#define CONFIG_SPI_FLASH_BAR
#endif

/* MTD */
#define CONFIG_MTD_PARTITIONS
#define CONFIG_MTD_DEVICE

#define CONFIG_GPIO_IP101G_RESET	GPIO_PB(28)
#define CONFIG_GPIO_IP101G_RESET_ENLEVEL	0

/*
	Platform Default GPIOs
	These shall be specific to the SoC model
*/

/*
 * Reference:
 * T31ZX Smart Video Application Processor DATA SHEET
 * Page 20
 * Note: "Pin 60 and 61 share IO with PB15 and PB16..."
 * 25 and 26 shall be set to only be set to input if
 * GMAC doesn't exist or is disabled.
 */

#define CONFIG_GPIO_SETTINGS \
"gpio_default=25ID 26IDU\0" \
"gpio_default_net=39o\0"

#endif /*__CONFIG_ISVP_T31_H__*/
