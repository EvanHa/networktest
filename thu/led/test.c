/* test.c */

#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>

#define MAX_BUFFER 2
char buf_in[MAX_BUFFER], buf_out[MAX_BUFFER];

int main()
{
  int fd;
  
  fd = open("/dev/led",O_RDWR);
  read(fd,buf_in,MAX_BUFFER);
  write(fd,buf_out,MAX_BUFFER);
  close(fd);
  return (0);
}
