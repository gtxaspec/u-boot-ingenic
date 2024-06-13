/*
 * M200 common routines
 *
 * Copyright (c) 2014 Ingenic Semiconductor Co.,Ltd
 * Author: Huddy <hyli@ingenic.cn>
 * Based on: xboot/boot/lcd/jz4775_android_lcd.h
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __TRULY_TFT240240_2_E_H__
#define __TRULY_TFT240240_2_E_H__

struct truly_tft240240_2_e_data {
	unsigned int gpio_lcd_rd;
	unsigned int gpio_lcd_rst;
	unsigned int gpio_lcd_cs;
	unsigned int gpio_lcd_bl;
};

extern struct truly_tft240240_2_e_data truly_tft240240_2_e_pdata;

#endif /* __TRULY_TFT240240_2_E_H__ */
