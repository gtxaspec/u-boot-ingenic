#include <common.h>
#include <div64.h>
#include <malloc.h>
#include <spi_flash.h>
#include <asm/io.h>

extern struct spi_flash *get_flash(void);

#define ERASE_BLOCK_SIZE            0x8000
#define KERNEL_MAGIC_NUMBER         0x56190527
#define KERNEL_MAGIC_OFFSET         0
#define SQUASHFS_BYTES_USED_OFFSET  40
#define SQUASHFS_MAGIC              0x73717368
#define SQUASHFS_MAGIC_OFFSET       0

// Declare function prototypes
static uint64_t find_kernel_start(struct spi_flash *flash, unsigned int start_addr, unsigned int end_addr);
static uint64_t align_to_erase_block(uint64_t size);
static uint64_t find_squashfs_start(struct spi_flash *flash, uint64_t start_search_addr);

// Function definitions
static uint64_t align_to_erase_block(uint64_t size) {
	if (size % ERASE_BLOCK_SIZE == 0) {
		return size; // Already aligned
	}
	return ((size / ERASE_BLOCK_SIZE) + 1) * ERASE_BLOCK_SIZE;
}

static uint64_t find_kernel_start(struct spi_flash *flash, unsigned int start_addr, unsigned int end_addr) {
	uint32_t magic_number;
	char buf[256];  // Buffer size as needed based on expected header size
	unsigned int addr;

	for (addr = start_addr; addr < end_addr; addr += ERASE_BLOCK_SIZE) {
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

	return 0;  // Return 0 if not found
}

static uint64_t find_squashfs_start(struct spi_flash *flash, uint64_t start_search_addr) {
	uint32_t magic_number;
	char buf[4];  // Buffer to read potential magic numbers
	uint64_t addr;

	for (addr = start_search_addr; ; addr += ERASE_BLOCK_SIZE) {
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

	return start_search_addr; // Return the starting search address if no SquashFS is found
}

static int update_kernel_env(struct spi_flash *flash, uint64_t *kernel_start_addr, uint64_t *squashfs_start_addr) {
	uint64_t k_start = find_kernel_start(flash, 0x40000, 0x60000);
	if (k_start == 0) {
		printf("SQ:    Kernel not found in specified range.\n");
		return -1; // Indicate failure
	}

	*kernel_start_addr = k_start;  // Set the kernel start address

	uint64_t s_start = find_squashfs_start(flash, k_start + ERASE_BLOCK_SIZE);
	*squashfs_start_addr = s_start;  // Set the SquashFS start address
	uint64_t kernel_size = s_start - k_start;
	uint64_t aligned_kernel_size = align_to_erase_block(kernel_size);

	char k_start_str[32];
	sprintf(k_start_str, "%llx", k_start);
	setenv("kern_addr", k_start_str);
	debug("SQ:    kern_addr env updated\n");

	char kern_size_str[32];
	sprintf(kern_size_str, "%lluk", aligned_kernel_size / 1024);
	setenv("kern_size", kern_size_str);
	debug("SQ:    kernel_size env updated\n");

	char kern_length_str[32];
	sprintf(kern_length_str, "%llx", aligned_kernel_size);
	setenv("kern_len", kern_length_str);
	debug("SQ:    kernel_len env updated\n");

	return 0;  // Success
}

static uint64_t update_squashfs_env(struct spi_flash *flash, uint64_t squashfs_start_addr) {
	char buf[64];
	if (spi_flash_read(flash, squashfs_start_addr, sizeof(buf), buf)) {
		printf("SQ:    Failed to read from SPI flash at 0x%llX\n", squashfs_start_addr);
		return; // Handle error appropriately
	}

	uint32_t bytes_used_low, bytes_used_high;
	memcpy(&bytes_used_low, buf + SQUASHFS_BYTES_USED_OFFSET, sizeof(uint32_t));
	memcpy(&bytes_used_high, buf + SQUASHFS_BYTES_USED_OFFSET + sizeof(uint32_t), sizeof(uint32_t));
	uint64_t bytes_used = ((uint64_t)bytes_used_high << 32) | bytes_used_low;
	uint64_t aligned_bytes_used = align_to_erase_block(bytes_used);

	char size_str[32];
	sprintf(size_str, "%lluk", aligned_bytes_used / 1024);
	setenv("rootfs_size", size_str);
	debug("SQ:    rootfs_size env updated\n");

	return aligned_bytes_used;  // Return the aligned size
}

static void update_mtdparts_env(struct spi_flash *flash, uint64_t kernel_start_addr) {
	uint64_t flashsize = flash->size;
	uint64_t upgrade_offset = kernel_start_addr;

	char flashlen_str[32];
	sprintf(flashlen_str, "%llx", flashsize);
	setenv("flash_len", flashlen_str);
	char flashsize_str[32];
	sprintf(flashsize_str, "%lluk", flashsize / 1024); // Convert to kilobytes
	debug("SQ:    Flash size reported as %s\n", flashsize_str);
	setenv("flash_size", flashsize_str);
	debug("SQ:    flash_size env updated\n");

	// Calculate upgrade size from the upgrade_offset to the end of the flash
	uint64_t upgrade_size = flashsize - upgrade_offset;

	char upgrade_offset_str[32], upgrade_size_str[32];

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

static void update_overlay_env(uint64_t overlay_addr) {
	char overlay_str[32];
	sprintf(overlay_str, "%llX", overlay_addr);
	printf("SQ:    Overlay start detected at address 0x%s\n", overlay_str);
	setenv("overlay", overlay_str);
	debug("SQ:    Overlay env updated\n");
}

int process_spi_flash_data(struct spi_flash *flash) {
	debug("SQ:    Starting process_spi_flash_data\n");

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

	uint64_t overlay_start_addr = squashfs_start_addr + aligned_bytes_used;
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
