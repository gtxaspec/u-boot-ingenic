/*
 * T40 clock common interface
 *
 * Copyright (C) 2017 Ingenic Semiconductor Co.,Ltd
 * Author: Zoro <ykli@ingenic.cn>
 * Based on: newxboot/modules/clk/jz4775_clk.c
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#define DEBUG

#include <config.h>
#include <common.h>
#include <asm/io.h>
#include <asm/gpio.h>
#include <asm/arch/cpm.h>
#include <asm/arch/clk.h>

DECLARE_GLOBAL_DATA_PTR;

//#define DUMP_CGU_SELECT
#ifdef DUMP_CGU_SELECT
static char clk_name[][10] = {
	[EL200] = {"el200"},
	[MACPHY] = {"macphy"},
	[I2S] = {"i2s"},
	[SFC0] = {"sfc0"},
	[SFC1] = {"sfc1"},
	[CIM] = {"cim"},
	[ISPA] = {"ispa"},
	[ISPS] = {"isps"},
	[ISPM] = {"ispm"},
	[DDR] = {"ddr"},
	[MSC0] = {"msc0"},
	[MSC1] = {"msc1"},
	[LCD] = {"lcd"},
#ifdef CONFIG_JZ_PWM
	[PWM] = {"pwm"},
#endif
};

static char * cgu_name(int clk) {
	return clk_name[clk];
}
#endif

struct cgu cgu_clk_sel[CGU_CNT] = {
	[DDR] = {1, CPM_DDRCDR, 30, CONFIG_DDR_SEL_PLL, {0, APLL, MPLL, -1}, 29, 28, 27},
	[EL200] = {1, CPM_EL200, 30, APLL, {APLL, MPLL, VPLL}, 29, 28, 27},
	[MACPHY] = {1, CPM_MACCDR, 30, MPLL, {APLL, MPLL, VPLL}, 29, 28, 27},
	[LCD] = {1, CPM_LPCDR, 30, VPLL, {APLL, MPLL, VPLL}, 29, 28, 27},
#ifdef CONFIG_JZ_PWM
	[PWM] = {1, CPM_PWMCDR, 30, VPLL, {APLL, MPLL, VPLL}, 29, 28, 27},
#endif
	[MSC0] = {1, CPM_MSC0CDR, 30, APLL, {APLL, MPLL, VPLL}, 29, 28, 27},
	[MSC1] = {1, CPM_MSC1CDR, 30, MPLL, {APLL, MPLL, VPLL}, 29, 28, 27},
	[I2S] = {1, CPM_I2STCDR, 30, VPLL, {APLL, MPLL, VPLL}, 29, 28, 27}, //需要再配置; i2s 使用VPLL
	[SFC0] = {1, CPM_SFC0CDR, 30, MPLL, {APLL, MPLL, VPLL}, 29, 28, 27}, /* TODO */
	[SFC1] = {1, CPM_SFC1CDR, 30, MPLL, {APLL, MPLL, VPLL}, 29, 28, 27}, /* TODO */
	[CIM0] = {1, CPM_CIM0CDR, 30, APLL, {APLL, MPLL, VPLL}, 29, 28, 27},
	[ISPA] = {1, CPM_ISPACDR, 30, MPLL, {APLL, MPLL, VPLL}, 29, 28, 27},
	[ISPS] = {1, CPM_ISPSCDR, 30, MPLL, {APLL, MPLL, VPLL}, 29, 28, 27},
	[ISPM] = {1, CPM_ISPMCDR, 30, MPLL, {APLL, MPLL, VPLL}, 29, 28, 27},
};

void clk_prepare(void) {
	/*stop clk and set div max*/
	int id;
	struct cgu *cgu = NULL;
	unsigned regval = 0, reg = 0;

	for (id = 0; id < CGU_CNT; id++) {
		cgu = &(cgu_clk_sel[id]);
		reg = CPM_BASE + cgu->off;

		if (id == MSC0 || id == MSC1)
			continue;

		regval = readl(reg);
		/*set div max*/
		regval |= 0xff | (1 << cgu->ce);
		while (readl(reg) & (1 << cgu->busy));
		writel(regval, reg);

		/*stop clk*/
		while (readl(reg) & (1 << cgu->busy));
		regval = readl(reg);
		regval |= ((1 << cgu->stop) | (1 << cgu->ce));
		writel(regval, reg);
		while (readl(reg) & (1 << cgu->busy));

		/*clear ce*/
		regval = readl(reg);
		regval &= ~((1 << cgu->ce) | (1 << cgu->stop));
		writel(regval, reg);
#ifdef DUMP_CGU_SELECT
		debug("%s(0x%x) :0x%x\n", clk_name[id], reg, readl(reg));
#endif
	}
}

/***************Peripheral clock source selection 30 31 bits******************/
void cgu_clks_set(struct cgu *cgu_clks, int nr_cgu_clks) {
	int i, j, id;
	unsigned int xcdr = 0;
	unsigned int reg = 0;
	extern struct cgu_clk_src cgu_clk_src[];

	for (i = 0; cgu_clk_src[i].cgu_clk != SRC_EOF; i++) {
		id = cgu_clk_src[i].cgu_clk;
		cgu_clks[id].sel_src = cgu_clk_src[i].src;
	}

	for (i = 0; i < nr_cgu_clks; i++) {
		for (j = 0; j < 4; j++) {
			if (cgu_clks[i].sel_src == cgu_clks[i].sel[j] &&
			    cgu_clks[i].en == 1) {
				reg = CPM_BASE + cgu_clks[i].off;
				xcdr = readl(reg);
				xcdr &= ~(3 << 30);
				xcdr |= j << cgu_clks[i].sel_bit;
				writel(xcdr, reg);
#ifdef DUMP_CGU_SELECT
				debug("%s: 0x%X: value=0x%X j = %d\n", cgu_name(i), reg, readl(reg), j);
#endif
				break;
			}
		}

	}
}

static unsigned int pll_get_rate(int pll) {
	unsigned int cpxpcr = 0;
	unsigned int m, n, od0, od1;

	switch (pll) {
		case APLL:
#ifdef CONFIG_SYS_APLL_FREQ
			return CONFIG_SYS_APLL_FREQ;
#endif
			cpxpcr = cpm_inl(CPM_CPAPCR);
			break;
		case MPLL:
#ifdef CONFIG_SYS_MPLL_FREQ
			return CONFIG_SYS_MPLL_FREQ;
#endif
			cpxpcr = cpm_inl(CPM_CPMPCR);
			break;
		case VPLL:
#ifdef CONFIG_SYS_VPLL_FREQ
			return CONFIG_SYS_VPLL_FREQ;
#endif
			cpxpcr = cpm_inl(CPM_CPVPCR);
			break;
		default:
			return 0;
	}

	m = (cpxpcr >> 20) & 0x1ff;
	n = (cpxpcr >> 14) & 0x3f;
	od1 = (cpxpcr >> 11) & 0x7;
	od0 = (cpxpcr >> 7) & 0xf;
#ifdef CONFIG_BURNER
	return (unsigned int) (((unsigned long) (gd->arch.gi->extal / 1000000) * (m+1) * 2) / ((n + 1) * (1 << od0) * (od1 + 1) ));
#else
	return (unsigned int) (((unsigned long) (CONFIG_SYS_EXTAL / 1000000) * (m + 1) * 2) / ((n + 1) * (1 << od0) * (od1 + 1)));
#endif
}

static unsigned int get_ddr_rate(void) {
	unsigned int ddrcdr = cpm_inl(CPM_DDRCDR);

	switch ((ddrcdr >> 30) & 3) {
		case 1:
			return pll_get_rate(APLL) / ((ddrcdr & 0xf) + 1);
		case 2:
			return pll_get_rate(MPLL) / ((ddrcdr & 0xf) + 1);
	}
	return 0;
}

static unsigned int get_cclk_rate(void) {
	unsigned int cpccr = cpm_inl(CPM_CPCCR);

	switch ((cpccr >> 28) & 3) {
		case 1:
			return pll_get_rate(APLL) / ((cpccr & 0xf) + 1);
		case 2:
			return pll_get_rate(MPLL) / ((cpccr & 0xf) + 1);
	}
	return 0;
}

static unsigned int get_mac_rate(unsigned int xcdr) {
	unsigned int maccdr = cpm_inl(CPM_MACCDR);

	switch (maccdr >> 30) {
		case 0:
			xcdr = pll_get_rate(APLL) / ((maccdr & 0xff) + 1);
			break;
		case 1:
			xcdr = pll_get_rate(MPLL) / ((maccdr & 0xff) + 1);
			break;
		case 2:
			xcdr = pll_get_rate(VPLL) / ((maccdr & 0xff) + 1);
			break;
		default:
			break;
	}

	return 0;
}

static unsigned int get_msc_rate(unsigned int xcdr) {
	unsigned int msc0cdr = cpm_inl(CPM_MSC0CDR);
	unsigned int mscxcdr = cpm_inl(xcdr);
	unsigned int ret = 1;

	switch (msc0cdr >> 30) {
		case 0:
			ret = pll_get_rate(APLL) / (((mscxcdr & 0xff) + 1) * 2);
			break;
		case 1:
			ret = pll_get_rate(MPLL) / (((mscxcdr & 0xff) + 1) * 2);
			break;
		case 2:
			ret = pll_get_rate(VPLL) / (((mscxcdr & 0xff) + 1) * 2);
			break;
		default:
			break;
	}

	return ret;
}

unsigned int cpm_get_h2clk(void) {
	int h2clk_div;
	unsigned int cpccr = cpm_inl(CPM_CPCCR);

	h2clk_div = (cpccr >> 12) & 0xf;

	switch ((cpccr >> 24) & 3) {
		case 1:
			return pll_get_rate(APLL) / (h2clk_div + 1);
		case 2:
			return pll_get_rate(MPLL) / (h2clk_div + 1);
	}
	return 0;
}

unsigned int clk_get_rate(int clk) {
	switch (clk) {
		case DDR:
			return get_ddr_rate();
		case CPU:
			return get_cclk_rate();
		case H2CLK:
			return cpm_get_h2clk();
		case MACPHY:
			return get_mac_rate(CPM_MACCDR);
		case MSC0:
			return get_msc_rate(CPM_MSC0CDR);
		case MSC1:
			return get_msc_rate(CPM_MSC1CDR);
		case APLL:
			return pll_get_rate(APLL);
		case MPLL:
			return pll_get_rate(MPLL);
		case VPLL:
			return pll_get_rate(VPLL);
	}

	return 0;
}

/*
 *  Sets the divider value of the peripheral and the enable bit.
 *
 ***/
void clk_set_rate(int clk, unsigned long rate) {
	unsigned int cdr;
	unsigned int pll_rate;
	struct cgu *cgu = NULL;
	unsigned regval = 0;

	if (clk >= CGU_CNT) {
		debug("set clk id error\n");
		return;
	}

	cgu = &(cgu_clk_sel[clk]);
	regval = cpm_inl(cgu->off);

	pll_rate = pll_get_rate(cgu->sel_src);

	if (!pll_rate) {
		debug("clk id %d: get pll error\n", clk);
		return;
	}

	if (pll_rate % rate >= rate / 2)
		pll_rate += rate - (pll_rate % rate);
	else
		pll_rate -= (pll_rate % rate);

	/****************Lower 7 bits Crossover value**************/

	if (clk == MSC0 || clk == MSC1)
		cdr = ((pll_rate / rate) / 2 - 1) & 0xff;
	else
		cdr = (pll_rate / rate - 1) & 0xff;
	//debug("pll_rate = %d, rate = %d, cdr = %d\n",pll_rate,rate,cdr);

	/****************Changing the low 30s****************/
	if (clk == DDR)
		regval &= ~(0xf | 0x3f << 24);
	else
		regval &= ~(3 << cgu->stop | 0xff);
	regval |= ((1 << cgu->ce) | cdr);
	cpm_outl(regval, cgu->off);
	while (cpm_inl(cgu->off) & (1 << cgu->busy));
#ifdef DUMP_CGU_SELECT
	debug("%s(0x%x) :0x%x\n",clk_name[clk] ,regval,  cpm_inl(cgu->off));
#endif
	return;
}

void clk_init(void) {
	/*********Turn on the peripheral clock gating switch**********************/
	unsigned int reg_clkgr = cpm_inl(CPM_CLKGR0);
	unsigned int reg_clkgr1 = cpm_inl(CPM_CLKGR1);
	unsigned int gate = 0
#ifdef CONFIG_JZ_MMC_MSC0
	| CPM_CLKGR_MSC0
#endif
#ifdef CONFIG_JZ_MMC_MSC1
	| CPM_CLKGR_MSC1
#endif
#ifdef CONFIG_SFC_NOR
	| CPM_CLKGR_SFC0
	| CPM_CLKGR1_SFC1
#endif
#ifdef CONFIG_JZ_ADC
	| CPM_CLKGR_SADC
#endif
	;

	reg_clkgr &= ~gate;
	cpm_outl(reg_clkgr, CPM_CLKGR0);

	gate = 0
#ifdef CONFIG_NET_GMAC
		| CPM_CLKGR1_GMAC
#endif
		| CPM_CLKGR1_LZMA
		| CPM_CLKGR1_IVDC
#ifdef CONFIG_JZ_PWM
		| CPM_CLKGR1_PWM
#endif
		;

	reg_clkgr1 &= ~gate;
	cpm_outl(reg_clkgr1, CPM_CLKGR1);

	/*************Setting the peripheral clock source 31:30 ***************/
	cgu_clks_set(cgu_clk_sel, ARRAY_SIZE(cgu_clk_sel));
}

/*************Enable Serial Clock Gating**********************/
void enable_uart_clk(void) {
	unsigned int clkgr = cpm_inl(CPM_CLKGR0);

	switch (gd->arch.gi->uart_idx) {
#define _CASE(U, N) case U: clkgr &= ~N; break
		_CASE(0, CPM_CLKGR_UART0);
		_CASE(1, CPM_CLKGR_UART1);
		_CASE(2, CPM_CLKGR_UART2);
		_CASE(3, CPM_CLKGR_UART3);
		_CASE(4, CPM_CLKGR_UART4);
		_CASE(5, CPM_CLKGR_UART5);
		default:
			break;
	}
	cpm_outl(clkgr, CPM_CLKGR0);
}

void otg_phy_init(enum otg_mode_t mode, unsigned extclk) {
#ifndef CONFIG_SPL_BUILD
	int tmp_reg = 0;

	tmp_reg = cpm_inl(CPM_USBPCR1);
	tmp_reg &= ~(USBPCR1_REFCLKSEL_MSK | USBPCR1_REFCLKDIV_MSK);
	tmp_reg |= USBPCR1_REFCLKSEL_CORE | USBPCR1_WORD_IF0_16_30;
	switch (extclk / 1000000) {
		case 12:
			tmp_reg |= USBPCR1_REFCLKDIV_12M;
			break;
		case 19:
			tmp_reg |= USBPCR1_REFCLKDIV_19_2M;
			break;
		case 48:
			tmp_reg |= USBPCR1_REFCLKDIV_48M;
			break;
		case 24:
			tmp_reg |= USBPCR1_REFCLKDIV_24M;
			break;
		default:
			tmp_reg |= USBPCR1_REFCLKDIV_24M;
			break;
	}
	cpm_outl(tmp_reg, CPM_USBPCR1);

	tmp_reg = cpm_inl(CPM_USBPCR);
	switch (mode) {
		case OTG_MODE:
		case HOST_ONLY_MODE:
			tmp_reg |= USBPCR_USB_MODE_ORG;
			tmp_reg &= ~(USBPCR_VBUSVLDEXTSEL | USBPCR_VBUSVLDEXT | USBPCR_OTG_DISABLE);
			break;
		case DEVICE_ONLY_MODE:
			tmp_reg &= ~USBPCR_USB_MODE_ORG;
			tmp_reg |= USBPCR_VBUSVLDEXTSEL | USBPCR_VBUSVLDEXT | USBPCR_OTG_DISABLE;
	}
	cpm_outl(tmp_reg, CPM_USBPCR);

	tmp_reg = cpm_inl(CPM_OPCR);
	tmp_reg |= OPCR_SPENDN0;
	cpm_outl(tmp_reg, CPM_OPCR);

	tmp_reg = cpm_inl(CPM_USBPCR);
	tmp_reg |= USBPCR_POR;
	cpm_outl(tmp_reg, CPM_USBPCR);
	udelay(30);
	tmp_reg = cpm_inl(CPM_USBPCR);
	tmp_reg &= ~USBPCR_POR;
	cpm_outl(tmp_reg, CPM_USBPCR);
	udelay(300);

	tmp_reg = cpm_inl(CPM_CLKGR0);
	tmp_reg &= ~CPM_CLKGR_OTG;
	cpm_outl(tmp_reg, CPM_CLKGR0);
#endif
}
