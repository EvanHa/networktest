#include <stdio.h>

int main()
{
	int retval;
	retval = syscall(353);
	printf("\n ret = %d\n",retval);
	
	retval = syscall(354);
	printf("\n ret = %d\n",retval);

	return 0;
}

