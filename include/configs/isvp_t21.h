/*
 * Ingenic isvp configuration
 *
 * Copyright (c) 2018  Ingenic Semiconductor Co.,Ltd
 * Author: Niky <xianghui.shen@ingenic.com>
 * Based on: include/configs/urboard.h
 *           Written by Paul Burton <paul.burton@imgtec.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_ISVP_T21_H__
#define __CONFIG_ISVP_T21_H__

#include "isvp_common.h"

/**
 * Boot arguments definitions.
 */
#define BOOTARGS_COMMON "mem=\\${osmem} rmem=\\${rmem}"
#if defined(CONFIG_DDR2_128M)
#define CONFIG_EXTRA_SETTINGS \
"osmem=106M@0x0\0" \
"rmem=22M@0x6a00000\0"
#else
#define CONFIG_EXTRA_SETTINGS \
"osmem=43M@0x0\0" \
"rmem=21M@0x2B00000\0"
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
/* #define CONFIG_SPL_SFC_NOR */
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

#if defined(CONFIG_NORFLASH_32M)
#define CONFIG_SPI_FLASH_BAR
#endif

/* MTD */
#define CONFIG_MTD_PARTITIONS
#define CONFIG_MTD_DEVICE

/*
 * Platform Default GPIOs
 * These shall be specific to the SoC model
*/
#define CONFIG_GPIO_SETTINGS \
""

#endif /*__CONFIG_ISVP_T21_H__*/
