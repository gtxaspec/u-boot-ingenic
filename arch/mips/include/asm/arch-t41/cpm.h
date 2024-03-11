/*
 * T21  cpm definitions
 *
 * Copyright (c) 2017 Ingenic Semiconductor Co.,Ltd
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

#ifndef __CPM_H__
#define __CPM_H__
#include <asm/arch/base.h>
/**********CGU registers Configuration*****/
#define CPM_CPCCR	        (0x00)/*Clock Control Register*/
#define CPM_CPPCR	        (0x0C)/*PLL Control Register*/
#define CPM_CPAPCR	        (0x10)/*APLL Control Register*/
#define CPM_CPMPCR	        (0x14)/*MPLL Control Register*/
#define CPM_DDRCDR	        (0x2C)/*DDR Memory clock divider Register*/
#define CPM_CPSPR 	        (0x34)/*CPM Scratch Pad Register*/
#define CPM_CPSPPR	        (0x38)/*CPM Scratch Pad Protected Register*/
#define CPM_USBPCR	        (0x3C)/*USB Parameter Control Register*/
#define CPM_USBRDT	        (0x40)/*USB Reset Detect Timer Register*/
#define CPM_USBVBFIL	    (0x44)/*USB VBUS Jitter Filter Register*/
#define CPM_USBPCR1	        (0x48)/*USB Parameter Control Register1*/
#define CPM_RSACDR	        (0x4C)/*RSA clock divider Register*/
#define CPM_SLVCDR          (0x50)/*SSI_SLV clock divider Register*/
#define CPM_MACCDR	        (0x54)/*MAC PHY clock divider Register*/
#define CPM_SFC0CDR         (0x60)/*SFC0 clock divider Register*/
#define CPM_LPCDR	        (0x64)/*LCD pixel clock divider Register*/
#define CPM_MSC0CDR	        (0x68)/*MSC0 device clock divider Register*/
#define CPM_MSC1CDR	        (0x6C)/*MSC1 device clock divider Register*/
#define CPM_I2STCDR	        (0x70)/*I2S Transmit clock divider Register*/
#define CPM_SSICDR	        (0x74)/*SSI device clock divider Register*/
#define CPM_SFC1CDR	        (0x7C)/*SFC1 clock divider Register*/
#define CPM_I2STCDR1	    (0x78)/*I2S Transmit clock divider Register*/
#define CPM_ISPMCDR	        (0x80)/*ISPM clock divider Register*/
#define CPM_I2SRCDR	        (0x84)/*I2S Receive clock divider Register*/
#define CPM_I2SRCDR1	    (0x88)/*I2S Receive clock divider Register 1*/
#define CPM_OSC_CTRL	    (0x8C)/*Oscillator Control Register*/
#define CPM_CIM0CDR	        (0x90)/*CIM0 MCLK clock divider Register*/
#define CPM_SOFTAPP	        (0x9C)/*Soft Configure For Application Register*/
#define CPM_LDCCDR	        (0xA0)/*LDC clock divider Register*/
#define CPM_ISPSCDR 	    (0xA8)/*ISPS clock divider Register*/
#define CPM_ISPACDR 	    (0xAC)/*ISPA clock divider Register*/
#define CPM_INTR	        (0xB0)/*CPM interrupt Register*/
#define CPM_INTRE	        (0xB4)/*CPM interrupt enable Register*/
#define CPM_BT0CDR	        (0xB8)/*BT0 CLK clock divider Register*/
#define CPM_EL200	        (0xBC)/*ALGENC clock divider Register*/
#define CPM_DRCG	        (0xD0)/*DDR Clock Gate Register*/
#define CPM_CPCSR	        (0xD4)/*Clock Status Register*/
#define CPM_CPVPCR	        (0xE0)/*VPLL Control Register*/
#define CPM_GMACPHYC	    (0xE8)/*MAC PHY Control Register*/
#define CPM_APR             (0xF0)/*ARBITER PRIORITY Register*/
#define CPM_PWMCDR          (0xFC)/*PWM clock divider Register*/

/************power/reset management*********/
#define CPM_LCR		        (0x04)
#define CPM_CLKGR0          (0x20)
#define CPM_OPCR            (0x24)
#define CPM_CLKGR1          (0x28)

#define CPM_SRBC0           (0xc4)
#define CPM_MESTSEL         (0xEC)
/******************************************/

/**********CLKGR0 0x20**************/
#define CPM_CLKGR_DDR		(1 << 31)
#define CPM_CLKGR_TCU		(1 << 30)
#define CPM_CLKGR_RTC		(1 << 29)
#define CPM_CLKGR_DES		(1 << 28)
#define CPM_CLKGR_RSA		(1 << 27)
#define CPM_CLKGR_VO		(1 << 26)
#define CPM_CLKGR_MIPI_CSI	(1 << 25)
#define CPM_CLKGR_LCD		(1 << 24)
#define CPM_CLKGR_ISP		(1 << 23)
#define CPM_CLKGR_PDMA		(1 << 22)
#define CPM_CLKGR_SFC0		(1 << 21)
#define CPM_CLKGR_SSI1		(1 << 20)
#define CPM_CLKGR_UART5 	(1 << 19)
#define CPM_CLKGR_UART4 	(1 << 18)
#define CPM_CLKGR_UART3     (1 << 17)
#define CPM_CLKGR_UART2		(1 << 16)
#define CPM_CLKGR_UART1		(1 << 15)
#define CPM_CLKGR_UART0		(1 << 14)
#define CPM_CLKGR_SADC		(1 << 13)
#define CPM_CLKGR_DMIC		(1 << 12)
#define CPM_CLKGR_AIC		(1 << 11)
#define CPM_CLKGR_SSI_SLV	(1 << 10)
#define CPM_CLKGR_I2C2		(1 << 9)
#define CPM_CLKGR_I2C1		(1 << 8)
#define CPM_CLKGR_I2C0		(1 << 7)
#define CPM_CLKGR_SSI0		(1 << 6)
#define CPM_CLKGR_MSC1		(1 << 5)
#define CPM_CLKGR_MSC0		(1 << 4)
#define CPM_CLKGR_OTG		(1 << 3)
#define CPM_CLKGR_SC_HASH	(1 << 2)
#define CPM_CLKGR_EFUSE		(1 << 1)
#define CPM_CLKGR_NEMC		(1 << 0)

/**********CLKGR1 0x28**************/
#define CPM_CLKGR1_IVDC		(1 << 17)
#define CPM_CLKGR1_CPU		(1 << 16)
#define CPM_CLKGR1_APB0		(1 << 15)
#define CPM_CLKGR1_JPEG		(1 << 14)
#define CPM_CLKGR1_LDC		(1 << 13)
#define CPM_CLKGR1_SFC1		(1 << 12)
#define CPM_CLKGR1_SYS_OST	(1 << 11)
#define CPM_CLKGR1_AHB0		(1 << 10)
#define CPM_CLKGR1_BUS_MONITOR		(1 << 9)
#define CPM_CLKGR1_I2D		(1 << 8)
#define CPM_CLKGR1_PWM  	(1 << 7)
#define CPM_CLKGR1_DRAWBOX  (1 << 6)
#define CPM_CLKGR1_AES		(1 << 5)
#define CPM_CLKGR1_GMAC		(1 << 4)
#define CPM_CLKGR1_LZMA 	(1 << 3)
#define CPM_CLKGR1_IPU		(1 << 2)
#define CPM_CLKGR1_DTRNG	(1 << 1)
#define CPM_CLKGR1_EL200	(1 << 0)

#define CPM_RSR_HR		    (1 << 3)
#define CPM_RSR_P0R		    (1 << 2)
#define CPM_RSR_WR		    (1 << 1)
#define CPM_RSR_PR		    (1 << 0)

#define OPCR_ERCS		    (0x1<<2)
#define OPCR_PD			    (0x1<<3)
#define OPCR_IDLE		    (0x1<<31)

#define cpm_readl(off)          readl(CPM_BASE + (off))
#define cpm_writel(val,off)     writel(val, CPM_BASE + (off))
#define cpm_inl(off)		readl(CPM_BASE + (off))
#define cpm_outl(val,off)	writel(val, CPM_BASE + (off))
#define cpm_test_bit(bit,off)	(cpm_inl(off) & 0x1<<(bit))
#define cpm_set_bit(bit,off)	(cpm_outl((cpm_inl(off) | 0x1<<(bit)),off))
#define cpm_clear_bit(bit,off)	(cpm_outl(cpm_inl(off) & ~(0x1 << bit), off))

/*USBCDR*/
#define USBCDR_UCS_PLL		(1 << 31)
#define USBCDR_UPCS_MPLL	(1 << 30)
#define USBCDR_CE_USB		(1 << 29)
#define USBCDR_USB_BUSY		(1 << 28)
#define USBCDR_USB_STOP		(1 << 27)
#define USBCDR_USB_DIS		(1 << 26)
#define USBCDR_MIPI_CS		(1 << 25)
#define USBCDR_USBCDR_MSK	(0xff)

/*USBPCR*/
#define USBPCR_USB_MODE_ORG	(1 << 31)
#define USBPCR_VBUSVLDEXT	(1 << 24)
#define USBPCR_VBUSVLDEXTSEL	(1 << 23)
#define USBPCR_POR		(1 << 22)
#define USBPCR_OTG_DISABLE	(1 << 20)

/*USBPCR1*/
#define USBPCR1_REFCLKSEL_BIT	(26)
#define USBPCR1_REFCLKSEL_MSK	(0x3 << USBPCR1_REFCLKSEL_BIT)
#define USBPCR1_REFCLKSEL_CORE	(0x2 << USBPCR1_REFCLKSEL_BIT)
#define USBPCR1_REFCLKSEL_EXT	(0x1 << USBPCR1_REFCLKSEL_BIT)
#define USBPCR1_REFCLKSEL_CSL	(0x0 << USBPCR1_REFCLKSEL_BIT)
#define USBPCR1_REFCLKDIV_BIT	(24)
#define USBPCR1_REFCLKDIV_MSK	(0X3 << USBPCR1_REFCLKDIV_BIT)
#define USBPCR1_REFCLKDIV_19_2M	(0x3 << USBPCR1_REFCLKDIV_BIT)
#define USBPCR1_REFCLKDIV_48M	(0x2 << USBPCR1_REFCLKDIV_BIT)
#define USBPCR1_REFCLKDIV_24M	(0x1 << USBPCR1_REFCLKDIV_BIT)
#define USBPCR1_REFCLKDIV_12M	(0x0 << USBPCR1_REFCLKDIV_BIT)
#define USBPCR1_WORD_IF0_16_30	(1 << 19)

/*OPCR*/
#define OPCR_SPENDN0		(1 << 7)

/* CPM scratch pad protected register(CPSPPR) */
#define CPSPPR_CPSPR_WRITABLE   (0x00005a5a)
#define RECOVERY_SIGNATURE      (0x1a1a)        /* means "RECY" */
#define RECOVERY_SIGNATURE_SEC  0x800           /* means "RECY" */
#define FASTBOOT_SIGNATURE      (0x0666)        /* means "FASTBOOT" */

#define cpm_get_scrpad()        readl(CPM_BASE + CPM_CPSPR)
#define cpm_set_scrpad(data)                    \
do {                                            \
	volatile int i = 0x3fff;                \
	writel(0x5a5a,CPM_BASE + CPM_CPSPPR);		\
	while(i--);				\
	writel(data,CPM_BASE + CPM_CPSPR);			\
	writel(0xa5a5,CPM_BASE + CPM_CPSPPR);      	\
} while (0)

#endif /* __CPM_H__ */
