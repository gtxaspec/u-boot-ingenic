
/*
 * Ingenic burn command
 *
 * Copyright (c) 2013 jykang <jykang@ingenic.cn>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <malloc.h>
#include <asm/io.h>
#include <asm/errno.h>

#include <asm/arch/gpio.h>
#include <asm/arch/cpm.h>

int start_serial_cloner();
static int serial_burner()
{
	start_serial_cloner();
}

static int do_serial_burn(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	const char *str_env;
	char *s = "vdr";	//vendor
	char *env_bkp;
	int ret;

	if (argc > 1)
		return CMD_RET_USAGE;
	serial_burner();
	return CMD_RET_SUCCESS;
}

U_BOOT_CMD(serial_burn, CONFIG_SYS_MAXARGS, 1, do_serial_burn,
	"Ingenic serial burner",
	"No params\n"
);
