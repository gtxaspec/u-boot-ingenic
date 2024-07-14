/*
 * DDR parameters data structure.
 *
 * Copyright (C) 2013 Ingenic Semiconductor Co.,Ltd
 * Author: Zoro <ykli@ingenic.cn>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __DDR_PARAMS_H__
#define __DDR_PARAMS_H__

enum ddr_type{
	DDR3,
	LPDDR,
	LPDDR2,
	DDR2,
	VARIABLE,
	UNKOWN,
};

/* ----------------------- */
struct size {
	uint32_t chip0;
	uint32_t chip1;
};

#define ddr_common_params			\
	uint32_t tRAS;				\
	uint32_t tRP;				\
	uint32_t tRCD;				\
	uint32_t tRC;				\
	uint32_t tWR;				\
	uint32_t tRRD;				\
	uint32_t tWTR;				\
	uint32_t tRFC;				\
	uint32_t tXP;				\
	uint32_t tCKE;				\
	uint32_t RL;				\
	uint32_t WL;				\
	uint32_t tREFI;

struct ddr3_params {
	ddr_common_params;
	uint32_t tCKESR;
	uint32_t tCKSRE;
	uint32_t tXSDLL;
	uint32_t tMOD;
	uint32_t tXPDLL;
	uint32_t tXS;
	uint32_t tRTP;
	uint32_t tCCD;
	uint32_t tFAW;
	uint32_t tMRD;
};

struct ddr2_params {
	ddr_common_params;
	uint32_t tCKESR;
	uint32_t tCWL;
	uint32_t tXSNR;
	uint32_t tXARD;
	uint32_t tXARDS;
	uint32_t tXSRD;
	uint32_t tRTP;
	uint32_t tCCD;
	uint32_t tFAW;
	uint32_t tMRD;
};

struct lpddr2_params {
	ddr_common_params;
	uint32_t tCKESR;
	uint32_t tXSR;
	uint32_t tMOD;
	uint32_t tDQSCK;
	uint32_t tDQSCKMAX;
	uint32_t tRTP;
	uint32_t tCCD;
	uint32_t tFAW;
};
struct lpddr_params {
	ddr_common_params;
	uint32_t tMRD;
	uint32_t tDQSSMAX;
	uint32_t tXSR;

};
struct ddr_params_common
{
	ddr_common_params;
};

union private_params {
	struct ddr_params_common ddr_base_params;
	struct ddr3_params ddr3_params;
	struct lpddr_params lpddr_params;
	struct lpddr2_params lpddr2_params;
	struct ddr2_params  ddr2_params;
};

struct ddr_params {
	uint32_t type;
	uint32_t freq;
	uint32_t div;
	uint32_t cs0;
	uint32_t cs1;
	uint32_t dw32;
	uint32_t cl;
	uint32_t bl;
	uint32_t col;
	uint32_t row;
	uint32_t col1;
	uint32_t row1;
	uint32_t bank8;
	struct size size;
	union private_params private_params;
};

#endif /* __DDR_PARAMS_H__ */
