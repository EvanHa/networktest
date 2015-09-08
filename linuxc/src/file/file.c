#include <stdio.h>

void main(void)
{
	FILE *fp;
	char *str;
	int buff_size = 1024;

	int i=0;

	fp = fopen("./test.txt", "wb");
	if (fp == NULL) {
		perror("fopen()\n");
		return -1;
	}

	for(i;i<10;i++) {
	fputc((char)i, fp);
	}

	fclose(fp);

}
