#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define MAX_BUF 2
static int device;
static unsigned char buf_out[MAX_BUF];

static void led_ctrl(unsigned char led1, unsigned char led2, unsigned char led3, unsigned char led4)
{
	unsigned char val = (led4 << 3) | (led3 << 2) | (led2 << 1) | (led1 << 0);
	buf_out[0] = val;
	write(device, buf_out, MAX_BUF);
}


struct _msg
{
	unsigned char b1;
	unsigned char b2;
	unsigned char b3;
	unsigned char b4;
};

int main(int argc, char *argv[])
{
	int listen_sock, cli_sock;
	struct sockaddr_in servaddr, cliaddr;
	socklen_t addrlen;
	int res;
	struct _msg msg;
	ssize_t nbytes;

	if(argc != 2){
		fprintf(stderr, "Usage: %s <Port>\n", argv[0]);
		exit(1);
	}

	device = open("/dev/led", O_RDWR);
	if(device < 0)
	{
		perror("open()");
		exit(1);
	}

	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_sock < 0) { perror("socket()"); exit(1); }

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(0);
	servaddr.sin_port = htons(atoi(argv[1]));

	res = bind(listen_sock, (struct sockaddr *)&servaddr, sizeof(servaddr));
	if(res < 0) { perror("bind()"); exit(1); }

	res = listen(listen_sock, 1);
	if(res < 0) { perror("listen()"); exit(1); }

	printf("LED control server running...\n");
	while(1){
		addrlen = sizeof(cliaddr);
		cli_sock = accept(listen_sock, (struct sockaddr *)&cliaddr, &addrlen);
		if(cli_sock < 0){ perror("accept()"); break; }
		printf("LED control client connected!    (%s/%d)\n",
			inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));

		while(1){
			nbytes = recv(cli_sock, &msg, sizeof(msg), MSG_WAITALL);
			if(nbytes > 0){
				led_ctrl(msg.b1, msg.b2, msg.b3, msg.b4);
			}
			else if(nbytes <= 0){
				break;
			}
		}
		printf("LED control client disconnected! (%s/%d)\n",
			inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
		close(cli_sock);
	}

	close(device);
	close(listen_sock);

	return 0;
}
