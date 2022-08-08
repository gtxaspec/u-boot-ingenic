/* linux/driver/video/jz_mipi_dsi/jz_mipi_dsi_regs.h
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#ifndef _JZ_MIPI_DSI_REGS_H
#define _JZ_MIPI_DSI_REGS_H

#define R_DSI_HOST_VERSION                0x000
#define R_DSI_HOST_PWR_UP                 0x004
#define R_DSI_HOST_CLKMGR_CFG             0x008
#define R_DSI_HOST_DPI_VCID               0x00c
#define R_DSI_HOST_DPI_COLOR_CODING       0x010
#define R_DSI_HOST_DPI_CFG_POL            0x014
#define R_DSI_HOST_DPI_LP_CMD_TIM         0x018
#define R_DSI_HOST_DBI_VCID               0x01c
#define R_DSI_HOST_DBI_CFG                0x020
#define R_DSI_HOST_DBI_PARTITIONING_EN    0x024
#define R_DSI_HOST_DBI_CMDSIZE            0x028
#define R_DSI_HOST_PCKHDL_CFG             0x02c
#define R_DSI_HOST_GEN_VCID               0x030
#define R_DSI_HOST_MODE_CFG               0x034
#define R_DSI_HOST_VID_MODE_CFG           0x038
#define R_DSI_HOST_VID_PKT_SIZE           0x03c
#define R_DSI_HOST_VID_NUM_CHUNKS         0x040
#define R_DSI_HOST_VID_NULL_SIZE          0x044
#define R_DSI_HOST_VID_HSA_TIME           0x048
#define R_DSI_HOST_VID_HBP_TIME           0x04c
#define R_DSI_HOST_VID_HLINE_TIME         0x050
#define R_DSI_HOST_VID_VSA_LINES          0x054
#define R_DSI_HOST_VID_VBP_LINES          0x058
#define R_DSI_HOST_VID_VFP_LINES          0x05c
#define R_DSI_HOST_VID_VACTIVE_LINES      0x060
#define R_DSI_HOST_EDPI_CMD_SIZE          0x064
#define R_DSI_HOST_CMD_MODE_CFG           0x068
#define R_DSI_HOST_GEN_HDR                0x06c
#define R_DSI_HOST_GEN_PLD_DATA           0x070
#define R_DSI_HOST_CMD_PKT_STATUS         0x074
#define R_DSI_HOST_TO_CNT_CFG             0x078
#define R_DSI_HOST_HS_RD_TO_CNT           0x07c
#define R_DSI_HOST_LP_RD_TO_CNT           0x080
#define R_DSI_HOST_HS_WR_TO_CNT           0x084
#define R_DSI_HOST_LP_WR_TO_CNT           0x088
#define R_DSI_HOST_BTA_TO_CNT             0x08c
#define R_DSI_HOST_SDF_3D                 0x090
#define R_DSI_HOST_LPCLK_CTRL             0x094
#define R_DSI_HOST_PHY_TMR_LPCLK_CFG      0x098
#define R_DSI_HOST_PHY_TMR_CFG            0x09c
#define R_DSI_HOST_PHY_RSTZ               0x0a0
#define R_DSI_HOST_PHY_IF_CFG             0x0a4
#define R_DSI_HOST_PHY_ULPS_CTRL          0x0a8
#define R_DSI_HOST_PHY_TX_TRIGGERS        0x0ac
#define R_DSI_HOST_PHY_STATUS             0x0b0
#define R_DSI_HOST_PHY_TST_CTRL0          0x0b4
#define R_DSI_HOST_PHY_TST_CTRL1          0x0b8
#define R_DSI_HOST_INT_ST0                0x0bC
#define R_DSI_HOST_INT_ST1                0x0C0
#define R_DSI_HOST_INT_MSK0               0x0C4
#define R_DSI_HOST_INT_MSK1               0x0C8
#define R_DSI_HOST_INT_FORCE0             0x0D8
#define R_DSI_HOST_INT_FORCE1             0x0DC
#define R_DSI_HOST_VID_SHADOW_CTRL        0x100
#define R_DSI_HOST_DPI_VCID_ACT           0x10C
#define R_DSI_HOST_DPI_COLOR_CODING_ACT   0x110
#define R_DSI_HOST_DPI_LP_CMD_TIM_ACT     0x118
#define R_DSI_HOST_VID_MODE_CFG_ACT       0x138
#define R_DSI_HOST_VID_PKT_SIZE_ACT       0x13C
#define R_DSI_HOST_VID_NUM_CHUNKS_ACT     0x140
#define R_DSI_HOST_VID_NULL_SIZE_ACT      0x144
#define R_DSI_HOST_VID_HSA_TIME_ACT       0x148
#define R_DSI_HOST_VID_HBP_TIME_ACT       0x14C
#define R_DSI_HOST_VID_HLINE_TIME_ACT     0x150
#define R_DSI_HOST_VID_VSA_LINES_ACT      0x154
#define R_DSI_HOST_VID_VBP_LINES_ACT      0x158
#define R_DSI_HOST_VID_VFP_LINES_ACT      0x15C
#define R_DSI_HOST_VID_VACTIVE_LINES_ACT  0x160
#define R_DSI_HOST_SDF_3D_ACT             0x190
#endif /* _JZ_MIPI_DSI_REGS_H */
