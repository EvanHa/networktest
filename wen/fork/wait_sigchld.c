#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

void do_child(void);
void do_parent(void);

void
sighandler(int n)
{
	int stat, chld_pid;
	sigset_t newset, oldset;

	chld_pid = wait(&stat);
	printf("signal ID [%d]\n", n);
	

	if(WIFEXITED(stat)) {
		printf("normal   termination, exit status [%d]\n", WEXITSTATUS(stat));
	} else if(WIFSIGNALED(stat)) {
		printf("abnormal termination, signal number [%d]\n", WTERMSIG(stat));
	} else if(WIFSTOPPED(stat)) {
		printf("child stopped, signal number [%d]\n", WSTOPSIG(stat));
	}
	
	exit(1);
}

int
main( )
{
	int pid;
	struct sigaction sigset, oldset;

	sigset.sa_handler = sighandler;
	sigset.sa_flags = 0;
	sigemptyset(&sigset.sa_mask);
	sigaction(SIGCHLD, &sigset, &oldset);

	if((pid = fork( )) == 0) {
		do_child( );
	} else if(pid > 0) {
		do_parent( );
	}
}

void
do_child( )
{
	exit(100);
}

void do_parent( )
{
	sleep(10);
}

