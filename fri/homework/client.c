#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_THREAD 10
pthread_t thread_id[MAX_THREAD];
char quit[ ] = "exit";

void *readSock(void *);
void *do_keyboard(void *);

int main(int argc, char *argv[ ]) {

	int	connSock;
	struct sockaddr_in server_addr;
	char	*serverAddr;
	int	serverPort;
 
	int	thr_id=0, status=0;
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

	if(connect(connSock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
		printf("talk Client Can't connect \n");
		return -1;
	}
	printf("Talk Client connect to Talk server \n");

	if((status = pthread_create(&thread_id[thr_id++], NULL, do_keyboard, (void *)connSock))!=0) {
		perror("thread create error\n");
		return -1;
	}
	if((status = pthread_create(&thread_id[thr_id++], NULL, readSock, (void *)connSock))!=0) {
		perror("thread create error\n");
		return -1;
	}
	while(1) {
		sleep(3);
	}
	close(connSock);
}

void *do_keyboard(void *data)
{
	int	n;
	char	sbuf[BUFSIZ];
	int	connSock = (int) data;

	while((n = read(0, sbuf, BUFSIZ)) > 0) {
		if(write(connSock, sbuf, n) != n) {
			printf("Talk Server fail in sending\n");
		}

		if(strncmp(sbuf, quit, 4) == 0) {
			//pthread_kill(pid[1],SIGINT);
			break;
		}
	}
}

void * readSock(void *data)
{
	int n;
	char rcvBuffer[BUFSIZ];
	int c_socket = (int)data;
	while((n=read(c_socket, rcvBuffer, sizeof(rcvBuffer)))!=0)
	{
		rcvBuffer[n] = '\0';
		printf("%s", rcvBuffer);
	}
}
