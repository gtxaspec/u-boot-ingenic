/*
 * T40 pll configuration
 *
 * Copyright (c) 2017 Ingenic Semiconductor Co.,Ltd
 * Author: Zoro <ykli@ingenic.cn>
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

#define DEBUG

#include <config.h>
#include <common.h>
#include <asm/io.h>
#include <asm/errno.h>
#include <asm/arch/cpm.h>
#include <asm/arch/clk.h>

DECLARE_GLOBAL_DATA_PTR;

struct pll_cfg {
    unsigned apll_freq;
    unsigned mpll_freq;
    unsigned vpll_freq;
    unsigned cdiv;
    unsigned l2div;
    unsigned h0div;
    unsigned h2div;
    unsigned pdiv;
} pll_cfg;

#ifndef CONFIG_SYS_CPCCR_SEL
#define SEL_SRC     0x2
#define SEL_CPLL    ((CONFIG_CPU_SEL_PLL == APLL) ? 0x1 : 0x2)
#define SEL_H0CLK   ((CONFIG_DDR_SEL_PLL == APLL) ? 0x1 : 0x2)
#define SEL_H2CLK   SEL_H0CLK

#define CPCCR_CFG                              \
        (((SEL_SRC& 3) << 30)                  \
         | ((SEL_CPLL & 3) << 28)              \
         | ((SEL_H0CLK & 3) << 26)             \
         | ((SEL_H2CLK & 3) << 24)             \
         | (((pll_cfg.pdiv - 1) & 0xf) << 16)  \
         | (((pll_cfg.h2div - 1) & 0xf) << 12) \
         | (((pll_cfg.h0div - 1) & 0xf) << 8)  \
         | (((pll_cfg.l2div - 1) & 0xf) << 4)  \
         | (((pll_cfg.cdiv - 1) & 0xf) << 0))
#else
/**
 * Board CPCCR configure.
 * CONFIG_SYS_CPCCR_SEL should be define in [board].h
 */
#define CPCCR_CFG CONFIG_SYS_CPCCR_SEL
#endif

#if (!defined (CONFIG_SYS_APLL_MNOD)) || \
    (!defined (CONFIG_SYS_MPLL_MNOD)) || \
    (!defined (CONFIG_SYS_VPLL_MNOD))

static unsigned int get_pllreg_value(int freq) {
	int fref = gd->arch.gi->extal / 1000000;
	int fout = freq / 1000000;

	int m, n, od, od1, no;
	int rg_v, rg;
	int fvco;

	cpm_cpxpcr_t cppcr;

	for (m = 1; m <= 512; m++) {
		for (n = 1; n <= 64; n++) {
			for (od = 1; od <= 6; od++) {
				for (od1 = 1; od1 <= 16; od1++) {

					rg_v = fref / n;
					no = (1 << od) * od1;

					if ((fref % n) || (rg_v < 7) || (rg_v > 200)) {
						continue;
					}

					fvco = fout * no;

					if (((rg_v * m * 2) != (fvco)) || (fvco < 3000) || (fvco > 6000)) {
						continue;
					}

					if (rg_v >= 7 && fref <= 11) {
						rg = 1;
					} else if (rg_v > 11 && rg_v <= 18) {
						rg = 2;
					} else if (rg_v > 18 && rg_v <= 30) {
						rg = 3;
					} else if (rg_v > 30 && rg_v <= 50) {
						rg = 4;
					} else if (rg_v > 50 && rg_v <= 80) {
						rg = 5;
					} else if (rg_v > 80 && rg_v <= 130) {
						rg = 6;
					} else if (rg_v > 130 && rg_v <= 200) {
						rg = 7;
					}

					cppcr.b.PLLRG = rg;
					cppcr.b.PLLM = m - 1;
					cppcr.b.PLLN = n - 1;
					cppcr.b.PLLOD0 = od;
					cppcr.b.PLLOD1 = od1 - 1;
					return cppcr.d32;
				}
			}
		}
	}
	return 0;
}
#endif

/*********set CPXPCR register************/
static void pll_set(int pll, int freq) {
#ifndef CONFIG_SYS_APLL_MNOD
	unsigned int regvalue = 0;
#endif
	unsigned int tmp = 0;
	cpm_cpxpcr_t cppcr;

#if (!defined (CONFIG_SYS_APLL_MNOD)) || \
    (!defined (CONFIG_SYS_MPLL_MNOD)) || \
    (!defined (CONFIG_SYS_VPLL_MNOD))

#ifndef CONFIG_SIMULATION
	regvalue = get_pllreg_value(freq);
	if (regvalue == 0)
		return;
#endif

#endif
	switch (pll) {
		case APLL:
			/* Init APLL */
#ifdef CONFIG_SYS_APLL_MNOD
			cppcr.d32 = CONFIG_SYS_APLL_MNOD;
#else /* !CONFIG_SYS_APLL_MNOD */
			cppcr.d32 = regvalue;
#endif /* CONFIG_SYS_APLL_MNOD */
			tmp = cpm_inl(CPM_CPAPCR);
			tmp &= ~(0x1 << 0);
			cpm_outl(tmp, CPM_CPAPCR);
			udelay(1);
			cpm_outl(cppcr.d32 | 1 << 0, CPM_CPAPCR);
			while (!(cpm_inl(CPM_CPAPCR) & (0x1 << 3))) {}
			debug("CPM_CPAPCR %x\n", cpm_inl(CPM_CPAPCR));
			break;
		case MPLL:
			/* Init MPLL */
#ifdef CONFIG_SYS_MPLL_MNOD
			cppcr.d32 = CONFIG_SYS_MPLL_MNOD;
#else /* !CONFIG_SYS_MPLL_MNOD */
			cppcr.d32 = regvalue;
#endif /* CONFIG_SYS_MPLL_MNOD */
			tmp = cpm_inl(CPM_CPMPCR);
			tmp &= ~(0x1 << 0);
			cpm_outl(tmp, CPM_CPMPCR);
			udelay(1);
			cpm_outl(cppcr.d32 | 1 << 0, CPM_CPMPCR);
			while (!(cpm_inl(CPM_CPMPCR) & (0x1 << 3))) {}
			debug("CPM_CPMPCR %x\n", cpm_inl(CPM_CPMPCR));
			break;
		case VPLL:
			/* Init VPLL */
#ifdef CONFIG_SYS_VPLL_MNOD
			cppcr.d32 = CONFIG_SYS_VPLL_MNOD;
#else /* !CONFIG_SYS_VPLL_MNOD */
			cppcr.d32 = regvalue;
#endif /* CONFIG_SYS_VPLL_MNOD */
			tmp = cpm_inl(CPM_CPVPCR);
			tmp &= ~(0x1 << 0);
			cpm_outl(tmp, CPM_CPVPCR);
			udelay(1);
			cpm_outl(cppcr.d32 | 1 << 0, CPM_CPVPCR);
			while (!(cpm_inl(CPM_CPVPCR) & (0x1 << 3))) {}
			debug("CPM_CPVPCR %x\n", cpm_inl(CPM_CPVPCR));
			break;
		default:
			break;
	}
}

/*
 * bit 20:22 Enable write function for crossover value
 */
static void cpccr_init(void) {
	unsigned int cpccr;

	/* change div
	 * Change low 24 bits
	 * Change crossover value */
	cpccr = (cpm_inl(CPM_CPCCR) & (0xff << 24))
		| (CPCCR_CFG & ~(0xff << 24))
		| (7 << 20);
	cpm_outl(cpccr, CPM_CPCCR);
	while (cpm_inl(CPM_CPCSR) & 0x7);

	/* change sel
	 * Change high 8 bits
	 * Select clock source */
	cpccr = (CPCCR_CFG & (0xff << 24)) | (cpm_inl(CPM_CPCCR) & ~(0xff << 24));
	cpm_outl(cpccr, CPM_CPCCR);
	debug("cppcr 0x%x\n", cpm_inl(CPM_CPCCR));
}

/* pllfreq align*/
static int inline

align_pll(unsigned pllfreq, unsigned alfreq) {
	int div = 0;
	if (!(pllfreq % alfreq)) {
		div = pllfreq / alfreq ? pllfreq / alfreq : 1;
	} else {
		error("pll freq is not integer times than cpu freq or/and ddr freq");
		asm volatile ("wait\n\t");
	}
	return alfreq * div;
}

/* Least Common Multiple */
/*
static unsigned int lcm(unsigned int a, unsigned int b, unsigned int limit) {
	unsigned int lcm_unit = a > b ? a : b;
	unsigned int lcm_resv = a > b ? b : a;
	unsigned int lcm = lcm_unit;;

	debug("calculate lcm :a(cpu:%d) and b(ddr%d) 's\t", a, b);
	while (lcm % lcm_resv && lcm < limit)
		lcm += lcm_unit;

	if (lcm % lcm_resv) {
		error("\n a(cpu %d), b(ddr %d) : Can not find Least Common Multiple in range of limit\n", a, b);
		asm volatile ("wait\n\t");
	}
	debug("lcm is %d\n", lcm);
	return lcm;
}
*/

/*
 * ***********pll global variable assignment*********************
 * ***********Required to set the CPCCR register***************
 * */
static void final_fill_div(int cpll, int pclk) {
	unsigned cpu_pll_freq = (cpll == APLL) ? pll_cfg.apll_freq : pll_cfg.mpll_freq;
	unsigned Periph_pll_freq = (pclk == APLL) ? pll_cfg.apll_freq : pll_cfg.mpll_freq;

	/*DDRDIV*/
	gd->arch.gi->ddr_div = Periph_pll_freq / gd->arch.gi->ddrfreq;
	/*cdiv*/
	pll_cfg.cdiv = cpu_pll_freq / gd->arch.gi->cpufreq;

	switch (Periph_pll_freq / 100000000) {
		case 10 ... 18:
			pll_cfg.pdiv = 10;
			pll_cfg.h0div = 5;
			pll_cfg.h2div = 5;
			break;
		case 7 ... 9:
			pll_cfg.pdiv = 10;
			pll_cfg.h0div = 5;
			pll_cfg.h2div = 5;
			break;
		default:
			error("Periph pll freq %d is out of range\n", Periph_pll_freq);
	}

	pll_cfg.l2div = 2;

	printf("pll_cfg.pdiv = %d, pll_cfg.h2div = %d, pll_cfg.h0div = %d, pll_cfg.cdiv = %d, pll_cfg.l2div = %d\n",
	       pll_cfg.pdiv, pll_cfg.h2div, pll_cfg.h0div, pll_cfg.cdiv, pll_cfg.l2div);
	return;
}

static int freq_correcting(void) {
	// unsigned int pll_freq = 0;
	pll_cfg.mpll_freq = CONFIG_SYS_MPLL_FREQ > 0 ? CONFIG_SYS_MPLL_FREQ : 0;
#ifdef CONFIG_SLT
	{
		GPIO_CPUFREQ_TABLE;
		CPU_FREQ_TABLE;
#define get_cpufreq_index()					\
		({						\
			int i, val;				\
			int index = 0;				\
			int gpio_cnt = sizeof(gpio_cpufreq_table) / sizeof(gpio_cpufreq_table[0]); \
			for (i = 0; i < gpio_cnt; i++) {	\
				val = gpio_get_value(gpio_cpufreq_table[i]); \
				index |= val << i;		\
			}					\
			index;					\
		})
		pll_cfg.apll_freq = cpufreq_table[get_cpufreq_index()];
	}
#else /* CONFIG_SLT */
	pll_cfg.apll_freq = CONFIG_SYS_APLL_FREQ > 0 ? CONFIG_SYS_APLL_FREQ : 0;
	pll_cfg.vpll_freq = CONFIG_SYS_VPLL_FREQ > 0 ? CONFIG_SYS_VPLL_FREQ : 0;
#endif /* CONFIG_SLT */

	if (!gd->arch.gi->cpufreq && !gd->arch.gi->ddrfreq) {
		error("cpufreq = %d and ddrfreq = %d can not be zero, check board config\n",
		      gd->arch.gi->cpufreq, gd->arch.gi->ddrfreq);
		asm volatile ("wait\n\t");
	}
	//final_fill_div( CONFIG_CPU_SEL_PLL , CONFIG_DDR_SEL_PLL );

#define SEL_MAP(cpu, ddr) ((cpu<<16)|(ddr&0xffff))
#define PLL_MAXVAL (3000000000UL)
	switch (SEL_MAP(CONFIG_CPU_SEL_PLL, CONFIG_DDR_SEL_PLL)) {
/*
		case SEL_MAP(APLL, APLL):
			pll_freq = lcm(gd->arch.gi->cpufreq, gd->arch.gi->ddrfreq, PLL_MAXVAL);
			pll_cfg.apll_freq = align_pll(pll_cfg.apll_freq, pll_freq);
			final_fill_div(APLL, APLL);
			break;
		case SEL_MAP(MPLL, MPLL):
			pll_freq = lcm(gd->arch.gi->cpufreq, gd->arch.gi->ddrfreq, PLL_MAXVAL);
			pll_cfg.mpll_freq = align_pll(pll_cfg.mpll_freq, pll_freq);
			final_fill_div(MPLL, MPLL);
			break;
*/
		case SEL_MAP(APLL, MPLL):
			pll_cfg.mpll_freq = align_pll(pll_cfg.mpll_freq, gd->arch.gi->ddrfreq);
			pll_cfg.apll_freq = align_pll(pll_cfg.apll_freq, gd->arch.gi->cpufreq);
			final_fill_div(APLL, MPLL);
			break;
		case SEL_MAP(MPLL, APLL):
			pll_cfg.apll_freq = align_pll(pll_cfg.apll_freq, gd->arch.gi->ddrfreq);
			pll_cfg.mpll_freq = align_pll(pll_cfg.mpll_freq, gd->arch.gi->cpufreq);
			final_fill_div(MPLL, APLL);
			break;
	}
#undef PLL_MAXVAL
#undef SEL_MAP
	return 0;
}

#if 0
void pll_test(int pll)
{
	unsigned i = 0, count = 0;
	while (1) {
		for (i = 24000000; i <= 1200000000; i += 24000000) {
			pll_set(pll,i);
			debug("time = %d ,apll = %d\n", count * 100 + i/100000000, clk_get_rate(pll));
		}
		for (i = 1200000000; i >= 24000000 ; i -= 24000000) {
			pll_set(pll,i);
			debug("time = %d, apll = %d\n", count * 100 + 50 + i/100000000, clk_get_rate(pll));
		}
		count++;
	}
}
#endif

int pll_init(void) {
	freq_correcting();
	/* set bus selelet to extal */
	cpm_outl((cpm_inl(CPM_CPCCR) & ~(0xff << 24)) | 0x55 << 24, CPM_CPCCR);
	pll_set(APLL, pll_cfg.apll_freq);
	pll_set(MPLL, pll_cfg.mpll_freq);
	pll_set(VPLL, pll_cfg.vpll_freq);
	cpccr_init();
	{
		unsigned apll, mpll, cclk, l2clk, h0clk, h2clk, pclk, pll_tmp;
		unsigned vpll;
		apll = clk_get_rate(APLL);
		mpll = clk_get_rate(MPLL);
		vpll = clk_get_rate(VPLL);
#ifndef CONFIG_FAST_BOOT
		debug("apll_freq = %d\n", apll);
		debug("mpll_freq = %d\n", mpll);
		debug("vpll_freq = %d\n", vpll);
#endif
		if (CONFIG_DDR_SEL_PLL == APLL)
			pll_tmp = apll;
		else
			pll_tmp = mpll;

		gd->arch.gi->ddrfreq = pll_tmp / gd->arch.gi->ddr_div;
		h0clk = pll_tmp / pll_cfg.h0div;
		h2clk = pll_tmp / pll_cfg.h2div;
		pclk = pll_tmp / pll_cfg.pdiv;
		if (CONFIG_CPU_SEL_PLL == APLL)
			pll_tmp = apll;
		else
			pll_tmp = mpll;
		cclk = gd->arch.gi->cpufreq = pll_tmp / pll_cfg.cdiv;
		l2clk = pll_tmp / pll_cfg.l2div;
#ifndef CONFIG_FAST_BOOT
		debug("cclk = %d\n", cclk);
		debug("l2clk = %d\n", l2clk);
		debug("h0clk = %d\n", h0clk);
		debug("h2clk = %d\n", h2clk);
		debug("pclk = %d\n", pclk);
		debug("cpu clk source: %s\n",
		       CONFIG_CPU_SEL_PLL == APLL ? "apll" : "mpll");
		debug("ddr clk source: %s\n",
		       CONFIG_DDR_SEL_PLL == APLL ? "apll" : "mpll");
#endif
	}
	return 0;
}
