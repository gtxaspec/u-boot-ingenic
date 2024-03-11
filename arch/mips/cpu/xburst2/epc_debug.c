/*
 * 获取CPU0/CPU1 RESET ERROR PC
 * Copyright (c) 2023
 *
 * 使用注意：
 * 第一次上电后获取的EPC值不正确，需要对uboot进行一次复位（注意不要进内核后复位），
 * uboot复位后再次打印的EPC值属于有效值。
 */

void epc_debug(void)
{
	unsigned int tmp0;
	unsigned int tmp1;
	unsigned int tmp2;
	unsigned int tmp3;
	unsigned int core1_error_epc;
	unsigned int errorpc;
	__asm__ __volatile__ (
			"mfc0  %0, $30,  0   \n\t"
			"nop                  \n\t"
			:"=r"(errorpc)
			:);
	printf("CPU0 RESET ERROR PC: %x\n", errorpc);

	__asm__ __volatile__(
			".set push                             \n"
			".set noat                             \n"
			".set noreorder                        \n"
			"   lui  %0,0xb220                     \n"
			"   lui  %1,0xb266                     \n"
			"#change reset entry into oram         \n"
			"   lw   %4,0x0f00(%0)                 \n"
			"   sw   %1,0x0f00(%0)                 \n"
			"   la   %0,__read_error_epc_begin     \n"
			"   la   %3,__read_error_epc_end       \n"
			"   subu %3, %3, %0                    \n"
			"   add  %3, %3, %1                    \n"
			"_move_to_oram:                        \n"
			"   lw   %2, 0(%0)                     \n"
			"   sw   %2, 0(%1)                     \n"
			"   addiu %0, %0, 4                    \n"
			"   bne   %1, %3, _move_to_oram        \n"
			"   addiu %1, %1, 4                    \n"
			"   lui   %0,0xb220                    \n"
			"#wakeup core1                         \n"
			"   sw    $0,0x40(%0)                  \n"
			"   li    %1,0x2                       \n"
			"__core0_wait_core1_sleep:             \n"
			"   lw    %2, 0x20(%0)                 \n"
			"   and   %2, %2,%1                    \n"
			"   beqz  %2,__core0_wait_core1_sleep  \n"
			"   nop                                \n"
			"   sw    %1,0x40(%0)                  \n"
			"   j     __read_error_epc_end         \n"
			"   nop                                \n"
			"__read_error_epc_begin:               \n"
			"   mfc0  %0, $30,0                    \n"
			"   lui   %1, 0xb220                   \n"
			"#CCU_MBR1                             \n"
			"   sw    %0, 0x1004(%1)               \n"
			"   wait                               \n"
			"   nop                                \n"
			"   nop                                \n"
			"   nop                                \n"
			"__read_error_epc_end:                 \n"
			"   lui   %0,0xb220                    \n"
			"   sw    %4,0x0f00(%0)                \n"
			"   lw    %4,0x1004(%0)                \n"
			".set pop                              \n"
			::"r"(tmp0), "r"(tmp1), "r"(tmp2), "r"(tmp3),"r"(core1_error_epc));
	printf("CPU1 RESET ERROR PC: %x\n",core1_error_epc);

}
