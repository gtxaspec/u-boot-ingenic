/*
 * T41 GPIO definitions
 * Copyright (c) 2019 Ingenic Semiconductor Co.,Ltd
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __GPIO_H__
#define __GPIO_H__

#include <asm/arch/base.h>

#define GPIO_PA(n) 	(0*32 + n)
#define GPIO_PB(n) 	(1*32 + n)
#define GPIO_PC(n) 	(2*32 + n)
#define GPIO_PD(n) 	(3*32 + n)
#define GPIO_PE(n) 	(4*32 + n)
#define GPIO_PF(n) 	(5*32 + n)
#define GPIO_PG(n) 	(6*32 + n)

enum gpio_function {
	GPIO_FUNC_0     = 0x00,  //0000, GPIO as function 0 / device 0
	GPIO_FUNC_1     = 0x01,  //0001, GPIO as function 1 / device 1
	GPIO_FUNC_2     = 0x02,  //0010, GPIO as function 2 / device 2
	GPIO_FUNC_3     = 0x03,  //0011, GPIO as function 3 / device 3
	GPIO_INPUT	= 0x06,	 //0110, GPIO as input
};

enum gpio_port {
	GPIO_PORT_A,
	GPIO_PORT_B,
	GPIO_PORT_C,
	GPIO_PORT_D,
	GPIO_PORT_E,
	GPIO_PORT_F,
	GPIO_PORT_G,
	/* this must be last */
	GPIO_NR_PORTS,
};

typedef enum {
	DS_2_MA = 2,/* reserved, not support */
	DS_3_MA,    /* reserved, not support */
	DS_4_MA,
	RESERVED5,
	DS_6_MA ,
	RESERVED7,
	DS_8_MA,
	DS_9_MA,
	RESERVED10,
	DS_11_MA,
	DS_12_MA,
	RESERVED13,
	DS_14_MA,
	RESERVED15,
	DS_16_MA,
	DS_17_MA,
	RESERVED18,
	DS_19_MA,
	DS_20_MA,
	RESERVED21,
	DS_22_MA,
	RESERVED23,
	DS_24_MA,
} gpio_drv_level_t;

enum gpio_pull {
	GPIO_PULL_UP,  //Pull Up
	GPIO_PULL_SUP, //Strong Pull Up
	GPIO_PULL_DOWN,//Pull Down
	GPIO_DISABLE_PULL,//Disable UP and Down
};

struct jz_gpio_func_def {
	int port;
	int func;
	unsigned long pins;
    int driver_strength;
};

typedef struct gpio_drive_strength_table {
    enum gpio_port port;
	unsigned long pins;
	gpio_drv_level_t drv_level;
} gpio_drive_strength_table_t;


/*************************************************************************
 * GPIO (General-Purpose I/O Ports)
 *************************************************************************/
#define MAX_GPIO_NUM	192

#define PXPIN		0x00   /* PIN Level Register */
#define PXINT		0x10   /* Port Interrupt Register */
#define PXINTS		0x14   /* Port Interrupt Set Register */
#define PXINTC		0x18   /* Port Interrupt Clear Register */
#define PXMSK		0x20   /* Port Interrupt Mask Reg */
#define PXMSKS		0x24   /* Port Interrupt Mask Set Reg */
#define PXMSKC		0x28   /* Port Interrupt Mask Clear Reg */
#define PXPAT1		0x30   /* Port Pattern 1 Set Reg. */
#define PXPAT1S		0x34   /* Port Pattern 1 Set Reg. */
#define PXPAT1C		0x38   /* Port Pattern 1 Clear Reg. */
#define PXPAT0		0x40   /* Port Pattern 0 Register */
#define PXPAT0S		0x44   /* Port Pattern 0 Set Register */
#define PXPAT0C		0x48   /* Port Pattern 0 Clear Register */
#define PXFLG		0x50   /* Port Flag Register */
#define PXFLGC		0x58   /* Port Flag clear Register */


#define PXGFCFG0	0x70   /* Port Glitch Filter Configure 0 Register */
#define PXGFCFG0S	0x74   /* Port Glitch Filter Configure 0 Set Register */
#define PXGFCFG0C	0x78   /* Port Glitch Filter Configure 0 Clear Register */
#define PXGFCFG1	0x80   /* Port Glitch Filter Configure 1 Register */
#define PXGFCFG1S	0x84   /* Port Glitch Filter Configure 1 Set Register */
#define PXGFCFG1C	0x88   /* Port Glitch Filter Configure 1 Clear Register */
#define PXGFCFG2	0x90   /* Port Glitch Filter Configure 2 Register */
#define PXGFCFG2S	0x94   /* Port Glitch Filter Configure 2 Set Register */
#define PXGFCFG2C	0x98   /* Port Glitch Filter Configure 2 Clear Register */
#define PXGFCFG3	0xa0   /* Port Glitch Filter Configure 3 Register */
#define PXGFCFG3S	0xa4   /* Port Glitch Filter Configure 3 Set Register */
#define PXGFCFG3C	0xa8   /* Port Glitch Filter Configure 3 Clear Register */


#define PXGPDCR		0x100   /* Port IO Power Domain control Register */
#define PXGPDCRS	0x104   /* Port IO Power Domain control Set Register */
#define PXGPDCRC	0x108   /* Port IO Power Domain control Clear Register */
#define PXPUEN		0x110   /* Port Pull-up status Register */
#define PXPUENS		0x114   /* Port Pull-up status Set Register */
#define PXPUENC		0x118   /* Port Pull-up status Clear Register */
#define PXPDEN		0x120   /* Port Pull-down status Register */
#define PXPDENS		0x124   /* Port Pull-down status Set Register */
#define PXPDENC		0x128   /* Port Pull-down status Clear Register */
#define PXPDS0		0x130   /* Port Driver-strength 0  Register */
#define PXPDS0S		0x134   /* Port Driver-strength 0 Set Register */
#define PXPDS0C		0x138   /* Port Driver-strength 0 Clear Register */
#define PXPDS1		0x140   /* Port Driver-strength 1  Register */
#define PXPDS1S		0x144   /* Port Driver-strength 1 Set Register */
#define PXPDS1C		0x148   /* Port Driver-strength 1 Clear Register */
#define PXPDS2		0x150   /* Port Driver-strength 2  Register */
#define PXPDS2S		0x154   /* Port Driver-strength 2 Set Register */
#define PXPDS2C		0x158   /* Port Driver-strength 2 Clear Register */
#define PXPSLW		0x160   /* Port Slew Rate Register */
#define PXPSLWS		0x164   /* Port Slew Rate Set Register */
#define PXPSLWC		0x168   /* Port Slew Rate Clear Register */
#define PXPSMT		0x170   /* Port Schmitt Trigger Register */
#define PXPSMTS		0x174   /* Port Schmitt Trigger Set Register */
#define PXPSMTC		0x178   /* Port Schmitt Trigger Clear Register */

/* Only PA6-19 use */
#define PXPDS3		0x1a0   /* PortA Driver Strength Bit 3 Register */
#define PXPDS3S		0x1a4   /* PortA Driver Strength Bit 3 Set Register */
#define PXPDS3C		0x1a8   /* PortA Driver Strength Bit 3 Clear Register */
#define PXPSPU		0x1b0   /* PortA Strong Pull Up Register */
#define PXPSPUS		0x1b4   /* PortA Strong Pull Up Set Register */
#define PXPSPUC		0x1b8   /* PortA Strong Pull Up Clear Register */

#define PSHADOW_OFF		0x7000
#define PZGID2LD        0xF0    /* GPIOZ Group ID to load */

#define GPIO_PXPIN(n)	(GPIO_BASE + (PXPIN + (n)*0x1000))     /* PIN Level Register */
#define GPIO_PXINT(n)	(GPIO_BASE + (PXINT + (n)*0x1000))     /* Port Interrupt Register */
#define GPIO_PXINTS(n)	(GPIO_BASE + (PXINTS + (n)*0x1000))    /* Port Interrupt Set Register */
#define GPIO_PXINTC(n)	(GPIO_BASE + (PXINTC + (n)*0x1000))    /* Port Interrupt Clear Register */
#define GPIO_PXMSK(n)	(GPIO_BASE + (PXMSK + (n)*0x1000))     /* Port Interrupt Mask Register */
#define GPIO_PXMSKS(n)	(GPIO_BASE + (PXMSKS + (n)*0x1000))    /* Port Interrupt Mask Set Reg */
#define GPIO_PXMSKC(n)	(GPIO_BASE + (PXMSKC + (n)*0x1000))    /* Port Interrupt Mask Clear Reg */
#define GPIO_PXPAT1(n)	(GPIO_BASE + (PXPAT1 + (n)*0x1000))    /* Port Pattern 1 Register */
#define GPIO_PXPAT1S(n)	(GPIO_BASE + (PXPAT1S + (n)*0x1000))   /* Port Pattern 1 Set Reg. */
#define GPIO_PXPAT1C(n)	(GPIO_BASE + (PXPAT1C + (n)*0x1000))   /* Port Pattern 1 Clear Reg. */
#define GPIO_PXPAT0(n)	(GPIO_BASE + (PXPAT0 + (n)*0x1000))    /* Port Pattern 0 Register */
#define GPIO_PXPAT0S(n)	(GPIO_BASE + (PXPAT0S + (n)*0x1000))   /* Port Pattern 0 Set Register */
#define GPIO_PXPAT0C(n)	(GPIO_BASE + (PXPAT0C + (n)*0x1000))   /* Port Pattern 0 Clear Register */
#define GPIO_PXFLG(n)	(GPIO_BASE + (PXFLG + (n)*0x1000))     /* Port Flag Register */
#define GPIO_PXFLGC(n)	(GPIO_BASE + (PXFLGC + (n)*0x1000))    /* Port Flag clear Register */
#define GPIO_PXPUEN(n)	(GPIO_BASE + (PXPUEN + (n)*0x1000))    /* Port Pull-up status Register */
#define GPIO_PXPUENS(n)	(GPIO_BASE + (PXPUENS + (n)*0x1000))   /* Port Pull-up status Set Register */
#define GPIO_PXPUENC(n)	(GPIO_BASE + (PXPUENC + (n)*0x1000))   /* Port Pull-up status Clear Register */
#define GPIO_PXPDEN(n) 	(GPIO_BASE + (PXPDEN + (n)*0x1000))    /* Port Pull-down status Register */
#define GPIO_PXPDENS(n)	(GPIO_BASE + (PXPDENS + (n)*0x1000))   /* Port Pull-down status Set Register */
#define GPIO_PXPDENC(n)	(GPIO_BASE + (PXPDENC + (n)*0x1000))   /* Port Pull-down status Clear Register */

/* Only PA6-19 use */
#define GPIO_PXPSPU(n)  (GPIO_BASE + (PXPSPU + (n)*0x1000))
#define GPIO_PXPSPUS(n) (GPIO_BASE + (PXPSPUS + (n)*0x1000))
#define GPIO_PXPSPUC(n) (GPIO_BASE + (PXPSPUC + (n)*0x1000))

void gpio_set_func(enum gpio_port n, enum gpio_function func, unsigned int pins);
void gpio_port_set_value(int port, int pin, int value);
void gpio_port_direction_input(int port, int pin);
void gpio_port_direction_output(int port, int pin, int value);
void gpio_set_driver_strength(enum gpio_port n, unsigned int pins, int ds);
void gpio_init(void);
void gpio_set_pull(unsigned gpio, enum gpio_pull pull);
void gpio_as_irq_high_level(unsigned gpio);
void gpio_as_irq_low_level(unsigned gpio);
void gpio_as_irq_rise_edge(unsigned gpio);
void gpio_as_irq_fall_edge(unsigned gpio);
void gpio_ack_irq(unsigned gpio);

#endif /* __GPIO_H__ */
