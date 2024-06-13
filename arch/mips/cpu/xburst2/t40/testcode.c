int sort(int *data, int num) {
	int i = 0;
	int j = 0;
	for (i = 0; i < num - 1; i++) {
		for (j = (num - 2); j >= i; j--) {
			if (data[j] > data[j + 1]) {
				int temp = data[j];
				data[j] = data[j + 1];
				data[j + 1] = temp;
			}
		}
	}
	return 0;
}

#if 0
int main(int argc, unsigned char *argv[]) {
    TEST_CODES
    return 0;
}
#endif
