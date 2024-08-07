
/*
 * DDR driver for inno DDR PHY.
 * Used by XBurst1 SoC
 *
 * Copyright (C) 2017 Ingenic Semiconductor Co.,Ltd
 * Author: Zoro <ykli@ingenic.cn>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/* #define DEBUG */
#include <config.h>
#include <common.h>
#include <ddr/ddr_common.h>
#include <generated/ddr_reg_values.h>

#include <asm/io.h>
#include <asm/arch/clk.h>
#include <asm/ddr_innophy.h>
#include "ddr_debug.h"

#if (CONFIG_DDR_CS1 == 1)
#ifndef DDR_ROW1
#error "please define DDR_ROW1"
#endif /* DDR_ROW1 */
#ifndef DDR_COL1
#error "please define DDR_COL1"
#endif /* DDR_COL1 */
#endif /* CONFIG_DDR_CS1 */

DECLARE_GLOBAL_DATA_PTR;
extern unsigned int sdram_size(int cs, struct ddr_params *p);

struct ddr_params *ddr_params_p = NULL;
#ifndef CONFIG_FPGA
extern void reset_dll(void);
#endif

#define BYPASS_ENABLE       1
#define BYPASS_DISABLE      0
#define IS_BYPASS_MODE(x)     (((x) & 1) == BYPASS_ENABLE)
	/* DDR3, */
	/* LPDDR, */
	/* LPDDR2, */
	/* DDR2,  */
	/* VARIABLE, */

#define DDR_TYPE_MODE(x)     (((x) >> 1) & 0xf)

#ifdef DEBUG
#define FUNC_ENTER()    debug("%s enter.\n",__FUNCTION__);
#define FUNC_EXIT()     debug("%s exit.\n",__FUNCTION__);

static void dump_ddrc_register(void)
{
	debug("DDRC_STATUS         0x%x\n", ddr_readl(DDRC_STATUS));
	debug("DDRC_CFG            0x%x\n", ddr_readl(DDRC_CFG));
	debug("DDRC_CTRL           0x%x\n", ddr_readl(DDRC_CTRL));
	debug("DDRC_LMR            0x%x\n", ddr_readl(DDRC_LMR));
	debug("DDRC_DLP            0x%x\n", ddr_readl(DDRC_DLP));
	debug("DDRC_TIMING1        0x%x\n", ddr_readl(DDRC_TIMING(1)));
	debug("DDRC_TIMING2        0x%x\n", ddr_readl(DDRC_TIMING(2)));
	debug("DDRC_TIMING3        0x%x\n", ddr_readl(DDRC_TIMING(3)));
	debug("DDRC_TIMING4        0x%x\n", ddr_readl(DDRC_TIMING(4)));
	debug("DDRC_TIMING5        0x%x\n", ddr_readl(DDRC_TIMING(5)));
	debug("DDRC_TIMING6        0x%x\n", ddr_readl(DDRC_TIMING(6)));
	debug("DDRC_REFCNT         0x%x\n", ddr_readl(DDRC_REFCNT));
	debug("DDRC_MMAP0          0x%x\n", ddr_readl(DDRC_MMAP0));
	debug("DDRC_MMAP1          0x%x\n", ddr_readl(DDRC_MMAP1));
	debug("DDRC_REMAP1         0x%x\n", ddr_readl(DDRC_REMAP(1)));
	debug("DDRC_REMAP2         0x%x\n", ddr_readl(DDRC_REMAP(2)));
	debug("DDRC_REMAP3         0x%x\n", ddr_readl(DDRC_REMAP(3)));
	debug("DDRC_REMAP4         0x%x\n", ddr_readl(DDRC_REMAP(4)));
	debug("DDRC_REMAP5         0x%x\n", ddr_readl(DDRC_REMAP(5)));
	debug("DDRC_AUTOSR_EN      0x%x\n", ddr_readl(DDRC_AUTOSR_EN));
}

void dump_inno_phy(void)
{
	debug("INNO_DQ_WIDTH   :%X\n",phy_readl(INNO_DQ_WIDTH));
	debug("INNO_PLL_FBDIV  :%X\n",phy_readl(INNO_PLL_FBDIV));
	debug("INNO_PLL_PDIV   :%X\n",phy_readl(INNO_PLL_PDIV));
	debug("INNO_MEM_CFG    :%X\n",phy_readl(INNO_MEM_CFG));
	debug("INNO_PLL_CTRL   :%X\n",phy_readl(INNO_PLL_CTRL));
	debug("INNO_CHANNEL_EN :%X\n",phy_readl(INNO_CHANNEL_EN));
	debug("INNO_CWL        :%X\n",phy_readl(INNO_CWL));
	debug("INNO_CL         :%X\n",phy_readl(INNO_CL));
}
#else
#define dump_ddrc_register()
#define dump_inno_phy()

#define FUNC_ENTER()
#define FUNC_EXIT()
#endif

static void reset_controller(void)
{
#ifndef CONFIG_FPGA
	ddr_writel(0xf << 20, DDRC_CTRL);
#else
	ddr_writel(0xc0 << 16, DDRC_CTRL);
#endif
	mdelay(5);
	ddr_writel(0, DDRC_CTRL);
	mdelay(5);
}

static void remap_swap(int a, int b)
{
	uint32_t remmap[2], tmp[2];

	remmap[0] = ddr_readl(DDRC_REMAP(a / 4 + 1));
	remmap[1] = ddr_readl(DDRC_REMAP(b / 4 + 1));

#define BIT(bit) ((bit % 4) * 8)
#define MASK(bit) (0x1f << BIT(bit))
	tmp[0] = (remmap[0] & MASK(a)) >> BIT(a);
	tmp[1] = (remmap[1] & MASK(b)) >> BIT(b);

	remmap[0] &= ~MASK(a);
	remmap[1] &= ~MASK(b);

	ddr_writel(remmap[0] | (tmp[1] << BIT(a)), DDRC_REMAP(a / 4 + 1));
	ddr_writel(remmap[1] | (tmp[0] << BIT(b)), DDRC_REMAP(b / 4 + 1));
#undef BIT
#undef MASK
}

static void mem_remap(void)
{
	uint32_t start = 0, num = 0;
	int row, col, dw32, bank8, cs0, cs1;
	uint32_t size0 = 0, size1 = 0;

#ifdef CONFIG_DDR_HOST_CC
#if (CONFIG_DDR_CS0 == 1)
			row = DDR_ROW;
			col = DDR_COL;
			dw32 = CONFIG_DDR_DW32;
			bank8 = DDR_BANK8;
#endif

	size0 = (unsigned int)(DDR_CHIP_0_SIZE);
	size1 = (unsigned int)(DDR_CHIP_1_SIZE);

	/* For two different size ddr chips, just don't remmap */

#if (CONFIG_DDR_CS1 == 1)
	if (size0 != size1)
		return;
#endif

#if (CONFIG_DDR_CS0 == 1)
#if (CONFIG_DDR_CS1 == 1)
	if (size1 && size0) {
		if (size1 <= size0) {
			row = DDR_ROW1;
			col = DDR_COL1;
			dw32 = CONFIG_DDR_DW32;
			bank8 = DDR_BANK8;
		} else {
			row = DDR_ROW;
			col = DDR_COL;
			dw32 = CONFIG_DDR_DW32;
			bank8 = DDR_BANK8;
		}
	} else {
		debug("Error: The DDR size is 0\n");
		hang();
	}
#else /*CONFIG_DDR_CS1 == 1*/
	if (size0) {
		row = DDR_ROW;
		col = DDR_COL;
		dw32 = CONFIG_DDR_DW32;
		bank8 = DDR_BANK8;
	} else {
		debug("Error: The DDR size is 0\n");
		hang();
	}

#endif /* CONFIG_DDR_CS1 == 1 */
#else /* CONFIG_DDR_CS0 == 1 */
	if (size1) {
		row = DDR_ROW1;
		col = DDR_COL1;
		dw32 = CONFIG_DDR_DW32;
		bank8 = DDR_BANK8;
	} else {
		debug("Error: The DDR size is 0\n");
		hang();
	}

#endif /* CONFIG_DDR_CS0 == 1 */
	cs0 = CONFIG_DDR_CS0;
	cs1 = CONFIG_DDR_CS1;
#else /* CONFIG_DDR_HOST_CC */
	size0 = ddr_params_p->size.chip0;
	size1 = ddr_params_p->size.chip1;
	if (size0 && size1) {
		if (size1 <= size0) {
			row = ddr_params_p->row1;
			col = ddr_params_p->col1;
			dw32 = ddr_params_p->dw32;
			bank8 = ddr_params_p->bank8;
		} else {
			row = ddr_params_p->row;
			col = ddr_params_p->col;
			dw32 = ddr_params_p->dw32;
			bank8 = ddr_params_p->bank8;
		}
	} else if (size0) {
		row = ddr_params_p->row;
		col = ddr_params_p->col;
		dw32 = ddr_params_p->dw32;
		bank8 = ddr_params_p->bank8;
	} else if (size1) {
		row = ddr_params_p->row1;
		col = ddr_params_p->col1;
		dw32 = ddr_params_p->dw32;
		bank8 = ddr_params_p->bank8;
	} else {
		debug("Error: The DDR size is 0\n");
		hang();
	}

	cs0 = ddr_params_p->cs0;
	cs1 = ddr_params_p->cs1;
#endif /* CONFIG_DDR_HOST_CC */
	start += row + col + (dw32 ? 4 : 2) / 2;
	start -= 12;

	if (bank8)
		num += 3;
	else
		num += 2;

	if (cs0 && cs1)
		num++;

	for (; num > 0; num--)
		remap_swap(0 + num - 1, start + num - 1);
}

void ddr_controller_init(void)
{
	debug("DDR Controller init\n");
//dsqiu
//mdelay(1);
	ddr_writel(DDRC_CTRL_CKE | DDRC_CTRL_ALH, DDRC_CTRL);
	ddr_writel(0, DDRC_CTRL);
	/* DDRC CFG init*/
	ddr_writel(DDRC_CFG_VALUE, DDRC_CFG);
	/* DDRC timing init*/
	ddr_writel(DDRC_TIMING1_VALUE, DDRC_TIMING(1));
//	ddr_writel(0x040e0806, DDRC_TIMING(1));
	ddr_writel(DDRC_TIMING2_VALUE, DDRC_TIMING(2));
	ddr_writel(DDRC_TIMING3_VALUE, DDRC_TIMING(3));
	ddr_writel(DDRC_TIMING4_VALUE, DDRC_TIMING(4));
	ddr_writel(DDRC_TIMING5_VALUE, DDRC_TIMING(5));
//	ddr_writel(0xff070405, DDRC_TIMING(5));
	ddr_writel(DDRC_TIMING6_VALUE, DDRC_TIMING(6));

	/* DDRC memory map configure*/
	ddr_writel(DDRC_MMAP0_VALUE, DDRC_MMAP0);
	ddr_writel(DDRC_MMAP1_VALUE, DDRC_MMAP1);
	ddr_writel(DDRC_CTRL_CKE | DDRC_CTRL_ALH, DDRC_CTRL);
	ddr_writel(DDRC_REFCNT_VALUE, DDRC_REFCNT);
	ddr_writel(DDRC_CTRL_VALUE & 0xffff8fff, DDRC_CTRL);
}

void ddr_inno_phy_init()
{
	u32 reg = 0;
	debug("ddr_inno_phy_init ..!\n");
	phy_writel(0x14, INNO_PLL_FBDIV); // ADDR:9’h020 [7:0]  PLL Feedbackdivide [7:0] (fbdiv[7:0])
	phy_writel(0x1a, INNO_PLL_CTRL);  // ADDR:9’h021
	phy_writel(0x5, INNO_PLL_PDIV);   // ADDR:9’h022
	phy_writel(0x18, INNO_PLL_CTRL);  // ADDR:9’h021

	while (!(phy_readl(INNO_PLL_LOCK) & (1 << 3)))
		; // ADDR:9’h032 wait pll lock

	phy_writel(0x0, INNO_TRAINING_CTRL); // ADDR:9’h002
	phy_writel(0x03, INNO_DQ_WIDTH);     // ADDR:9’h01f
#if defined(CONFIG_DDR_TYPE_DDR3)
	phy_writel(0x30, INNO_MEM_CFG);                                                    // ADDR:9’h011 MEMSEL = DDR3, BURSEL = burst8
#if defined(CONFIG_T23)
	phy_writel((readl(PHY_BASE + INNO_A_DQS1_TXPLL) & 0xffffff8f), INNO_A_DQS1_TXPLL); // ADDR:9’h055
	phy_writel((readl(PHY_BASE + INNO_A_DQS0_TXPLL) & 0xffffff8f), INNO_A_DQS0_TXPLL); // ADDR:9’h045
#else
	phy_writel((readl(PHY_BASE + 0x154) & 0xffffff8f), 0x154); // ADDR:9’h055
	phy_writel((readl(PHY_BASE + 0x114) & 0xffffff8f), 0x114); // ADDR:9’h045
#endif
	phy_writel(0x0d, INNO_CHANNEL_EN);                                                 // ADDR:9’h000
	phy_writel(0x6, INNO_CWL);                                                         // ADDR:9’h007
	phy_writel(0x8, INNO_CL);                                                          // ADDR:9’h005
#else
	phy_writel(0x11, INNO_MEM_CFG);                           // ADDR:9’h001 MEMSEL = DDR2, BURSEL = burst8
	phy_writel(0x0d, INNO_CHANNEL_EN);                        // ADDR:9’h000
	phy_writel(((DDRP_MR0_VALUE & 0xf0) >> 4) - 1, INNO_CWL); // ADDR:9’h007
	reg = ((DDRP_MR0_VALUE & 0xf0) >> 4);
	phy_writel(reg, INNO_CL); // ADDR:9’h005
#endif
	debug("phy reg = 0x%x, CL = 0x%x\n", reg, phy_readl(INNO_CL));
	phy_writel(0x00, INNO_AL); // ADDR:9’h006

	// phy strength
#if 0
	debug("phy strength 1 0x%x\n", phy_readl(0x10*4));
	debug("phy strength 2 0x%x\n", phy_readl(0x11*4));
	debug("phy strength 3 0x%x\n", phy_readl(0x12*4));
	debug("phy strength 4 0x%x\n", phy_readl(0x13*4));
	debug("phy 0x14 0x%x\n", phy_readl(0x14*4));
#if 0
	phy_writel(0x2, 0x10*4);
	phy_writel(0x2, 0x11*4);
	phy_writel(0x2, 0x12*4);
	phy_writel(0x2, 0x13*4);
#endif
#if 0
	phy_writel(15, 0x129*4);
	phy_writel(15, 0x134*4);
#endif
	debug("phy 0x129 0x%x\n", phy_readl(0x129*4));
	debug("phy 0x134 0x%x\n", phy_readl(0x134*4));
#endif
	writel(0, DDR_APB_PHY_INIT); // start high
	while (!(readl(DDR_APB_PHY_INIT) & (1 << 2))) {
		/* do nothing, wait until resolves as false */
	} /* pll locked */
	debug("ddr_inno_phy_init ..! 11:  %X\n", readl(DDR_APB_PHY_INIT));
	writel(0, REG_DDR_CTRL);

	while (!(readl(DDR_APB_PHY_INIT) & (1 << 1))) {
		/* do nothing, wait until resolves as false */
	} /* init_complete */
	debug("ddr_inno_phy_init ..! 22:  %X\n", readl(DDR_APB_PHY_INIT));
	while (!readl(XBURST1_INIT_COMP)) {
		/* do nothing, wait until resolves as false */
	}
	debug("ddr_inno_phy_init ..! 33:  %X\n", readl(DDR_APB_PHY_INIT));
	writel(0, REG_DDR_CTRL);

	/* DDR3 init procedure:
	 *     poweron-phyinit-DFI_RST(kgdreset)_SET-delay200us-DFI_RST(kgdreset)_CLEAR-delay500us-LMR */
#ifdef CONFIG_DDR_TYPE_DDR3
	ddr_writel(DDRC_CTRL_DFI_RST, DDRC_CTRL);
	udelay(200);
	ddr_writel(0, DDRC_CTRL);
	udelay(500);
#endif
	writel(DDRC_CFG_VALUE, REG_DDR_CFG); // r=13 , c=10 , bank=4 , bitwidth=16 ,  0x0a688a40
	writel(0x0a, REG_DDR_CTRL);

#ifdef CONFIG_DDR_TYPE_DDR3
	writel((0x08 << 12) | 0x211, REG_DDR_LMR);
	debug("REG_DDR_LMR: %x\n", readl(REG_DDR_LMR));
	writel(0, REG_DDR_LMR);

	writel(0x311, REG_DDR_LMR);
	debug("REG_DDR_LMR: %x\n", readl(REG_DDR_LMR));
	writel(0, REG_DDR_LMR);

	writel((0x6 << 12) | 0x111, REG_DDR_LMR); // 0x84
	debug("REG_DDR_LMR: %x\n", readl(REG_DDR_LMR));
	writel(0, REG_DDR_LMR);

	reg = ((DDRP_MR0_VALUE) << 12) | 0x011;
	writel(reg, REG_DDR_LMR);
//	writel((0x830 << 12) | 0x011, REG_DDR_LMR);
	debug("REG_DDR_LMR, MR0: %x\n", readl(REG_DDR_LMR));
	writel(0, REG_DDR_LMR);

	writel(0x19, REG_DDR_LMR);
	debug("REG_DDR_LMR: %x\n", readl(REG_DDR_LMR));
	writel(0, REG_DDR_LMR);
#else
	writel(0x400001, REG_DDR_LMR);
	while ((0x1 & readl(REG_DDR_LMR)) == 1) {
		/* do nothing, wait until resolves as false */
	}

	writel(0x211, REG_DDR_LMR);
	debug("REG_DDR_LMR: %x\n", readl(REG_DDR_LMR));
	while ((0x1 & readl(REG_DDR_LMR)) == 1) {
		/* do nothing, wait until resolves as false */
	}

	writel(0x311, REG_DDR_LMR);
	debug("REG_DDR_LMR: %x\n", readl(REG_DDR_LMR));
	while ((0x1 & readl(REG_DDR_LMR)) == 1) {
		/* do nothing, wait until resolves as false */
	}

	writel(0x111, REG_DDR_LMR);
	debug("REG_DDR_LMR: %x\n", readl(REG_DDR_LMR));
	while ((0x1 & readl(REG_DDR_LMR)) == 1) {
		/* do nothing, wait until resolves as false */
	}

	reg = ((DDRP_MR0_VALUE) << 12) | 0x011;
	writel(reg, REG_DDR_LMR);
	debug("REG_DDR_LMR, MR0: %x\n", reg);
	while ((0x1 & readl(REG_DDR_LMR)) == 1) {
		/* do nothing, wait until resolves as false */
	}
	udelay(5 * 1000);

	writel(0x400001, REG_DDR_LMR);
	while ((0x1 & readl(REG_DDR_LMR)) == 1) {
		/* do nothing, wait until resolves as false */
	}

	writel(0x400009, REG_DDR_LMR);
	while ((0x1 & readl(REG_DDR_LMR)) == 1) {
		/* do nothing, wait until resolves as false */
	}

	writel(0x400009, REG_DDR_LMR);
	while ((0x1 & readl(REG_DDR_LMR)) == 1) {
		/* do nothing, wait until resolves as false */
	}
	udelay(5 * 1000);

#endif
#if 0
	writel(0x19, REG_DDR_LMR);
	debug("REG_DDR_LMR: %x\n", readl(REG_DDR_LMR));
	writel(0, REG_DDR_LMR);
#endif

#ifdef CONFIG_DDR_TYPE_DDR3
	// write level
	writel(0x4, 0xb301100C);
	writel(0x40, 0xb3011010);
	writel(0xa4, 0xb3011008);

	while (0x3 != readl((PHY_BASE + 0xc0))) {
		/* do nothing, wait until resolves as false */
	}
	debug("XBURST1_c0: %x\n", readl((PHY_BASE + 0xc0)));

	writel(0xa1, 0xb3011008);
#endif
	phy_calibration(0x1, 0x1, 0x1, 0x1);

#ifdef CONFIG_DDR_TYPE_DDR3
	writel(0x50, 0xb3011004);
#else
	writel(0x51, 0xb3011004);
#endif
	writel(0x24, 0xb3011028);
	debug("jz-04 :  0x%x \n", readl(0xb3011004));
	debug("jz-08 :  0x%x \n", readl(0xb3011008));
	debug("jz-28 :  0x%x \n", readl(0xb3011028));
#if 0
	int i = 0;
	debug("------------------------\n");
	for (i = 0; i < 500; i++) {
		debug("phy 0x%x, 0x%x\n", i, phy_readl(i*4));
	}
#endif
#if 1
	while (((readl(PHY_BASE + 0x190) & 0xe0) <= 0x20) && ((readl(PHY_BASE + 0x194) & 0xe0) <= 0x20)) {
		writel((readl(PHY_BASE + 0x04) | 0x40), (PHY_BASE + 0x04));
		writel((((readl(PHY_BASE + 0x28) & ~(0xe)) | 0x6)), (PHY_BASE + 0x28));
		break;
	}
#endif
	debug("DDR PHY init OK\n");
}

void phy_dqs_delay(int delay_l, int delay_h)
{
	writel(delay_l, XBURST1_DQS_DELAY_L);
	writel(delay_h, XBURST1_DQS_DELAY_H);

	debug("XBURST1_DQS_DELAY_L: %x\n", readl(XBURST1_DQS_DELAY_L));
	debug("XBURST1_DQS_DELAY_H: %x\n", readl(XBURST1_DQS_DELAY_H));
}
/*
 * Name     : phy_calibration()
 * Function : control the RX DQS window delay to the DQS
 *
 * a_low_8bit_delay	= al8_2x * clk_2x + al8_1x * clk_1x;
 * a_high_8bit_delay	= ah8_2x * clk_2x + ah8_1x * clk_1x;
 *
 * */
void phy_calibration(int al8_1x, int ah8_1x, int al8_2x, int ah8_2x)
{
#if 1
	debug("XBURST1_0x5: %x\n", readl(PHY_BASE + 0x14));
	debug("XBURST1_0x15: %x\n", readl(PHY_BASE + 0x54));
	debug("XBURST1_0x4: %x\n", readl(PHY_BASE + 0x10));
	debug("XBURST1_0x14: %x\n", readl(PHY_BASE + 0x50));

	int m = phy_readl(INNO_TRAINING_CTRL);
	debug("INNO_TRAINING_CTRL 1: %x\n", phy_readl(INNO_TRAINING_CTRL));
	m = (0xa1);
	phy_writel(m, INNO_TRAINING_CTRL);
	debug("INNO_TRAINING_CTRL 2: %x\n", phy_readl(INNO_TRAINING_CTRL));
	while (0x3 != readl((PHY_BASE + 0xcc)))
		;
	debug("XBURST1_cc: %x\n", readl((PHY_BASE + 0xcc)));
	phy_writel(0xa0, INNO_TRAINING_CTRL);
	debug("INNO_TRAINING_CTRL 3: %x\n", phy_readl(INNO_TRAINING_CTRL));
	debug("XBURST1_118: %x\n", readl((PHY_BASE + 0x118)));
	debug("XBURST1_158: %x\n", readl((PHY_BASE + 0x158)));
	debug("XBURST1_190: %x\n", readl((PHY_BASE + 0x190)));
	debug("XBURST1_194: %x\n", readl((PHY_BASE + 0x194)));
#else
	int m = phy_readl(INNO_TRAINING_CTRL);
	m = (0x1 << 1);
	phy_writel(m, INNO_TRAINING_CTRL);
	// debug("XBURST1_REG02: %x\n", phy_readl(INNO_TRAINING_CTRL));
	int x = readl(XBURST1_REG46);
	int y = readl(XBURST1_REG56);
	x = (x & (0x83) + (al8_1x << 3) + (al8_2x << 4));
	y = (y & (0x83) + (ah8_1x << 3) + (ah8_2x << 4));
	// debug for t30
	writel(30, XBURST1_REG46);
	writel(27, XBURST1_REG56);
	debug("XBURST1_REG46: %x\n", readl(XBURST1_REG46));
	debug("XBURST1_REG56: %x\n", readl(XBURST1_REG56));
#endif
}
/* DDR sdram init */
void sdram_init(void)
{
	int type = VARIABLE;
	unsigned int mode;
	unsigned int bypass = 0;
	unsigned int rate;
#ifdef CONFIG_DDR_TYPE_DDR3
	type = DDR3;
#endif
#ifdef CONFIG_DDR_TYPE_LPDDR
	type = LPDDR;
#endif
#ifdef CONFIG_DDR_TYPE_LPDDR2
	type = LPDDR2;
#endif

#ifdef CONFIG_DDR_TYPE_DDR2
	type = DDR2;
#endif

#ifndef CONFIG_DDR_HOST_CC
	struct ddrc_reg ddrc;
	struct ddrp_reg ddrp;
#ifndef CONFIG_DDR_TYPE_VARIABLE
	struct ddr_params ddr_params;
	ddr_params_p = &ddr_params;
#else
	ddr_params_p = &gd->arch.gi->ddr_params;
	ddr_params_p->freq = gd->arch.gi->cpufreq / gd->arch.gi->ddr_div;
#endif
	fill_in_params(ddr_params_p, type);
	ddr_params_creator(&ddrc, &ddrp, ddr_params_p);
	ddr_params_assign(&ddrc, &ddrp, ddr_params_p);
#endif /* CONFIG_DDR_HOST_CC */

	printf("sdram init start\n");
#ifndef CONFIG_FPGA
	clk_set_rate(DDR, gd->arch.gi->ddrfreq);
	reset_dll();
	rate = clk_get_rate(DDR);
	printf("DDR clk rate %d\n", rate);
#else
	rate = gd->arch.gi->ddrfreq;
#endif
#ifdef CONFIG_M200
	if(rate <= 150000000)
		bypass = 1;
#endif
	reset_controller();

#ifdef CONFIG_DDR_AUTO_SELF_REFRESH
	ddr_writel(0x0 ,DDRC_AUTOSR_EN);
#endif

	ddr_inno_phy_init();
	dump_inno_phy();
#if 0
	/*force CKE1 HIGH*/
	ddr_writel(DDRC_CFG_VALUE, DDRC_CFG);
	ddr_writel((1 << 1), DDRC_CTRL);
	mode = (type << 1) | (bypass & 1);
	/* DDR PHY init*/
	ddr_writel(0, DDRC_CTRL);
#endif
	/* DDR Controller init*/
	ddr_controller_init();
	/* DDRC address remap configure*/
	mem_remap();						// open remap function
	ddr_writel(DDRC_CTRL_VALUE & 0xffff07ff, DDRC_CTRL);	// must modify after opening remap function

	ddr_writel(ddr_readl(DDRC_STATUS) & ~DDRC_DSTATUS_MISS, DDRC_STATUS);
#ifdef CONFIG_DDR_AUTO_SELF_REFRESH
	if(!bypass)
		ddr_writel(0 , DDRC_DLP);
#ifdef CONFIG_FPGA
	//ddr_writel(ddr_readl(DDRP_DSGCR)&~(1<<4),DDRP_DSGCR);
#endif
	ddr_writel(0x1 ,DDRC_AUTOSR_EN);
#endif
	ddr_writel(0 , DDRC_DLP);

	dump_ddrc_register();
	debug("sdram init finished\n");
#undef DDRTYPE
}

phys_size_t initdram(int board_type)
{
#ifdef CONFIG_DDR_HOST_CC
	/* SDRAM size was calculated when compiling. */
#ifndef EMC_LOW_SDRAM_SPACE_SIZE
#define EMC_LOW_SDRAM_SPACE_SIZE 0x10000000 /* 256M */
#endif /* EMC_LOW_SDRAM_SPACE_SIZE */
	unsigned int ram_size;
	ram_size = (unsigned int)(DDR_CHIP_0_SIZE) + (unsigned int)(DDR_CHIP_1_SIZE);

	if (ram_size > EMC_LOW_SDRAM_SPACE_SIZE)
		ram_size = EMC_LOW_SDRAM_SPACE_SIZE;

	return ram_size;
#elif defined (CONFIG_BURNER)
	/* SDRAM size was defined in global info. */
	ddr_params_p = &gd->arch.gi->ddr_params;
	return ddr_params_p->size.chip0 + ddr_params_p->size.chip1;
#else
	ddr_params_p->dw32 = CONFIG_DDR_DW32;
	ddr_params_p->bank8 = DDR_BANK8;
	ddr_params_p->cs0 = CONFIG_DDR_CS0;
	ddr_params_p->cs1 = CONFIG_DDR_CS1;
	ddr_params_p->row = DDR_ROW;
	ddr_params_p->col = DDR_COL;
#ifdef DDR_ROW1
	ddr_params_p->row1 = DDR_ROW1;
#endif
#ifdef DDR_COL1
	ddr_params_p->col1 = DDR_COL1;
#endif
	return sdram_size(0, ddr_params_p) + sdram_size(1, ddr_params_p);
#endif
}
