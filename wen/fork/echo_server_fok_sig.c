#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <signal.h>
#include <sys/wait.h>

#define PORT 8090

void do_echo(int);
void sigHandler(int);

main( )
{
	int	connSock, listenSock;
	struct sockaddr_in s_addr, c_addr;
	int	len;

	pid_t	pid;
	int	state;
	struct sigaction act;

	act.sa_handler = sigHandler;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;

	state=sigaction(SIGCHLD, &act, 0);

	if(state != 0) {
		printf("signal error\n");
		exit(1);
	}

	listenSock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(PORT);

	if(bind(listenSock, (struct sockaddr *) &s_addr, sizeof(s_addr)) == -1) {
		printf("Can not Bind\n");
		return -1;
	}

	if(listen(listenSock, 5) == -1) {
		printf("listen Fail\n");
		return -1;
	}

	while(1) {

		len = sizeof(c_addr);
		if((connSock=accept(listenSock, (struct sockaddr *) &c_addr, &len))<0) {
			printf("accept error\n");
		}

		if((pid = fork( )) < 0) {
			printf("echo server can not fork( )\n");
			return -1;
		} else if(pid > 0) {
			close(connSock);
			continue;
		} else if(pid == 0 ) {
			printf("child creat\n");
			close(listenSock);
			do_echo(connSock);
		}
	}
}

void
do_echo(int connSock)
{
	int	n;
	char	rcvBuffer[BUFSIZ];
	
	while((n = read(connSock, rcvBuffer, sizeof(rcvBuffer))) != 0) {
		printf("child(%d)\n", n);
		write(connSock, rcvBuffer, n);
	}
	exit(1);
}

void
sigHandler(int sig)
{
	int	pid;
	int	status;

	pid = wait(&status);
	printf("pid[%d] terminate\n", pid);

}
