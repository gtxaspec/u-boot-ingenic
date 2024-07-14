/*
 * Ingenic burn command
 *
 * Copyright (c) 2013 cli <cli@ingenic.cn>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <malloc.h>
#include <asm/errno.h>
#include <g_burntool.h>
#include <linux/usb/gadget.h>

static int do_burn(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	const char *str_env;
	char *s = "vdr";	//vendor
	char *env_bkp;
	int ret;

	if (argc > 1)
		return CMD_RET_USAGE;

	if (g_burntool_register(s))
		return CMD_RET_FAILURE;
	g_burntool_virtual_set_config(s);

#if !defined(CONFIG_USB_SELF_POLLING)
	while (1) {
		if (ctrlc())
			goto exit;
		usb_gadget_handle_interrupts();
	}
exit:
	g_burntool_unregister();
#endif
	return CMD_RET_SUCCESS;
}

U_BOOT_CMD(burn, CONFIG_SYS_MAXARGS, 1, do_burn,
	"Ingenic usb burner",
	"No params\n"
);
