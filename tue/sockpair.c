#include <stdio.h>
#include <sys/socket.h>

main( )
{
	int	sd[2], result;
	int	n1, n2;
	char	buf[BUFSIZ];
	char	data[ ] = "This is from sd[0]";

	result = socketpair(PF_LOCAL, SOCK_STREAM, 0, sd);

	n1 = write(sd[0], data, strlen(data));
	printf("[send] %s\n", data);

	n2 = read(sd[1], buf, BUFSIZ);
	buf[n2] = '\0';
	printf("[received] %s\n", buf);

	close(sd[0]);
	close(sd[1]);
}
