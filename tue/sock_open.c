#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

main( )
{
	int	tcpSd1, tcpSd2;
	int	udpSd1, udpSd2;

	if((tcpSd1 = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		perror("socket( )-tcpSd1");
	} else
	printf("TCP socket open = %d\n", tcpSd1);

	if((udpSd1 = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		perror("socket( )-udpSd1");
	} else
	printf("UDP socket open = %d\n", udpSd1);

	if((tcpSd2 = socket(PF_INET, SOCK_STREAM, IPPROTO_UDP)) < 0) {
		perror("socket( )-tcpSd2");
	} else
		printf("TCP socket open = %d\n", tcpSd2);

	if((udpSd2 = socket(PF_INET, SOCK_DGRAM, IPPROTO_TCP)) < 0) {
		perror("socket( )-udpSd2");
	} else
	printf("UDP socket open = %d\n", udpSd2);

	close(tcpSd1);
	close(udpSd1);
	close(tcpSd2);
	close(udpSd2);
}
