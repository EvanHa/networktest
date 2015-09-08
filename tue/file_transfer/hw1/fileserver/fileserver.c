#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE    1024

// 소켓 함수 오류 출력 후 종료
void err_quit(const char *msg)
{
	perror(msg);
	exit(1);
}

// 소켓 함수 오류 출력
void err_display(const char *msg)
{
	perror(msg);
}

int main(int argc, char *argv[])
{
	int retval;

	if(argc != 2){
		fprintf(stderr, "[Usage] %s port_number\n", argv[0]);
		exit(1);
	}

	// socket()
	int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_sock < 0) err_quit("socket()");

	// bind()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(atoi(argv[1]));
	retval = bind(listen_sock, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	if(retval < 0) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, 5);
	if(retval < 0) err_quit("listen()");

	// 데이터 통신에 사용할 변수
	int client_sock;
	struct sockaddr_in clientaddr;
	socklen_t addrlen;
	char buf[BUFSIZE+1];

	while(1){
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr *)&clientaddr, &addrlen);
		if(client_sock < 0){
			err_display("accept()");
			break;
		}

		// 접속한 클라이언트 정보 출력
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// 클라이언트와 데이터 통신
		do {
			// 파일 이름 받기(256바이트 고정 길이)
			char filename[256];
			retval = recv(client_sock, filename, 256, MSG_WAITALL);
			if(retval <= 0){
				err_display("recv()");
				break;
			}
			printf("-> 받을 파일 이름: %s\n", filename);

			// 파일 열기
			FILE *fp = fopen(filename, "wb");
			if(fp == NULL){
				perror("fopen()");
				break;
			}

			// 파일 데이터 받기
			// : 파일 데이터의 길이를 미리 알지 못한다고 가정하고,
			//   recv() 함수의 리턴값이 0이 나올 때까지 읽는다.
			int totalbytes = 0;
			while(1){
				retval = recv(client_sock, buf, BUFSIZE, 0);
				if(retval < 0){
					err_display("recv()");
					break;
				}
				else if(retval == 0)
					break;
				else{
					fwrite(buf, 1, retval, fp);
					if(ferror(fp)){
						perror("fwrite()");
						break;
					}
					totalbytes += retval;
				}
			}
			fclose(fp);

			// 전송 결과 출력
			if(retval == 0)
				printf("-> 파일 전송 완료! (%d바이트)\n", totalbytes);
			else
				printf("-> 파일 전송 실패!\n");
		} while(0);

		// close()
		close(client_sock);
		printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	}

	// close()
	close(listen_sock);

	return 0;
}
