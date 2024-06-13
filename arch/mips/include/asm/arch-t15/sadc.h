/*
 * T15 sadc definitions
 *
 * Copyright (c) 2013 Ingenic Semiconductor Co.,Ltd
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __SADC_H__
#define __SADC_H__

#include <asm/arch/base.h>

/*************************************************************************
 * SADC (SAR A/D Controller)
 *************************************************************************/
#define SADC_ADENA		0x00
#define SADC_ADCFG		0x04
#define SADC_ADCTRL		0x08
#define SADC_ADSTATE		0x0C
#define SADC_ADSAME		0x10
#define SADC_ADWAIT		0x14
#define SADC_ADTCH		0x18
#define SADC_ADVDAT		0x1C
#define SADC_ADADAT		0x20
#define SADC_ADCMD		0x24
#define SADC_ADCLK		0x28
#define SADC_ADSTB		0x34

/* ADC Enable Register */
#define SADC_ADENA_POWER	(1 << 7)
#define SADC_ADENA_SLP_MD	(1 << 6)
#define SADC_ADENA_PENDEN	(1 << 3)
#define SADC_ADENA_TCHEN	(1 << 2)
#define SADC_ADENA_VBATEN	(1 << 1)
#define SADC_ADENA_AUXEN	(1 << 0)

/* ADC Status Register */
#define SADC_ADSTATE_SLP_RDY	(1 << 7)
#define SADC_ADSTATE_SLPEND	(1 << 5)
#define SADC_ADSTATE_PEND	(1 << 4)
#define SADC_ADSTATE_PENU	(1 << 3)
#define SADC_ADSTATE_DTCH	(1 << 2)
#define SADC_ADSTATE_VRDY	(1 << 1)
#define SADC_ADSTATE_ARDY	(1 << 0)

#endif /* __SADC_H__ */
