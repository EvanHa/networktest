#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ucontext.h>
#include <fcntl.h>
static pid_t id;
#include <sys/wait.h>

void handler(int signo, siginfo_t *sf, ucontext_t *uc)
{
	int status;
	psignal(signo, "Received Signal");
	
	id = fork();
	switch(id)
	{
		case -1:
			perror("fork");
			exit(1);
			break;
		case 0:
			printf("child process: MY[%d], PAR[%d]\n", (int)getpid(), (int)getppid());
			sleep(3);
			break;
		default:
			while(waitpid(id, &status, WNOHANG) == 0)
			{
				printf("Parent process: My[%d[, Child[%d]\n", (int)getpid(), (int)id);
				sleep(1);
				break;
			}
	}
}


int main(void)
{
	pid_t pid;
	int fd;
	struct sigaction act;
	unsigned char key;
	int sys_ret;
	sys_ret = system("mknod /dev/key c 240 0");
	
	pid = (int)getpid();
	printf("user PID=%d\n", pid);
	fd = open("/dev/key", O_RDWR);
	write(fd,&pid, 4);
	signal(SIGUSR1, handler);

	while(key != 'q')
	{
		scanf("%c", &key);
	
	}
	//pause();
	return 0;
}
