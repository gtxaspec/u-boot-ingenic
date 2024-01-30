#include <common.h>
#include <div64.h>
#include <malloc.h>
#include <spi_flash.h>

#include <asm/io.h>

extern struct spi_flash *get_flash(void);

#define SQUASHFS_MAGIC_OFFSET       0
#define SQUASHFS_BYTES_USED_OFFSET  40
#define ERASE_BLOCK_SIZE            0x8000
#define KERNEL_START_ADDR           0x50000

static uint64_t align_to_erase_block(uint64_t size) {
	if (size % ERASE_BLOCK_SIZE == 0) {
		return size; // Already aligned
	}
	return ((size / ERASE_BLOCK_SIZE) + 1) * ERASE_BLOCK_SIZE;
}

int process_spi_flash_data(struct spi_flash *flash) {
	debug("Starting process_spi_flash_data\n");

	// Starting address is right after the kernel, adjust as necessary
	unsigned int start_addr	= 0x180000;
	unsigned int end_addr	= 0x360000; // Adjust as necessary
	unsigned int addr; // Declare outside the for loop for compatibility

	debug("Searching SquashFS from 0x%X to 0x%X\n", start_addr, end_addr);

	for (addr = start_addr; addr < end_addr; addr += ERASE_BLOCK_SIZE) {
		char buf[64];

		debug("Reading from address 0x%X\n", addr);
		if (spi_flash_read(flash, addr, sizeof(buf), buf)) {
			printf("SQ:    Failed to read from SPI flash at 0x%X\n", addr);
			continue; // Skip to the next block
		}

		uint32_t magic_number;
		memcpy(&magic_number, buf + SQUASHFS_MAGIC_OFFSET, sizeof(magic_number));

		debug("Magic number at 0x%X: 0x%08X\n", addr, magic_number);

		if (magic_number == 0x73717368) {
			printf("SQ:    SquashFS found at 0x%X\n", addr);

			// Calculate kernel size
			uint64_t kernel_size = addr - KERNEL_START_ADDR;
			uint64_t aligned_kernel_size = align_to_erase_block(kernel_size);

			// Store the size in kilobytes in 'kern_size'
			char kern_size_str[32];
			sprintf(kern_size_str, "%lluk", aligned_kernel_size / 1024); // Convert to kilobytes
			setenv("kern_size", kern_size_str);
			debug("kernel_size env updated");

			// Store the length in hexadecimal in 'kern_length'
			char kern_length_str[32];
			sprintf(kern_length_str, "%llx", aligned_kernel_size); // Format as hexadecimal
			setenv("kern_len", kern_length_str);
			debug("kernel_len env updated");

			// Extract and process SquashFS size
			uint32_t bytes_used_low, bytes_used_high;
			memcpy(&bytes_used_low, buf + SQUASHFS_BYTES_USED_OFFSET, sizeof(uint32_t));
			memcpy(&bytes_used_high, buf + SQUASHFS_BYTES_USED_OFFSET + sizeof(uint32_t), sizeof(uint32_t));
			uint64_t bytes_used = ((uint64_t)bytes_used_high << 32) | bytes_used_low;

			debug("Size at 0x%X: %llu bytes\n", addr, bytes_used);

			// Align and set SquashFS environment variables
			uint64_t aligned_bytes_used = align_to_erase_block(bytes_used);
			char size_str[32];
			sprintf(size_str, "%lluk", aligned_bytes_used / 1024);
			setenv("rootfs_size", size_str);
			debug("rootfs_size env updated");

			// Set rootsize based on actual file size in memory
			uint64_t file_size = getenv_ulong("filesize", 16, 0);
			if (file_size > 0) {
				uint64_t aligned_file_size = align_to_erase_block(file_size);
				sprintf(size_str, "%lluk", aligned_file_size / 1024);
				setenv("root_size", size_str);
				debug("root_size env updated");
			} else {
				sprintf(size_str, "%lluk", aligned_bytes_used / 1024);
				setenv("root_size", size_str);
				debug("root_size env updated");
			}

			// Calculate and set 'updatesize', which is from KERNEL_START_ADDR to the end of the chip
			uint64_t updatesize = flash->size - KERNEL_START_ADDR;
			char updatesize_str[32];
			sprintf(updatesize_str, "%lluk", updatesize / 1024); // Convert to kilobytes
			setenv("updatesize", updatesize_str);
			debug("updatesize env updated: %s\n", updatesize_str);

			// Set 'flashsize' based on the size of the flash chip
			uint64_t flashsize = flash->size;
			char flashsize_str[32];
			sprintf(flashsize_str, "%lluk", flashsize / 1024); // Convert to kilobytes
			setenv("flashsize", flashsize_str);
			debug("flashsize env updated: %s\n", flashsize_str);

			// Conditionally enable the offset arguments in the mtdparts string to allow compatability with unpatched kernels
			const char *enable_update_str = getenv("enable_updates");

			// Buffer for the update string
			char update_str[256];

			if (enable_update_str != NULL && strcmp(enable_update_str, "true") == 0) {
				sprintf(update_str, ",%s@0x50000(upgrade),%s@0(all)", updatesize_str, flashsize_str);
			} else {
				strcpy(update_str, "");
			}

			setenv("update", update_str);

			return 0; // Success
		}
	}

	printf("SQ:    SquashFS not found.\n");
	return 1; // SquashFS not found
}

static int do_sq(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]) {
	// Check for correct number of arguments and the 'probe' argument
	if (argc != 2 || strcmp(argv[1], "probe") != 0) {
		puts("Usage: sq probe\n");
		return CMD_RET_USAGE;
	}

	struct spi_flash *flash = get_flash();
	if (!flash) {
		puts("No SPI flash device available.\n");
		return CMD_RET_FAILURE;
	}

	// Execute the function only if 'probe' argument is provided
	if (process_spi_flash_data(flash) == 0) {
		debug("SquashFS processing complete.\n");
	} else {
		puts("SquashFS processing failed.\n");
	}

	return CMD_RET_SUCCESS;
}

U_BOOT_CMD(
	sq, 2, 1, do_sq,
	"Probe SquashFS data in SPI flash",
	"probe - Probe SquashFS data in SPI flash and update ENV"
);
