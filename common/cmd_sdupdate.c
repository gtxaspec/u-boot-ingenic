/*
 *SD update support
 */

#include <common.h>
#include <environment.h>
#include <command.h>
#include <malloc.h>
#include <image.h>
#include <asm/byteorder.h>
#include <asm/io.h>
#include <spi_flash.h>
#include <linux/mtd/mtd.h>
#include <fat.h>

#ifdef CONFIG_AUTO_UPDATE  /* cover the whole file */

#ifdef CONFIG_AUTO_SD_UPDATE
#ifndef CONFIG_MMC
#error "should have defined CONFIG_MMC"
#endif
#include <mmc.h>
#endif

#undef AU_DEBUG
#undef debug
/*#define	AU_DEBUG*/
#ifdef	AU_DEBUG
#define debug(fmt, args...)	printf(fmt, ##args)
#else
#define debug(fmt, args...)
#endif	/* AU_DEBUG */

/* possible names of files on the medium. */
#define AU_UBOOT	"autoupdate-uboot.bin"
#define AU_KERNEL	"autoupdate-kernel.bin"
#define AU_ROOTFS	"autoupdate-rootfs.bin"
#define AU_FW		"autoupdate-full.bin"

struct flash_layout {
	long start;
	long end;
};
static struct spi_flash *flash;

struct medium_interface {
	char name[20];
	int (*init) (void);
	void (*exit) (void);
};

/* layout of the FLASH. ST = start address, ND = end address. */
#define AU_FL_UBOOT_ST		0x0
#define AU_FL_UBOOT_ND		0x40000
#define AU_FL_KERNEL_ST		0x50000
#define AU_FL_KERNEL_ND		0x350000
#define AU_FL_ROOTFS_ST		0x350000
#define AU_FL_ROOTFS_ND		0xd50000
#define AU_FL_FW_ST		0x000000
#define AU_FL_FW_ND		0x1000000

static int au_stor_curr_dev; /* current device */

/* index of each file in the following arrays */
#define IDX_UBOOT	0
#define IDX_KERNEL	1
#define IDX_ROOTFS	2
#define IDX_FW		3

/* max. number of files which could interest us */
#define AU_MAXFILES	4

/* pointers to file names */
char *aufile[AU_MAXFILES] = {
	AU_UBOOT,
	AU_KERNEL,
	AU_ROOTFS,
	AU_FW
};

/* sizes of flash areas for each file */
long ausize[AU_MAXFILES] = {
	AU_FL_UBOOT_ND - AU_FL_UBOOT_ST,
	AU_FL_KERNEL_ND - AU_FL_KERNEL_ST,
	AU_FL_ROOTFS_ND - AU_FL_ROOTFS_ST,
	AU_FL_FW_ND - AU_FL_FW_ST
};

/* array of flash areas start and end addresses */
struct flash_layout aufl_layout[AU_MAXFILES] = {
	{ AU_FL_UBOOT_ST,	AU_FL_UBOOT_ND, },
	{ AU_FL_KERNEL_ST,	AU_FL_KERNEL_ND,   },
	{ AU_FL_ROOTFS_ST,	AU_FL_ROOTFS_ND,   },
	{ AU_FL_FW_ST,	AU_FL_FW_ND,   },
};

/* where to load files into memory */
#define LOAD_ADDR ((unsigned char *)0x80600000)

/* the app is the largest image */
#define MAX_LOADSZ ausize[IDX_FW]

int LOAD_ID = -1; //default update all

static int au_check_cksum_valid(int idx, long nbytes)
{
	image_header_t *hdr;
	unsigned long checksum;

	hdr = (image_header_t *)LOAD_ADDR;

	if (nbytes != (sizeof(*hdr) + ntohl(hdr->ih_size))) {
		printf("Image %s bad total SIZE\n", aufile[idx]);
		return -1;
	}
	/* check the data CRC */
	checksum = ntohl(hdr->ih_dcrc);

	if (crc32(0, (unsigned char const *)(LOAD_ADDR + sizeof(*hdr)),
			ntohl(hdr->ih_size)) != checksum) {
		printf("Image %s bad data checksum\n", aufile[idx]);
		return -1;
	}

	return 0;
}

static int au_check_header_valid(int idx, long nbytes)
{
	image_header_t *hdr;
	unsigned long checksum;

	char env[20];
	char auversion[20];

	hdr = (image_header_t *)LOAD_ADDR;
	/* check the easy ones first */
#if 0
	#define CHECK_VALID_DEBUG
#else
	#undef CHECK_VALID_DEBUG
#endif

#ifdef CHECK_VALID_DEBUG
	printf("\nmagic %#x %#x\n", ntohl(hdr->ih_magic), IH_MAGIC);
	printf("arch %#x %#x\n", hdr->ih_arch, IH_ARCH_MIPS);
	printf("size %#x %#lx\n", ntohl(hdr->ih_size), nbytes);
	printf("type %#x %#x\n", hdr->ih_type, IH_TYPE_FIRMWARE);
#endif
	if (nbytes < sizeof(*hdr)) {
		printf("Image %s bad header SIZE\n", aufile[idx]);
		return -1;
	}
	if (ntohl(hdr->ih_magic) != IH_MAGIC || hdr->ih_arch != IH_ARCH_MIPS) {
		printf("Image %s bad MAGIC or ARCH\n", aufile[idx]);
		return -1;
	}
	/* check the hdr CRC */
	checksum = ntohl(hdr->ih_hcrc);
	hdr->ih_hcrc = 0;

	if (crc32(0, (unsigned char const *)hdr, sizeof(*hdr)) != checksum) {
		printf("Image %s bad header checksum\n", aufile[idx]);
		return -1;
	}
	hdr->ih_hcrc = htonl(checksum);
	/* check the type - could do this all in one gigantic if() */
	if ((idx == IDX_UBOOT) && (hdr->ih_type != IH_TYPE_FIRMWARE)) {
		printf("Image %s wrong type\n", aufile[idx]);
		return -1;
	}
	if ((idx == IDX_KERNEL) && (hdr->ih_type != IH_TYPE_KERNEL)) {
		printf("Image %s wrong type\n", aufile[idx]);
		return -1;
	}
	if ((idx == IDX_ROOTFS) &&
			(hdr->ih_type != IH_TYPE_RAMDISK) &&
			(hdr->ih_type != IH_TYPE_FILESYSTEM)) {
		printf("Image %s wrong type\n", aufile[idx]);
		ausize[idx] = 0;
		return -1;
	}

	if ((idx == IDX_FW) && (hdr->ih_type != IH_TYPE_FIRMWARE)) {
		printf("Image %s wrong type\n", aufile[idx]);
		return -1;
	}
	/* recycle checksum */
	checksum = ntohl(hdr->ih_size);
	/* for kernel and app the image header must also fit into flash */
	if ((idx == IDX_KERNEL) && (idx == IH_TYPE_RAMDISK))
		checksum += sizeof(*hdr);

	/* check the size does not exceed space in flash. HUSH scripts */
	/* all have ausize[] set to 0 */
	if ((ausize[idx] != 0) && (ausize[idx] < checksum)) {
		printf("Image %s is bigger than FLASH\n", aufile[idx]);
		return -1;
	}

	sprintf(env, "%lx", (unsigned long)ntohl(hdr->ih_time));
	/*setenv(auversion, env);*/

	return 0;
}

static int au_do_update(int idx, long sz)
{
	image_header_t *hdr;
	unsigned long start, len;
	unsigned long write_len;
	int rc;
	void *buf;
	char *pbuf;

	hdr = (image_header_t *)LOAD_ADDR;

	// For full image, use the size of the loaded image directly
	if (idx == IDX_FW) {
		start = AU_FL_FW_ST; // Adjust start address if needed
		len = sz; // Use the actual size of the loaded image
	} else {
		start = ntohl(hdr->ih_load);
		len = ntohl(hdr->ih_ep) - ntohl(hdr->ih_load);
	}

	flash = spi_flash_probe(0, 0, 1000000, 0x3);
	if (!flash) {
		printf("Failed to initialize SPI flash\n");
		return -1;
	}
	
	/* erase the address range. */
	printf("flash erase...\n");
	rc = flash->erase(flash, start, len);
	if (rc) {
		printf("SPI flash sector erase failed\n");
		return 1;
	}

	buf = map_physmem((unsigned long)LOAD_ADDR, len, MAP_WRBACK);
	if (!buf) {
		puts("Failed to map physical memory\n");
		return 1;
	}

	if (idx == IDX_FW) {
		pbuf = buf; // For full image, use the buffer directly
		write_len = len; // Use the actual size of the loaded image
	} else {
		// For other images, strip the header and use ih_size
		pbuf = (buf + sizeof(*hdr));
		write_len = ntohl(hdr->ih_size);
	}

	/* copy the data from RAM to FLASH */
	printf("flash write...\n");
	rc = flash->write(flash, start, write_len, pbuf);
	if (rc) {
		printf("SPI flash write failed, return %d\n", rc);
		return 1;
	}

	unmap_physmem(buf, len);

	return 0;
}

/*
 * If none of the update file(u-boot, kernel or rootfs) was found
 * in the medium, return -1;
 * If u-boot has been updated, return 1;
 * Others, return 0;
 */
static int update_to_flash(void)
{
	int i = 0;
	long sz;
	int res;
	int uboot_updated = 0;
	int image_found = 0;

	for (i = 0; i < AU_MAXFILES; i++) {
		if (LOAD_ID != -1) {
			i = LOAD_ID;
		}

		if (strcmp(aufile[i], AU_FW) == 0) {
			// Check if autoupdate-full.done exists
			if (file_fat_read("autoupdate-full.done", LOAD_ADDR, 1) >= 0) {
				printf("Flag file autoupdate-full.done exists, skipping %s\n", AU_FW);
				continue; // Skip this file
			}
		}

		sz = file_fat_read(aufile[i], LOAD_ADDR, sizeof(image_header_t));
		if (sz <= 0 || sz < sizeof(image_header_t)) {
			debug("%s not found\n", aufile[i]);
			continue;
		}

		image_found = 1;

		if (i != IDX_FW && au_check_header_valid(i, sz) < 0) {
			debug("%s header not valid\n", aufile[i]);
			continue;
		}

		sz = file_fat_read(aufile[i], LOAD_ADDR, (i != IDX_FW) ? MAX_LOADSZ : flash->size);
		if (sz <= 0) {
			debug("%s not found\n", aufile[i]);
			continue;
		}

		if (i != IDX_FW && au_check_cksum_valid(i, sz) < 0) {
			debug("%s checksum not valid\n", aufile[i]);
			continue;
		}

		if (i == IDX_UBOOT) {
			uboot_updated = 1;
		}

		res = au_do_update(i, sz);
		if (res != 0) {
			return res;
		}

		if (res == 0 && strcmp(aufile[i], AU_FW) == 0) {
			// Write the autoupdate-full.done file after successful flash
			char empty_flag[1] = {0};
			if (file_fat_write("autoupdate-full.done", empty_flag, sizeof(empty_flag)) < 0) {
				printf("Error creating flag file autoupdate-full.done\n");
			} else {
				printf("Flag file autoupdate-full.done created\n");
				printf("Auto-update completed successfully. Saving environment...\n");
				run_command("saveenv", 0);
				printf("Environment saved.\n");
			}
		}

		if (LOAD_ID != -1) {
			break; // Update only the specified file
		}
	}

	return image_found ? (uboot_updated ? 1 : 0) : -1;
}

/*
 * This is called by board_init() after the hardware has been set up
 * and is usable. Only if SPI flash initialization failed will this function
 * return -1, otherwise it will return 0;
 */
int do_auto_update(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	block_dev_desc_t *stor_dev;
	int old_ctrlc;
	int state = -1;
	long start = -1, end = 0;

	printf("MMC:   Checking for autoupdate files... \n");

	if (argc == 1) {
		// Default behavior
	} else if (argc == 2) {
		LOAD_ID = simple_strtoul(argv[1], NULL, 16);
		if (LOAD_ID < IDX_UBOOT || LOAD_ID > AU_MAXFILES) {
			printf("Unsupported ID!\n");
			return CMD_RET_USAGE;
		}
	} else if (argc == 4) {
		LOAD_ID = simple_strtoul(argv[1], NULL, 16);
		if (LOAD_ID < IDX_UBOOT || LOAD_ID > AU_MAXFILES) {
			printf("Unsupported ID!\n");
			return CMD_RET_USAGE;
		}

		start = simple_strtoul(argv[2], NULL, 16);
		end = simple_strtoul(argv[3], NULL, 16);
		if (start >= 0 && end && end > start) {
			ausize[LOAD_ID] = end - start;
			aufl_layout[LOAD_ID].start = start;
			aufl_layout[LOAD_ID].end = end;
		} else {
			printf("Wrong address, use default\n");
		}
	} else {
		return CMD_RET_USAGE;
	}

	debug("Device name: mmc\n");
	stor_dev = get_dev("mmc", 0);
	if (!stor_dev) {
		debug("Unknown device type!\n");
		return 0;
	}

	if (fat_register_device(stor_dev, 1) != 0) {
		debug("Unable to use mmc 1:1 for fatls\n");
		return -1;
	}

/* As with cmd_sdstart.c, lets disable this for now, if we are running, assume the FS is valid
	if (file_fat_detectfs() != 0) {
		debug("file_fat_detectfs failed\n");
		return -1;
	}
*/
	/*
	 * make sure that we see CTRL-C
	 * and save the old state
	 */
	old_ctrlc = disable_ctrlc(0);

	state = update_to_flash();

	/* restore the old state */
	disable_ctrlc(old_ctrlc);

	LOAD_ID = -1;

	if (state == 1) {
		saveenv();
	}

	return (state == -1) ? CMD_RET_FAILURE : CMD_RET_SUCCESS;
}

U_BOOT_CMD(
	sdupdate,	9,	1,	do_auto_update,
	"auto upgrade file from mmc to flash",
	"LOAD_ID ADDR_START ADDR_END\n"
	"LOAD_ID: 0-->u-boot\n"
	"	 1-->kernel\n"
	"	 2-->rootfs\n"
	"	 3-->full\n"
	"ex:\n"
	"	sdupdate   (update all)\n"
	"or \n"
	"	sdupdate 0 0x0 0x40000"
);
#endif /* CONFIG_AUTO_UPDATE */
