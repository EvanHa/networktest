#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFSIZE 1024

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

// 현재 디렉터리에서 파일 존재 여부를 확인한다.
int search_file(const char *filename)
{
	return (access(filename, F_OK) == 0);
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
	char buf[BUFSIZE];

	while(1){
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr *)&clientaddr, &addrlen);
		if(client_sock < 0){
			err_display("accept()");
			break;
		}
		printf("\nfileclient 접속: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// 파일 이름 받기
		char filename[256+1];
		retval = recv(client_sock, filename, 256, MSG_WAITALL);
		if(retval < 0){
			err_display("recv()");
			close(client_sock);
			continue;
		}
		filename[retval] = '\0';
		printf("-> 받을 파일 이름: %s\n", filename);

		// 파일 검색
		int currbytes = 0;
		if(search_file(filename)){
			// 현재의 파일 크기 얻기
			FILE *fp = fopen(filename, "rb");
			if(fp == NULL){
				perror("파일 입출력 오류");
				close(client_sock);
				continue;
			}
			fseek(fp, 0, SEEK_END);
			currbytes = ftell(fp);
			fclose(fp);
		}

		// 전송을 시작할 위치(=현재의 파일 크기) 보내기
		retval = send(client_sock, (char *)&currbytes, sizeof(currbytes), 0);
		if(retval < 0){
			err_display("send()");
			close(client_sock);
			continue;
		}

		// 전송 받을 데이터 크기 받기
		int totalbytes;
		retval = recv(client_sock, (char *)&totalbytes, sizeof(totalbytes), MSG_WAITALL);
		if(retval < 0){
			err_display("recv()");
			close(client_sock);
			continue;
		}
		printf("-> 전송 받을 데이터 크기: %d 바이트 중 %d 바이트\n"
		       "                         (%d 바이트는 이미 받음)\n",
			currbytes+totalbytes, totalbytes, currbytes);

		// 파일 열기
		FILE *fp = fopen(filename, "ab"); // append & binary mode
		if(fp == NULL){
			perror("파일 입출력 오류");
			close(client_sock);
			continue;
		}

		// 파일 데이터 받기
		int numtotal = 0;
		while(1){
			retval = recv(client_sock, buf, BUFSIZE, MSG_WAITALL);
			if(retval > 0){	
				fwrite(buf, 1, retval, fp);
				if(ferror(fp)){
					perror("파일 입출력 오류");
					break;
				}
				numtotal += retval;
			}
			else if(retval == 0){
				break;
			}
			else{
				err_display("recv()");
				break;
			}
		}
		fclose(fp);

		// 전송 결과 출력
		if(numtotal == totalbytes)
			printf("-> 파일 전송 완료!\n");
		else
			printf("-> 파일 전송 실패!\n");

		// close()
		close(client_sock);
		printf("fileclient 종료: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	}

	// close()
	close(listen_sock);
	return 0;
}
