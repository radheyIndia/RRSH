/*Header Files*/


/* COmmand structure*/
struct my_cmd{
	char *command;	//command name
	char *args[];	//list of arguments
	int no_of_arguments;	//number of arguments
	int command_type;	//internal or external
	int is_background;	//background process or not
};

struct my_cmd c1;
c1.command = "whatever";
c1.args[0] = "-l";
c1.args[1] = "second argument";
....
...



int error_flag=0, ie_flag=0;

/*Function Prototypes*/
char *get_command(void);
int check_for_error(char*);
int check_for_ie(char*);

/*Main Module*/
int main(int argc, char const *argv[])
{
	/* Data Declaration */
	char cmd[256];
	
	/*Get command from user*/
	cmd = get_command();

	/*Validiate command*/
	error_flag = check_for_error(cmd);
	if(error_flag==0)
	{
		printf("Error.\n");
	}
	else{
		/*Check for internal or external*/
		ie_flag = check_for_ie(cmd);
		/*
			ie_flag=1 ==> internal
			ie_flag=0 ==> external
		*/
		/*Take action*/	
		if(ie_flag==1){
			/*Call procedure*/
		}	
		else{
			/*
			Fork
			Exec
			*/
		}
	}

	return 0;
}

char *get_command(void)
{
	printf("Enter the command:-");
	scanf("%s",cmd);
}

int check_for_error(char *cmd)
{
	if(strlen(cmd)<=256)
	{
		error_flag=1;
	}
	return 1;
}

int check_for_ie(char *cmd)
{		
	FILE *fp;
	fp=fopen("internal_command.txt","r");
		int result=access(argv[1],F_OK);
		while(!feof(fp))
		{
			if(result==0)
			{
				ie_flag=1;
			}
			
		}
	fclose(fp);	

}































