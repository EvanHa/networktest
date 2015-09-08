#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <strings.h>

#define PORT 9000
#define IPADDR "127.0.0.1"

char	buffer[BUFSIZ];

main( )
{
	int	c_socket;
	struct sockaddr_in c_addr;
	int	len;

	char	rcvBuffer[BUFSIZ];

	char	*temp;
	int	length = 0;

	int	n;
	
	c_socket = socket(PF_INET, SOCK_STREAM, 0);

	memset(&c_addr, 0, sizeof(c_addr) );
	c_addr.sin_addr.s_addr = inet_addr(IPADDR);
	c_addr.sin_family = AF_INET;
	c_addr.sin_port = htons(PORT);

	if(connect(c_socket, (struct sockaddr *) &c_addr, sizeof(c_addr)) == -1) {
		printf("Can not connect\n");
		close(c_socket);
		return -1;
	}

	//TODO
	//1. file name is input data
	//2. file which has name by user's input is opend
	//3. read file's data
	//4. send data
	//5. close socket
	//6. close file

	scanf("%s", buffer);
	buffer[strlen(buffer)] = '\0';
	if((n = write(c_socket, buffer, strlen(buffer)+1)) < 0) {
		printf("write error\n");
		exit(-1);
	}

	temp = rcvBuffer;
	while((n = read(c_socket, temp, 1)) > 0) {
		if(length == BUFSIZ) break;
		temp++; length++;
	}

	rcvBuffer[length] = '\0';
	
	printf("received Data : %s\n", rcvBuffer);

	close(c_socket);
}
