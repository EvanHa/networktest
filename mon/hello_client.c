#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define BUFSIZE 1024
#define PORT 9000
//#define IPADDR "192.168.100.5"
#define IPADDR "127.0.0.1" // loop back

void main(void) {
	int c_socket;
	struct sockaddr_in c_addr;
	int len;
	int n;

	char rcvBuffer[BUFSIZE];

	c_socket = socket(PF_INET, SOCK_STREAM, 0);

	memset(&c_addr, 0, sizeof(c_addr));
	c_addr.sin_addr.s_addr = inet_addr(IPADDR);
	c_addr.sin_family = AF_INET;
	c_addr.sin_port = htons(PORT);

	if(connect(c_socket, (struct sockaddr *) &c_addr, sizeof(c_addr)) == -1) {
		printf("Can not connect\n");
		close(c_socket);
		return -1;
	}

	if((n = read(c_socket, rcvBuffer, sizeof(rcvBuffer))) < 0 ) {
		return -1;
	}
	
	int i=0;
	for(i=0;i<n;i++) {
		printf("%c", rcvBuffer[i]);
	}
	printf("\n");
	
	close(c_socket);
}
