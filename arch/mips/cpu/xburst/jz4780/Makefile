#
# Ingenic JZ4780 makefile
#
# Copyright (c) 2013 Ingenic Semiconductor Co.,Ltd
# Author: Justin <ptkang@ingenic.cn>
# Based on: arch/mips/cpu/xburst/jz4780/Makefile
#           Written by Paul Burton <paul.burton@imgtec.com>
#
# SPDX-License-Identifier:	GPL-2.0+

include $(TOPDIR)/config.mk

LIB	= $(obj)lib$(SOC).o

SOBJS-y	=
COBJS-y	= soc.o
COBJS-y += clk.o
COBJS-y += ddr_set_dll.o
COBJS-y += ../ddr_dwc.o
ifdef CONFIG_SPL_DDR_SOFT_TRAINING
COBJS-y += ../ddr_dqs_training.o
else
ifdef CONFIG_DDR_FORCE_SOFT_TRAINING
COBJS-y += ../ddr_dqs_training.o
endif
endif

ifdef CONFIG_BURNER
COBJS-y += dym_pll.o
else
COBJS-y += pll.o
endif

SRCS	:= $(START:.o=.S) $(SOBJS-y:.o=.S) $(COBJS-y:.o=.c)
OBJS	:= $(addprefix $(obj),$(SOBJS-y) $(COBJS-y))
START	:= $(addprefix $(obj),$(START))

all:	$(obj).depend $(LIB)

$(LIB):	$(OBJS)
	$(call cmd_link_o_target, $(OBJS))

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
