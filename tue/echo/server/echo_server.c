#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <dirent.h>

#define PORT 9000

char	err_1[ ]="Directory Error";
char	rBuffer[BUFSIZ];

main( )
{
	int	c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int	len, length;
	int	n, i;
	char	*temp;

	DIR *dp;
	struct dirent *dir;

	s_socket = socket(PF_INET, SOCK_STREAM, 0);

	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(PORT);

	if(bind(s_socket, (struct sockaddr *) &s_addr, sizeof(s_addr)) == -1) {
		printf("Can not Bind\n");
		return -1;
	}

	if(listen(s_socket, 5) == -1) {
		printf("listen Fail\n");
		return -1;
	}	
	
	len = sizeof(c_addr);
	c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);

	while(1) {
		length = 0;
		temp = rBuffer;
		while((n = read(c_socket, temp, 1)) > 0) {
			if(*temp == '\r') continue;
			if(*temp == '\n') break;
			if(*temp == '\0') break;

			if(length == BUFSIZ) break;
			temp++; length++;
		}

		rBuffer[length] = '\0';

		if(strcmp(rBuffer, "quit")) {
			printf("recevied: %s\n", rBuffer);
			if ((n=write(c_socket, rBuffer, strlen(rBuffer)+1))<0) {
				printf("write error\n");
				break;
			}
		} else {
			break;
		}
	}
	close(c_socket);
	close(s_socket);
}
