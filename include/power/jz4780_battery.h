/*
 * Battery measurement code for Ingenic JZ SoC
 *
 * Copyright (c) 2013 Ingenic Semiconductor Co.,Ltd
 * Author: Kage Shen <kkshen@ingenic.cn>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __JZ4780_BATTERY_H__
#define __JZ4780_BATTERY_H__

enum battery_status {
	CHARGER_AC = 'B',
	CHARGER_USB,
	CHARGER_NONE,
};

struct charger_ops {
	/* enumerate supported voltages */
	int (*get_charger_status) (void);

	/* enable/disable regulator */
	int (*charger_enable) (struct regulator *);
	int (*charger_disable) (struct regulator *);
	int (*charger_is_enabled) (struct regulator *);

	int (*set_current) (struct regulator *, int min_uA, int max_uA);
	int (*get_current) (struct regulator *);
};

struct battery_dec {
	char *name;
	struct regulator *reg;
	/* enumerate supported voltages */
	struct charger_ops *ops;
};

extern int jz4780_battery_cherger(struct charger_ops *);
extern int get_battery_voltage(void);

#endif
