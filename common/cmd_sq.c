#include <common.h>
#include <div64.h>
#include <malloc.h>
#include <spi_flash.h>
#include <asm/io.h>

extern struct spi_flash *get_flash(void);

#define SEARCH_SECTOR_SIZE          0x1000
#define KERNEL_MAGIC_NUMBER         0x56190527
#define KERNEL_MAGIC_OFFSET         0
#define SQUASHFS_BYTES_USED_OFFSET  40
#define SQUASHFS_MAGIC              0x73717368
#define SQUASHFS_MAGIC_OFFSET       0

// Function to align using the erase block size
static uint64_t align_to_erase_block(uint64_t size, uint64_t erase_block_size) {
	if (size % erase_block_size == 0) {
		return size; // Already aligned
	}
	return ((size / erase_block_size) + 1) * erase_block_size;
}

// Function to find the start of the kernel
static uint64_t find_kernel_start(struct spi_flash *flash, unsigned int start_addr, unsigned int end_addr) {
	uint32_t magic_number;
	char buf[256];  // Buffer size as needed based on expected header size
	unsigned int addr;

	for (addr = start_addr; addr < end_addr; addr += SEARCH_SECTOR_SIZE) {
		if (spi_flash_read(flash, addr, sizeof(buf), buf)) {
			printf("SQ:    Failed to read from SPI flash at address 0x%X\n", addr);
			continue;  // Skip to the next block
		}

		memcpy(&magic_number, buf + KERNEL_MAGIC_OFFSET, sizeof(magic_number));
		if (magic_number == KERNEL_MAGIC_NUMBER) {
			printf("SQ:    Kernel start detected at address 0x%X\n", addr);
			return addr;  // Return the address where the kernel is found
		}
	}

	return 0; // Kernel not found
}

// Function to find the start of the SquashFS
static uint64_t find_squashfs_start(struct spi_flash *flash, uint64_t start_search_addr) {
	uint32_t magic_number;
	char buf[4];  // Buffer to read potential magic numbers
	uint64_t addr;

	for (addr = start_search_addr; addr < flash->size; addr += SEARCH_SECTOR_SIZE) {
		if (spi_flash_read(flash, addr, sizeof(buf), buf) != 0) {
			printf("SQ:    Failed to read from SPI flash at address 0x%llX\n", addr);
			continue;
		}

		memcpy(&magic_number, buf, sizeof(magic_number));
		if (magic_number == SQUASHFS_MAGIC) {
			printf("SQ:    SquashFS start detected at address 0x%llX\n", addr);
			return addr;
		}
	}

	return 0; // SquashFS not found
}

// Function to update kernel environment variables
static int update_kernel_env(struct spi_flash *flash, uint64_t *kernel_start_addr, uint64_t *squashfs_start_addr) {
	uint64_t k_start = find_kernel_start(flash, 0x40000, 0x60000);
	if (k_start == 0) {
		printf("SQ:    Kernel not found in specified range.\n");
		return -1; // Indicate failure
	}

	*kernel_start_addr = k_start;  // Set the kernel start address

	uint64_t s_start = find_squashfs_start(flash, k_start + SEARCH_SECTOR_SIZE);
	*squashfs_start_addr = s_start;  // Set the SquashFS start address
	uint64_t kernel_size = s_start - k_start;
	// No alignment for the environment variables, use raw size
	char k_start_str[32];
	sprintf(k_start_str, "%llx", k_start);
	setenv("kern_addr", k_start_str);
	debug("SQ:    kern_addr env set to: %s\n", k_start_str);

	char kern_size_str[32];
	sprintf(kern_size_str, "%lluk", kernel_size / 1024); // Report raw size in kilobytes
	setenv("kern_size", kern_size_str);
	debug("SQ:    kern_size env set to: %llu kB\n", kernel_size / 1024);

	char kern_length_str[32];
	sprintf(kern_length_str, "%llx", kernel_size); // Report raw size in hex
	setenv("kern_len", kern_length_str);
	debug("SQ:    kernel_len env set to: %s\n", kern_length_str);
	return 0;  // Success
}

static uint64_t update_squashfs_env(struct spi_flash *flash, uint64_t squashfs_start_addr) {
	char buf[64];
	if (spi_flash_read(flash, squashfs_start_addr, sizeof(buf), buf)) {
		printf("SQ:    Failed to read from SPI flash at 0x%llX\n", squashfs_start_addr);
		return 0; // Handle error appropriately and return 0
	}

	// Extract bytes used from the SquashFS header
	uint32_t bytes_used_low, bytes_used_high;
	memcpy(&bytes_used_low, buf + SQUASHFS_BYTES_USED_OFFSET, sizeof(uint32_t));
	memcpy(&bytes_used_high, buf + SQUASHFS_BYTES_USED_OFFSET + sizeof(uint32_t), sizeof(uint32_t));
	uint64_t bytes_used = ((uint64_t)bytes_used_high << 32) | bytes_used_low;
	// Align the size using the erase block size
	uint64_t aligned_bytes_used = align_to_erase_block(bytes_used, flash->sector_size);

	// Set the rootfs size environment variable using the aligned size
	char size_str[32];
	sprintf(size_str, "%lluk", aligned_bytes_used / 1024);
	setenv("rootfs_size", size_str);
	debug("SQ:    rootfs size env set to: %llu kB\n", aligned_bytes_used / 1024);

	// Return the aligned size for further calculations
	return aligned_bytes_used;
}

// Function to update MTD partition information
static void update_mtdparts_env(struct spi_flash *flash, uint64_t kernel_start_addr) {
	uint64_t flashsize = flash->size;
	uint64_t upgrade_offset = kernel_start_addr;
	char flashlen_str[32], flashsize_str[32], upgrade_offset_str[32], upgrade_size_str[32];

	sprintf(flashlen_str, "%llx", flashsize);
	setenv("flash_len", flashlen_str);
	sprintf(flashsize_str, "%lluk", flashsize / 1024); // Convert to kilobytes
	setenv("flash_size", flashsize_str);
	debug("SQ:    flash_size env to to %s\n", flashsize_str);

	uint64_t upgrade_size = flashsize - upgrade_offset;
	// Convert the flash size to kilobytes and format
	sprintf(upgrade_offset_str, "0x%llX", upgrade_offset);
	sprintf(upgrade_size_str, "%lluk", upgrade_size / 1024);

	const char *enable_update_str = getenv("enable_updates");
	if (enable_update_str != NULL && strcmp(enable_update_str, "true") == 0) {
		char update_str[256];
		if (enable_update_str != NULL && strcmp(enable_update_str, "true") == 0) {
			sprintf(update_str, ",%s@%s(upgrade),%s@0(all)", upgrade_size_str, upgrade_offset_str, flashsize_str);
		} else {
			strcpy(update_str, "");
		}
		setenv("update", update_str);
		debug("SQ:    Update ENV updated with: %s\n", update_str);
	}
}

// Function to update overlay partition
static void update_overlay_env(uint64_t overlay_addr) {
	char overlay_str[32];
	sprintf(overlay_str, "%llX", overlay_addr);
	setenv("overlay", overlay_str);
	printf("SQ:    Overlay start set to address 0x%s\n", overlay_str);
}

// Function to process SPI flash data and update environment variables
int process_spi_flash_data(struct spi_flash *flash) {
	debug("SQ:    Starting process_spi_flash_data\n");
	debug("SQ:    Erase sector size: 0x%X bytes\n", flash->sector_size);

	uint64_t kernel_start_addr = 0, squashfs_start_addr = 0;
	if (update_kernel_env(flash, &kernel_start_addr, &squashfs_start_addr) != 0) {
		printf("SQ:    Failed to find kernel or SquashFS start.\n");
		return 1;  // Indicate error
	}

	// Call update_squashfs_env and get the aligned size returned
	uint64_t aligned_bytes_used = update_squashfs_env(flash, squashfs_start_addr);
	if (aligned_bytes_used == 0) {
		printf("SQ:    Failed to calculate aligned SquashFS size.\n");
		return 1;  // Handle error if SquashFS size calculation fails
	}

	update_mtdparts_env(flash, kernel_start_addr);  // Use the actual kernel start address

	// Calculate the overlay start address without over-aligning
	uint64_t overlay_start_addr = squashfs_start_addr + aligned_bytes_used;
	// Correct alignment: ensure you are not adding extra alignment
	overlay_start_addr = overlay_start_addr & ~(flash->sector_size - 1); // Align overlay start
	update_overlay_env(overlay_start_addr);

	return 0;  // Success
}

static int do_sq(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]) {
	if (argc != 2 || strcmp(argv[1], "probe") != 0) {
		puts("Usage: sq probe\n");
		return CMD_RET_USAGE;
	}

	struct spi_flash *flash = get_flash();
	if (!flash) {
		printf("SQ:    No SPI flash device available.\n");
		return CMD_RET_FAILURE;
	}

	if (process_spi_flash_data(flash) == 0) {
		printf("SQ:    SquashFS processing complete.\n");
	} else {
		printf("SQ:    SquashFS processing failed.\n");
	}

	return CMD_RET_SUCCESS;
}

U_BOOT_CMD(
	sq, 2, 1, do_sq,
	"Probe SquashFS data in SPI flash",
	"probe - Probe SquashFS data in SPI flash and update ENV"
);
