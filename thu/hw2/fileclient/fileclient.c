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

int main(int argc, char *argv[])
{
	int retval;

	if(argc != 4){
		fprintf(stderr, "[Usage] %s ip_address port_number file_name\n", argv[0]);
		exit(1);
	}

	// socket()
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0) err_quit("socket()");

	// connect()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	serveraddr.sin_port = htons(atoi(argv[2]));
	retval = connect(sock, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	if(retval < 0) err_quit("connect()");

	// 파일 열기
	FILE *fp = fopen(argv[3], "rb");
	if(fp == NULL){
		perror("파일 입출력 오류");
		return 1;
	}

	// 파일 이름 보내기
	char filename[256];
	strncpy(filename, argv[3], sizeof(filename));
	retval = send(sock, filename, 256, 0);
	if(retval < 0) err_quit("send()");

	// 전송 시작할 위치(=현재의 파일 크기) 받기
	int currbytes;
	retval = recv(sock, (char *)&currbytes, sizeof(currbytes), MSG_WAITALL);
	if(retval < 0) err_quit("recv()");
	printf("### 옵셋 %d 바이트 지점부터 전송을 시작합니다. ###\n", currbytes);

	// 파일 크기 얻기
	fseek(fp, 0, SEEK_END);
	int filesize = ftell(fp);
	int totalbytes = filesize - currbytes;

	// 전송할 데이터 크기 보내기
	retval = send(sock, (char *)&totalbytes, sizeof(totalbytes), 0);
	if(retval < 0) err_quit("send()");

	// 파일 데이터 전송에 사용할 변수
	char buf[BUFSIZE];
	int numread;
	int numtotal = 0;

	// 파일 데이터 보내기
	fseek(fp, currbytes, SEEK_SET); // 파일 포인터를 전송 시작 위치로 이동
	while(1){
		numread = fread(buf, 1, BUFSIZE, fp);
		if(numread > 0){
			retval = send(sock, buf, numread, 0);
			if(retval < 0){
				err_display("send()");
				break;
			}
			numtotal += numread;
			printf("."); fflush(stdout); // 전송 상황을 표시
			usleep(10000); // 전송 중단 실험을 위해 속도를 느리게 함
		}
		else if(numread == 0 && numtotal == totalbytes){
			printf("\n파일 전송 완료!: %d 바이트 전송됨\n", filesize);
			break;
		}
		else{
			perror("파일 입출력 오류");
			break;
		}
	}
	fclose(fp);

	// close()
	close(sock);

	return 0;
}
