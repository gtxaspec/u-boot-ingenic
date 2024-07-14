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

enum {
	DDR3,
	LPDDR,
	LPDDR2,
	DDR2,
	VARIABLE,
};

struct tck {
	uint32_t ps;
	uint32_t ns;
};
/*only for lpddr2 tRL and tWL*/
struct RL_LPDDR2 {
	uint32_t memclk;
	uint32_t RL;
};

struct WL_LPDDR2 {
	uint32_t memclk;
	uint32_t WL;
};
/* ----------------------- */
struct size {
	uint32_t chip0;
	uint32_t chip1;
};

struct ddr3_params {
	uint32_t tCWL;
	uint32_t tRAS;
	uint32_t tRP;
	uint32_t tRCD;
	uint32_t tRC;
	uint32_t tWR;
	uint32_t tRRD;	//
	uint32_t tRTP;	//
	uint32_t tWTR;	//
	uint32_t tRFC;
	uint32_t tMINSR;
	uint32_t tXP;
	uint32_t tMRD;
	uint32_t tCCD;
	uint32_t tFAW;
	uint32_t tCKE;	//
	uint32_t tRL;
	uint32_t tWL;
	uint32_t tRDLAT;
	uint32_t tWDLAT;
	uint32_t tRTW;
	uint32_t tCKSRE;	//
	uint32_t tDLLLOCK;
	uint32_t tXSDLL;	//
	uint32_t tMOD;		//
	uint32_t tXPDLL;	//
	uint32_t tXS;		//
	uint32_t tXSRD;
	uint32_t tREFI;
	uint32_t tDLLSRST; /* In PHY PTR0 */
};

struct lpddr_params {
	uint32_t tRAS;
	uint32_t tRP;
	uint32_t tRCD;
	uint32_t tRC;
	uint32_t tWR;
	uint32_t tRRD;
	uint32_t tRTP;	//
	uint32_t tWTR;	//
	uint32_t tRFC;
	uint32_t tMINSR;
	uint32_t tXP;
	uint32_t tMRD;
	uint32_t tCCD;
	uint32_t tFAW;
	uint32_t tCKE;
	uint32_t tRL;
	uint32_t tWL;
	uint32_t tRDLAT;
	uint32_t tWDLAT;
	uint32_t tRTW;
	uint32_t tCKSRE;
	uint32_t tXS;
	uint32_t tXSRD;
	uint32_t tREFI;
	uint32_t tDLLSRST; /* In PHY PTR0 */
};

struct lpddr2_params {
	uint32_t tRAS;
	uint32_t tRP;	//
	uint32_t tRCD;	//
	uint32_t tRC;	// tRAS + tRP
	uint32_t tWR;	//
	uint32_t tRRD;	//
	uint32_t tRTP;	//
	uint32_t tWTR;	//
	uint32_t tRFC;
	uint32_t tMINSR;
	uint32_t tXP;	//
	uint32_t tMRD;
	uint32_t tCCD;
	uint32_t tFAW;	//
	uint32_t tCKE;
	uint32_t tRL;
	uint32_t tWL;
	uint32_t tRDLAT;
	uint32_t tWDLAT;
	uint32_t tRTW;		//
	uint32_t tCKSRE;	//
	uint32_t tDLLLOCK;
	uint32_t tXSDLL;
	uint32_t tMOD;
	uint32_t tXPDLL;
	uint32_t tXS;		//
	uint32_t tXSRD;		//
	uint32_t tREFI;
	uint32_t tDLLSRST; /* In PHY PTR0 */
	uint32_t tDQSCK;
	uint32_t tDQSCKmax;
};

struct ddr2_params {
	uint32_t tCWL;
	uint32_t tRAS;
	uint32_t tRP;
	uint32_t tRCD;
	uint32_t tRC;
	uint32_t tWR;
	uint32_t tRRD;	//
	uint32_t tRTP;	//
	uint32_t tWTR;	//
	uint32_t tRFC;
	uint32_t tMINSR;
	uint32_t tXP;
	uint32_t tMRD;
	uint32_t tCCD;
	uint32_t tFAW;	//
	uint32_t tCKE;
	uint32_t tRL;
	uint32_t tWL;
	uint32_t tRDLAT;
	uint32_t tDLLLOCK;
	uint32_t tWDLAT;
	uint32_t tRTW;		//
	uint32_t tCKSRE;	//
	uint32_t tXSRD;		//
	uint32_t tXS;
	uint32_t tREFI;
	uint32_t tDLLSRST; /* In PHY PTR0 */
};

union private_params {
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
	struct tck tck;
	struct size size;
	union private_params private_params;
};

#endif /* __DDR_PARAMS_H__ */
