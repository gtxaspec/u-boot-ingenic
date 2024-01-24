#include <common.h>
#include <command.h>

// Helper macros for extracting bits and high word
#define HIWORD(x) (((x) >> 16) & 0xFFFF)
#define BYTE2(x) (((x) >> 8) & 0xFF)

static int get_cpu_id() {
    unsigned int soc_id = *((volatile unsigned int *)(0x1300002C));
    unsigned int cppsr = *((volatile unsigned int *)(0x10000034));
    unsigned int subsoctype = *((volatile unsigned int *)(0xb3540238));
    unsigned int subremark = *((volatile unsigned int *)(0xb3540231));
    unsigned int subsoctypet40 = *((volatile unsigned int *)(0xb3540250));
    unsigned int subsoctypet41 = *((volatile unsigned int *)(0xb3540250));
    unsigned int chip_generation = 0;

    // Debug prints
    printf("soc_id: 0x%X\n", soc_id);
    printf("cppsr: 0x%X\n", cppsr);
    printf("subsoctype: 0x%X\n", subsoctype);
    printf("subremark: 0x%X\n", subremark);
    printf("subsoctypet40: 0x%X\n", subsoctypet40);
    printf("subsoctypet41: 0x%X\n", subsoctypet41);

    if (soc_id >> 28 != 1) {
        return -1;
    }

    switch ((soc_id << 4) >> 16) {
        case 5:
            chip_generation = 0x10;
            switch (cppsr & 0xFF) {
                case 0: return 1;
                case 1: return 0;
                case 0x10: return 2;
                default: return -1;
            }
        case 0x2000:
            chip_generation = 0x20;
            switch (cppsr & 0xFF) {
                case 1: return 3;
                case 16: return 4;
                default: return -1;
            }
    }

    switch ((soc_id >> 12) & 0xFF) {
        case 0x30:
            chip_generation = 0x30;
            if (cppsr & 0xFF == 1) {
                switch (HIWORD(subsoctype)) {
                    case 0x1111: return 7;
                    case 0x3333: return 7;
                    case 0x2222: return 8;
                    case 0x4444: return 9;
                    case 0x5555: return 10;
                    default: return 8;
                }
            } else if (cppsr & 0xFF == 0x10) {
                return 6;
            }
            break;
        case 0x21:
            chip_generation = 0x21;
            if (cppsr & 0xFF == 1) {
                switch (HIWORD(subsoctype)) {
                    case 0x3333: return 11;
                    case 0x1111: return 12;
                    case 0x5555: return 14;
                    default: return 13;
                }
            } else if (cppsr & 0xFF != 0x10) {
                return -1;
            }
            return 11;
        case 0x31:
            chip_generation = 0x31;
            if (cppsr & 0xFF == 1) {
                if (BYTE2(subremark)) {
                    switch (BYTE2(subremark)) {
                        case 3:
                        case 7: return 15;
                        case 0xF: return 16;
                        default: return -1;
                    }
                } else {
                    switch (HIWORD(subsoctype)) {
                        case 0x1111:
                        case 0x2222:
                        case 0x3333: return 15;
                        case 0x4444: return 18;
                        case 0x5555: return 20;
                        case 0x6666: return 23;
                        case 0xcccc: return 19;
                        case 0xdddd: return 21;
                        case 0xeeee: return 22;
                        default: return 16;
                    }
                }
            } else if (cppsr & 0xFF != 0x10) {
                return -1;
            }
            return 15;
        case 0x40:
            chip_generation = 0x40;
            switch (HIWORD(subsoctypet40)) {
                case 0x1111: return 24;
                case 0x8888: return 25;
                case 0x7777: return 26;
                case 0x4444: return 27;
                default: return -1;
            }
        case 0x41:
            chip_generation = 0x41;
            switch (HIWORD(subsoctypet41)) {
                case 0x3333: return 28;
                case 0x5555: return 29;
                case 0x8888: return 30;
                case 0x9999: return 31;
                case 0x1111: return 32;
                case 0x7777: return 33;
                case 0xAAAA: return 34;
                case 0x6666: return 35;
                default: return -1;
            }
        default: return -1;
    }

    return -1; // Default for unhandled cases
}

static const char *ingenic_cpu_name(int cpu_id) {
    switch (cpu_id) {
        case 0: return "T10";
        case 1: return "T10L";
        case 2: return "T10L";
        case 3: return "T20";
        case 4: return "T20L";
        case 5: return "T20X";
        case 6: return "T30L";
        case 7: return "T30N";
        case 8: return "T30X";
        case 9: return "T30A";
        case 10: return "T30Z";
        case 11: return "T21L";
        case 12: return "T21N";
        case 13: return "T21X";
        case 14: return "T21Z";
        case 15: return "T31L";
        case 16: return "T31N";
        case 17: return "T31X";
        case 18: return "T31A";
        case 19: return "T31AL";
        case 20: return "T31ZL";
        case 21: return "T31ZC";
        case 22: return "T31LC";
        case 23: return "T31ZX";
        case 24: return "T40N";
        case 25: return "T40NN";
        case 26: return "T40XP";
        case 27: return "T40A";
        case 28: return "T41L";
        case 29: return "T41ZL";
        case 30: return "T41LC";
        case 31: return "T41LQ";
        case 32: return "T41N";
        case 33: return "T41ZN";
        case 34: return "T41NQ";
        case 35: return "T41X";
        default: return "Unknown";
    }
}

static int do_socinfo(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]) {
    int cpu_id = get_cpu_id();
    const char *cpu_name = ingenic_cpu_name(cpu_id);

    printf("CPU ID: %d, CPU Name: %s\n", cpu_id, cpu_name);

    return CMD_RET_SUCCESS;
}

U_BOOT_CMD(
    socinfo, CONFIG_SYS_MAXARGS, 1, do_socinfo,
    "Display SOC information",
    ""
);
