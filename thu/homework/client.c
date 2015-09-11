#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORT 9000
#define IPADDR "192.168.100.5"

main( )
{
	int	c_socket;
	struct sockaddr_in c_addr;
	int	len;

	char	sndBuffer[BUFSIZ], rcvBuffer[BUFSIZ];

	int	n;
	int	n_left, n_recv;

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

	while(1) {
		if((n = read(0, sndBuffer, BUFSIZ)) > 0) {
			sndBuffer[n] = '\0';
			if(!strcmp(sndBuffer, "5\n")) break;

			if((n = write(c_socket, sndBuffer, strlen(sndBuffer))) < 0) {
				return (-1);
			}
		}
	}
	close(c_socket);
}
