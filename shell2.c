#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>



/*Function Prototypes*/
int check_for_error(char*);
int check_for_ie(char*);
void rshell_exit(void);

/*Main Module*/
int main(int argc, char *argv[])
{
	/* Data Declaration */
	char cmd[256];
	int error_flag=0, ie_flag=0;
	while(1){
	/*Get command from user*/
	printf("radhey@Ubuntu:~");
	scanf("%s",cmd);

	/*Validiate command*/
	error_flag = check_for_error(cmd);
	if(error_flag==1)
	{
		printf("Error in command.\n");
	}
	else{
		/*Check for internal or external*/
		ie_flag = check_for_ie(cmd);
		/*Take action*/	
		if(ie_flag==1){
			/*Call procedure*/
			if(strcmp(cmd,"exit")==0){
				rshell_exit();
			}
			if (strcmp(cmd,"cd")==0)
			{
				printf("Command not yet ready.\n");
			}
		}	
		else{
			int childid = fork();
			if (childid==0)
			{
				execlp(cmd,"",NULL);
			}
			else{
				waitpid(childid);
			}
		}
	}
}
	return 0;
}

int check_for_error(char *cmd)
{	
	if(strlen(cmd)>256)
		return 1;
	return 0;
}

int check_for_ie(char *cmd)
{	
	int i;
	char *list[2]={"exit","cd"};
	for (i = 0; i < 2; ++i)
	{
		if (strcmp(list[i],cmd)==0)
		{
			return 1;
		}
	}
	return 0;

}

void rshell_exit(){
	printf("Bye.\n\n");
	exit(0);
}
