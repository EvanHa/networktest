#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_THREAD_SIZE 10
#define MAX_USER		10
int sock_connect[MAX_USER];
int user_cnt=0;

void * readSock(void *data);
void * writeSock(int id, char *buf, int len);

int main(int argc, char *argv[ ]) {

	int	sock_listen;
	struct sockaddr_in client_addr, server_addr;
	int len_addr;
	int thread_status;

	pthread_t thread_id[MAX_THREAD_SIZE];

	if(argc < 2) {
		printf("Usage:  talk_server port_num ");
		return -1;
	}

	if((sock_listen=socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Server: Can't open socket\n");
		return -1;
	}

	bzero((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi(argv[1]));

	if(bind(sock_listen, (struct sockaddr *) &server_addr, sizeof(server_addr))<0){
		printf("talk Server Can't bind \n");
		return -1;
	}

	listen(sock_listen, MAX_USER);

	while(1) {
		len_addr = sizeof(client_addr);
		if((sock_connect[user_cnt] = accept(sock_listen,(struct sockaddr *) &client_addr, &len_addr))<0){
			printf("Talk Server failed in accepting\n");
			return -1;
		}
	
		printf("Talk Server accept new request : %d\n", sock_connect);
		
		if((thread_status = pthread_create(&thread_id[user_cnt++], NULL, readSock, (void *)sock_connect[user_cnt]))!=0) {
			perror("thread create error\n");
			exit(1);
		}
		printf("thread %x is create \n", thread_id[user_cnt-1]);
		sleep(3);
	}
	
	close(sock_listen);
	close(sock_connect);
}

void * writeSock(int id, char *buf, int len)
{
	int i=0;
	for(i=0; i<user_cnt; i++) {
		if (id == sock_connect[i]) {continue;}
		printf("send to user[%d]\n", i+1);
		if(write(sock_connect[i], buf, len) != len) {
			printf("Talk Server fail in sending\n");
		}
	}
}

void * readSock(void *data)
{
	int n;
	char rcvBuffer[BUFSIZ];
	int c_socket = (int)data;
	printf("Talk Server accept new request : %d\n",c_socket);
	while((n=read(c_socket, rcvBuffer, sizeof(rcvBuffer)))!=0)
	{
		rcvBuffer[n] = '\0';
		printf("%s", rcvBuffer);
		if (strncmp(rcvBuffer, "quit", 4) == 0) {
			printf("quit\n");
		}
		writeSock(c_socket, rcvBuffer, n);
	}
}
