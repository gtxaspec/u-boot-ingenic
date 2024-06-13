/*
 * Ingenic Fastboot Command Explain CMD
 *
 *  Copyright (C) 2013 Ingenic Semiconductor Co., LTD.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <asm/errno.h>
#include <g_fastboot.h>
#include <fastboot.h>

__attribute__ ((weak)) struct partition_info partition_info[16] = {
	[0] = {.pname = "ndxboot",.offset = 0*1024*1024,.size = 3*1024*1024},
	[1] = {.pname = "ndboot",.offset = 3*1024*1024,.size = 8*1024*1024},
	[2] = {.pname = "ndrecovery",.offset = 11*1024*1024,.size = 8*1024*1024},
	[3] = {.pname = "ndmisc",.offset = 19*1024*1024,.size = 4*1024*1024},
	[4] = {.pname = "ndbattery",.offset = 23*1024*1024,.size = 1*1024*1024},
	[5] = {.pname = "ndcache",.offset = 24*1024*1024,.size = 30*1024*1024},
	[6] = {.pname = "nddevice_id",.offset = 54*1024*1024,.size = 2*1024*1024},
	[7] = {.pname = "ndsystem",.offset = 56*1024*1024,.size = 512*1024*1024},
	[8] = {.pname = "nddata",.offset = 568*1024*1024,.size = 1024*1024*1024},
	[9] = {.pname = "ndstorage1",.offset = 1592*1024*1024,.size = 1024*1024*1024},
	[10] = {.pname = "ndcmdline",.offset = 2616*1024*1024,.size = 3*1024*1024},
};

static int do_fastboot(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	char *s = "fastboot";

	if (argc > 1)
		return CMD_RET_USAGE;

	g_fastboot_register(s);

	handle_fastboot_cmd();

	g_fastboot_unregister();

	return CMD_RET_SUCCESS;
}

U_BOOT_CMD(
	fastboot, 1, 1, do_fastboot,
	"enter fastboot mode",
	"enter fastboot mode"
);
