#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>

#define PORT 9000

void *func_thread(void *);

main( )
{
	int	c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int	len;

	int sts;
	pthread_t thread_id;


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

	while(1) {
		len = sizeof(c_addr);
		c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);
printf("c_socket : %d\n", c_socket);
		if((sts = pthread_create(&thread_id, NULL, func_thread, (void *)c_socket))!=0) {
			perror("thread create error\n");
			exit(1);
		}

		printf("thread %x is created \n", thread_id);
		sleep(3);
	}
	close(s_socket);
}

void *func_thread(void *arg)
{
	int	n;
	char	rcvBuffer[BUFSIZ];
	int c_socket = (int)arg;
	printf("c_socket : %d\n", c_socket);
	
	while((n=read(c_socket, rcvBuffer, sizeof(rcvBuffer))) !=0)
	{
		rcvBuffer[n] = '\0';
		printf("%s", rcvBuffer);
		write(c_socket, rcvBuffer, n);
	}
	close(c_socket);
}

