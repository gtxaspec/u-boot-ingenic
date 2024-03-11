/* allcate L2 cache size */
/***********************************
  L2 cache size
  reg addr: 0x12200060
  bit   12 11 10
  0   0  0   L2C=0KB
  0   0  1   L2C=128KB
  0   1  0   L2C=256KB
  0   1  1   L2C=512KB
  1   0  0   L2C=1024KB
 ***********************************/

#include <common.h>
#include <environment.h>
#include <command.h>
#include <image.h>
#include <asm/byteorder.h>
#include <asm/io.h>
#include <linux/mtd/mtd.h>
#include <linux/ctype.h>
#include <asm/cacheops.h>
#include <config.h>

#ifdef CONFIG_CMD_L2CACHE

int do_l2cache (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int size, size_reg, i;
	char *type = argv[1];

	/*size is a number*/
	for(i=0;type[i] != '\0';i++)
	{
		if(!isdigit(type[i]))
		{
			printf("unsupport the size!\n");
			return -1;
		}
	}

	size = simple_strtol(argv[1], NULL, 10);
	if(size == 0 || size == 128 || size == 256 || size == 512 || size == 1024)
	{
		size_reg = size >> 6;
		for(i = 0; size_reg != 1 && size_reg != 0; i++)
			size_reg = size_reg >> 1;
		flush_cache_all();
		/* wait l2cache ok */
		__asm__ volatile(
				".set push     \n\t"
				".set mips32r2 \n\t"
				"sync          \n\t"
				"lw $0,0(%0)   \n\t"
				".set pop      \n\t"
				::"r" (0xa0000000));
		*((volatile unsigned int *)(0xb2200060)) = (i << 10) & 0x1c00;
		__asm__ volatile(
				".set push     \n\t"
				".set mips32r2 \n\t"
				"sync          \n\t"
				"lw $0,0(%0)   \n\t"
				".set pop      \n\t"
				::"r" (0xa0000000));
		printf("l2cache size is %dK\n", CONFIG_SYS_L2CACHE_SIZE / 1024);
	}else{
		printf("unsupport the size!\n");
		return -1;
	}
	return 0;
}

U_BOOT_CMD(
	l2cache,   2,  1,  do_l2cache,
	"allcate L2 cache size",
	"<interface> <size>\n"
	"size=0		L2C=0KB\n"
	"size=128	L2C=128KB\n"
	"size=256	L2C=256KB\n"
	"size=512	L2C=512KB\n"
	"size=1024	L2C=1024KB\n"
	"other sizes are not supported"
);

#endif
