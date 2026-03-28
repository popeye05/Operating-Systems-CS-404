#include<stdlib.h>
#include<stdio.h>
//Important For Process Creation
#include <unistd.h>
#include <sys/types.h>


//Main Code
int main()
{
	printf("Hello  World, PID: %d", (int) getpid());
	int rc = fork(); //fork returns different value for different processes
	if(rc<0) exit(1); //Error
	else if(rc==0)
		printf("\n Hello I'm a child"); //Child Code
	else
	 	printf("\n Hello I'm a Parent of %d",rc); //Parent Code
}
