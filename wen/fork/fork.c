#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

int main(void) {
	pid_t pid;
	if((pid = fork()) > 0)
	{
		while(1);
	} else if(pid == 0)
	{
		return 0;
	}
	else
	{
		int errsv = errno;
		perror("fork error");
		return errsv;
	}
	return 0;
}
