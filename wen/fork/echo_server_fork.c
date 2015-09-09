#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <signal.h>

#define PORT 80900

void do_echo(int);

void do_child(void);
void do_parent(void);

void sig_handler(int signo)
{
	int stat, chld_pid;
	sigset_t newset, oldset;

	chld_pid = wait(&stat);
	printf("signal ID [%d]\n",signo);

	if(WIFEXITED(stat)) {
		printf("normal termination, exit status [%d]\n", WEXITSTATUS(stat));
	} else if (WIFSIGNALED(stat)) {
		printf("abnormal termination, signal number [%d]\n", WTERMSIG(stat));
	} else if (WIFSTOPPED(stat)) {
		printf("child stopped, signal number [%d]\n", WSTOPSIG(stat));
	}
	exit(1);
}

int main( )
{
	int	connSock, listenSock;
	struct sockaddr_in s_addr, c_addr;
	int	len;

	pid_t	pid;
	struct sigaction sigset, oldset;

	sigset.sa_handler = sig_handler;
	sigemptyset(&sigset.sa_mask);
	sigset.sa_flags=0;
	sigaction(SIGCHLD, &sigset, &oldset);

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
		connSock = accept(listenSock, (struct sockaddr *) &c_addr, &len);

		if((pid = fork( )) < 0) {
			printf("echo server can not fork( )\n");
			return -1;
		} else if(pid > 0) {
			close(connSock);
			do_parent();
			continue;
		} else if(pid == 0 ) {
			close(listenSock);
			do_echo(connSock);
			do_child();
		}
	}
}

void
do_echo(int connSock) {
	int	n;
	char	rcvBuffer[BUFSIZ];

	while((n = read(connSock, rcvBuffer, sizeof(rcvBuffer))) != 0) {
		printf("child\n");
		write(connSock, rcvBuffer, n);
	}
	exit(0);
}

void do_child() {
	exit(100);
}

void do_parent() {
	sleep(10);
}
