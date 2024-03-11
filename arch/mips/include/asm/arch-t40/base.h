/*
 * T40 REG_BASE definitions
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

#ifndef __BASE_H__
#define __BASE_H__

/*
 * Define the module base addresses
 */

/* AHB0 BUS Devices Base */
#define G_OST_BASE	0xb2000000
#define N_OST_BASE	0xb2100000
#define CCU_BASE	0xb2200000
#define INTCN_BASE	0xb2300000
#define SRAM_BASE	0xb2400000
#define NNDMA_BASE	0xb2500000
#define LEPOST_BASE	0xb2600000
#define LEPCCU_BASE	0xb2700000

#define HARB0_BASE	0xb3000000
#define DDRC_BASE	0xb34f0000
#define DDR_PHY_OFFSET	(-0x4e0000 + 0x1000)
#define DDRC_APB_OFFSET	(-0x4e0000 + 0x2000)
#define LCDC0_BASE	0xb3050000
#define MSC0_BASE	0xb3060000
#define MSC1_BASE	0xb3070000
#define IPU0_BASE	0xb3080000
#define BSCALER_BASE	0xb3090000
#define MONITOR_BASE	0xb30e0000
#define TIZIANO_BASE	0xb3300000

/* AHB1 BUS Devices Base */
#define RADIX_BASE	0xb3100000
#define EL150_BASE	0xb3200000

/* AHB2 BUS Devices Base */
#define HARB2_BASE	0xb3400000
#define NEMC_BASE	0xb3410000
#define PDMA_BASE	0xb3420000
#define AES_BASE	0xb3430000
#define SFC_BASE	0xb3440000
#define HASH_BASE	0xb3480000
#define GMAC_BASE	0xb34b0000
#define RSA_BASE	0xb34c0000
#define OTG_BASE	0xb3500000
#define EFUSE_BASE	0xb3540000


#define	RISCV_BASE	0xb2210000

/* APB BUS Devices Base */
#define	CPM_BASE	0xb0000000
#define	INTC_BASE	0xb0001000
#define	TCU_BASE	0xb0002000
#define	RTC_BASE	0xb0003000
#define	GPIO_BASE	0xb0010000
#define	AIC0_BASE	0xb0020000
#define	CODEC_BASE	0xb0021000
#define	MIPI_PHY_BASE	0xb0022000
#define	MIPI_BASE	0xb0023000
#define	UART0_BASE	0xb0030000
#define	UART1_BASE	0xb0031000
#define	UART2_BASE	0xb0032000
#define	UART3_BASE	0xb0033000
#define	DMIC_BASE	0xb0034000
#define	SSI_SLV_BASE	0xb0040000 //SSI_SLV
#define	SSI0_BASE	0xb0043000
#define	SSI1_BASE	0xb0044000
#define	I2C0_BASE	0xb0050000
#define	I2C1_BASE	0xb0052000
#define	I2C2_BASE	0xb0053000
#define	I2C3_BASE	0xb0051000
#define	USB_BASE	0xb0060000
#define	DES_BASE	0xb0061000
#define	SADC_BASE	0xb0070000
#define	DTRNG_BASE	0xb0072000
#define	WDT_BASE	0xb0002000

/* NAND CHIP Base Address*/
#define NEMC_CS1_BASE	0xbb000000
#define NEMC_CS2_BASE	0xba000000
#define NEMC_CS3_BASE	0xb9000000
#define NEMC_CS4_BASE	0xb8000000
#define NEMC_CS5_BASE	0xb7000000
#define NEMC_CS6_BASE	0xb6000000

#define AUX_BASE	0xb32a0000
#endif /* __BASE_H__ */
