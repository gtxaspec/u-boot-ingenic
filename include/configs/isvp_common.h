#ifndef __CONFIG_ISVP_COMMON_H__
#define __CONFIG_ISVP_COMMON_H__

#ifdef __CONFIG_ISVP_T10_H__
#define CONFIG_SOC "t10"
#elif defined(__CONFIG_ISVP_T20_H__)
#define CONFIG_SOC "t20"
#elif defined(__CONFIG_ISVP_T21_H__)
#define CONFIG_SOC "t21"
#elif defined(__CONFIG_ISVP_T30_H__)
#define CONFIG_SOC "t30"
#else
#define CONFIG_SOC "t31"
#endif

#define CONFIG_SFC_MIN_ALIGN		0x10000 /*0x1000->4K Erase, 0x8000->32K, 0x10000->64K*/
#define CONFIG_ENV_OFFSET		0x40000
#define CONFIG_ENV_SIZE			0x10000
#define CONFIG_ENV_SECT_SIZE		0x10000

#undef CONFIG_SYS_PROMPT
#define CONFIG_SYS_PROMPT		"OpenIPC # "

#define CONFIG_AUTOBOOT_KEYED
#define CONFIG_AUTOBOOT_PROMPT		\
    "Press Ctrl-c to abort autoboot... %d \n", bootdelay
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

#define CONFIG_BOOTDELAY		1
#define CONFIG_CMD_TFTPPUT		1
#define CONFIG_CMD_FAT			1
#define CONFIG_CMD_FS_GENERIC		1
#define CONFIG_CMD_GPIO			1
#define	CONFIG_SYS_HUSH_PARSER		1

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

#define CONFIG_BOOTCOMMAND \
	"setenv setargs setenv bootargs ${bootargs};" \
	"run setargs;" \
	"sf probe 0;" \
	"sf read ${baseaddr} 0x50000 0x300000;" \
	"bootm ${baseaddr}"

#if (defined(CONFIG_DDR2_128M) || defined(CONFIG_DDR3_128M))
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

#ifdef CONFIG_SPL_MMC_SUPPORT
#define CONFIG_BOOTCOMMAND \
	"mmc rescan;" \
	"setenv setargs setenv bootargs ${bootargs};" \
	"run setargs;" \
	"mmc read ${baseaddr} 0x1800 0x3000;"\
	"bootm ${baseaddr}"
#endif

#ifdef CONFIG_SFC_NOR
#define CONFIG_BOOTCOMMAND \
	"sf probe;" \
	"setenv setargs setenv bootargs ${bootargs};" \
	"run setargs;" \
	"sf read ${baseaddr} 0x50000 \\${kern_len};" \
	"bootm ${baseaddr}"
#endif

#ifdef CONFIG_SFC_NAND
#define CONFIG_BOOTCOMMAND \
	"sf probe;" \
	"setenv setargs setenv bootargs ${bootargs};" \
	"run setargs;" \
	"sfcnand read 0x50000 0x200000 ${baseaddr};" \
	"bootm ${baseaddr}"
#endif

#define CONFIG_BOOTARGS \
	"mem=\${osmem} rmem=\${rmem} console=\${serialport},\${baudrate}n8" \
	" panic=\${panic_timeout} root=/dev/mtdblock3 rootfstype=squashfs" \
	" init=/init mtdparts=jz_sfc:256k(boot),64k(env)," \
	"\\${kern_size}(kernel),\\${rootfs_size}(rootfs),-(rootfs_data)"

#define CONFIG_EXTRA_ENV_SETTINGS \
    "baseaddr=0x80600000\0" \
    "panic_timeout=10\0" \
    "serialport=ttyS1\0" \
    "restore=n\0" \
    "soc="CONFIG_SOC"\0" \
    CONFIG_EXTRA_SETTINGS \
    CONFIG_GPIO_SETTINGS \
	CONFIG_GPIO_IRCUT_SETTINGS

	/*
	IRCUT Default GPIOs:


*/

#define CONFIG_GPIO_IRCUT_SETTINGS \
    "gpio_dev_ircut=25o 26o 52o 53o 49o 50o\0"

#endif /*__CONFIG_ISVP_COMMON__*/
