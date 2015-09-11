// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct _msg
{
	unsigned char b1;
	unsigned char b2;
	unsigned char b3;
	unsigned char b4;
};

int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in servaddr;
	int res, which;
	struct _msg msg = {0, 0, 0, 0};

	if(argc != 3){
		fprintf(stderr, "Usage: %s <IP> <Port>\n", argv[0]);
		exit(1);
	}

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0) { perror("socket()"); exit(1); }

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);
	servaddr.sin_port = htons(atoi(argv[2]));

	res = connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr));
	if(res < 0) { perror("connect()"); exit(1); }

	while(1){
		printf("1)LED1   2)LED2   3)LED3   4)LED4   5)Quit : ");
		res = scanf("%d", &which);
		if(res != 1) which = 5;

		switch(which){
		case 1:
			msg.b1 = !msg.b1;
			break;
		case 2:
			msg.b2 = !msg.b2;
			break;
		case 3:
			msg.b3 = !msg.b3;
			break;
		case 4:
			msg.b4 = !msg.b4;
			break;
		case 5:
			close(sock);
			exit(0);
		default:
			continue;
		}

		res = send(sock, &msg, sizeof(msg), 0);
		if(res < 0) { perror("send()"); break; }
	}

	close(sock);
	return 0;
}
