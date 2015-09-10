#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

void *do_keyboard(void *);
void *do_socket(void *);

char quit[ ] = "exit";

#define MAX_THREAD_SIZE 3

int
main(int argc, char *argv[ ]) {

	int	thr_id=0;
	int	status;

	int	listenSock, connSock;
	struct sockaddr_in client_addr, server_addr;

	int	len;

	pthread_t thread_id[MAX_THREAD_SIZE];

	if(argc < 2) {
		printf("Usage:  talk_server port_num ");
		return -1;
	}

	if((listenSock=socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Server: Can't open socket\n");
		return -1;
	}

	bzero((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi(argv[1]));

	if(bind(listenSock, (struct sockaddr *) &server_addr, sizeof(server_addr))<0){
		printf("talk Server Can't bind \n");
		return -1;
	}

	listen(listenSock, 1);

	while(1) {
	len = sizeof(client_addr);
	if((connSock = accept(listenSock,(struct sockaddr *) &client_addr, &len))<0){
		printf("Talk Server failed in accepting\n");
		return -1;
	}
	printf("Talk Server accept new request : %d\n",connSock);
	if((status = pthread_create(&thread_id[thr_id++], NULL, do_socket, (void *)connSock))!=0) {
		perror("thread create error\n");
		exit(1);
	}
	printf("thread %x is create \n", thread_id[thr_id-1]);
	sleep(3);
	}
	
	close(listenSock);
	close(connSock);
}

void *
do_keyboard(void *data)
{
	int	n;
	char	sbuf[BUFSIZ];
	int	connSock = (int) data;

	while((n = read(0, sbuf, BUFSIZ)) > 0) {
		if(write(connSock, sbuf, n) != n) {
			printf("Talk Server fail in sending\n");
		}
		if(strncmp(sbuf, quit, 4) == 0) {
		//	pthread_kill(pid[1], SIGINT);
			break;
		}
	}
}

void *
do_socket(void *data)
{
	/* ������ �Լ��� �ۼ��غ����� */
         /* 1. read �Լ��� �̿��Ͽ� ������Ͽ��� ����Ÿ�� �о���� */
         /* 2. �о���� ���ڿ��� �ι��ڸ� ó���غ�����  */
         /* 3. ����� ��Ʈ���� ����Ϳ� ����غ����� */
        /*  4. ���ŵ� ���ڿ��� quit �� pthread_kill(������ID, SIGINT);�� �̿��Ͽ�
                �����带 �����Ű���� */

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
	}
}
