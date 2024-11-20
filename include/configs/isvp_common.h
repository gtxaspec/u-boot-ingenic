/*
 * Ingenic ISVP configuration
 *
 * Copyright (c) 2013,2018,2019 Ingenic Semiconductor Co.,Ltd
 * 		2024 thingino.com
 * Author:	Zoro <ykli@ingenic.cn>,
 *		Niky <xianghui.shen@ingenic.com>,
 *		Damon <jiansheng.zhang@ingenic.com>,
 *		gtxaspec <https://github.com/gtxaspec>,
 *		themactep <https://github.com/themactep>
 * Based on: include/configs/urboard.h
 *           Written by Paul Burton <paul.burton@imgtec.com>
 */

#ifndef __CONFIG_ISVP_COMMON_H__
#define __CONFIG_ISVP_COMMON_H__

/*
 * SoC
 */

#define CONFIG_MIPS32
#define CONFIG_CPU_XBURST
#define CONFIG_SYS_LITTLE_ENDIAN

#if defined(__CONFIG_ISVP_T10_H__)
#define CONFIG_T10
#define CONFIG_SOC "t10"
#elif defined(__CONFIG_ISVP_T15_H__)
#define CONFIG_T15
#define CONFIG_SOC "t15"
#elif defined(__CONFIG_ISVP_T20_H__)
#define CONFIG_T20
#define CONFIG_SOC "t20"
#elif defined(__CONFIG_ISVP_T21_H__)
#define CONFIG_T21
#define CONFIG_SOC "t21"
#elif defined(__CONFIG_ISVP_T23_H__)
#define CONFIG_T23
#define CONFIG_SOC "t23"
#elif defined(__CONFIG_ISVP_T30_H__)
#define CONFIG_T30
#define CONFIG_SOC "t30"
#elif defined(__CONFIG_ISVP_T31_H__)
#define CONFIG_T31
#define CONFIG_SOC "t31"
#else
#error please define ISVP config
#endif

#define T10N "T10N"
#define T10L "T10L"
#define T20N "T20N"
#define T20L "T20L"
#define T20X "T20X"
#define T21N "T21N"
#define T23N "T23N"
#define T30N "T30N"
#define T30L "T30L"
#define T30X "T30X"
#define T30A "T30A"
#define T30A1 "T30A1"
#define T31N "T31N"
#define T31L "T31L"
#define T31LC "T31LC"
#define T31X "T31X"
#define T31A "T31A"
#define T31AL "T31AL"

#if defined(CONFIG_T10) && defined(CONFIG_LITE_VERSION)
#define SOC_VAR T10L
#elif defined(CONFIG_T10)
#define SOC_VAR T10N
#elif defined(CONFIG_T20) && defined(CONFIG_DDR2_128M)
#define SOC_VAR T20X
#elif defined(CONFIG_T20) && defined(CONFIG_LITE_VERSION)
#define SOC_VAR T20L
#elif defined(CONFIG_T20)
#define SOC_VAR T20N
#elif defined(CONFIG_T21)
#define SOC_VAR T21N
#elif defined(CONFIG_T23)
#define SOC_VAR T23N
#elif defined(CONFIG_T30) && defined(CONFIG_DDR2_128M) && defined(CONFIG_T30A)
#define SOC_VAR T30A
#elif defined(CONFIG_T30) && defined(CONFIG_DDR2_128M) && defined(CONFIG_T30A1)
#define SOC_VAR T30A1
#elif defined(CONFIG_T30) && defined(CONFIG_DDR2_128M)
#define SOC_VAR T30X
#elif defined(CONFIG_T30) && defined(CONFIG_LITE_VERSION)
#define SOC_VAR T30L
#elif defined(CONFIG_T30)
#define SOC_VAR T30N
#elif defined(CONFIG_T31) && defined(CONFIG_DDR3_128M) && defined(CONFIG_T31A)
#define SOC_VAR T31A
#elif defined(CONFIG_T31) && defined(CONFIG_DDR2_128M) && defined(CONFIG_T31AL)
#define SOC_VAR T31AL
#elif defined(CONFIG_T31) && defined(CONFIG_DDR2_128M)
#define SOC_VAR T31X
#elif defined(CONFIG_T31) && defined(CONFIG_LITE_VERSION) && defined(CONFIG_T31LC)
#define SOC_VAR T31LC
#elif defined(CONFIG_T31) && defined(CONFIG_LITE_VERSION)
#define SOC_VAR T31L
#elif defined(CONFIG_T31)
#define SOC_VAR T31N
#endif

/*
 * Memory
 */

/*
#define CONFIG_DDR_TEST_CPU
#define CONFIG_DDR_TEST
#define CONFIG_DDR_AUTO_SELF_REFRESH
*/
#define CONFIG_SPL_DDR_SOFT_TRAINING

#if defined(CONFIG_T21) || defined(CONFIG_T23) || defined(CONFIG_T30) || defined(CONFIG_T31)
#define CONFIG_DDR_INNOPHY
#endif

#define CONFIG_DDR_PARAMS_CREATOR
#define CONFIG_DDR_HOST_CC
#define CONFIG_DDR_FORCE_SELECT_CS1
#define CONFIG_DDR_CS0			1	/* 1-connected, 0-disconnected */
#define CONFIG_DDR_CS1			0	/* 1-connected, 0-disconnected */
#define CONFIG_DDR_DW32			0	/* 1-32bit-width, 0-16bit-width */
#define CONFIG_DDRC_CTRL_PDT		DDRC_CTRL_PDT_128

#if defined(CONFIG_DDR3_128M)
#define CONFIG_DDR3_M15T1G1664A_2C
#define CONFIG_DDR_TYPE_DDR3
#elif defined(CONFIG_DDR3)
#define CONFIG_DDR3_TSD34096M1333C9_E
#define CONFIG_DDR_TYPE_DDR3
#elif defined(CONFIG_DDR2_128M)
#define CONFIG_DDR2_M14D1G1664A
#define CONFIG_DDR_TYPE_DDR2
#else
#define CONFIG_DDR2_M14D5121632A
#define CONFIG_DDR_TYPE_DDR2
#endif

#if defined(CONFIG_T23) || defined(CONFIG_T30) || defined(CONFIG_T31)
#define DDR2_CHIP_DRIVER_OUT_STRENGTH	0
#endif

#if defined(CONFIG_T23) || defined(CONFIG_T31)
#define DDR2_CHIP_MR0_DLL_RST
#endif

#define CONFIG_DDR_PHY_IMPEDANCE	40000
#define CONFIG_DDR_PHY_ODT_IMPEDANCE	50000 /* 75000 */
/*
#define CONFIG_DDR_PHY_IMPED_PULLUP	0xf
#define CONFIG_DDR_PHY_IMPED_PULLDOWN	0xf
#define CONFIG_DDR_DLL_OFF
#define CONFIG_DDR_CHIP_ODT
#define CONFIG_DDR_PHY_ODT
#define CONFIG_DDR_PHY_DQ_ODT
#define CONFIG_DDR_PHY_DQS_ODT
*/

#if defined(CONFIG_T10) && defined(CONFIG_LITE_VERSION)
#define APLL_712M
#define DDR_400M
#elif defined(CONFIG_T10)
#define APLL_860M
#define DDR_400M
#elif defined(CONFIG_T20) && defined(CONFIG_LITE_VERSION)
#define APLL_712M
#define DDR_500M
#elif defined(CONFIG_T20)
#define APLL_860M
#define DDR_500M
#elif defined(CONFIG_T21) && defined(CONFIG_HIGH_PERF)
#define APLL_1200M
#define DDR_500M
#elif defined(CONFIG_T21)
#define APLL_864M
#define DDR_450M
#elif defined(CONFIG_T23) && defined(CONFIG_T23N) && defined(CONFIG_HP)
#define APLL_1400M
#define DDR_600M
#elif defined(CONFIG_T23) && defined(CONFIG_T23N) && defined(CONFIG_LP)
#define APLL_936M
#define DDR_400M
#elif defined(CONFIG_T23) && defined(CONFIG_T23N)
#define APLL_1188M
#define DDR_600M
#elif defined(CONFIG_T23)
#define APLL_1000M
#define DDR_500M
#elif defined(CONFIG_T30) && defined(CONFIG_LITE_VERSION)
#define APLL_750M
#define DDR_500M
#elif defined(CONFIG_T30) && defined(CONFIG_T30A)
#define APLL_1200M
#define DDR_600M
#elif defined(CONFIG_T30) && defined(CONFIG_DDR3_128M)
#define APLL_900M
#define DDR_600M
#elif defined(CONFIG_T30)
#define APLL_900M
#define DDR_500M
#elif defined(CONFIG_T31) && defined(CONFIG_LITE_VERSION)
#define APLL_1008M
#define DDR_500M
#elif defined(CONFIG_T31) && defined(CONFIG_DDR2_128M)
#define APLL_1392M
#define DDR_600M
#elif defined(CONFIG_T31) && defined(CONFIG_T31A)
#define APLL_1392M
#define DDR_750M
#elif defined(CONFIG_T31) && defined(CONFIG_T31LC)
#define APLL_1104M
#define DDR_500M
#elif defined(CONFIG_T31)
#define APLL_1400M
#define DDR_500M
#endif

#define SEL_SCLKA			2
#define SEL_CPU				1
#define SEL_H0				2
#define SEL_H2				2

#if defined(CONFIG_T10)
#define DIV_PCLK			12
#define DIV_H2				6
#define DIV_H0				6
#elif defined(CONFIG_T20)
#define DIV_PCLK			10
#define DIV_H2				5
#define DIV_H0				5
#elif defined(DDR_400M)
#define DIV_PCLK			12
#define DIV_H2				6
#define DIV_H0				6
#elif defined(DDR_450M)
#define DIV_PCLK			8
#define DIV_H2				4
#define DIV_H0				4
#elif defined(DDR_500M)
#define DIV_PCLK			8
#define DIV_H2				4
#define DIV_H0				4
#elif defined(DDR_540M)
#define DIV_PCLK			8
#define DIV_H2				4
#define DIV_H0				4
#elif defined(DDR_550M)
#define DIV_PCLK			12
#define DIV_H2				6
#define DIV_H0				6
#elif defined(DDR_594M)
#define DIV_PCLK			12
#define DIV_H2				6
#define DIV_H0				6
#elif defined(DDR_600M)
#define DIV_PCLK			12
#define DIV_H2				6
#define DIV_H0				6
#elif defined(DDR_650M)
#define DIV_PCLK			12
#define DIV_H2				6
#define DIV_H0				6
#elif defined(DDR_700M)
#define DIV_PCLK			12
#define DIV_H2				6
#define DIV_H0				6
#elif defined(DDR_750M)
#define DIV_PCLK			12
#define DIV_H2				6
#define DIV_H0				6
#elif defined(DDR_762M)
#define DIV_PCLK			12
#define DIV_H2				6
#define DIV_H0				6
#elif defined(DDR_774M)
#define DIV_PCLK			12
#define DIV_H2				6
#define DIV_H0				6
#elif defined(DDR_786M)
#define DIV_PCLK			12
#define DIV_H2				6
#define DIV_H0				6
#elif defined(DDR_798M)
#define DIV_PCLK			12
#define DIV_H2				6
#define DIV_H0				6
#elif defined(DDR_800M)
#define DIV_PCLK			12
#define DIV_H2				6
#define DIV_H0				6
#elif defined(DDR_810M)
#define DIV_PCLK			12
#define DIV_H2				6
#define DIV_H0				6
#elif defined(DDR_816M)
#define DIV_PCLK			12
#define DIV_H2				6
#define DIV_H0				6
#elif defined(DDR_900M)
#define DIV_PCLK			12
#define DIV_H2				6
#define DIV_H0				6
#else
#error please define DDR_FREQ
#endif

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

#if defined(APLL_712M) && (defined(CONFIG_T10) || defined(CONFIG_T20))
#define CONFIG_SYS_APLL_FREQ		712704000	/*If APLL is not used, it must be set to 0*/
#define CONFIG_SYS_APLL_MNOD		((59 << 20) | (2 << 14) | (1 << 11) | (1 << 8))
#define CONFIG_SYS_APLL_FRAC		0x645a1c /* 6576668 */
#elif defined(APLL_750M)
#define CONFIG_SYS_APLL_FREQ		750000000	/*If APLL is not used, it must be set to 0*/
#define CONFIG_SYS_APLL_MNOD		((124 << 20) | (1 << 14) | (2 << 11) | (2 << 5))
#elif defined(APLL_804M)
#define CONFIG_SYS_APLL_FREQ		804000000	/*If APLL is not used, it must be set to 0*/
#define CONFIG_SYS_APLL_MNOD		((67 << 20) | (1 << 14) | (2 << 11) | (1 << 8))
#elif defined(APLL_860M) && (defined(CONFIG_T10) || defined(CONFIG_T20))
#define CONFIG_SYS_APLL_FREQ		860160000	/*If APLL is not used, it must be set to 0*/
#define CONFIG_SYS_APLL_MNOD		((71 << 20) | (2 << 14) | (1 << 11) | (1 << 8))
#define CONFIG_SYS_APLL_FRAC		0xae147a /* 11408506 */
#elif defined(APLL_864M) && defined(CONFIG_T21)
#define CONFIG_SYS_APLL_FREQ		864000000	/*If APLL is not used, it must be set to 0*/
#define CONFIG_SYS_APLL_MNOD		((71 << 20) | (1 << 14) | (1 << 11) | (2 << 5))
#elif defined(APLL_864M)
#define CONFIG_SYS_APLL_FREQ		864000000	/*If APLL is not used, it must be set to 0*/
#define CONFIG_SYS_APLL_MNOD		((72 << 20) | (1 << 14) | (2 << 11) | (1 << 8))
#elif defined(APLL_891M)
#define CONFIG_SYS_APLL_FREQ		891000000	/*If APLL is not used, it must be set to 0*/
#define CONFIG_SYS_APLL_MNOD		((297 << 20) | (4 << 14) | (2 << 11) | (1 << 8))
#elif defined(APLL_900M) && defined(CONFIG_T30)
#define CONFIG_SYS_APLL_FREQ		900000000	/*If APLL is not used, it must be set to 0*/
#define CONFIG_SYS_APLL_MNOD		((74 << 20) | (1 << 14) | (1 << 11) | (2 << 5))
#elif defined(APLL_900M)
#define CONFIG_SYS_APLL_FREQ		900000000	/*If APLL is not used, it must be set to 0*/
#define CONFIG_SYS_APLL_MNOD		((75 << 20) | (1 << 14) | (2 << 11) | (1 << 8))
#elif defined(APLL_936M)
#define CONFIG_SYS_APLL_FREQ		936000000	/*If APLL is not used, it must be set to 0*/
#define CONFIG_SYS_APLL_MNOD		((234 << 20) | (3 << 14) | (2 << 11) | (1 << 8))
#elif defined(APLL_1000M)
#define CONFIG_SYS_APLL_FREQ		1000000000	/*If APLL is not used, it must be set to 0*/
#define CONFIG_SYS_APLL_MNOD		((125 << 20) | (1 << 14) | (3 << 11) | (1 << 8))
#elif defined(APLL_1008M)
#define CONFIG_SYS_APLL_FREQ		1008000000	/*If APLL is not used, it must be set to 0*/
#define CONFIG_SYS_APLL_MNOD		((84 << 20) | (1 << 14) | (2 << 11) | (1 << 8))
#elif defined(APLL_1100M)
#define CONFIG_SYS_APLL_FREQ		1100000000	/*If APLL is not used, it must be set to 0*/
#define CONFIG_SYS_APLL_MNOD		((275 << 20) | (3 << 14) | (2 << 11) | (1 << 8))
#elif defined(APLL_1104M)
#define CONFIG_SYS_APLL_FREQ		1104000000	/*If APLL is not used, it must be set to 0*/
#define CONFIG_SYS_APLL_MNOD		((92 << 20) | (1 << 14) | (2 << 11) | (1 << 8))
#elif defined(APLL_1188M)
#define CONFIG_SYS_APLL_FREQ		1188000000	/*If APLL is not used, it must be set to 0*/
#define CONFIG_SYS_APLL_MNOD		((297 << 20) | (3 << 14) | (2 << 11) | (1 << 8))
#elif defined(APLL_1200M) && defined(CONFIG_T21)
#define CONFIG_SYS_APLL_FREQ		1200000000	/*If MPLL is not used, it must be set to 0*/
#define CONFIG_SYS_APLL_MNOD		((49 << 20) | (0 << 14) | (1 << 11) | (3 << 5))
#elif defined(APLL_1200M) && defined(CONFIG_T30)
#define CONFIG_SYS_APLL_FREQ		1200000000	/*If APLL is not used, it must be set to 0*/
#define CONFIG_SYS_APLL_MNOD		((149 << 20) | (2 << 14) | (1 << 11) | (1 << 5))
#elif defined(APLL_1200M)
#define CONFIG_SYS_APLL_FREQ		1200000000	/*If APLL is not used, it must be set to 0*/
#define CONFIG_SYS_APLL_MNOD		((100 << 20) | (1 << 14) | (2 << 11) | (1 << 8))
#elif defined(APLL_1300M)
#define CONFIG_SYS_APLL_FREQ		1300000000	/*If APLL is not used, it must be set to 0*/
#define CONFIG_SYS_APLL_MNOD		((325 << 20) | (3 << 14) | (2 << 11) | (1 << 8))
#elif defined(APLL_1392M)
#define CONFIG_SYS_APLL_FREQ		1392000000	/*If APLL is not used, it must be set to 0*/
#define CONFIG_SYS_APLL_MNOD		((116 << 20) | (1 << 14) | (2 << 11) | (1 << 8))
#elif defined(APLL_1400M) && defined(CONFIG_T23)
#define CONFIG_SYS_APLL_FREQ		1400000000	/*If APLL is not used, it must be set to 0*/
#define CONFIG_SYS_APLL_MNOD		((350 << 20) | (3 << 14) | (2 << 11) | (1 << 8))
#elif defined(APLL_1400M)
#define CONFIG_SYS_APLL_FREQ		1404000000	/*If APLL is not used, it must be set to 0*/
#define CONFIG_SYS_APLL_MNOD		((117 << 20) | (1 << 14) | (2 << 11) | (1 << 8))
#elif defined(APLL_1485M)
#define CONFIG_SYS_APLL_FREQ		1485000000	/*If APLL is not used, it must be set to 0*/
#define CONFIG_SYS_APLL_MNOD		((495 << 20) | (4 << 14) | (2 << 11) | (1 << 8))
#elif defined(APLL_1500M)
#define CONFIG_SYS_APLL_FREQ		1500000000	/*If APLL is not used, it must be set to 0*/
#define CONFIG_SYS_APLL_MNOD		((125 << 20) | (1 << 14) | (2 << 11) | (1 << 8))
#elif defined(APLL_1800M)
#define CONFIG_SYS_APLL_FREQ		1800000000	/*If APLL is not used, it must be set to 0*/
#define CONFIG_SYS_APLL_MNOD		((150 << 20) | (1 << 14) | (2 << 11) | (1 << 8))
#else
#error please define APLL_FREQ
#endif

#if defined(DDR_400M) && (defined(CONFIG_T21) || defined(CONFIG_T30))
#define CONFIG_SYS_MPLL_FREQ		1200000000	/*If MPLL is not used, it must be set to 0*/
#define CONFIG_SYS_MPLL_MNOD		((149 << 20) | (2 << 14) | (1 << 11) | (1 << 5))
#elif defined(DDR_400M)
#define CONFIG_SYS_MPLL_FREQ		1200000000	/*If MPLL is not used, it must be set to 0*/
#define CONFIG_SYS_MPLL_MNOD		((100 << 20) | (1 << 14) | (2 << 11) | (1 << 8))
#elif defined(DDR_450M) && defined(CONFIG_T21)
#define CONFIG_SYS_MPLL_FREQ		900000000	/*If MPLL is not used, it must be set to 0*/
#define CONFIG_SYS_MPLL_MNOD		((74 << 20) | (1 << 14) | (1 << 11) | (2 << 5))
#elif defined(DDR_450M)
#define CONFIG_SYS_MPLL_FREQ		900000000	/*If MPLL is not used, it must be set to 0*/
#define CONFIG_SYS_MPLL_MNOD		((75 << 20) | (1 << 14) | (2 << 11) | (1 << 8))
#elif defined(DDR_500M) && defined(CONFIG_T20)
#define CONFIG_SYS_MPLL_FREQ		1000000000	/*If MPLL is not used, it must be set to 0*/
#define CONFIG_SYS_MPLL_MNOD		((125 << 20) | (3 << 14) | (1 << 11) | (1 << 8))
#define CONFIG_SYS_VPLL_FREQ		1200000000	/*If VPLL is not used, it must be set to 0*/
#elif defined(DDR_500M) && (defined(CONFIG_T21) || defined(CONFIG_T30))
#define CONFIG_SYS_MPLL_FREQ		1000000000	/*If MPLL is not used, it must be set to 0*/
#define CONFIG_SYS_MPLL_MNOD		((124 << 20) | (2 << 14) | (1 << 11) | (1 << 5))
#if 0
#define CONFIG_SYS_MPLL_FREQ		1500000000	/*If MPLL is not used, it must be set to 0*/
#define CONFIG_SYS_MPLL_MNOD		((249 << 20) | (3 << 14) | (1 << 11) | (1 << 5))
#endif
#elif defined(DDR_500M)
#define CONFIG_SYS_MPLL_FREQ		1000000000	/*If MPLL is not used, it must be set to 0*/
#define CONFIG_SYS_MPLL_MNOD		((125 << 20) | (1 << 14) | (3 << 11) | (1 << 8))
#elif defined(DDR_540M)
#define CONFIG_SYS_MPLL_FREQ		1080000000	/*If MPLL is not used, it must be set to 0*/
#define CONFIG_SYS_MPLL_MNOD		((90 << 20) | (1 << 14) | (2 << 11) | (1 << 8))
#elif defined(DDR_550M)
#define CONFIG_SYS_MPLL_FREQ		1100000000	/*If MPLL is not used, it must be set to 0*/
#define CONFIG_SYS_MPLL_MNOD		((275 << 20) | (3 << 14) | (2 << 11) | (1 << 8))
#elif defined(DDR_594M)
#define CONFIG_SYS_MPLL_FREQ		1188000000	/*If MPLL is not used, it must be set to 0*/
#define CONFIG_SYS_MPLL_MNOD		((297 << 20) | (3 << 14) | (2 << 11) | (1 << 8))
#elif defined(DDR_600M) && (defined(CONFIG_T21) || defined(CONFIG_T30))
#define CONFIG_SYS_MPLL_FREQ		1200000000	/*If MPLL is not used, it must be set to 0*/
#define CONFIG_SYS_MPLL_MNOD		((149 << 20) | (2 << 14) | (1 << 11) | (1 << 5))
#elif defined(DDR_600M)
#define CONFIG_SYS_MPLL_FREQ		1200000000	/*If MPLL is not used, it must be set to 0*/
#define CONFIG_SYS_MPLL_MNOD		((100 << 20) | (1 << 14) | (2 << 11) | (1 << 8))
#elif defined(DDR_650M) && defined(CONFIG_T23)
#define CONFIG_SYS_MPLL_FREQ		1300000000	/*If MPLL is not used, it must be set to 0*/
#define CONFIG_SYS_MPLL_MNOD		((325 << 20) | (3 << 14) | (2 << 11) | (1 << 8))
#elif defined(DDR_650M)
#define CONFIG_SYS_MPLL_FREQ		1308000000	/*If MPLL is not used, it must be set to 0*/
#define CONFIG_SYS_MPLL_MNOD		((109 << 20) | (1 << 14) | (2 << 11) | (1 << 8))
#elif defined(DDR_700M)
#define CONFIG_SYS_MPLL_FREQ		1404000000	/*If MPLL is not used, it must be set to 0*/
#define CONFIG_SYS_MPLL_MNOD		((117 << 20) | (1 << 14) | (2 << 11) | (1 << 8))
#elif defined(DDR_750M)
#define CONFIG_SYS_MPLL_FREQ		1500000000	/*If MPLL is not used, it must be set to 0*/
#define CONFIG_SYS_MPLL_MNOD		((125 << 20) | (1 << 14) | (2 << 11) | (1 << 8))
#elif defined(DDR_762M)
#define CONFIG_SYS_MPLL_FREQ		1524000000	/*If MPLL is not used, it must be set to 0*/
#define CONFIG_SYS_MPLL_MNOD		((127 << 20) | (1 << 14) | (2 << 11) | (1 << 8))
#elif defined(DDR_774M)
#define CONFIG_SYS_MPLL_FREQ		1548000000	/*If MPLL is not used, it must be set to 0*/
#define CONFIG_SYS_MPLL_MNOD		((129 << 20) | (1 << 14) | (2 << 11) | (1 << 8))
#elif defined(DDR_786M)
#define CONFIG_SYS_MPLL_FREQ		1572000000	/*If MPLL is not used, it must be set to 0*/
#define CONFIG_SYS_MPLL_MNOD		((131 << 20) | (1 << 14) | (2 << 11) | (1 << 8))
#elif defined(DDR_798M)
#define CONFIG_SYS_MPLL_FREQ		1596000000	/*If MPLL is not used, it must be set to 0*/
#define CONFIG_SYS_MPLL_MNOD		((133 << 20) | (1 << 14) | (2 << 11) | (1 << 8))
#elif defined(DDR_800M)
#define CONFIG_SYS_MPLL_FREQ		1608000000	/*If MPLL is not used, it must be set to 0*/
#define CONFIG_SYS_MPLL_MNOD		((134 << 20) | (1 << 14) | (2 << 11) | (1 << 8))
#elif defined(DDR_810M)
#define CONFIG_SYS_MPLL_FREQ		1620000000	/*If MPLL is not used, it must be set to 0*/
#define CONFIG_SYS_MPLL_MNOD		((135 << 20) | (1 << 14) | (2 << 11) | (1 << 8))
#elif defined(DDR_816M)
#define CONFIG_SYS_MPLL_FREQ		1632000000	/*If MPLL is not used, it must be set to 0*/
#define CONFIG_SYS_MPLL_MNOD		((136 << 20) | (1 << 14) | (2 << 11) | (1 << 8))
#elif defined(DDR_900M)
#define CONFIG_SYS_MPLL_FREQ		1800000000	/*If MPLL is not used, it must be set to 0*/
#define CONFIG_SYS_MPLL_MNOD		((150 << 20) | (1 << 14) | (2 << 11) | (1 << 8))
#else
#error please define DDR_FREQ
#endif

#if defined(CONFIG_T21) || defined(CONFIG_T30) || defined(CONFIG_T31)
#define CONFIG_SYS_VPLL_FREQ		1200000000	/*If VPLL is not used, it must be set to 0*/
#endif

#if defined(CONFIG_T30)
#define CONFIG_SYS_EPLL_FREQ		1188000000
#define CONFIG_SYS_EPLL_MNOD		((197 << 20) | (3 << 14) | (1 << 11) | (1 << 5))
#endif

#define CONFIG_CPU_SEL_PLL		APLL
#define CONFIG_DDR_SEL_PLL		MPLL
#define CONFIG_SYS_CPU_FREQ		CONFIG_SYS_APLL_FREQ

#define CONFIG_SYS_EXTAL		24000000	/* EXTAL freq: 24 MHz */
#define CONFIG_SYS_HZ			1000		/* incrementer freq */

#if defined(CONFIG_T21)
#define CONFIG_SYS_DCACHE_SIZE		16384
#define CONFIG_SYS_ICACHE_SIZE		16384
#else
#define CONFIG_SYS_DCACHE_SIZE		32768
#define CONFIG_SYS_ICACHE_SIZE		32768
#endif
#define CONFIG_SYS_CACHELINE_SIZE	32

#if defined(CONFIG_T10)
#define CONFIG_SYS_MEM_FREQ		(CONFIG_SYS_MPLL_FREQ / 3)
#elif defined(CONFIG_T20)
#define CONFIG_SYS_MEM_FREQ		(CONFIG_SYS_MPLL_FREQ / 2)
#elif defined(DDR_400M)
#define CONFIG_SYS_MEM_FREQ		(CONFIG_SYS_MPLL_FREQ / 3)
#elif defined(DDR_450M)
#define CONFIG_SYS_MEM_FREQ		(CONFIG_SYS_MPLL_FREQ / 2)
#elif defined(DDR_500M)
#define CONFIG_SYS_MEM_FREQ		(CONFIG_SYS_MPLL_FREQ / 2)
#elif defined(DDR_540M)
#define CONFIG_SYS_MEM_FREQ		(CONFIG_SYS_MPLL_FREQ / 2)
#elif defined(DDR_550M)
#define CONFIG_SYS_MEM_FREQ		(CONFIG_SYS_MPLL_FREQ / 2)
#elif defined(DDR_594M)
#define CONFIG_SYS_MEM_FREQ		(CONFIG_SYS_MPLL_FREQ / 2)
#elif defined(DDR_600M)
#define CONFIG_SYS_MEM_FREQ		(CONFIG_SYS_MPLL_FREQ / 2)
#elif defined(DDR_650M)
#define CONFIG_SYS_MEM_FREQ		(CONFIG_SYS_MPLL_FREQ / 2)
#elif defined(DDR_700M)
#define CONFIG_SYS_MEM_FREQ		(CONFIG_SYS_MPLL_FREQ / 2)
#elif defined(DDR_750M)
#define CONFIG_SYS_MEM_FREQ		(CONFIG_SYS_MPLL_FREQ / 2)
#elif defined(DDR_762M)
#define CONFIG_SYS_MEM_FREQ		(CONFIG_SYS_MPLL_FREQ / 2)
#elif defined(DDR_774M)
#define CONFIG_SYS_MEM_FREQ		(CONFIG_SYS_MPLL_FREQ / 2)
#elif defined(DDR_786M)
#define CONFIG_SYS_MEM_FREQ		(CONFIG_SYS_MPLL_FREQ / 2)
#elif defined(DDR_798M)
#define CONFIG_SYS_MEM_FREQ		(CONFIG_SYS_MPLL_FREQ / 2)
#elif defined(DDR_800M)
#define CONFIG_SYS_MEM_FREQ		(CONFIG_SYS_MPLL_FREQ / 2)
#elif defined(DDR_810M)
#define CONFIG_SYS_MEM_FREQ		(CONFIG_SYS_MPLL_FREQ / 2)
#elif defined(DDR_816M)
#define CONFIG_SYS_MEM_FREQ		(CONFIG_SYS_MPLL_FREQ / 2)
#elif defined(DDR_900M)
#define CONFIG_SYS_MEM_FREQ		(CONFIG_SYS_MPLL_FREQ / 2)
#else
#error please define DDR_FREQ
#endif

/* GMAC */
#define GMAC_PHY_MII			1
#define GMAC_PHY_RMII			2
#define GMAC_PHY_GMII			3
#define GMAC_PHY_RGMII			4

/* PHY */
#define PHY_TYPE_DM9161			1
#define PHY_TYPE_88E1111		2
#define PHY_TYPE_8710A			3
#define PHY_TYPE_IP101G			4
#define PHY_TYPE_OMNI			5

#if defined(CONFIG_T21)
#define GMAC_PHY_INNER
#endif

#if defined(CONFIG_T21) && defined(GMAC_PHY_INNER)
#define CONFIG_NET_GMAC_PHY_MODE	GMAC_PHY_MII
#define CONFIG_NET_PHY_TYPE		PHY_TYPE_OMNI
#else
#define CONFIG_NET_GMAC_PHY_MODE	GMAC_PHY_RMII
#define CONFIG_NET_PHY_TYPE		PHY_TYPE_IP101G
#endif

#if defined(CONFIG_T23)
#define CONFIG_GMAC_PHY_RATE     25000000
#else
#define CONFIG_GMAC_PHY_RATE     50000000
#endif

#define CONFIG_NET_GMAC

/**
 * Environment
 */
#if defined(CONFIG_ENV_IS_IN_MMC)
#define CONFIG_SYS_MMC_ENV_DEV		0
#define CONFIG_ENV_SIZE			(32 << 10)
#define CONFIG_ENV_OFFSET		(CONFIG_SYS_MONITOR_LEN + CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR * 512)
#elif defined(CONFIG_ENV_IS_IN_SPI_FLASH)
#define CONFIG_ENV_OFFSET		0x40000
#define CONFIG_ENV_SIZE			0x10000
#define CONFIG_ENV_SECT_SIZE		0x10000
#else
#define CONFIG_ENV_IS_NOWHERE
#define CONFIG_ENV_SIZE			(32 << 10)
#define CONFIG_ENV_OFFSET		(CONFIG_SYS_NAND_BLOCK_SIZE * 5)
#endif

#define CONFIG_U_BOOT_BUILD_NAME	"thingino_" SOC_VAR
#define CONFIG_SFC_MIN_ALIGN		0x8000 /*0x1000->4K Erase, 0x8000->32K, 0x10000->64K*/

#define CONFIG_AUTOBOOT_KEYED
#define CONFIG_AUTOBOOT_PROMPT		"KEY:   ###### Press Ctrl-C now to interrupt boot... loading in %ds ######\n"
#define CONFIG_AUTOBOOT_STOP_STR	"\x3"

#define CONFIG_SYS_LONGHELP
#define CONFIG_SYS_MAXARGS		64
#define CONFIG_SYS_NO_FLASH

#define CONFIG_SYS_CBSIZE		2048
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_MALLOC_LEN		(32 * 1024 * 1024)
#define CONFIG_SYS_BOOTPARAMS_LEN	(256 * 1024)
#define CONFIG_SYS_MONITOR_LEN		(230 * 1024)

#define CONFIG_SKIP_LOWLEVEL_INIT

#define CONFIG_SYS_UART_INDEX		1
#define CONFIG_BAUDRATE			115200
#if defined(CONFIG_T21) || defined(CONFIG_T23) || defined(CONFIG_T30) || defined(CONFIG_T31)
#define CONFIG_SYS_UART_CONTROLLER_STEP	0x1000
#endif

#define CONFIG_BOOTDELAY		3
#define CONFIG_FAT_WRITE

#if !defined(CONFIG_T31LC) && !defined(CONFIG_XIAOMI_SPL)
#define CONFIG_SYS_HUSH_PARSER
#endif

#define CONFIG_NETMASK			255.255.255.0
#define CONFIG_GATEWAYIP		192.168.1.1
#define CONFIG_SERVERIP			192.168.1.254
#define CONFIG_IPADDR			192.168.1.10
#define CONFIG_RANDOM_MACADDR

#define CONFIG_AUTO_UPDATE

/**
 * Command configuration.
 */

#define CONFIG_AUTO_COMPLETE
#define CONFIG_CMDLINE_EDITING
#define CONFIG_CMDLINE_TAG

#define CONFIG_CMD_BOOTD
#define CONFIG_CMD_CONSOLE
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_ECHO
#if !defined(CONFIG_T31LC) && !defined(CONFIG_XIAOMI_SPL)
#define CONFIG_CMD_EXT2
#define CONFIG_CMD_EXT4
#endif
#define CONFIG_CMD_FAT
#define CONFIG_CMD_FS_GENERIC
#define CONFIG_CMD_GETTIME
#define CONFIG_CMD_GPIO
#if !defined(CONFIG_T31LC) && !defined(CONFIG_XIAOMI_SPL)
#define CONFIG_CMD_JFFS2
#endif
#define CONFIG_CMD_LOADB
#define CONFIG_CMD_MEMORY
#define CONFIG_CMD_MISC
#define CONFIG_CMD_MMC
#define CONFIG_CMD_NET
#if !defined(CONFIG_T31LC) && !defined(CONFIG_XIAOMI_SPL)
#define CONFIG_CMD_NFS
#endif
#define CONFIG_CMD_PING
#define CONFIG_CMD_RUN
#define CONFIG_CMD_SAVEENV
#define CONFIG_CMD_SDSTART
#define CONFIG_CMD_SDUPDATE
#define CONFIG_CMD_SOC_INFO
#define CONFIG_CMD_SOURCE
#define CONFIG_CMD_SQUASH_PROBE
#if !defined(CONFIG_T31LC) && !defined(CONFIG_XIAOMI_SPL)
#define CONFIG_CMD_TFTPDOWNLOAD
#define CONFIG_CMD_TFTPPUT
#define CONFIG_CMD_TFTPSRV
#endif
#define CONFIG_CMD_WATCHDOG

/* JFFS2 configuration */
#if defined(CONFIG_CMD_JFFS2)
#define CONFIG_CMD_FLASH
#define CONFIG_SYS_MAX_FLASH_BANKS	1
#define CONFIG_SYS_MAX_FLASH_SECT	256
#undef CONFIG_CMD_MTDPARTS
#undef CONFIG_JFFS2_CMDLINE
#define CONFIG_JFFS2_DEV			"nor0"
#define CONFIG_JFFS2_PART_OFFSET	0x4C0000
#define CONFIG_JFFS2_PART_SIZE		0xB40000
#define CONFIG_START_VIRTUAL_ADDRESS	0x80600000
#else
#define CONFIG_SYS_MAX_FLASH_SECT	0
#endif

/* GPIO */
#define CONFIG_JZ_GPIO

/************************ USB CONFIG ***************************/
#if !defined(CONFIG_T31LC) && !defined(CONFIG_XIAOMI_SPL)
#define CONFIG_CMD_USB
#if defined(CONFIG_CMD_USB)
#define CONFIG_USB_DWC2
#define CONFIG_USB_DWC2_REG_ADDR	0x13500000
#define CONFIG_USB_HOST_ETHER
#define CONFIG_USB_ETHER_ASIX
#define CONFIG_USB_STORAGE
#endif
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
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)

#define CONFIG_SYS_SDRAM_BASE		0x80000000 /* cached (KSEG0) address */
#define CONFIG_SYS_SDRAM_MAX_TOP	0x84000000 /* don't run into IO space */

#define CONFIG_SYS_MEMTEST_START	0x80000000
#define CONFIG_SYS_MEMTEST_END		0x84000000

#define CONFIG_SYS_LOAD_ADDR		0x82000000
#define CONFIG_SYS_INIT_SP_OFFSET	0x400000

#define CONFIG_SYS_TEXT_BASE		0x80100000
#define CONFIG_SYS_MONITOR_BASE		CONFIG_SYS_TEXT_BASE

/**
 * SPL configuration
 */
#define CONFIG_SPL
#define CONFIG_SPL_FRAMEWORK

#if !defined(CONFIG_T31LC) && !defined(CONFIG_XIAOMI_SPL)
#define CONFIG_SPL_LZOP
#endif

#if defined(CONFIG_SPL_LZOP)
#define CONFIG_DECMP_BUFFER_ADRS	0x80200000
#endif

#define CONFIG_SPL_PAD_TO		26624
#define CONFIG_SPL_MAX_SIZE		(26 * 1024)

#define CONFIG_SPL_NO_CPU_SUPPORT_CODE
#define CONFIG_SPL_START_S_PATH		"$(CPUDIR)/$(SOC)"

#if defined(CONFIG_SPL_NOR_SUPPORT)
#define CONFIG_SPL_LDSCRIPT		"$(CONFIG_SPL_START_S_PATH)/u-boot-nor-spl.lds"
#define CONFIG_SPL_TEXT_BASE		0xba000000
#else
#define CONFIG_SPL_LDSCRIPT		"$(CONFIG_SPL_START_S_PATH)/u-boot-spl.lds"
#define CONFIG_SPL_TEXT_BASE		0x80001000
#endif

#define CONFIG_UBOOT_OFFSET				CONFIG_SPL_MAX_SIZE
#define CONFIG_MMC_RAW_UBOOT_OFFSET			(CONFIG_UBOOT_OFFSET / 1024 + 17)
#define CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR		(CONFIG_MMC_RAW_UBOOT_OFFSET * 2)
#define CONFIG_SYS_U_BOOT_MAX_SIZE_SECTORS		0x400 /* 512 KB */

#define CONFIG_SPL_BOARD_INIT
#define CONFIG_SPL_LIBGENERIC_SUPPORT
#define CONFIG_SPL_GPIO_SUPPORT

#if defined(CONFIG_SPL_MMC_SUPPORT)
#define CONFIG_SPL_SERIAL_SUPPORT
#endif

#if defined(CONFIG_SPL_SPI_SUPPORT)
#define CONFIG_SPL_SERIAL_SUPPORT
#define CONFIG_SPI_SPL_CHECK
#define CONFIG_SYS_SPI_BOOT_FREQ	1000000
#endif

#if defined(CONFIG_SPL_NOR_SUPPORT)
#define CONFIG_SPL_SERIAL_SUPPORT
#define CONFIG_SYS_UBOOT_BASE		(CONFIG_SPL_TEXT_BASE + CONFIG_SPL_PAD_TO - 0x40) /* 0x40 = sizeof (image_header) */
#define CONFIG_SYS_OS_BASE		0
#define CONFIG_SYS_SPL_ARGS_ADDR	0
#define CONFIG_SYS_FDT_BASE		0
#endif

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
#define CONFIG_MBR_P2_TYPE		linux

#define CONFIG_MBR_P3_OFF		1609mb
#define CONFIG_MBR_P3_END		7800mb
#define CONFIG_MBR_P3_TYPE		fat
#endif

/*
#define CONFIG_CMD_FLOCK
#define CONFIG_CMD_I2C
#define CONFIG_CMD_NAND
#define CONFIG_CMD_SF
#define CONFIG_CMD_SFCNAND
#define CONFIG_CMD_SFC_NAND
#define CONFIG_CMD_UBI
#define CONFIG_CMD_UBIFS
#define CONFIG_CMD_USB
*/

#if defined(CONFIG_SPL_MMC_SUPPORT)
#define CONFIG_BOOTCOMMAND \
"setenv setargs setenv bootargs ${bootargs};run setargs;" \
"mmc rescan;mmc read ${baseaddr} 0x1800 0x3000;" \
"bootm ${baseaddr};"
#elif defined(CONFIG_SFC_NOR)
#define CONFIG_BOOTCOMMAND \
"sf probe;sq probe;setenv setargs setenv bootargs ${bootargs};run setargs;" \
"sf read ${baseaddr} \\${kern_addr} \\${kern_len};" \
"bootm ${baseaddr};"
#elif defined(CONFIG_SFC_NAND)
#define CONFIG_BOOTCOMMAND \
"sf probe;sq probe;setenv setargs setenv bootargs ${bootargs};run setargs;" \
"sfcnand read \\${kern_addr} \\${kern_len} ${baseaddr};" \
"bootm ${baseaddr};"
#else
#define CONFIG_BOOTCOMMAND \
"sf probe;sq probe;setenv setargs setenv bootargs ${bootargs};run setargs;" \
"sf read ${baseaddr} 0x50000 0x250000;" \
"bootm ${baseaddr};"
#endif

#define CONFIG_BOOTARGS \
BOOTARGS_COMMON \
" console=\\${serialport},\\${baudrate}n8" \
" panic=\\${panic_timeout} root=/dev/mtdblock3 rootfstype=squashfs init=/init" \
" mtdparts=jz_sfc:256k(boot),64k(env),\\${kern_size}(kernel),\\${rootfs_size}(rootfs),-(rootfs_data)\\${update}"

#define CONFIG_EXTRA_ENV_SETTINGS \
"baseaddr=0x80600000\0" \
"panic_timeout=10\0" \
"serialport=ttyS1\0" \
"restore=n\0" \
"disable_eth=false\0" \
"disable_sd=false\0" \
"enable_updates=false\0" \
"boot_complete=false\0" \
"soc="CONFIG_SOC"\0" \
CONFIG_EXTRA_SETTINGS \
CONFIG_GPIO_SETTINGS \
CONFIG_GPIO_IRCUT_SETTINGS

/* IRCUT Default GPIOs */

#define CONFIG_GPIO_IRCUT_SETTINGS \
"gpio_ircut=52I 53I 49I 50I 57I 58I\0"

#endif /*__CONFIG_ISVP_COMMON__*/
