
#define DDR_CS0
//#define DDR_CS1
void ddr_test_refresh(unsigned int start_addr, unsigned int end_addr)
{

	unsigned int start_addr_cs0 = start_addr;
	unsigned int end_addr_cs0 = end_addr;
//	unsigned int start_addr_cs0 = 0xa0000000;
//	unsigned int end_addr_cs0 = 0xa0200000;
	unsigned int start_addr_cs1 = 0xa8000000;
	unsigned int end_addr_cs1 =   0xaa000000;
	unsigned int count = 0, num = 0;
	unsigned int i = 0, j = 0;
	int flag = 1;
	int run = 0;
	int cxtan = 64;
	unsigned int r_data;
#ifdef DDR_CS0
	count = 0;
	num = 0;
	for (i = start_addr_cs0; i < end_addr_cs0; i += cxtan){
		*(volatile unsigned int *)i = i;
		count++;
		if (count == 30000){
			//mdelay(5);
			count = 0;
			num++;
			if (num % 30 == 0)
				printf("this program cs0 is alive, pd num is %d\n", num);
		}
	}
#endif
#ifdef DDR_CS1
		count = 0;
		num = 0;
		for (j = start_addr_cs1; j <= end_addr_cs1; j += cxtan){
			*(volatile unsigned int *)j = j;
			count++;
			if (count == 15000){
				//mdelay(5);
				count = 0;
				num++;
				if (num % 30 == 0)
					printf("this program cs1 is alive, pd num is %d\n", num);
			}
		}
#endif
	while(flag){
		run++;
		printf("now run is %d\n", run);
#ifdef DDR_CS0
		count = 0;
		num = 0;
		for (i = start_addr_cs0; i < end_addr_cs0; i += cxtan){
			if (*(volatile unsigned int *)i != i){
				printf("ddr cs0 error address is %x, error data is %x, right data is %x\n",i, *(volatile unsigned int *)i, i);
			}
			count++;
			if (count == 30000){
				mdelay(5);
				count = 0;
				num++;
				if (num % 30 == 0)
					printf("this cs0 program is alive ps num is %d\n", num);
			}
		}
#endif
#ifdef DDR_CS1
		count = 0;
		num = 0;
		for (j = start_addr_cs1; j <= end_addr_cs1; j += cxtan){
			if (*(volatile unsigned int *)j != j){
				printf("ddr cs1 error address is %x, error data is %x, right data is %x\n", j, *(volatile unsigned int *)j, j);
			}
			count++;
			if (count == 15000){
				//mdelay(5);
				count = 0;
				num++;
				if (num % 30 == 0)
					printf("this cs1 program is alive ps num is %d\n", num);
			}
		}
#endif
	}
}
