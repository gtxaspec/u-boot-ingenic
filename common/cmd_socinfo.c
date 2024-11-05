#include <common.h>
#include <command.h>

int debug_socinfo = 0;

typedef struct {
	const char* name;
	unsigned int socId;
	unsigned int cppsr;
	unsigned int subremark;
	unsigned int subsoctype1;
	unsigned int subsoctype2;
} SocInfo;

SocInfo socInfoTable[] = {
	{"T10L", 0x0005, 0x0001, 0x0000, 0x0000, 0x0000},
	{"T20X", 0x2000, 0x0001, 0x0000, 0x2222, 0x0000},
	{"T20L", 0x2000, 0x0001, 0x0000, 0x3333, 0x0000},
	{"T21N", 0x0021, 0x0001, 0x0000, 0x1111, 0x0000},
	{"T23N", 0x0023, 0x0000, 0x0000, 0x1111, 0x0000},
	{"T23ZN", 0x0023, 0x0000, 0x0000, 0x6666, 0x0000},
	{"T30L", 0x0030, 0x0010, 0x0000, 0x3333, 0x0000},
	{"T30X", 0x0030, 0x0010, 0x0000, 0x2222, 0x0000},
	{"T31A", 0x0031, 0x0001, 0x0000, 0x4444, 0x0000},
	{"T31AL", 0x0031, 0x0008, 0x0000, 0xCCCC, 0x0000},
	{"T31L", 0x0031, 0x0010, 0x0000, 0x3333, 0x0000},
	{"T31LC", 0x0031, 0x0001, 0x0000, 0xEEEE, 0x300F},
	{"T31N", 0x0031, 0x0001, 0x0000, 0x1111, 0x0000},
	{"T31X", 0x0031, 0x0001, 0x0000, 0x2222, 0x0000},
	{"T31ZC", 0x0031, 0x0001, 0x0000, 0xDDDD, 0x0000},
	{"T31ZL", 0x0031, 0x0001, 0x0000, 0x5555, 0x0000},
	{"T31ZX", 0x0031, 0x0001, 0x0000, 0x6666, 0x0000},
	{"T40NN", 0x0040, 0x00ED, 0x0000, 0x0000, 0x8888},
	{"T40XP", 0x0040, 0x00f9, 0x0000, 0x0000, 0x7777},
};

static const char* get_soc_name(void) {
	unsigned int soc_id = *((volatile unsigned int *)(0x1300002C));
	unsigned int cppsr = *((volatile unsigned int *)(0x10000034));
	unsigned int subremark = *((volatile unsigned int *)(0x13540231));
	unsigned int subsoctype1 = *((volatile unsigned int *)(0x13540238));
	unsigned int subsoctype2 = *((volatile unsigned int *)(0x13540250));

	unsigned int cpu_id = (soc_id >> 12) & 0xFFFF;
	unsigned int cppsr_extracted = cppsr & 0xFF;
	unsigned int subremark_shifted = (subremark >> 8) & 0xFF;
	unsigned int subsoctype1_shifted = (subsoctype1 >> 16) & 0xFFFF;
	unsigned int subsoctype2_shifted = (subsoctype2 >> 16) & 0xFFFF;

	if (debug_socinfo) {
		printf("soc_id:      0x%08X [0x%04X]\n", soc_id, cpu_id);
		printf("cppsr:       0x%08X [0x%02X]\n", cppsr, cppsr_extracted);
		printf("subremark:   0x%08X [0x%02X]\n", subremark, subremark_shifted);
		printf("subsoctype1: 0x%08X [0x%04X]\n", subsoctype1, subsoctype1_shifted);
		printf("subsoctype2: 0x%08X [0x%04X]\n", subsoctype2, subsoctype2_shifted);
	}

	int i;
	for (i = 0; i < sizeof(socInfoTable) / sizeof(SocInfo); i++) {
		SocInfo s = socInfoTable[i];
		if (cpu_id == s.socId &&
			// cppsr_extracted == s.cppsr && // Disable for now
			subremark_shifted == s.subremark &&
			subsoctype1_shifted == s.subsoctype1 &&
			subsoctype2_shifted == s.subsoctype2) {
			return s.name;
		}
	}

	return "Unknown";
}

int do_socinfo(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]) {
	const char *soc_name = get_soc_name();
	printf("SOC Name: %s\n", soc_name);
	return CMD_RET_SUCCESS;
}

int do_socinfo_cmd_wrapper(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]) {
	debug_socinfo = 1; // Enable debug prints for command line calls
	int result = do_socinfo(cmdtp, flag, argc, argv);
	debug_socinfo = 0;
	return result;
}

U_BOOT_CMD(
	socinfo, CONFIG_SYS_MAXARGS, 1, do_socinfo_cmd_wrapper,
	"Display SOC information",
	"Usage: socinfo - Displays the SOC type based on the ingenic hardware registers"
);
