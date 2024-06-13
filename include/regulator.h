/*
 * SoC Regulator driver support
 *
 * Copyright (c) 2013 Ingenic Semiconductor Co.,Ltd
 * Author: yqfu <yqfu@ingenic.cn>
 * Based on: u-boot-ak47/include/regulator.h
 *           Written by Kage Shen <kkshen@ingenic.cn>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __REGULATOR_H__
#define __REGULATOR_H__

#include <linux/list.h>

/*
 * struct regulator
 *
 * One for each consumer device.
 */

struct regulator {
	struct list_head list;
	char *name;
	int id;
	int n_voltages;

	void *reg_data;		/* regulator data */
	int min_uV;
	int max_uV;
	int min_uA;
	int max_uA;

	/* optional regulator machine specific init */
	int (*regulator_init)(void *driver_data);

	struct regulator_ops *ops;
};

struct regulator_ops {

	/* enumerate supported voltages */
	int (*list_voltage) (struct regulator *, unsigned selector);
	/* get/set regulator voltage */
	int (*set_voltage) (struct regulator *, int min_uV, int max_uV);
	int (*get_voltage) (struct regulator *);

	int (*set_current) (struct regulator *, int min_uA, int max_uA);
	int (*get_current) (struct regulator *);

	/* enable/disable regulator */
	int (*enable) (struct regulator *);
	int (*disable) (struct regulator *);
	int (*is_enabled) (struct regulator *);
};

/* regulator output control and status */
int regulator_enable(struct regulator *regulator);
int regulator_disable(struct regulator *regulator);
int regulator_is_enabled(struct regulator *regulator);

int regulator_set_voltage(struct regulator *regulator, int min_uV, int max_uV);
int regulator_get_voltage(struct regulator *regulator);

int regulator_register(struct regulator *regulator, void *driver_data);
struct regulator *regulator_get(const char *);
void *regulator_get_drvdata(struct regulator *);

int regulator_set_current_limit(struct regulator *regulator,
			       int min_uA, int max_uA);
int regulator_get_current_limit(struct regulator *regulator);

enum regulator_outnum { REGULATOR_CORE = 1, REGULATOR_MEM, REGULATOR_IO };
int spl_regulator_set_voltage(enum regulator_outnum outnum, int vol_mv);
int spl_regulator_init();

#endif
