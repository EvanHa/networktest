#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_THREAD 3
pthread_t thread_id[MAX_THREAD];
char quit[ ] = "exit";

void *do_socket(void *);
void *do_keyboard(void *);
int
main(int argc, char *argv[ ]) {

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

	if((status = pthread_create(&thread_id[thr_id++], NULL, do_socket, (void *)connSock))!=0) {
		perror("thread create error\n");
		return -1;
	}
	if((status = pthread_create(&thread_id[thr_id++], NULL, do_keyboard, (void *)connSock))!=0) {
		perror("thread create error\n");
		return -1;
	}
	while(1) {
		sleep(3);
	}
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
			pthread_kill(pid[1],SIGINT);
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

	int	n;
	char	sbuf[BUFSIZ];
	int	connSock = (int) data;

	while((n = read(connSock, sbuf, BUFSIZ)) > 0) {
		printf("%s\n",sbuf);
		if(strncmp(sbuf, quit, 4) == 0) {
	//		pthread_kill(pid[1],SIGINT);
			break;
		}
	}
}
