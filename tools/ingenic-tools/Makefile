#
# Ingenic tools makefile
#
# Copyright (c) 2013 Ingenic Semiconductor Co.,Ltd
# Author: Zoro <ykli@ingenic.cn>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

include $(TOPDIR)/config.mk

ifndef CONFIG_SPL_BUILD
HOSTCPPFLAGS = -I$(TOPDIR)/include -DUSE_HOSTCC

DDR-$(CONFIG_DDR_PARAMS_CREATOR)	+= ddr_params_creator ddr_reg_values.h
MBR-$(CONFIG_MBR_CREATOR)		+= mbr_creator mbr.bin
GPT-$(CONFIG_GPT_CREATOR)		+= mbr-gpt.bin
SPI-$(CONFIG_SPI_SPL_CHECK)		+= spi_checksum
SPI-$(CONFIG_SFC_NAND)			+= sfc_timing_val.h
SPI-$(CONFIG_SFC_NAND)			+= sfc_nand_params.h
SPL-$(CONFIG_SPL_PARAMS_FIXER)		+= spl_params_fixer
SPL-$(CONFIG_SPL_PAD_TO_BLOCK)		+= spl_pad_to_block
#IMG2RLE-$(CONFIG_RLE_LCD_LOGO)		+= img2rle
#BIN2ARRAY-$(CONFIG_RLE_LCD_LOGO)	+= bin2array
LOGO-$(CONFIG_RLE_LCD_LOGO) = $(RLE_BOOT_LOGO_H)
ifndef CONFIG_BATTERYDET_LED
CHARGE_LOGO-$(CONFIG_CMD_BATTERYDET) = $(RLE_CHARGE_LOGO_H)
endif
RLE_BOOT_LOGO_H = $(OBJTREE)/include/rle_boot_logo.h
ifndef CONFIG_BATTERYDET_LED
RLE_CHARGE_LOGO_H = $(OBJTREE)/include/rle_charge_logo.h
endif

TOOLS	= $(DDR-y) $(MBR-y) $(GPT-y) $(SPI-y) $(SPL-y) $(LOGO-y) $(CHARGE_LOGO-y) #$(IMG2RLE-y) $(BIN2ARRAY-y)
TOOLS	:= $(addprefix $(obj),$(TOOLS))

ifndef CONFIG_BURNER
DDR-TYPE-y = ddr_params_creator.o
DDR-TYPE-$(CONFIG_DDR_TYPE_LPDDR2) += lpddr2_params.o
DDR-TYPE-$(CONFIG_DDR_TYPE_LPDDR) += lpddr_params.o
DDR-TYPE-$(CONFIG_DDR_TYPE_DDR2) += ddr2_params.o
DDR-TYPE-$(CONFIG_DDR_TYPE_DDR3) += ddr3_params.o

define PRIVATE_BUILD
$(1):$(2) $(3)
	$(HOSTCC) $(HOSTCFLAGS_NOPED) $(HOSTLDFLAGS) -c -o $(1) $(2)
endef

ALL_INCLUDE_PRIVATE := $(SRCTREE)/include/configs/$(BOARD).h ddr_params_creator.h
all:	$(obj).depend $(TOOLS)

$(obj)ddr_params_creator: $(DDR-TYPE-y)
	$(HOSTCC) $(HOSTCFLAGS_NOPED) $(HOSTLDFLAGS) -o $@ $^
	$(HOSTSTRIP) $@

$(foreach f,$(DDR-TYPE-y), \
	$(eval $(call PRIVATE_BUILD,$(f),$(f:.o=.c),$(ALL_INCLUDE_PRIVATE))))

$(obj)ddr_reg_values.h:	$(obj)ddr_params_creator
	$(OBJTREE)/tools/ingenic-tools/ddr_params_creator > $(OBJTREE)/include/generated/$@
$(obj)sfc_timing_val.h: $(obj)sfc_timing_params
	$(OBJTREE)/tools/ingenic-tools/sfc_timing_params > $(OBJTREE)/include/generated/$@
$(obj)sfc_nand_params.h: $(obj)sfc_nand_params
	$(OBJTREE)/tools/ingenic-tools/sfc_nand_params > $(OBJTREE)/include/generated/$@
endif
$(obj)mbr_creator:	mbr_creator.c
	$(HOSTCC) $(HOSTCFLAGS_NOPED) $(HOSTLDFLAGS) -o $@ $^
	$(HOSTSTRIP) $@

$(obj)mbr.bin:	$(obj)mbr_creator
	$(OBJTREE)/tools/ingenic-tools/mbr_creator	\
		p0off=$(CONFIG_MBR_P0_OFF),p0end=$(CONFIG_MBR_P0_END),p0type=$(CONFIG_MBR_P0_TYPE) \
		p1off=$(CONFIG_MBR_P1_OFF),p1end=$(CONFIG_MBR_P1_END),p1type=$(CONFIG_MBR_P1_TYPE) \
		p2off=$(CONFIG_MBR_P2_OFF),p2end=$(CONFIG_MBR_P2_END),p2type=$(CONFIG_MBR_P2_TYPE) \
		p3off=$(CONFIG_MBR_P3_OFF),p3end=$(CONFIG_MBR_P3_END),p3type=$(CONFIG_MBR_P3_TYPE) \
		-o $@ > /dev/zero

$(obj)gpt_creator:	$(SRCTREE)/lib/crc32.c gpt_creator.c
	$(HOSTCC) $(HOSTCFLAGS_NOPED) $(HOSTLDFLAGS) -o $@ $^
	$(HOSTSTRIP) $@

ifdef CONFIG_GPT_TABLE_PATH
$(obj)mbr-gpt.bin:	$(obj)gpt_creator
	$(OBJTREE)/tools/ingenic-tools/gpt_creator		\
	$(CONFIG_GPT_TABLE_PATH)/partitions.tab mbr-of-gpt.bin gpt.bin
	cat mbr-of-gpt.bin gpt.bin > $@
	dd if=/dev/zero of=file.bin bs=512 count=33
	cat $@ file.bin > file2.bin
	dd if=file2.bin of=$@ bs=512 count=34
	rm file* -rf
else
$(obj)mbr-gpt.bin:	$(obj)mbr.bin
	dd if=/dev/zero of=gpt.bin bs=512 count=33
	cat mbr.bin gpt.bin > $@
endif

$(obj)spi_checksum:	spi_checksum.c
	$(HOSTCC) $(HOSTCFLAGS_NOPED) $(HOSTLDFLAGS) -o $@ $^
	$(HOSTSTRIP) $@

$(obj)spl_params_fixer:	spl_params_fixer.c
	$(HOSTCC) $(HOSTCFLAGS_NOPED) $(HOSTLDFLAGS) -o $@ $^
	$(HOSTSTRIP) $@
$(obj)sfc_timing_params: sfc_timing_params.c
	$(HOSTCC) $(HOSTCFLAGS_NOPED) $(HOSTLDFLAGS) -o $@ $^
	$(HOSTSTRIP) $@

$(obj)spl_pad_to_block:	spl_pad_to_block.c
	$(HOSTCC) $(HOSTCFLAGS_NOPED) $(HOSTLDFLAGS) -o $@ $^
	$(HOSTSTRIP) $@

$(obj)sfc_nand_params: nand_device/
	make -C $<

clean:
	rm -f $(TOOLS) $(obj).depend  \
		$(TOPDIR)/tools/logos/*.rle \
		$(TOPDIR)/tools/charge_logo/*.rle \
		$(TOPDIR)/board/$(BOARDDIR)/logo/*.rle \
		$(TOPDIR)/board/$(BOARDDIR)/charge_logo/*.rle \
		$(obj)*.o \
		$(obj)ddr_params_creator \
		$(obj)sfc_nand_params \
		$(obj)sfc_timing_params

distclean:	clean

else
LIB	= $(obj)lib_ddr_params_creator.o

COBJS	:= ddr_params_creator.o

SRCS	:= $(SOBJS:.o=.S) $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(SOBJS) $(COBJS))

$(LIB):	$(obj).depend $(OBJS) $(SOBJS)
	$(call cmd_link_o_target, $(OBJS))
endif

#########################################################################

#ifndef CONFIG_SPL_BUILD
#include $(obj)logo.mk
#endif
# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
