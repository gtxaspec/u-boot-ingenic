#
# Copyright (C) 2011 Xiangfu Liu <xiangfu@openmobilefree.net>
#
# See file CREDITS for list of people who contributed to this
# project.
#
# SPDX-License-Identifier:	GPL-2.0+

include $(TOPDIR)/config.mk

LIB	= $(obj)lib$(CPU).o

START	= start.o
SOBJS-y	=
COBJS-y	= jz_serial.o
COBJS-y	+= cpu.o gost_timer.o epc_debug.o
COBJS-$(CONFIG_XBURST2_TRAPS) += traps.o
COBJS-$(CONFIG_SPL_OS_BOOT) += os_boot.o

ifdef	CONFIG_SPL_BUILD
ifdef	CONFIG_SPL_NO_CPU_SUPPORT_CODE
START	:=
endif
endif

SRCS	:= $(START:.o=.S) $(SOBJS-y:.o=.S) $(COBJS-y:.o=.c)
OBJS	:= $(addprefix $(obj),$(SOBJS-y) $(COBJS-y))
START	:= $(addprefix $(obj),$(START))

all:	$(obj).depend $(START) $(LIB)

$(LIB):	$(OBJS)
	$(call cmd_link_o_target, $(OBJS))

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
