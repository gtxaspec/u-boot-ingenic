#ifdef __CONFIG_ISVP_H__
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

#define CONFIG_BOOTCOMMAND "setenv setargs setenv bootargs ${bootargs}; run setargs; sf probe 0; sf read ${baseaddr} 0x50000 0x300000; bootm ${baseaddr}; reset"

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

/* USEFUL STUFF, BUT LATER...
#ifdef CONFIG_SPL_MMC_SUPPORT
	#define CONFIG_BOOTARGS "mem=\${osmem} rmem=\${rmem} init=/init root=/dev/mmcblk0p2 rootdelay=1"
#elif defined(CONFIG_SFC_NOR)
	#define CONFIG_BOOTARGS "mem=\${osmem} rmem=\${rmem} root=/dev/mtdblock3 rootfstype=squashfs init=/init mtdparts=jz_sfc:256k(boot),64k(env),2048k(kernel),5120k(rootfs),-(rootfs_data)"
#elif defined(CONFIG_SFC_NAND)
	#define CONFIG_BOOTARGS "mem=\${osmem} rmem=\${rmem} ip=off init=/init ubi.mtd=2 root=ubi0:rootfs ubi.mtd=3 rootfstype=ubifs rw"
#endif
*/

#ifdef CONFIG_SPL_MMC_SUPPORT
    #define CONFIG_BOOTCOMMAND "setenv setargs setenv bootargs ${bootargs}; run setargs; sf probe 0; mmc read ${baseaddr} 0x1800 0x3000; bootm ${baseaddr}"
#endif
#ifdef CONFIG_SFC_NOR
    #define CONFIG_BOOTCOMMAND "setenv setargs setenv bootargs ${bootargs}; run setargs; sf probe; sf read ${baseaddr} 0x50000 0x300000; bootm ${baseaddr}; reset"
#endif
#ifdef CONFIG_SFC_NAND
    #define CONFIG_BOOTCOMMAND "setenv setargs setenv bootargs ${bootargs}; run setargs; sf probe 0; sfcnand read 0x50000 0x200000 ${baseaddr} ; bootm ${baseaddr}; reset"
#endif

#define CONFIG_BOOTARGS "mem=\${osmem} rmem=\${rmem} console=ttyS1,115200n8 panic=20 root=/dev/mtdblock3 rootfstype=squashfs init=/init mtdparts=\${mtdparts} \${extras}"

#define CONFIG_EXTRA_ENV_SETTINGS \
    "baseaddr=0x80600000\0" \
    "uknor8m=mw.b ${baseaddr} ff 1000000; tftpboot ${baseaddr} uImage.${soc} ; sf probe 0; sf erase 0x50000 0x200000; sf write ${baseaddr} 0x50000 ${filesize}\0" \
    "uknor16m=mw.b ${baseaddr} ff 1000000; tftpboot ${baseaddr} uImage.${soc} ; sf probe 0; sf erase 0x50000 0x300000; sf write ${baseaddr} 0x50000 ${filesize}\0" \
    "urnor8m=mw.b ${baseaddr} ff 1000000; tftpboot ${baseaddr} rootfs.squashfs.${soc} ; sf probe 0; sf erase 0x250000 0x500000; sf write ${baseaddr} 0x250000 ${filesize}\0" \
    "urnor16m=mw.b ${baseaddr} ff 1000000; tftpboot ${baseaddr} rootfs.squashfs.${soc} ; sf probe 0; sf erase 0x350000 0xa00000; sf write ${baseaddr} 0x350000 ${filesize}\0" \
    "uknand=mw.b ${baseaddr} ff 1000000; tftpboot ${baseaddr} uImage.${soc} ; nand erase 0x100000 0x300000; nand write ${baseaddr} 0x100000 0x300000\0" \
    "urnand=mw.b ${baseaddr} ff 1000000; tftpboot ${baseaddr} rootfs.ubi.${soc} ; nand erase 0x400000 0x7c00000; nand write ${baseaddr} 0x400000 ${filesize}\0" \
    "mtdparts=jz_sfc:256k(boot),64k(env),2048k(kernel),5120k(rootfs),-(rootfs_data)\0" \
    "mtdpartsubi=setenv mtdparts jz_sfcnand:256k(boot),768k(wtf),3072k(kernel),-(ubi)\0" \
    "mtdpartsnand=setenv mtdparts jz_sfcnand:256k(boot),768k(wtf),3072k(kernel),10240k(rootfs),-(rootfs_data)\0" \
    "mtdpartsnor8m=setenv mtdparts jz_sfc:256k(boot),64k(env),2048k(kernel),5120k(rootfs),-(rootfs_data)\0" \
    "mtdpartsnor16m=setenv mtdparts jz_sfc:256k(boot),64k(env),3072k(kernel),10240k(rootfs),-(rootfs_data)\0" \
    "nfsroot=/srv/nfs/"CONFIG_SOC"\0" \
    "bootargsnfs=mem=\${osmem} console=ttyS1,115200n8 panic=20 root=/dev/nfs rootfstype=nfs ip=${ipaddr}:::255.255.255.0::eth0 nfsroot=${serverip}:${nfsroot},v3,nolock rw \${extras}\0" \
    "bootargsubi=mem=\${osmem} console=ttyS1,115200n8 panic=20 init=/init root=ubi0:rootfs rootfstype=ubifs ubi.mtd=3,2048 mtdparts=\${mtdparts} \${extras}\0" \
    "bootnfs=setenv setargs setenv bootargs ${bootargsnfs}; run setargs; tftpboot ${baseaddr} uImage.${soc}; bootm ${baseaddr}\0" \
    "bootcmdnand=setenv setargs setenv bootargs ${bootargs}; run setargs; nand read ${baseaddr} 0x100000 0x300000; bootm ${baseaddr}\0" \
    "bootcmdubi=setenv setargs setenv bootargs ${bootargsubi}; run setargs; nand read ${baseaddr} 0x100000 0x300000; bootm ${baseaddr}\0" \
    "bootcmdnor=setenv setargs setenv bootargs ${bootargs}; run setargs; sf probe 0; sf read ${baseaddr} 0x50000 0x300000; bootm ${baseaddr}\0" \
    "setnand=run mtdpartsubi; setenv bootcmd ${bootcmdubi}; saveenv; reset\0" \
    "setnor8m=run mtdpartsnor8m; setenv bootcmd ${bootcmdnor}; saveenv; reset\0" \
    "setnor16m=run mtdpartsnor16m; setenv bootcmd ${bootcmdnor}; saveenv; reset\0" \
    "restore=n\0" \
    "soc="CONFIG_SOC"\0" \
    CONFIG_EXTRA_SETTINGS

#define CONFIG_SFC_MIN_ALIGN 0x10000

#define CONFIG_ENV_OFFSET       0x40000
#define CONFIG_ENV_SIZE         0x10000
#define CONFIG_ENV_SECT_SIZE    0x10000

#undef CONFIG_SYS_PROMPT
#define CONFIG_SYS_PROMPT	"OpenIPC # "

#define CONFIG_AUTOBOOT_KEYED
#define CONFIG_AUTOBOOT_PROMPT		\
    "Press Ctrl-c to abort autoboot... %d \n", bootdelay
#define CONFIG_AUTOBOOT_STOP_STR	"\x3"

#define CONFIG_SYS_LONGHELP
#define CONFIG_CMDLINE_EDITING
#define CONFIG_SYS_MAXARGS      64
#define CONFIG_SYS_NO_FLASH

#define CONFIG_SYS_CBSIZE 2048 /* Console I/O Buffer Size */
#define CONFIG_SYS_PBSIZE (CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_MALLOC_LEN (32 * 1024 * 1024)
#define CONFIG_SYS_BOOTPARAMS_LEN (256 * 1024)
#define CONFIG_SYS_MONITOR_LEN (230 * 1024)

#define CONFIG_SKIP_LOWLEVEL_INIT
#define CONFIG_CMDLINE_TAG

#define CONFIG_BOOTDELAY 1
#define CONFIG_CMD_TFTPPUT 1
#define CONFIG_CMD_FAT 1
#define CONFIG_CMD_FS_GENERIC 1
#define CONFIG_CMD_GPIO 1
#define CONFIG_NETMASK 255.255.255.0
#define CONFIG_GATEWAYIP 192.168.1.1
#define CONFIG_SERVERIP 192.168.1.254
#define CONFIG_IPADDR 192.168.1.10
#define CONFIG_ETHADDR 00:00:23:34:45:66

#define CONFIG_AUTO_UPDATE	1
#ifdef CONFIG_AUTO_UPDATE
    #define CONFIG_CMD_SDUPDATE	1
#endif
#define CONFIG_CMD_SDSTART	1
