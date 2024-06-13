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

#ifndef __CONFIG_ISVP_T20_H__
#define __CONFIG_ISVP_T20_H__

#include "isvp_common.h"

/**
 * Boot arguments definitions.
 */
#define BOOTARGS_COMMON "mem=\\${osmem} ispmem=\\${ispmem} rmem=\\${rmem}"
#if defined(CONFIG_DDR2_128M)
#define CONFIG_EXTRA_SETTINGS \
"osmem=99M@0x0\0" \
"ispmem=8M@0x6000000\0" \
"rmem=24M@0x6800000\0"
#else
#define CONFIG_EXTRA_SETTINGS \
"osmem=39M@0x0\0" \
"ispmem=5M@0x2000000\0" \
"rmem=20M@0x2800000\0"
#endif

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

/*
*	Platform Default GPIOs
*	These shall be specific to the SoC model
*	48 low = Wyze V2 MMC0 enable
*/
#define CONFIG_GPIO_SETTINGS \
"gpio_default_net=48o\0"

#endif /*__CONFIG_ISVP_T20_H__*/
