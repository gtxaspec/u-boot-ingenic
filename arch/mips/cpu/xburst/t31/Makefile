#
# Ingenic T31 makefile
#
# Copyright (c) 2017  Ingenic Semiconductor Co.,Ltd
# Author: Zoro <ykli@ingenic.cn>
# Based on: arch/mips/cpu/xburst/jz4780/Makefile
#           Written by Paul Burton <paul.burton@imgtec.com>
#
# SPDX-License-Identifier:	GPL-2.0+

include $(TOPDIR)/config.mk

LIB	= $(obj)lib$(SOC).o

SOBJS-y	=
COBJS-y	= soc.o
ifndef CONFIG_FPGA
COBJS-y += pll.o
COBJS-y += clk.o
endif
COBJS-y += ddr_set_dll.o
ifdef CONFIG_DDR_INNOPHY
COBJS-y += ../ddr_innophy.o
else
COBJS-y += ../ddr_dwc.o
endif
COBJS-$(CONFIG_DDR_TEST) += ../ddr_test.o
COBJS-$(CONFIG_SPL_DDR_SOFT_TRAINING) += ../ddr_dqs_training.o

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
