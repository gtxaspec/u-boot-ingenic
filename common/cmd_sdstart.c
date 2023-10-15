#include <common.h>
#include <command.h>
#include <fdtdec.h>
#include <malloc.h>
#include <menu.h>
#include <post.h>
#include <version.h>
#include <watchdog.h>
#include <linux/ctype.h>
#include <environment.h>
#include <image.h>
#include <asm/byteorder.h>
#include <asm/io.h>
#include <spi_flash.h>
#include <linux/mtd/mtd.h>
#include <fat.h>
#include <mmc.h>

#ifdef CONFIG_DDR2_128M
#define CONFIG_BOOTARGS2 "console=ttyS1,115200n8 mem=64M@0x0 rmem=64M@0x4000000 root=/dev/ram0 rw rdinit=/linuxrc"
#define CONFIG_BOOTCMD2 "bootm 0x84000000"
#define LOAD_ADDR ((unsigned char *)0x84000000)
#else
#define CONFIG_BOOTARGS2 "console=ttyS1,115200n8 mem=48M@0x0 rmem=16M@0x3000000 root=/dev/ram0 rw rdinit=/linuxrc"
#define CONFIG_BOOTCMD2 "bootm 0x83000000"
#define LOAD_ADDR ((unsigned char *)0x83000000)
#endif

#define MAX_LOADSZ 0x500000
#define DEFAULT_DELAY 1 // Default delay is 1 second

long sz = 0;
char *aufiles = "factory_0P3N1PC_kernel";

// This function prompts the user to press Ctrl-C to interrupt the kernel loading.
// The prompt lasts for the specified delay (in seconds).
static void prompt_and_wait_for_interrupt(int delay) {
    printf("You can interrupt kernel loading by pressing Ctrl-C within the next %d second(s)...\n", delay);
    int start = get_timer(0);
    while (get_timer(start) < (delay * 1000)) {
        if (ctrlc()) {
            printf("Kernel loading interrupted by user.\n");
            return;
        }
        udelay(10000); // Check every 10ms
    }
}

// This function checks if the checksum of the loaded kernel is valid.
static int au_check_cksum_valid(long nbytes) {
    image_header_t *hdr;
    unsigned long checksum;

    hdr = (image_header_t *)LOAD_ADDR;
    if (nbytes != (sizeof(*hdr) + ntohl(hdr->ih_size))) {
        printf("Error: Invalid image size for %s.\n", aufiles);
        return -1;
    }
    checksum = ntohl(hdr->ih_dcrc);
    if (crc32(0, (unsigned char const *)(LOAD_ADDR + sizeof(*hdr)), ntohl(hdr->ih_size)) != checksum) {
        printf("Error: Invalid data checksum for %s.\n", aufiles);
        return -1;
    }
    return 0;
}

// This function checks if the header of the loaded kernel is valid.
static int au_check_header_valid(long nbytes) {
    image_header_t *hdr;
    unsigned long checksum;
    hdr = (image_header_t *)LOAD_ADDR;

    if (nbytes < sizeof(*hdr)) {
        printf("Error: Invalid header size for %s.\n", aufiles);
        return -1;
    }
    if (ntohl(hdr->ih_magic) != IH_MAGIC || hdr->ih_arch != IH_ARCH_MIPS) {
        printf("Error: Invalid MAGIC or ARCH for %s.\n", aufiles);
        return -1;
    }
    checksum = ntohl(hdr->ih_hcrc);
    hdr->ih_hcrc = 0;
    if (crc32(0, (unsigned char const *)hdr, sizeof(*hdr)) != checksum) {
        printf("Error: Invalid header checksum for %s.\n", aufiles);
        return -1;
    }
    if (hdr->ih_type != IH_TYPE_KERNEL) {
        printf("Error: Invalid kernel type for %s.\n", aufiles);
        return -1;
    }
    return 0;
}

// This function loads the kernel from flash memory to RAM.
static int update_to_flash(void) {
    // Load the kernel header
    sz = file_fat_read(aufiles, LOAD_ADDR, sizeof(image_header_t));
    if (sz <= 0) {
        printf("Error: File %s not found.\n", aufiles);
        return -1;
    }
    // Validate the loaded kernel header
    if (au_check_header_valid(sz) < 0) {
        printf("Error: Invalid header for %s.\n", aufiles);
        return -1;
    }
    // Load the entire kernel
    sz = file_fat_read(aufiles, LOAD_ADDR, MAX_LOADSZ);
    if (sz <= 0) {
        printf("Error: File %s not found.\n", aufiles);
        return -1;
    }
    // Validate the loaded kernel checksum
    if (au_check_cksum_valid(sz) < 0) {
        printf("Error: Invalid checksum for %s.\n", aufiles);
        return -1;
    }
    return 0;
}

// This function checks if the SD card is present and contains a valid FAT filesystem.
static int do_check_sd(void) {
    int ret = 0;
    block_dev_desc_t *stor_dev;

    stor_dev = get_dev("mmc", 0);
    if(NULL == stor_dev) {
        printf("Error: MMC card is not present.\n");
        return -1;
    }
    ret = fat_register_device(stor_dev, 1);
    if(ret != 0) {
        printf("Error: Failed to register FAT device.\n");
        return -1;
    }
    ret = file_fat_detectfs();
    if (ret != 0) {
        printf("Error: Failed to detect FAT filesystem.\n");
        return -1;
    }
    printf("FAT filesystem detected successfully.\n");
    return 0;
}

// This function sets the environment variables for booting the kernel.
static void setenv_sd_start(void) {
    char bootargs[512];
    const char* osmem = getenv("osmem");
    const char* rmem = getenv("rmem");
    const char* mtdparts = getenv("mtdparts");
    const char* extras = getenv("extras");

    snprintf(bootargs, sizeof(bootargs), "mem=%s rmem=%s console=ttyS1,115200n8 panic=20 root=/dev/mtdblock3 rootfstype=squashfs init=/init mtdparts=%s %s",
             osmem ? osmem : "default_value",
             rmem ? rmem : "default_value",
             mtdparts ? mtdparts : "default_value",
             extras ? extras : "");

    setenv("bootargs", bootargs);
    setenv("bootcmd", CONFIG_BOOTCMD2);
    run_command("boot", 0);
}

// This is the main function that gets executed when the "sdstart" command is run in U-Boot.
int do_sd_start(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[]) {
    int old_ctrlc = 0;
    int delay = DEFAULT_DELAY;

    // Check if the user has provided a custom delay value
    if (argc > 1) {
        delay = simple_strtol(argv[1], NULL, 10);
    }
    prompt_and_wait_for_interrupt(delay);
    if (ctrlc()) {
        printf("Operation was canceled during the prompt.\n");
        return 0;
    }
    if (do_check_sd() == 0) {
        old_ctrlc = disable_ctrlc(0);
        if(update_to_flash() == 0) {
            setenv_sd_start();
            return 0;
        }
        disable_ctrlc(old_ctrlc);
    }
    return 0;
}

U_BOOT_CMD(
    sdstart,    2,    0,    do_sd_start,
    "Load a kernel from the MMC card with an interruptible delay.",
    "[delay_in_seconds]"
);
