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
	/* 쓰레드 함수를 작성해보세요 */
         /* 1. read 함수를 이용하여 연결소켓에서 데이타를 읽어보세요 */
         /* 2. 읽어들인 문자열에 널문자를 처리해보세요  */
         /* 3. 저장된 스트링을 모니터에 출력해보세요 */
        /*  4. 수신된 문자열이 quit 면 pthread_kill(쓰레드ID, SIGINT);를 이용하여
                쓰레드를 종료시키세요 */

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
