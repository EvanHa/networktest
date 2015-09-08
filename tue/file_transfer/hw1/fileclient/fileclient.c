#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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
		perror("fopen()");
		return 1;
	}

	// 파일 이름 보내기(256바이트 고정 길이)
	char filename[256];
	strncpy(filename, argv[3], sizeof(filename));
	retval = send(sock, filename, 256, 0);
	if(retval < 0) err_quit("send()");

	// 파일 데이터 보내기(전송 후 연결 종료)

	/*--- 파일 크기 얻기 ---*/
	fseek(fp, 0, SEEK_END);
	int length = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	/*--- 파일 데이터 보내기 ---*/
	char *buf = (char *)malloc(length);
	if(buf == NULL){
		fprintf(stderr, "malloc() error!\n");
		exit(1);
	}

	int nbytes = fread(buf, 1, length, fp);
	if(nbytes == length){
		retval = send(sock, buf, nbytes, 0);
		if(retval < 0) err_quit("send()");
		printf("-> 파일 전송 완료! (%d바이트)\n", nbytes);
	}
	else{
		perror("fread()");
	}
	fclose(fp);

	// close()
	close(sock);

	return 0;
}
