/*
 * Ingenic JZ SPI driver
 *
 * Copyright (c) 2013 Ingenic Semiconductor Co.,Ltd
 * Author: Tiger <lbh@ingenic.cn>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __JZ_SPI_H__
#define __JZ_SPI_H__
#ifdef CONFIG_JZ_SFC
#ifdef CONFIG_SFC_NAND
#include <asm/arch/sfc_nand.h>
#else
#include <asm/arch/sfc.h>
#endif
#endif /* CONFIG_JZ_SFC */
#define SPL_TYPE_FLAG_LEN	6
#define SSI_BASE SSI0_BASE
#define COMMAND_MAX_LENGTH	8
#define SIZEOF_NAME		32
#define FIFI_THRESHOLD		64
#define SPI_WRITE_CHECK_TIMES	50

#define SIZE_256B       256
#define SIZE_4KB       4096
#define SIZE_8KB       8192
#define SIZE_16KB     16384
#define SIZE_32KB     32768
#define SIZE_64KB     65536
#define SIZE_8MB    8388608
#define SIZE_16MB  16777216
#define SIZE_32MB  33554432
#define SIZE_64MB  67108864

struct spi_nor_block_info {
	u32 blocksize;
	u8 cmd_blockerase;
	/* MAX Busytime for block erase, unit: ms */
	u32 be_maxbusy;
};

struct spi_quad_mode {
	u8 dummy_byte;
	u8 RDSR_CMD;
	u8 WRSR_CMD;
	unsigned int RDSR_DATE;     /* the data is write the spi status register for QE bit */
	unsigned int RD_DATE_SIZE;  /* the data is write the spi status register for QE bit */
	unsigned int WRSR_DATE;     /* this bit should be the flash QUAD mode enable */
	unsigned int WD_DATE_SIZE;  /* the data is write the spi status register for QE bit */
	u8 cmd_read;
	u8 sfc_mode;
};

struct jz_spi_support {
	unsigned int id_manufactory;
	unsigned char id_device;
	char name[SIZEOF_NAME];
	int page_size;
	int oobsize;
	int sector_size;
	int block_size;
	int addr_size;
	int size;
	int page_num;
	unsigned int *page_list;
	unsigned short column_cmdaddr_bits;/* read from cache ,the bits of cmd + addr */
	struct spi_quad_mode quad_mode;
};

#ifdef CONFIG_JZ_SFC
struct norflash_params {
	char name[SIZEOF_NAME];
	u32 pagesize;
	u32 sectorsize;
	u32 chipsize;
	u32 erasesize;
	int id;
	/* Flash Address size, unit: Bytes */
	int addrsize;

	/* MAX Busytime for page program, unit: ms */
	u32 pp_maxbusy;
	/* MAX Busytime for sector erase, unit: ms */
	u32 se_maxbusy;
	/* MAX Busytime for chip erase, unit: ms */
	u32 ce_maxbusy;

	/* Flash status register num, Max support 3 register */
	int st_regnum;
	/* Some NOR flash has different blocksize and block erase command,
	 * One command with One blocksize. */
	struct spi_nor_block_info block_info;
	struct spi_quad_mode quad_mode;
};

struct nor_sharing_params {
	uint32_t magic;
	uint32_t version;
	struct norflash_params norflash_params;
	struct norflash_partitions norflash_partitions;
};
#endif

struct jz_spi_slave {
	struct spi_slave slave;
	unsigned int mode;
	unsigned int max_hz;
};

struct jz_spi_support_from_burner{
	unsigned int id_manufactory;
	unsigned char id_device;
	char name[32];
	int page_size;
	int oobsize;
	int sector_size;
	int block_size;
	int size;
	int page_num;
	uint32_t tRD_maxbusy;
	uint32_t tPROG_maxbusy;
	uint32_t tBERS_maxbusy;
	unsigned short column_cmdaddr_bits;
};

struct nand_param_from_burner {
	int version;
	int flash_type;
	int para_num;
	struct jz_spi_support_from_burner *addr;
	int partition_num;
	struct jz_spinand_partition *partition;
};

static struct jz_spi_support jz_spi_nand_support_table[] = {
	{
		.id_manufactory = 0xc8,
		.id_device = 0xf1,
		.name = "GD5F1GQ4U",
		.page_size = 2 * 1024,
		.block_size = 128 * 1024,
		.size = 128 * 1024 * 1024,
	},
	{
		.id_manufactory = 0xc8,
		.id_device = 0xf2,
		.name = "GD5F2GQ4U",
		.page_size = 2 * 1024,
		.block_size = 128 * 1024,
		.size = 256 * 1024 * 1024,
	},
	{
		.id_manufactory = 0xc8,
		.id_device = 0xf4,
		.name = "GD5F4GQ4U",
		.page_size = 2 * 1024,
		.block_size = 128 * 1024,
		.size = 512 * 1024 * 1024,
	}
};

static struct jz_spi_support jz_spi_support_table[] = {
	{
		.name = "XT25F64B",
		.id_manufactory = 0x0b4017,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 3,
		.size = SIZE_8MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR,
			.RDSR_DATE = 0x2, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x200, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 2,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "XT25F128B",
		.id_manufactory = 0x0b4018,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 3,
		.size = SIZE_16MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR,
			.RDSR_DATE = 0x2, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x200, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 2,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "XT25F256",
		.id_manufactory = 0x0b4019,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 4,
		.size = SIZE_32MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "GM25Q64A",
		.id_manufactory = 0x1c4017,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 3,
		.size = SIZE_8MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "GM25Q128A",
		.id_manufactory = 0x1c4018,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 3,
		.size = SIZE_16MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "EN25QH64",
		.id_manufactory = 0x1c7017,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 3,
		.size = SIZE_8MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "EN25QH128A",
		.id_manufactory = 0x1c7018,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 3,
		.size = SIZE_16MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "EN25QH256A",
		.id_manufactory = 0x1c7019,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 4,
		.size = SIZE_32MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR,
			.WRSR_CMD = CMD_WRSR,
			.RDSR_DATE = 0x40, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x40, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "EN25QX128A",
		.id_manufactory = 0x1c7118,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 3,
		.size = SIZE_16MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "XM25QH64C",
		.id_manufactory = 0x204017,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 3,
		.size = SIZE_8MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "XM25QH128C",
		.id_manufactory = 0x204018,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 3,
		.size = SIZE_16MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "XM25QH256C",
		.id_manufactory = 0x204019,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 4,
		.size = SIZE_32MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "XM25QH64B",
		.id_manufactory = 0x206017,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 3,
		.size = SIZE_8MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR,
			.WRSR_CMD = CMD_WRSR,
			.RDSR_DATE = 0x40, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x40, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "XM25QH128B",
		.id_manufactory = 0x206018,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 3,
		.size = SIZE_16MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR,
			.WRSR_CMD = CMD_WRSR,
			.RDSR_DATE = 0x40, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x40, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "XM25QH64A",
		.id_manufactory = 0x207017,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 3,
		.size = SIZE_8MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR,
			.WRSR_CMD = CMD_WRSR,
			.RDSR_DATE = 0x40, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x40, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "XM25QH128A",
		.id_manufactory = 0x207018,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 3,
		.size = SIZE_16MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR,
			.WRSR_CMD = CMD_WRSR,
			.RDSR_DATE = 0x40, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x40, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "SK25Q128",
		.id_manufactory = 0x256018,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 3,
		.size = SIZE_16MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR,
			.WRSR_CMD = CMD_WRSR,
			.RDSR_DATE = 0x40, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x40, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "NM25Q128EVB",
		.id_manufactory = 0x522118,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 3,
		.size = SIZE_16MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x4, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x4, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "NM25Q64EVB",
		.id_manufactory = 0x522217,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 3,
		.size = SIZE_8MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x4, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x4, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "DQ25Q128AL",
		.id_manufactory = 0x546018,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 3,
		.size = SIZE_16MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "ZB25VQ64",
		.id_manufactory = 0x5e4017,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 3,
		.size = SIZE_8MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "ZB25VQ128",
		.id_manufactory = 0x5e4018,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 3,
		.size = SIZE_16MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "BY25Q64AS",
		.id_manufactory = 0x684017,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 3,
		.size = SIZE_8MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "BY25Q128AS",
		.id_manufactory = 0x684018,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 3,
		.size = SIZE_16MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "PY25Q128HA",
		.id_manufactory = 0x852018,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 3,
		.size = SIZE_16MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "P25Q64H",
		.id_manufactory = 0x856017,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 3,
		.size = SIZE_8MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "P25Q128H",
		.id_manufactory = 0x856018,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 3,
		.size = SIZE_16MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "IS25LP064D",
		.id_manufactory = 0x9d6017,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 3,
		.size = SIZE_8MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR,
			.WRSR_CMD = CMD_WRSR,
			.RDSR_DATE = 0x40, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x40, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "IS25LP128F",
		.id_manufactory = 0x9d6018,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 3,
		.size = SIZE_16MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR,
			.WRSR_CMD = CMD_WRSR,
			.RDSR_DATE = 0x40, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x40, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "FM25W128",
		.id_manufactory = 0xa12818,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 3,
		.size = SIZE_16MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "FS25Q064F2TFI1",
		.id_manufactory = 0xa14017,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 3,
		.size = SIZE_8MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "FM25Q128A",
		.id_manufactory = 0xa14018,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 3,
		.size = SIZE_16MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "ZD25Q64B",
		.id_manufactory = 0xba3217,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 3,
		.size = SIZE_8MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "MX25L64**E",
		.id_manufactory = 0xc22017,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 3,
		.size = SIZE_8MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR,
			.WRSR_CMD = CMD_WRSR,
			.RDSR_DATE = 0x40, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x40, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "MX25L128**E",
		.id_manufactory = 0xc22018,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 3,
		.size = SIZE_16MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR,
			.WRSR_CMD = CMD_WRSR,
			.RDSR_DATE = 0x40, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x40, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "MX25L25645G/35F",
		.id_manufactory = 0xc22019,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 4,
		.size = SIZE_32MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR,
			.WRSR_CMD = CMD_WRSR,
			.RDSR_DATE = 0x40, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x40, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "MX25L51245G",
		.id_manufactory = 0xc2201A,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 4,
		.size = (64 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR,
			.WRSR_CMD = CMD_WRSR,
			.RDSR_DATE = 0x40, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x40, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "GD25Q64C",
		.id_manufactory = 0xc84017,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 3,
		.size = SIZE_8MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "GD25Q128",
		.id_manufactory = 0xc84018,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 3,
		.size = SIZE_16MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "GD25Q256D/E",
		.id_manufactory = 0xc84019,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 4,
		.size = SIZE_32MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "XD25Q128C",
		.id_manufactory = 0xd84018,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 3,
		.size = SIZE_16MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "W25Q64",
		.id_manufactory = 0xef4017,
		.page_size = SIZE_256B,
		.addr_size = 3,
		.sector_size = SIZE_4KB,
		.size = SIZE_8MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "W25Q128",
		.id_manufactory = 0xef4018,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 3,
		.size = SIZE_16MB,
		.quad_mode = {
			.dummy_byte = 6,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_IO_FAST_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_IO_QUAD,
#endif
		},
	},
	{
		.name = "W25Q256JV",
		.id_manufactory = 0xef4019,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 4,
		.size = SIZE_32MB,
		.quad_mode = {
			.dummy_byte = 6,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_IO_FAST_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_IO_QUAD,
#endif
		}
	},
	{
		.name = "W25Q64-QPI",
		.id_manufactory = 0xef6017,
		.page_size = SIZE_256B,
		.addr_size = 3,
		.sector_size = SIZE_4KB,
		.size = SIZE_8MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "W25Q128-QPI",
		.id_manufactory = 0xef6018,
		.page_size = SIZE_256B,
		.addr_size = 3,
		.sector_size = SIZE_4KB,
		.size = SIZE_16MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "FM25Q64",
		.id_manufactory = 0xf83217,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 3,
		.size = SIZE_8MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
	{
		.name = "GENERIC*8MB",
		.id_manufactory = 0xbabe17,
		.page_size = SIZE_256B,
		.sector_size = SIZE_4KB,
		.addr_size = 3,
		.size = SIZE_8MB,
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2, /* the data is write the spi status register for QE bit */
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2, /* this bit should be the flash QUAD mode enable */
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_SPI_QUAD,
#endif
		},
	},
};

#endif /* __JZ_SPI_H__ */
