#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

main(int argc, char *argv[ ])
{
	int	sd;
	struct sockaddr_in s_addr, c_addr;
	char	buff[BUFSIZ];
	int	n, n_recv;
	int	addr_len;

	sd = socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&s_addr, sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr.sin_port = htons(9200);

	if(bind(sd, (struct sockaddr *) &s_addr, sizeof(s_addr)) < 0){
		fprintf(stderr, "bind( ) error");
		exit(-2);
	}

	while(1) {
	
		fprintf(stderr, "waiting\n");
	
		addr_len = sizeof(c_addr);
		if((n_recv = recvfrom(sd, buff, sizeof(buff), 0, (struct sockaddr *)&c_addr, &addr_len)) < 0) {
			fprintf(stderr, "recvfrom( ) error");
			exit(-3);
		}
#if 0
		if((n = sendto(sd, buff, n_recv, 0, (struct sockaddr *) &c_addr, sizeof(c_addr))) < 0){
			fprintf(stderr, "sendto( ) error");
			exit(-3);
		}
#endif
	}
	close(sd);
}
