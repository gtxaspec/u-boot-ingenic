#
# Ingenic T30 configuration
#
# Copyright (c) 2017 Ingenic Semiconductor Co.,Ltd
# Author: Zoro <ykli@ingenic.cn>
# Based on: arch/mips/cpu/xburst/jz4780/config.mk
#           Written by Paul Burton <paul.burton@imgtec.com>
#
# SPDX-License-Identifier:	GPL-2.0+

# branch likely triggers a reserved instruction exception
PLATFORM_CPPFLAGS += -mno-branch-likely
