/* test_mydrv.c */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>

#define MAX_BUFFER 100
char buf_in[MAX_BUFFER], buf_out[MAX_BUFFER];
char buf[MAX_BUFFER];

typedef struct
{
	int age;
	char name[30];
	char address[20];
	int phone_number;
	char depart[20];
} __attribute__((packed))mydrv_data;

int main()
{
  int fd,i;
  char *pData;
  mydrv_data data;
  int len = sizeof(data);

  pData = (char *)&data;
  data.age = 35;
  data.phone_number = 1234;
  memcpy(&data.address, "HONG_KILDONG", strlen("HONG_KILDONG"));
  memcpy(&data.depart, "ELAYER", strlen("ELAYER"));
  memcpy(buf, pData, len);

  fd = open("/dev/mydrv",O_RDWR);
  read(fd,buf_in,MAX_BUFFER);
  printf("buf_in = %s\n",buf_in);
  
  for(i = 0;i < MAX_BUFFER;i++)
     buf_out[i] = 'a' + i;

  write(fd,buf,MAX_BUFFER);
  close(fd);
  return (0);
}
