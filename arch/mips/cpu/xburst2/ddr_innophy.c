/*
 * DDR driver for Synopsys DWC DDR PHY.
 * Used by Jz4775, JZ4780...
 *
 * Copyright (C) 2013 Ingenic Semiconductor Co.,Ltd
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
/* #define DEBUG */
/* #define DEBUG_READ_WRITE */
#include <config.h>
#include <common.h>
#include <ddr/ddr_common.h>
#include <generated/ddr_reg_values.h>
#include <asm/io.h>
#include <asm/arch/clk.h>

#include "ddr_debug.h"
#define ddr_hang() do{						\
		debug("%s %d\n",__FUNCTION__,__LINE__);	\
		hang();						\
	}while(0)

DECLARE_GLOBAL_DATA_PTR;

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
	debug("DDRC_REFCNT         0x%x\n", ddr_readl(DDRC_REFCNT));
	debug("DDRC_AUTOSR_CNT     0x%x\n", ddr_readl(DDRC_AUTOSR_CNT));
	debug("DDRC_AUTOSR_EN      0x%x\n", ddr_readl(DDRC_AUTOSR_EN));
	debug("DDRC_MMAP0          0x%x\n", ddr_readl(DDRC_MMAP0));
	debug("DDRC_MMAP1          0x%x\n", ddr_readl(DDRC_MMAP1));
	debug("DDRC_REMAP1         0x%x\n", ddr_readl(DDRC_REMAP(1)));
	debug("DDRC_REMAP2         0x%x\n", ddr_readl(DDRC_REMAP(2)));
	debug("DDRC_REMAP3         0x%x\n", ddr_readl(DDRC_REMAP(3)));
	debug("DDRC_REMAP4         0x%x\n", ddr_readl(DDRC_REMAP(4)));
	debug("DDRC_REMAP5         0x%x\n", ddr_readl(DDRC_REMAP(5)));
	debug("DDRC_DWCFG          0x%x\n", ddr_readl(DDRC_DWCFG));
	debug("DDRC_HREGPRO        0x%x\n", ddr_readl(DDRC_HREGPRO));
	debug("DDRC_PREGPRO        0x%x\n", ddr_readl(DDRC_PREGPRO));
	debug("DDRC_CGUC0          0x%x\n", ddr_readl(DDRC_CGUC0));
	debug("DDRC_CGUC1          0x%x\n", ddr_readl(DDRC_CGUC1));
#if 0
	debug("#define timing1_tWL         %d\n", timing1_tWL);
	debug("#define timing1_tWR         %d\n", timing1_tWR);
	debug("#define timing1_tWTR        %d\n", timing1_tWTR);
	debug("#define timing1_tWDLAT      %d\n", timing1_tWDLAT);

	debug("#define timing2_tRL         %d\n", timing2_tRL);
	debug("#define timing2_tRTP        %d\n", timing2_tRTP);
	debug("#define timing2_tRTW        %d\n", timing2_tRTW);
	debug("#define timing2_tRDLAT      %d\n", timing2_tRDLAT);

	debug("#define timing3_tRP         %d\n", timing3_tRP);
	debug("#define timing3_tCCD        %d\n", timing3_tCCD);
	debug("#define timing3_tRCD        %d\n", timing3_tRCD);
	debug("#define timing3_ttEXTRW     %d\n", timing3_ttEXTRW);

	debug("#define timing4_tRRD        %d\n", timing4_tRRD);
	debug("#define timing4_tRAS        %d\n", timing4_tRAS);
	debug("#define timing4_tRC         %d\n", timing4_tRC);
	debug("#define timing4_tFAW        %d\n", timing4_tFAW);

	debug("#define timing5_tCKE        %d\n", timing5_tCKE);
	debug("#define timing5_tXP         %d\n", timing5_tXP);
	debug("#define timing5_tCKSRE      %d\n", timing5_tCKSRE);
	debug("#define timing5_tCKESR      %d\n", timing5_tCKESR);
	debug("#define timing5_tXS         %d\n", timing5_tXS);
#endif
}

static void dump_ddrp_register(void)
{
	debug("DDRP_INNOPHY_INNO_PHY_RST	         0x%x\n", ddr_readl(DDRP_INNOPHY_INNO_PHY_RST	));
	debug("DDRP_INNOPHY_MEM_CFG		         0x%x\n", ddr_readl(DDRP_INNOPHY_MEM_CFG	    	));
	debug("DDRP_INNOPHY_DQ_WIDTH		         0x%x\n", ddr_readl(DDRP_INNOPHY_DQ_WIDTH    	));
	debug("DDRP_INNOPHY_CL				         0x%x\n", ddr_readl(DDRP_INNOPHY_CL				));
	debug("DDRP_INNOPHY_AL				         0x%x\n", ddr_readl(DDRP_INNOPHY_AL				));
	debug("DDRP_INNOPHY_CWL			         0x%x\n", ddr_readl(DDRP_INNOPHY_CWL			));
	debug("DDRP_INNOPHY_PLL_FBDIVL		         0x%x\n", ddr_readl(DDRP_INNOPHY_PLL_FBDIVL		));
	debug("DDRP_INNOPHY_PLL_FBDIVH		         0x%x\n", ddr_readl(DDRP_INNOPHY_PLL_FBDIVH		));
	debug("DDRP_INNOPHY_PLL_CTRL		         0x%x\n", ddr_readl(DDRP_INNOPHY_PLL_CTRL		));
	debug("DDRP_INNOPHY_PLL_PDIV		         0x%x\n", ddr_readl(DDRP_INNOPHY_PLL_PDIV		));
	debug("DDRP_INNOPHY_PLL_LOCK		         0x%x\n", ddr_readl(DDRP_INNOPHY_PLL_LOCK		));
	debug("DDRP_INNOPHY_TRAINING_CTRL	         0x%x\n", ddr_readl(DDRP_INNOPHY_TRAINING_CTRL	));
	debug("DDRP_INNOPHY_CALIB_DONE		         0x%x\n", ddr_readl(DDRP_INNOPHY_CALIB_DONE		));
	debug("DDRP_INNOPHY_CALIB_DELAY_AL_RESULT1  0x%x\n", ddr_readl(DDRP_INNOPHY_CALIB_DELAY_AL_RESULT1));
	debug("DDRP_INNOPHY_CALIB_DELAY_AL_RESULT2	 0x%x\n", ddr_readl(DDRP_INNOPHY_CALIB_DELAY_AL_RESULT2));
	debug("DDRP_INNOPHY_CALIB_DELAY_AH_RESULT1  0x%x\n", ddr_readl(DDRP_INNOPHY_CALIB_DELAY_AH_RESULT1));
	debug("DDRP_INNOPHY_CALIB_DELAY_AH_RESULT2	 0x%x\n", ddr_readl(DDRP_INNOPHY_CALIB_DELAY_AH_RESULT2));
	//debug("DDRP_INNOPHY_CALIB_BYPASS_AL	0x%x\n", ddr_readl(DDRP_INNOPHY_CALIB_BYPASS_AL	));
	//debug("DDRP_INNOPHY_CALIB_BYPASS_AH	0x%x\n", ddr_readl(DDRP_INNOPHY_CALIB_BYPASS_AH	));
	//debug("DDRP_INNOPHY_CALIB_DELAY_BL		0x%x\n", ddr_readl(DDRP_INNOPHY_CALIB_DELAY_BL	));
	//debug("DDRP_INNOPHY_CALIB_DELAY_BH		0x%x\n", ddr_readl(DDRP_INNOPHY_CALIB_DELAY_BH	));
	debug("DDRP_INNOPHY_WL_MODE1			    0x%x\n", ddr_readl(DDRP_INNOPHY_WL_MODE1		));
	debug("DDRP_INNOPHY_WL_MODE2			    0x%x\n", ddr_readl(DDRP_INNOPHY_WL_MODE2		));
	debug("DDRP_INNOPHY_WL_DONE			    0x%x\n", ddr_readl(DDRP_INNOPHY_WL_DONE	    	));
	//debug("DDRP_INNOPHY_INIT_COMP			0x%x\n", ddr_readl(DDRP_INNOPHY_INIT_COMP    	));

    debug("DDRP_INNOPHY_TRAINING_2c		    0x%x\n", ddr_readl(DDRP_INNOPHY_TRAINING_2c	));
	debug("DDRP_INNOPHY_TRAINING_3c		    0x%x\n", ddr_readl(DDRP_INNOPHY_TRAINING_3c	));
	debug("DDRP_INNOPHY_TRAINING_4c		    0x%x\n", ddr_readl(DDRP_INNOPHY_TRAINING_4c	));
	debug("DDRP_INNOPHY_TRAINING_5c		    0x%x\n", ddr_readl(DDRP_INNOPHY_TRAINING_5c	));
}

#else
#define FUNC_ENTER()
#define FUNC_EXIT()

#define dump_ddrc_register()
#define dump_ddrp_register()
#endif

static void ddrp_set_drv_odt(void)
{

	int odt_pu,odt_pd;

	int drvcmd_pd, drvcmd_pu;
	int drvclk_pd, drvclk_pu;

	int drval_pu,drval_pd;
	int drvah_pu,drvah_pd;

#if defined(CONFIG_DDR_TYPE_DDR3) /* ddr type */
#ifdef CONFIG_T41N /* T41N */ /* ddr3 soc type */
	odt_pd = 0x2;
	odt_pu = 0x2;

	drvcmd_pd = 0x5; drvcmd_pu = 0x5;
	drvclk_pd = 0x5; drvclk_pu = 0x5;

	drval_pd = 0x10; drval_pu = 0x10;
	drvah_pd = 0x10; drvah_pu = 0x10;

#elif defined(CONFIG_T41NQ) /* T41NQ */
	odt_pd = 0x0;
	odt_pu = 0x0;

	drvcmd_pd = 0xf; drvcmd_pu = 0xf;
	drvclk_pd = 0xf; drvclk_pu = 0xf;

	drval_pd = 0xf; drval_pu = 0xf;
	drvah_pd = 0xf; drvah_pu = 0xf;

#elif defined(CONFIG_T41ZX) /* T41ZX */
	odt_pd = 0x0;
	odt_pu = 0x0;

	drvcmd_pd = 0x5; drvcmd_pu = 0x5;
	drvclk_pd = 0x5; drvclk_pu = 0x5;

	drval_pd = 0xf; drval_pu = 0xf;
	drvah_pd = 0xf; drvah_pu = 0xf;

#elif defined(CONFIG_T41ZN) /* T41ZN */
	odt_pd = 0x0;
	odt_pu = 0x0;

	drvcmd_pd = 0xf; drvcmd_pu = 0xf;
	drvclk_pd = 0x2; drvclk_pu = 0x2;

	drval_pd = 0xf; drval_pu = 0xf;
	drvah_pd = 0xf; drvah_pu = 0xf;

#elif defined(CONFIG_T41XQ) /* T41XQ */
	odt_pd = 0x0;
	odt_pu = 0x0;

	drvcmd_pd = 0x5; drvcmd_pu = 0x5;
	drvclk_pd = 0x5; drvclk_pu = 0x5;

	drval_pd = 0xf; drval_pu = 0xf;
	drvah_pd = 0xf; drvah_pu = 0xf;


#elif defined(CONFIG_T41A) /* T41A */
	odt_pd = 0x3;
	odt_pu = 0x3;

	drvcmd_pd = 0x8; drvcmd_pu = 0x8;
	drvclk_pd = 0x8; drvclk_pu = 0x8;

	drval_pd = 0x10; drval_pu = 0x10;
	drvah_pd = 0x10; drvah_pu = 0x10;

#else /* old version */
	odt_pd = 0x0;
	odt_pu = 0x0;

	drvcmd_pd = 0xe; drvcmd_pu = 0xe;
	drvclk_pd = 0x1; drvclk_pu = 0x1;

	drval_pd = 0x14; drval_pu = 0x14;
	drvah_pd = 0x1f; drvah_pu = 0x04;

#endif /* ddr3 soc type */

#elif defined(CONFIG_DDR_TYPE_DDR2)
#ifdef CONFIG_T41L /* T41L */ /* ddr2 soc type */
	odt_pd = 0x0;
	odt_pu = 0x0;

	drvcmd_pd = 0x5; drvcmd_pu = 0x5;
	drvclk_pd = 0x5; drvclk_pu = 0x5;

	drval_pd = 0xf; drval_pu = 0xf;
	drvah_pd = 0xf; drvah_pu = 0xf;

#elif defined(CONFIG_T41LQ) /* T41LQ */
	odt_pd = 0x0;
	odt_pu = 0x0;

	drvcmd_pd = 0x5; drvcmd_pu = 0x5;
	drvclk_pd = 0x5; drvclk_pu = 0x5;

	drval_pd = 0xf; drval_pu = 0xf;
	drvah_pd = 0xf; drvah_pu = 0xf;

#else /* old version */
	odt_pd = 0x1;
	odt_pu = 0x0;

	drvcmd_pd = 0xe; drvcmd_pu = 0xe;
	drvclk_pd = 0xe; drvclk_pu = 0xe;

	drval_pd = 0x14; drval_pu = 0x14;
	drvah_pd = 0x14; drvah_pu = 0x14;
#endif /* ddr2 soc type */

#else/* LPDDR2 LPDDR3 */
	odt_pd = 0x1;
	odt_pu = 0x0;

	drvcmd_pd = 0xe; drvcmd_pu = 0xe;
	drvclk_pd = 0xe; drvclk_pu = 0xe;

	drval_pd = 0x14; drval_pu = 0x14;
	drvah_pd = 0x14; drvah_pu = 0x14;

#endif /* ddr type */

	//odt
	//pull down
	writel(odt_pd, 0xb3011000+4*0x140);
	writel(odt_pd, 0xb3011000+4*0x150);
	//pull up
	writel(odt_pu, 0xb3011000+4*0x141);
	writel(odt_pu, 0xb3011000+4*0x151);

	//driver
	writel(drvcmd_pd, 0xb3011000+4*0x130);
	writel(drvcmd_pu, 0xb3011000+4*0x131);
	writel(drvclk_pd, 0xb3011000+4*0x132);
	writel(drvclk_pu, 0xb3011000+4*0x133);

	writel(drval_pd, 0xb3011000+4*0x142);
	writel(drval_pu, 0xb3011000+4*0x143);
	writel(drvah_pd, 0xb3011000+4*0x152);
	writel(drvah_pu, 0xb3011000+4*0x153);
}


#ifdef CONFIG_DDR_SELF_REFRESH
void sumalte_ddr_self_refresh(void)
{
    unsigned int tmp = 0,ddrc_ctrl;
    *(volatile unsigned int*)(0xb34f0000 + 0xD8) = 0;
    ddr_writel(0, DDRC_AUTOSR_EN);
    tmp = *(volatile unsigned int *)0xa0000000;
    /* DDR self refresh */
    ddrc_ctrl = ddr_readl(DDRC_CTRL);
    ddrc_ctrl |= 1 << 5;
    ddr_writel(ddrc_ctrl, DDRC_CTRL);
    while(!(ddr_readl(DDRC_STATUS) & (1<<2)));

}

void sumalte_ddr_exit_self_refresh(void)
{
    unsigned int tmp = 0,ddrc_ctrl;
    *(volatile unsigned int*)(0xb34f0000 + 0xD8) = 0;
    ddrc_ctrl = *(volatile unsigned int *)0xb34f0010;

    /* exit ddr self refresh */
    ddrc_ctrl &= ~(1<<5);
    ddrc_ctrl |= 1 << 1;
    *(volatile unsigned int*)0xb34f0010 = ddrc_ctrl;

    while((*(volatile unsigned int *)0xb34f0000) & (1<<2));
}
#endif

static void mem_remap(void)
{
	int i;
	unsigned int remap_array[] = REMMAP_ARRAY;
	for(i = 0;i < ARRAY_SIZE(remap_array);i++)
	{
		ddr_writel(remap_array[i],DDRC_REMAP(i+1));
	}
}

static void ddrc_post_init(void)
{
	ddr_writel(DDRC_REFCNT_VALUE, DDRC_REFCNT);

#ifdef CONFIG_DDR_TYPE_DDR3
	mem_remap();
#endif

    ddr_writel(DDRC_CTRL_VALUE, DDRC_CTRL);

	ddr_writel(DDRC_CGUC0_VALUE, DDRC_CGUC0);

    ddr_writel(DDRC_CGUC1_VALUE, DDRC_CGUC1);
}
static void ddrc_prev_init(void)
{
	FUNC_ENTER();
	/* DDRC CFG init*/
	/* /\* DDRC CFG init*\/ */
	/* ddr_writel(DDRC_CFG_VALUE, DDRC_CFG); */
	/* DDRC timing init*/
	ddr_writel(DDRC_TIMING1_VALUE, DDRC_TIMING(1));
	ddr_writel(DDRC_TIMING2_VALUE, DDRC_TIMING(2));
	ddr_writel(DDRC_TIMING3_VALUE, DDRC_TIMING(3));
	ddr_writel(DDRC_TIMING4_VALUE, DDRC_TIMING(4));
	ddr_writel(DDRC_TIMING5_VALUE, DDRC_TIMING(5));

	/* DDRC memory map configure*/
	ddr_writel(DDRC_MMAP0_VALUE, DDRC_MMAP0);
	ddr_writel(DDRC_MMAP1_VALUE, DDRC_MMAP1);

	/* ddr_writel(DDRC_CTRL_CKE, DDRC_CTRL); */
	ddr_writel(DDRC_CTRL_VALUE & ~(7 << 12), DDRC_CTRL);

	FUNC_EXIT();
}

static enum ddr_type get_ddr_type(void)
{
	int type;

    type = DDR_TYPE;
    switch(type){
	case DDR2:
        debug("DDR: %s type is : DDR2\n",DDR_NAME);
		break;
	case DDR3:
        debug("DDR: %s type is : DDR3\n",DDR_NAME);
		break;
	case LPDDR2:
        debug("DDR: %s type is : LPDDR2\n",DDR_NAME);
		break;
	case LPDDR3:
        debug("DDR: %s type is : LPDDR3\n",DDR_NAME);
		break;
	default:
		type = UNKOWN;
		debug("unsupport ddr type!\n");
		ddr_hang();
	}
	return type;
}

static void ddrc_reset_phy(void)
{
	FUNC_ENTER();
	ddr_writel(0xf << 20, DDRC_CTRL);
	mdelay(1);
	ddr_writel(0x8 << 20, DDRC_CTRL);  //dfi_reset_n low for innophy
	mdelay(1);
	FUNC_EXIT();
}

union ddrp_calib {
	/** raw register data */
	uint8_t d8;
	/** register bits */
	struct {
		unsigned dllsel:3;
		unsigned ophsel:1;
		unsigned cyclesel:3;
		unsigned reserved7:1;
	} calib;					/* calib delay/bypass al/ah */
};

static void ddrp_hardware_calibration(void)
{
	unsigned int val;
	unsigned int timeout = 1000000;
	ddr_writel(0x0, DDRP_INNOPHY_TRAINING_CTRL);
    ddr_readl(DDRP_INNOPHY_TRAINING_CTRL);
	ddr_writel(0x1, DDRP_INNOPHY_TRAINING_CTRL);
	do
	{
		val = ddr_readl(DDRP_INNOPHY_CALIB_DONE);
	} while (((val & 0xf) != 0x3) && timeout--);

	if(!timeout) {
		debug("timeout:INNOPHY_CALIB_DONE %x\n", ddr_readl(DDRP_INNOPHY_CALIB_DONE));
		hang();
	}

	ddr_writel(0x0, DDRP_INNOPHY_TRAINING_CTRL);
	{
		int reg1, reg2;
		debug("ddrp rx hard calibration:\n");
		reg1 = ddr_readl(DDRP_INNOPHY_CALIB_DELAY_AL_RESULT1);
		reg2 = ddr_readl(DDRP_INNOPHY_CALIB_DELAY_AL_RESULT2);
		debug("CALIB_AL: cyc %d, oph %d, dll %d\n", reg1, reg2&0x3, reg2>>3&0x1f);
		reg1 = ddr_readl(DDRP_INNOPHY_CALIB_DELAY_AH_RESULT1);
		reg2 = ddr_readl(DDRP_INNOPHY_CALIB_DELAY_AH_RESULT2);
		debug("CALIB_AH: cyc %d, oph %d, dll %d\n", reg1, reg2&0x3, reg2>>3&0x1f);
	}
}

/*
 * Name     : ddrp_calibration_manual()
 * Function : control the RX DQS window delay to the DQS
 *
 * a_low_8bit_delay	= al8_2x * clk_2x + al8_1x * clk_1x;
 * a_high_8bit_delay	= ah8_2x * clk_2x + ah8_1x * clk_1x;
 *
 * */
static void ddrp_reg_set_range(u32 offset, u32 startbit, u32 bitscnt, u32 value)
{
	u32 reg = 0;
	u32 mask = 0;
	mask = ((0xffffffff>>startbit)<<(startbit))&((0xffffffff<<(32-startbit-bitscnt))>>(32-startbit-bitscnt));
	reg = readl(DDRC_BASE+DDR_PHY_OFFSET+(offset*4));
	reg = (reg&(~mask))|((value<<startbit)&mask);
	//debug("value = %x, reg = %x, mask = %x", value, reg, mask);
	writel(reg, DDRC_BASE+DDR_PHY_OFFSET+(offset*4));
}

#ifdef CONFIG_DDR_SOFT_TRAINING
static void ddrp_software_calibration(void)
{
	int c;
	unsigned int addr = 0xa1000000;
	unsigned int i, m = 0;
	unsigned int sel = 0;
	int ret = -1;
	int q;

	unsigned int ddbuf[8] = {0};
	unsigned short calv[0x3ff] = {0};
	ddrp_reg_set_range(0x2, 1, 1, 1);

    for(c = 0; c <=0x3ff; c++) {
		ddr_writel((c>>7)&0x7, DDRP_INNOPHY_CALIB_DELAY_AL1);
		ddr_writel((c&0x1f)<<3 | ((c>>5)&0x3), DDRP_INNOPHY_CALIB_DELAY_AL2);
		ddr_writel((c>>7)&0x7, DDRP_INNOPHY_CALIB_DELAY_AH1);
		ddr_writel((c&0x1f)<<3 | ((c>>5)&0x3), DDRP_INNOPHY_CALIB_DELAY_AH2);
		unsigned int value = 0x12345678;
		for(i = 0; i < 4 * 1024; i += 8) {
			*(volatile unsigned int *)(addr + (i + 0) * 4) = value;
			*(volatile unsigned int *)(addr + (i + 1) * 4) = value;
			*(volatile unsigned int *)(addr + (i + 2) * 4) = value;
			*(volatile unsigned int *)(addr + (i + 3) * 4) = value;
			*(volatile unsigned int *)(addr + (i + 4) * 4) = value;
			*(volatile unsigned int *)(addr + (i + 5) * 4) = value;
			*(volatile unsigned int *)(addr + (i + 6) * 4) = value;
			*(volatile unsigned int *)(addr + (i + 7) * 4) = value;
		}
		for(i = 0; i < 4 * 1024; i += 8) {
			ddbuf[0] = *(volatile unsigned int *)(addr + (i + 0) * 4);
			ddbuf[1] = *(volatile unsigned int *)(addr + (i + 1) * 4);
			ddbuf[2] = *(volatile unsigned int *)(addr + (i + 2) * 4);
			ddbuf[3] = *(volatile unsigned int *)(addr + (i + 3) * 4);
			ddbuf[4] = *(volatile unsigned int *)(addr + (i + 4) * 4);
			ddbuf[5] = *(volatile unsigned int *)(addr + (i + 5) * 4);
			ddbuf[6] = *(volatile unsigned int *)(addr + (i + 6) * 4);
			ddbuf[7] = *(volatile unsigned int *)(addr + (i + 7) * 4);
			for(q = 0; q < 8; q++) {
				if ((ddbuf[q]&0xffff0000) != (value&0xffff0000)) {
					;//debug("#####################################   high error want 0x%x get 0x%x\n", value, ddbuf[q]);
				}
				if ((ddbuf[q]&0xffff) != (value&0xffff)) {
					//debug("SET AL,AH %x q[%d] fail want 0x%x  get 0x%x \n", c, q, value, ddbuf[q]);
					ret = -1;
					break;
				} else {
					//debug("SET AL,AH %x q[%d] pass want 0x%x  get 0x%x \n", c, q, value, ddbuf[q]);
					//debug("SET %d  AL[%d] pass want 0x%x  get 0x%x \n", c, q, value, ddbuf[q]);
					ret = 0;
				}
			}
			if (ret) {
				break;
			}
		}

		if(i == 4 * 1024) {
			calv[m] = c;
			m++;
			debug("calib a once idx = %d,  value = %x\n", m, c);
		}
	}

	if(!m) {
		debug("####################### AL a calib bypass fail\n");
		ddr_writel(0x1c, DDRP_INNOPHY_CALIB_DELAY_AL1);
		ddr_writel(0x1c, DDRP_INNOPHY_CALIB_DELAY_AH1);
		//		return;
	} else {
		/* choose the middle parameter */
		sel = m * 1 / 2;
		ddr_writel((calv[sel]>>7)&0x7, DDRP_INNOPHY_CALIB_DELAY_AL1);
		ddr_writel((calv[sel]&0x1f)<<3 | ((calv[sel]>>5)&0x3), DDRP_INNOPHY_CALIB_DELAY_AL2);
		ddr_writel((calv[sel]>>7)&0x7, DDRP_INNOPHY_CALIB_DELAY_AH1);
		ddr_writel((calv[sel]&0x1f)<<3 | ((calv[sel]>>5)&0x3), DDRP_INNOPHY_CALIB_DELAY_AH2);
	}
}
#endif
static void ddr_phy_init(unsigned int rate, int pll_sel)
{
	unsigned int val;
	int i = 0;
	/***
	 * pllprediv[4:0]:0x52[4:0]() must set 1
	 * pllfbdiv[8:0]:0x51[0],0x50[7:0] must set 1
	 * pllpostdiven:0x51[7] must set 1,使能postdiv
	 * pllpostdiv:0x53[7:5] 不同频率配置不同参数，具体如下(0x53寄存器)：
	 * 推荐配置	0x20: 560MHz ~ 1066 MHz
	 *			0x40: 280MHz ~ 560MHz
	 *			0x60: 140MHz ~ 280MHz
	 *			0xc0: 70MHz  ~ 140MHz
	 *			0xe0: 35MHz  ~ 70MHz
	 ***/

retry:
	if(pll_sel > 1)
	{
		//printf("ddr_phy_init fail!\n");
		do_reset(NULL, 0, 0, NULL);
	}
	/* pllfbdiv */
	val = ddr_readl(DDRP_INNOPHY_PLL_FBDIVL);
	val &= ~(0xff);
	val |= 0x1;
	ddr_writel(val, DDRP_INNOPHY_PLL_FBDIVL);

	/* pllpostdiven; pllfbdiv */
	val = ddr_readl(DDRP_INNOPHY_PLL_FBDIVH);
	val &= ~(0xff);
	val |= 0x80;
	ddr_writel(val, DDRP_INNOPHY_PLL_FBDIVH);

	/* pllpostdiv */
	val = ddr_readl(DDRP_INNOPHY_PLL_CTRL);
	val &= ~(0xff);
	if(rate > 560000000)
	{
		if(pll_sel)
			val |= 0x48;
		else
			val |= 0x28;
	}else{
		if(pll_sel)
			val |= 0x28;
		else
			val |= 0x48;
	}
	ddr_writel(val, DDRP_INNOPHY_PLL_CTRL);
	udelay(500);
	printf("DDRP_INNOPHY_PLL_CTRL: %x\n", ddr_readl(DDRP_INNOPHY_PLL_CTRL));
	/* pllprediv */
	val = ddr_readl(DDRP_INNOPHY_PLL_PDIV);
	val &= ~(0x1f);
	val |= 0x1;
	ddr_writel(val, DDRP_INNOPHY_PLL_PDIV);

	val = ddr_readl(DDRP_INNOPHY_PLL_CTRL);
	val &= ~(0xff);
	if(rate > 560000000)
	{
		if(pll_sel)
			val = 0x40;
		else
			val = 0x20;
	}else{
		if(pll_sel)
			val = 0x20;
		else
			val = 0x40;
	}
	ddr_writel(val, DDRP_INNOPHY_PLL_CTRL);
	udelay(500);
	printf("DDRP_INNOPHY_PLL_CTRL: %x\n", ddr_readl(DDRP_INNOPHY_PLL_CTRL));
	while(!(ddr_readl(DDRP_INNOPHY_PLL_LOCK) & 1 << 2))
	{
		i++;
		if(i > 500)
		{
			pll_sel++;
			ddrc_reset_phy();
			goto retry;
		}
	}

	val = ddr_readl(DDRP_INNOPHY_MEM_CFG);
	val &= ~(0xff);
    val |= DDRP_MEMCFG_VALUE;
    ddr_writel(val, DDRP_INNOPHY_MEM_CFG);

    val = DDRP_DQ_WIDTH_DQ_H | DDRP_DQ_WIDTH_DQ_L;
    ddr_writel(val, DDRP_INNOPHY_DQ_WIDTH);/* 0x3:16bit */


	val = ddr_readl(DDRP_INNOPHY_INNO_PHY_RST);
	val &= ~(0xff);
	val |= 0x0d;
	ddr_writel(val, DDRP_INNOPHY_INNO_PHY_RST);

    /*config CWL*/
	val = ddr_readl(DDRP_INNOPHY_CWL);
	val &= ~(0xf);
	val |= DDRP_CWL_VALUE;
	/*val |= 0x6;*/
	ddr_writel(val, DDRP_INNOPHY_CWL);

    /*config CL*/
	val = ddr_readl(DDRP_INNOPHY_CL);
	val &= ~(0xf);
	val |= DDRP_CL_VALUE;
	/*val |= 0x7;*/
	ddr_writel(val, DDRP_INNOPHY_CL);

	val = 0x0;
	ddr_writel(val, DDRP_INNOPHY_AL);
}

static void ddrc_dfi_init(enum ddr_type type, int bypass)
{
	FUNC_ENTER();
	ddr_writel(DDRC_DWCFG_DFI_INIT_START, DDRC_DWCFG); // dfi_init_start high
	ddr_writel(0, DDRC_DWCFG); // set buswidth 16bit
	while(!(ddr_readl(DDRC_DWSTATUS) & DDRC_DWSTATUS_DFI_INIT_COMP)); //polling dfi_init_complete

    ddr_writel(0, DDRC_CTRL); //set dfi_reset_n high
	udelay(5);
    ddr_writel(DDRC_CFG_VALUE, DDRC_CFG);
    udelay(5);
    ddr_writel(DDRC_CTRL_CKE, DDRC_CTRL); // set CKE to high
    udelay(5);

	switch(type) {
		case DDR2:
#define DDRC_LMR_MR(n)												\
			1 << 1 | DDRC_LMR_START | DDRC_LMR_CMD_LMR |		\
			((DDR_MR##n##_VALUE & 0x1fff) << DDRC_LMR_DDR_ADDR_BIT) |	\
			(((DDR_MR##n##_VALUE >> 13) & 0x3) << DDRC_LMR_BA_BIT)
			while (ddr_readl(DDRC_LMR) & (1 << 0));
			ddr_writel(0x400003, DDRC_LMR);
			udelay(100);
			ddr_writel(DDRC_LMR_MR(2), DDRC_LMR); //MR2
			udelay(5);
			ddr_writel(DDRC_LMR_MR(3), DDRC_LMR); //MR3
			udelay(5);
			ddr_writel(DDRC_LMR_MR(1), DDRC_LMR); //MR1
			udelay(5);
			ddr_writel(DDRC_LMR_MR(0), DDRC_LMR); //MR0
			ddr_writel(0x400003, DDRC_LMR);
			udelay(1 * 10);
			ddr_writel(0x43, DDRC_LMR);
			udelay(1 * 10);
			ddr_writel(0x43, DDRC_LMR);
			udelay(1 * 10);
			ddr_writel(0xa73083, DDRC_LMR); //MR0
			ddr_writel(0x384283, DDRC_LMR);
			ddr_writel(0x4283, DDRC_LMR);
			ddr_writel(0xc3, DDRC_LMR);
			udelay(5 * 100);
#undef DDRC_LMR_MR
            break;

	case DDR3:
#define DDRC_LMR_MR(n)												\
		DDRC_DLMR_VALUE | DDRC_LMR_START | DDRC_LMR_CMD_LMR |		\
		((DDR_MR##n##_VALUE & 0xffff) << DDRC_LMR_DDR_ADDR_BIT) |	\
			(((DDR_MR##n##_VALUE >> 16) & 0x7) << DDRC_LMR_BA_BIT)

        ddr_writel(0, DDRC_LMR);udelay(5);
		ddr_writel(DDRC_LMR_MR(2), DDRC_LMR); //MR2
        udelay(5);
        ddr_writel(0, DDRC_LMR);udelay(5);
		ddr_writel(DDRC_LMR_MR(3), DDRC_LMR); //MR3
        udelay(5);
        ddr_writel(0, DDRC_LMR);udelay(5);
		ddr_writel(DDRC_LMR_MR(1), DDRC_LMR); //MR1
        udelay(5);
        ddr_writel(0, DDRC_LMR);udelay(5);
		ddr_writel(DDRC_LMR_MR(0), DDRC_LMR); //MR0
        udelay(5);
		ddr_writel(DDRC_DLMR_VALUE | DDRC_LMR_START | DDRC_LMR_CMD_ZQCL_CS0, DDRC_LMR); //ZQCL
        udelay(5);
#undef DDRC_LMR_MR
		break;

	case LPDDR2:
#define DDRC_LMR_MR(n)													\
		1 << 1| DDRC_DLMR_VALUE | DDRC_LMR_START | DDRC_LMR_CMD_LMR |	\
			((DDR_MR##n##_VALUE & 0xff) << 24) |						\
			(((DDR_MR##n##_VALUE >> 8) & 0xff) << (16))
		ddr_writel(DDRC_LMR_MR(63), DDRC_LMR); //set MRS reset
		ddr_writel(DDRC_LMR_MR(10), DDRC_LMR); //set IO calibration
		ddr_writel(DDRC_LMR_MR(1), DDRC_LMR); //set MR1
		ddr_writel(DDRC_LMR_MR(2), DDRC_LMR); //set MR2
		ddr_writel(DDRC_LMR_MR(3), DDRC_LMR); //set MR3
#undef DDRC_LMR_MR
		break;

    case LPDDR3:
#define DDRC_LMR_MR(n)                                                          \
            DDRC_DLMR_VALUE | DDRC_LMR_START | DDRC_LMR_CMD_LMR |     \
        ((DDR_MR##n##_VALUE & 0xff) << 24)  |                           \
        (((DDR_MR##n##_VALUE >> 8) & 0xff) << (16))
        ddr_writel(DDRC_LMR_MR(63), DDRC_LMR); //set MRS reset
        udelay(10);
        ddr_writel(DDRC_LMR_MR(10), DDRC_LMR); //set IO calibration
        udelay(10);
        ddr_writel(DDRC_LMR_MR(1), DDRC_LMR); //set MR1
        udelay(10);
        ddr_writel(DDRC_LMR_MR(2), DDRC_LMR); //set MR2
        udelay(10);
        ddr_writel(DDRC_LMR_MR(3), DDRC_LMR); //set MR3
        udelay(10);
        ddr_writel(DDRC_LMR_MR(11), DDRC_LMR); //set MR11
        udelay(10);
#undef DDRC_LMR_MR
        break;
	default:
		ddr_hang();
	}

	FUNC_EXIT();
}

struct ddr_calib_value {
	unsigned int rate;
	unsigned int refcnt;
	unsigned char bypass_al;
	unsigned char bypass_ah;
};

/*
	trx value
	0:nor set
	1:set rx
	2:set tx
	3:set rx&tx (default)
*/
static void ddr_set_vref_skew(unsigned int trx)
{
	int DQxOFFSET_rx[] = {0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x10, 0x17, 0x19, 0x1b, 0x1d, 0x1f, 0x21, 0x23, 0x25}; // DQxxx RX寄存器偏移
	int DQxOFFSET_tx[] = {0x3, 0x5, 0x7, 0x9, 0xb, 0xd, 0xf, 0x11, 0x18, 0x1a, 0x1c, 0x1e, 0x20, 0x22, 0x24, 0x26}; // DQxxx TX寄存器偏移
	int dqs0r = 0x7, dqs1r = 0x7;
	int dqs0t = 0x7, dqs1t = 0x7;
	int dqr = 0x7, dqt = 0x7;
	int vref_def = 0x80;
	int i, wl;

#ifdef CONFIG_T41NQ
	printf("T41NQ ddr phy skew set\n");
	dqs0r = 0x11;
	dqs1r = dqs0r;
	dqr = 0xa;

	dqs0t = 0x8;
	dqs1t = dqs0t;
	dqt = 0xa;

	vref_def = 0x8b;

#elif defined CONFIG_T41LQ
	printf("T41LQ ddr phy skew set\n");
	dqs0r = 0x15;
	dqs1r = dqs0r;
	dqr = 0xc;

	dqs0t = 0x7;
	dqs1t = dqs0t;
	dqt = 0xb;

	vref_def = 0x8b;

#elif defined CONFIG_T41L
	return ;
	printf("T41L ddr phy skew set\n");
	dqs0r = 0x1c;
	dqs1r = dqs0r;
	dqr = 0xf;

#elif defined CONFIG_T41N
	printf("T41N ddr phy skew set\n");
	dqs0r = 0x15;
	dqs1r = dqs0r;
	dqr = 0xe;

	dqs0t = 0xd;
	dqs1t = dqs0t;
	dqt = 0xe;

	vref_def = 0x8b;

#elif defined CONFIG_T41ZX
	printf("T41ZX ddr phy skew set\n");
	dqs0r = 0x12;
	dqs1r = dqs0r;
	dqr = 0xb;

	dqs0t = 0x8;
	dqs1t = dqs0t;
	dqt = 0xb;

	vref_def = 0x8b;

#elif defined CONFIG_T41ZN
	printf("T41ZN ddr phy skew set\n");
	dqs0r = 0x12;
	dqs1r = dqs0r;
	dqr = 0xb;

	dqs0t = 0x9;
	dqs1t = dqs0t;
	dqt = 0x9;

	vref_def = 0x8b;

#elif defined CONFIG_T41XQ
	printf("T41XQ ddr phy skew set\n");
	dqs0r = 0x12;
	dqs1r = dqs0r;
	dqr = 0xb;

	dqs0t = 0x8;
	dqs1t = dqs0t;
	dqt = 0xb;

	vref_def = 0x8b;

#elif defined CONFIG_T41A
	printf("T41A ddr phy skew set\n");
	// 650M配置
	dqs0r = 0x2c;
	dqs1r = dqs0r;
	dqr = 0x1d;

	dqs0t = 0x12;
	dqs1t = dqs0t;
	dqt = 0x14;

	vref_def = 0x8b;

#else
	return ;
#endif

	// VREF A_L
	ddrp_reg_set_range(0x147, 0, 8, vref_def);
	// VREF A_H
	ddrp_reg_set_range(0x157, 0, 8, vref_def);

	/* RX */
	if (trx & 0x1)
	{
		writel(dqs0r, 0xb3011000 + (0x1c0 + 0x12) * 4);
		writel(dqs0r, 0xb3011000 + (0x1c0 + 0x27) * 4);
		writel(dqs0r, 0xb3011000 + (0x1c0 + 0x2a) * 4);
		writel(dqs0r, 0xb3011000 + (0x1c0 + 0x2b) * 4);
		writel(dqs0r, 0xb3011000 + (0x1c0 + 0x00) * 4);
		writel(dqs0r, 0xb3011000 + (0x1c0 + 0x15) * 4);
		for (i = 0; i < 16; i++)
		{
			writel(dqr, 0xb3011000 + (0x1c0 + DQxOFFSET_rx[i]) * 4);
		}
	}

	/* TX */
	if (trx & 0x2)
	{
		/* TX BYPASS FUNCTION */
		wl = readl(0xb3011000 + (0x2) * 4);
		wl |= 0x8;
		writel(wl, 0xb3011000 + (0x2) * 4);

		writel(dqs0t, 0xb3011000 + (0x1c0 + 0x13) * 4);
		writel(dqs0t, 0xb3011000 + (0x1c0 + 0x14) * 4);
		writel(dqs0t, 0xb3011000 + (0x1c0 + 0x28) * 4);
		writel(dqs0t, 0xb3011000 + (0x1c0 + 0x29) * 4);
		writel(dqs0t, 0xb3011000 + (0x1c0 + 0x01) * 4);
		writel(dqs0t, 0xb3011000 + (0x1c0 + 0x16) * 4);

		// cmd
		for (i = 0; i < 0x1d; i++)
		{
			writel(dqs0t, 0xb3011000 + (0x340 + i) * 4);
		}
		writel(dqs0t, 0xb3011000 + (0x340 + 0x1e) * 4);
		writel(dqs0t, 0xb3011000 + (0x340 + 0x1f) * 4);
		writel(dqs0t, 0xb3011000 + (0x340 + 0x1e) * 4);

		for (i = 0; i < 16; i++)
		{
			writel(dqt, 0xb3011000 + (0x1c0 + DQxOFFSET_tx[i]) * 4);
		}
	}
}

#define REG32(addr) *(volatile unsigned int *)(addr)
#define CPM_DDRCDR (0xb000002c)

/* DDR sdram init */
void sdram_init(int pll_sel)
{
	enum ddr_type type;
	unsigned int rate;
	int bypass = 0;
#ifndef CONFIG_UART_LOAD
	printf("sdram init start\n");
#endif
	type = get_ddr_type();
#ifndef CONFIG_FPGA
	clk_set_rate(DDR, gd->arch.gi->ddrfreq);
	rate = clk_get_rate(DDR);
#else
	rate = gd->arch.gi->ddrfreq;
#endif
#ifndef CONFIG_UART_LOAD
	printf("DDR clk rate %d\n", rate);
#endif
	ddrc_reset_phy();

	ddr_phy_init(rate, pll_sel);

	ddrc_dfi_init(type, bypass);

	ddrp_set_drv_odt();
	ddrc_prev_init();

#ifdef CONFIG_DDR_HARDWARE_TRAINING
    ddrp_hardware_calibration();
#endif/*CONFIG_DDR_HARDWARE_TRAINING*/
#ifdef CONFIG_DDR_SOFT_TRAINING
    ddrp_software_calibration();
#endif/*CONFIG_DDR_SOFT_TRAINING*/

	dump_ddrp_register();

	ddrc_post_init();

	if(DDRC_AUTOSR_EN_VALUE) {
        ddr_writel(DDRC_AUTOSR_CNT_VALUE, DDRC_AUTOSR_CNT);
		ddr_writel(1, DDRC_AUTOSR_EN);
	} else {
        ddr_writel(DDRC_AUTOSR_CNT_VALUE, DDRC_AUTOSR_CNT);
		ddr_writel(0, DDRC_AUTOSR_EN);
	}

	ddr_writel(DDRC_HREGPRO_VALUE, DDRC_HREGPRO);
	ddr_writel(DDRC_PREGPRO_VALUE, DDRC_PREGPRO);

	dump_ddrc_register();
	debug("sdram init finished\n");
    /* Optimize DDR bandwidth */
	writel(0x0, 0xb301206c);
	writel(0xff000000, 0xb3012040);
	writel(0x2bd07460, 0xb3012048);
	writel(0x1, 0xb301206c);

	ddr_set_vref_skew(0x3);
}

phys_size_t initdram(int board_type)
{
	/* SDRAM size was calculated when compiling. */
#ifndef EMC_LOW_SDRAM_SPACE_SIZE
#define EMC_LOW_SDRAM_SPACE_SIZE 0x20000000 /* 512M */
#endif /* EMC_LOW_SDRAM_SPACE_SIZE */
#if 1
	unsigned int ram_size;
	ram_size = (unsigned int)(DDR_CHIP_0_SIZE) + (unsigned int)(DDR_CHIP_1_SIZE);

	if (ram_size > EMC_LOW_SDRAM_SPACE_SIZE)
		ram_size = EMC_LOW_SDRAM_SPACE_SIZE;
#ifndef CONFIG_UART_LOAD
	printf("DRAM size is %d MiB\n", ram_size/1024/1024);
#endif
    return ram_size;
#endif
}
