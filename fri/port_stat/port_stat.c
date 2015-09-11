#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

main(int argc, char **argv)
{
	int	c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int	len;
	char	greeting[ ] = "TCP state test";

	if(argc != 2) {
		fprintf(stderr, "usage: TCPserver port_number\n");
		exit -1;
	}

	s_socket = socket(PF_INET, SOCK_STREAM, 0);

	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(atoi(argv[1]));

	if(bind(s_socket, (struct sockaddr *) &s_addr, sizeof(s_addr)) == -1) {
		printf("Can not Bind\n");
		return -1;
	}

	if(listen(s_socket, 5) == -1) {
		printf("listen Fail\n");
		return -1;
	}

	while(1) {
		len = sizeof(c_addr);
		c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);

		write(c_socket, greeting, strlen(greeting));
	}
}
