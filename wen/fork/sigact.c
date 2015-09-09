#include <stdio.h>
#include <signal.h>

struct sigaction act_new;
struct sigaction act_old;

void sig_handler(int signo)
{
	printf("First Ctrl-C pressed !!\n");
	sigaction(SIGINT, &act_old, NULL);
}

int main(void)
{
	act_new.sa_handler = sig_handler;
	sigemptyset(&act_new.sa_mask);
	act_new.sa_flags = 0;

	sigaction(SIGINT, &act_new, &act_old);
	while(1)
	{
		printf("sigaction test\n");
		sleep(1);
	}
}
