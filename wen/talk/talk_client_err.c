#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void do_keyboard(int);
void do_socket(int);

char quit[ ] = "exit";

pid_t pid;

int
main(int argc, char *argv[ ]) {

	int	connSock;
	struct sockaddr_in server_addr;
	char	*serverAddr;
	int	serverPort;

	int	len;

	if(argc != 3) {
		printf("Usage:  talk_client server_ip port_num ");
		return -1;
	} else {
		serverAddr = argv[1];
		serverPort = atoi(argv[2]);
	}

	if((connSock=socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Talk Client Can't open socket\n");
		return -1;
	}

	bzero((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(serverAddr);
	server_addr.sin_port = htons(serverPort);

	if(connect(connSock,(struct sockaddr *) &server_addr,sizeof(server_addr))<0) {
		printf("talk Client Can't connect \n");
		return -1;
	}
	printf("Talk Client connect to Talk server \n");

	pid = fork();
	if(pid < 0) {
	}else if (pid == 0) {
		do_socket(connSock);
	}else if (pid > 0) {
		do_keyboard(connSock);
	}

	close(connSock);
}

void
do_keyboard(int connSock)
{
	int	n;
	char	sbuf[BUFSIZ];

	while((n = read(0, sbuf, BUFSIZ)) > 0) {
		if(write(connSock, sbuf, n) != n) {
			printf("Talk Server fail in sending\n");
		}

		if(strncmp(sbuf, quit, 4) == 0) {
			kill(pid, SIGQUIT);
			break;
		}
	}
}

void
do_socket(int connSock)
{
	int 	n;
	char	rbuf[BUFSIZ];

	while(1) {
		if((n = read(connSock, rbuf, BUFSIZ)) > 0) {
			rbuf[n] = '\0';
			printf("%s", rbuf);

			if(strncmp(rbuf, quit, 4) == 0) {
				kill(getppid( ), SIGQUIT);
				break;
			}
		}
	}
}
