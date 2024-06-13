/*
 * JZ4775 common routines
 *
 * Copyright (c) 2013 Ingenic Semiconductor Co.,Ltd
 * Author: Huddy <hyli@ingenic.cn>
 * Based on: xboot/boot/lcd/jz4775_android_lcd.h
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __BYD_BM8766U_H__
#define __BYD_BM8766U_H__
/*
 * @gpio_lcd_vsync: vertical sync input
 * @gpio_lcd_hsync: horizontal sync input
 * @gpio_lcd_disp: display on/off
 * @gpio_lcd_de: 1: select DE mode, 0: select SYNC mode
 */
struct byd_bm8766u_data {
	unsigned int gpio_lcd_disp;
	unsigned int gpio_lcd_vsync;
	unsigned int gpio_lcd_hsync;
	unsigned int gpio_lcd_de;
};

extern struct byd_bm8766u_data byd_bm8766u_pdata;

#endif /* __BYD_BM8766U_H__ */
