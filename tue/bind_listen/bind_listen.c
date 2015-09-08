#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define MYIP "192.168.232.129"

void main(void) {
	int s_socket;
	struct sockaddr_in s_addr;

	s_socket = socket(PF_INET, SOCK_STREAM, 0);

	memset(&s_addr, 0x00, sizeof(s_addr));
	s_addr.sin_addr.s_addr = inet_addr(MYIP);
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(0);

	if(bind(s_socket, (struct sockaddr *)&s_addr, sizeof(s_addr)) == -1) {
			printf("Can not Bind\n");
			return -1;
	}

	listen(s_socket, 0);

	while(1) {
		sleep(2);
	}

	close(s_socket);
}
