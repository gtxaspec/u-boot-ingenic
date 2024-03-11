/*
 * X2000 GLOBAL OST definitions
 *
 * Copyright (c) 2016 Ingenic Semiconductor Co.,Ltd
 * 	qipengzhen <aric.pzqi@ingenic.com>
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

#ifndef __G_OST_H__
#define __G_OST_H__

#include <asm/arch/base.h>


/* X2000 has 4 event OST for eache cpu core, and 1 Global ost */


#define G_OSTCCR   0x00
#define G_OSTER    0x04
#define G_OSTCR    0x08
#define G_OSTCNTH  0x0C
#define G_OSTCNTL  0x10
#define G_OSTCNTB  0x14

#define G_OSTCCR_PRESCALE_1		(0 << 0)
#define G_OSTCCR_PRESCALE_4		(1 << 0)
#define G_OSTCCR_PRESCALE_16		(2 << 0)

#define G_OST_DIV				4

#if (G_OST_DIV == 1)
#define G_OSTCCR_PRESCALE			G_OSTCCR_PRESCALE_1
#elif (G_OST_DIV == 4)
#define G_OSTCCR_PRESCALE			G_OSTCCR_PRESCALE_4
#elif (G_OST_DIV == 16)
#define G_OSTCCR_PRESCALE			G_OSTCCR_PRESCALE_16
#endif


#endif /* __G_OST_H__ */
