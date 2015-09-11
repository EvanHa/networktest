/* test.c */

#include <stdio.h>
#include <sys/types.h>
#include <sys/times.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER 2
char buf_in[MAX_BUFFER], buf_out[MAX_BUFFER];
int signal_count=0;

void usersignal(int sig)
{
     printf("\n [App] : Usersignal = %d", signal_count++);
}

int main()
{
 int fd;
 pid_t id;
 FILE *rfp, *wfp;
 int c;
 char *fname;
 char fntmp[BUFSIZ];
 char template[32];
 int i;
 time_t t;
 struct tms mytms;
 clock_t t1, t2;


  (void)signal(SIGUSR1, usersignal);

  id = getpid();
  printf("\n[APP] pid = %d", id);
  
  fd = open("/dev/led",O_RDWR);
  //read(fd,buf_in,MAX_BUFFER);

  write(fd, &id, 4);
//  write(fd,buf_out,MAX_BUFFER);

  while(1)
     ;
  close(fd);
  return (0);
}
