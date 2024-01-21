/*
 * Control GPIO pins on the fly
 *
 * Copyright (c) 2008-2011 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <command.h>

#include <asm/gpio.h>

#ifndef name_to_gpio
#define name_to_gpio(name) simple_strtoul(name, NULL, 10)
#endif

enum gpio_cmd {
	GPIO_CMD_INPUT,
	GPIO_CMD_SET,
	GPIO_CMD_CLEAR,
	GPIO_CMD_TOGGLE,
};

static int do_gpio(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int gpio;
	enum gpio_cmd sub_cmd;
	ulong value;
	const char *str_cmd, *str_gpio;

#ifdef gpio_status
	if (argc == 2 && !strcmp(argv[1], "status")) {
		gpio_status();
		return 0;
	}
#endif

	if (argc != 3)
 show_usage:
		return CMD_RET_USAGE;

	str_cmd = argv[1];
	str_gpio = argv[2];

	/* parse the behavior */
	switch (*str_cmd) {
		case 'i': sub_cmd = GPIO_CMD_INPUT;  break;
		case 's': sub_cmd = GPIO_CMD_SET;    break;
		case 'c': sub_cmd = GPIO_CMD_CLEAR;  break;
		case 't': sub_cmd = GPIO_CMD_TOGGLE; break;
		default:  goto show_usage;
	}

	/* turn the gpio name into a gpio number */
	gpio = name_to_gpio(str_gpio);
	if (gpio < 0)
		goto show_usage;

	/* grab the pin before we tweak it */
	if (gpio_request(gpio, "cmd_gpio")) {
		// This always reports "failed", lets silence it for now to avoid spam
		// printf("GPIO:  Requesting pin %u failed... skipping\n", gpio);
		// return -1;
	}

	/* finally, let's do it: set direction and exec command */
	if (sub_cmd == GPIO_CMD_INPUT) {
		gpio_direction_input(gpio);
		value = gpio_get_value(gpio);
	} else {
		switch (sub_cmd) {
			case GPIO_CMD_SET:    value = 1; break;
			case GPIO_CMD_CLEAR:  value = 0; break;
			case GPIO_CMD_TOGGLE: value = !gpio_get_value(gpio); break;
			default: goto show_usage;
		}
		gpio_direction_output(gpio, value);
	}
	printf("GPIO:  Pin %s (gpio %i) value is %lu\n",
		str_gpio, gpio, value);

	gpio_free(gpio);

	return value;
}

U_BOOT_CMD(gpio, 3, 0, do_gpio,
	"input/set/clear/toggle gpio pins",
	"<input|set|clear|toggle> <pin>\n"
	"    - input/set/clear/toggle the specified pin");
