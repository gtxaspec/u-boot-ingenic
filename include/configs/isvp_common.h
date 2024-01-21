#ifndef __CONFIG_ISVP_COMMON_H__
#define __CONFIG_ISVP_COMMON_H__

#define CONFIG_MIPS32		/* MIPS32 CPU core */
#define CONFIG_CPU_XBURST
#define CONFIG_SYS_LITTLE_ENDIAN
/*#define CONFIG_DDR_AUTO_SELF_REFRESH*/
#define CONFIG_SPL_DDR_SOFT_TRAINING

#if defined(__CONFIG_ISVP_T10_H__)
#define CONFIG_T10		/* T10 SoC */
#define CONFIG_SOC "t10"
#elif defined(__CONFIG_ISVP_T20_H__)
#define CONFIG_T20		/* T20 SoC */
#define CONFIG_SOC "t20"
#elif defined(__CONFIG_ISVP_T21_H__)
#define CONFIG_T21		/* T21 SoC */
#define CONFIG_SOC "t21"
#elif defined(__CONFIG_ISVP_T30_H__)
#define CONFIG_T30		/* T30 SoC */
#define CONFIG_SOC "t30"
#else
#define CONFIG_T31		/* T31 SoC */
#define CONFIG_SOC "t31"
#endif

#define T10N "T10N"
#define T10L "T10L"

#define T20N "T20N"
#define T20L "T20L"
#define T20X "T20X"

#define T21N "T21N"

#define T30N "T30N"
#define T30L "T30L"
#define T30X "T30X"
#define T30A "T30A"
#define T30A1 "T30A1"

#define T31N "T31N"
#define T31L "T31L"
#define T31LC "T31LC"
#define T31X "T31X"
#define T31A "T31A"
#define T31AL "T31AL"


#ifdef CONFIG_T10
    #ifdef CONFIG_LITE_VERSION
        #define SOC_VAR T10L
    #else
        #define SOC_VAR T10N
    #endif
#endif

#ifdef CONFIG_T20
    #ifdef CONFIG_DDR2_128M
        #define SOC_VAR T20X
    #elif defined(CONFIG_LITE_VERSION)
        #define SOC_VAR T20L
    #else
        #define SOC_VAR T20N
    #endif
#endif

#ifdef CONFIG_T21
    #define SOC_VAR T21N
#endif

#ifdef CONFIG_T30
    #ifdef CONFIG_DDR2_128M
        #ifdef CONFIG_T30A
            #define SOC_VAR T30A
        #elif defined(CONFIG_T30A1)
            #define SOC_VAR T30A1
        #else
            #define SOC_VAR T30X
        #endif
    #elif defined(CONFIG_LITE_VERSION)
        #define SOC_VAR T30L
    #else
        #define SOC_VAR T30N
    #endif
#endif

#ifdef CONFIG_T31
    #ifdef CONFIG_DDR2_128M
        #ifdef CONFIG_T31A
            #define SOC_VAR T31A
        #elif defined(CONFIG_T31AL)
            #define SOC_VAR T31AL
        #else
            #define SOC_VAR T31X
        #endif
    #elif defined(CONFIG_LITE_VERSION)
        #ifdef CONFIG_T31LC
            #define SOC_VAR T31LC
        #else
            #define SOC_VAR T31L
        #endif
    #else
        #define SOC_VAR T31N
    #endif
#endif

#define CONFIG_U_BOOT_BUILD_NAME	"openipc_flex"
#define CONFIG_SFC_MIN_ALIGN		0x10000 /*0x1000->4K Erase, 0x8000->32K, 0x10000->64K*/

#undef CONFIG_SYS_PROMPT
#define CONFIG_SYS_PROMPT		"OpenIPC # "

#define CONFIG_AUTOBOOT_KEYED
#define CONFIG_AUTOBOOT_PROMPT		"Press Ctrl-c to abort autoboot... %d %d\n", bootdelay
#define CONFIG_AUTOBOOT_STOP_STR	"\x3"

#define CONFIG_SYS_LONGHELP
#define CONFIG_CMDLINE_EDITING
#define CONFIG_SYS_MAXARGS		64
#define CONFIG_SYS_NO_FLASH

#define CONFIG_SYS_CBSIZE		2048
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_MALLOC_LEN		(32 * 1024 * 1024)
#define CONFIG_SYS_BOOTPARAMS_LEN	(256 * 1024)
#define CONFIG_SYS_MONITOR_LEN		(230 * 1024)

#define CONFIG_SKIP_LOWLEVEL_INIT
#define CONFIG_CMDLINE_TAG

#define CONFIG_SYS_UART_INDEX		1
#define CONFIG_BAUDRATE			115200
#define CONFIG_SYS_UART_CONTROLLER_STEP	0x1000

#define CONFIG_BOOTDELAY		1
#define CONFIG_CMD_TFTPPUT		1
#define CONFIG_CMD_FAT			1
#define CONFIG_FAT_WRITE		1
#define CONFIG_CMD_FS_GENERIC		1
#define CONFIG_CMD_GPIO			1
#define	CONFIG_SYS_HUSH_PARSER		1
#define CONFIG_CMD_TFTPDOWNLOAD		1

#define CONFIG_NETMASK			255.255.255.0
#define CONFIG_GATEWAYIP		192.168.1.1
#define CONFIG_SERVERIP			192.168.1.254
#define CONFIG_IPADDR			192.168.1.10
#define CONFIG_ETHADDR			00:00:23:34:45:66

#define CONFIG_AUTO_UPDATE		1
#ifdef CONFIG_AUTO_UPDATE
#define CONFIG_CMD_SDUPDATE		1
#endif

#define CONFIG_CMD_SDSTART		1

/**
 * Environment
 */
#if defined(CONFIG_ENV_IS_IN_MMC)
#define CONFIG_SYS_MMC_ENV_DEV		0
#define CONFIG_ENV_SIZE			(32 << 10)
#define CONFIG_ENV_OFFSET		(CONFIG_SYS_MONITOR_LEN + CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR * 512)
#elif defined(CONFIG_ENV_IS_IN_SPI_FLASH)
#define CONFIG_ENV_OFFSET		0x40000
#define CONFIG_ENV_SIZE			0x10000
#define CONFIG_ENV_SECT_SIZE		0x10000
#else
#define CONFIG_ENV_IS_NOWHERE
#define CONFIG_ENV_SIZE			(32 << 10)
#define CONFIG_ENV_OFFSET		(CONFIG_SYS_NAND_BLOCK_SIZE * 5)
#endif

/**
 * Command configuration.
 */

#define CONFIG_AUTO_COMPLETE
#define CONFIG_CMDLINE_EDITING

#define CONFIG_CMD_BOOTD
#define CONFIG_CMD_CONSOLE
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_ECHO
#define CONFIG_CMD_EXT2
#define CONFIG_CMD_EXT4
#define CONFIG_CMD_GETTIME
#define CONFIG_CMD_LOADB
#define CONFIG_CMD_LOADS
#define CONFIG_CMD_MEMORY
#define CONFIG_CMD_MISC
#define CONFIG_CMD_MMC
#define CONFIG_CMD_NET
#define CONFIG_CMD_PING
#define CONFIG_CMD_RUN
#define CONFIG_CMD_SAVEENV
#define CONFIG_CMD_SOURCE
#define CONFIG_CMD_TFTPDOWNLOAD

/*
#define CONFIG_CMD_FAT
#define CONFIG_CMD_FLASH
#define CONFIG_CMD_FLOCK
#define CONFIG_CMD_I2C
#define CONFIG_CMD_JFFS2
#define CONFIG_CMD_MTDPARTS
#define CONFIG_CMD_SF
#define CONFIG_CMD_USB
#define CONFIG_CMD_WATCHDOG

#if defined(CONFIG_SPL_SFC_NAND) || defined(CONFIG_SFC_NAND_COMMAND)
#define CONFIG_CMD_MTDPARTS
#define CONFIG_CMD_NAND
#define CONFIG_CMD_SFCNAND
#define CONFIG_CMD_SFC_NAND
#define CONFIG_CMD_UBI
#define CONFIG_CMD_UBIFS
#endif
*/

#if defined(CONFIG_DDR2_128M) || defined(CONFIG_DDR3_128M)
#define CONFIG_EXTRA_SETTINGS \
"totalmem=128M\0" \
"osmem=64M\0" \
"rmem=64M@0x4000000\0"
#else
#define CONFIG_EXTRA_SETTINGS \
"totalmem=64M\0" \
"osmem=40M\0" \
"rmem=24M@0x2800000\0"
#endif

#if defined(CONFIG_SPL_MMC_SUPPORT)
#define CONFIG_BOOTCOMMAND \
"setenv setargs setenv bootargs ${bootargs};run setargs;" \
"mmc rescan;mmc read ${baseaddr} 0x1800 0x3000;" \
"bootm ${baseaddr};"
#elif defined(CONFIG_SFC_NOR)
#define CONFIG_BOOTCOMMAND \
"sf probe;setenv setargs setenv bootargs ${bootargs};run setargs;" \
"sf read ${baseaddr} 0x50000 \\${kern_len};" \
"bootm ${baseaddr};"
#elif defined(CONFIG_SFC_NAND)
#define CONFIG_BOOTCOMMAND \
"sf probe;setenv setargs setenv bootargs ${bootargs};run setargs;" \
"sfcnand read 0x50000 0x200000 ${baseaddr};" \
"bootm ${baseaddr};"
#else
#define CONFIG_BOOTCOMMAND \
"sf probe;setenv setargs setenv bootargs ${bootargs};run setargs;" \
"sf read ${baseaddr} 0x50000 0x250000;" \
"bootm ${baseaddr};"
#endif

#define CONFIG_BOOTARGS \
"mem=\\${osmem} rmem=\\${rmem} console=\\${serialport},\\${baudrate}n8" \
" panic=\\${panic_timeout} root=/dev/mtdblock3 rootfstype=squashfs init=/init" \
" mtdparts=jz_sfc:256k(boot),64k(env),\\${kern_size}(kernel),\\${rootfs_size}(rootfs),-(rootfs_data)"

#define CONFIG_EXTRA_ENV_SETTINGS \
"baseaddr=0x80600000\0" \
"panic_timeout=10\0" \
"serialport=ttyS1\0" \
"restore=n\0" \
"soc="CONFIG_SOC"\0" \
CONFIG_EXTRA_SETTINGS \
CONFIG_GPIO_SETTINGS \
CONFIG_GPIO_IRCUT_SETTINGS

/* IRCUT Default GPIOs */

#define CONFIG_GPIO_IRCUT_SETTINGS \
"gpio_default_ircut=25I 26I 52I 53I 49I 50I\0"

#endif /*__CONFIG_ISVP_COMMON__*/
