/*
 * rshell.c
 * Author: Rohit
 * Session: 2015 - 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include <sys/types.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "lex.yy.c"
#include "y.tab.c"
#include "internal_commands.c"
//#include "helper.c"

char *builtInCommands[] = {"cd","history","jobs","exit","kill","help","which","pushd","popd","alias","fg","bg","printenv","setenv"};
/*Check for internal commands*/

int isBuiltInCommand(parseInfo *info){
	char *temp = builtInCommands[0];
	int i=0;
	while(temp!=NULL){
		if(strcmp(info->CommArray[0].command, temp)==0)
			return i;
		i++;
		temp = builtInCommands[i];
	}
	return -1;
}

/*Check for background job*/
int isBackgroundJob(parseInfo *info){
	return 0;
}

/*Execute Build in command*/
int executeBuiltInCommand(parseInfo *info, int builtInCommandId){
	switch(builtInCommandId){
		case 0: rshell_cd(info);
				break;
		case 1: rshell_history(info);
				break;
		case 2: rshell_jobs(info);
				break;
		case 3: rshell_exit(info);
				break;
		case 4: rshell_kill(info);
				break;
		case 5: rshell_help(info);
				break;
		case 6: rshell_which(info);
				break;
		case 7: rshell_pushd(info);
				break;
		case 8: rshell_popd(info);
				break;
		case 9: rshell_alias(info);
				break;
		case 10: rshell_fg(info);
				break;
		case 11: rshell_bg(info);
				break;
		case 12: rshell_printenv(info);
				break;
		case 13: rshell_setenv(info);
				break;
		default: printf("Invalid Command.\n");
	}
}

/*Execute Command*/
int executeCommand(parseInfo *info){
	int childStatus;
    FILE *fp;
    if(info->boolInfile == 1){
    	fp = fopen(info->inFile, "r");
    	if(fp){
    		dup2(fileno(fp), 0);
    	}
    	else{
    		printf("InFile: %s not found.\n", info->inFile);
    	}
    }
    if(info->boolOutfile == 1){
    	fp = fopen(info->outFile, "w");
    	if(fp){
    		dup2(fileno(fp), 1);
    	}
    	else{
    		printf("OutFile: %s not found.\n", info->outFile);
    	}    	
    }
    switch(info->CommArray[0].VarNum){
    	case 0: childStatus = execlp(info->CommArray[0].command, "", NULL);
       			break;
       	case 1: childStatus = execlp(info->CommArray[0].command, info->CommArray[0].command, info->CommArray[0].VarList[0], NULL);
       			break;
       	case 2: childStatus = execlp(info->CommArray[0].command, info->CommArray[0].command, info->CommArray[0].VarList[0], info->CommArray[0].VarList[1], NULL);
       			break;
       	case 3: childStatus = execlp(info->CommArray[0].command, info->CommArray[0].command, info->CommArray[0].VarList[0], info->CommArray[0].VarList[1], info->CommArray[0].VarList[2], NULL);
       			break;
       	case 4: childStatus = execlp(info->CommArray[0].command, info->CommArray[0].command, info->CommArray[0].VarList[0], info->CommArray[0].VarList[1], info->CommArray[0].VarList[2], info->CommArray[0].VarList[3], NULL);
       			break;
       	case 5: childStatus = execlp(info->CommArray[0].command, info->CommArray[0].command, info->CommArray[0].VarList[0], info->CommArray[0].VarList[1], info->CommArray[0].VarList[2], info->CommArray[0].VarList[3], info->CommArray[0].VarList[4], NULL);
       			break;
       	default: printf("Number of arguments exceeds the limit.\n");
    }
    if (childStatus == -1){
    	printf("Command not found.\n");
       	exit(0);
    }
}

/*Prompt String*/
char *printPrompt(){
	char *prompt,*cwd;
	size_t cwd_len=256;
	cwd=(char *)malloc(cwd_len);
	prompt=(char *)malloc(cwd_len+32);

	/*Get current working directory*/
	getcwd(cwd, cwd_len);

	sprintf(prompt,"$%s/> ",cwd);
	free(cwd);
	return prompt;
}

int main(int argc, char **argv) {
	char *cmdLine, *host;
	int childPid, builtInCommandId, parsingStatus;
	parseInfo *info;
	struct passwd *passwd;
	uid_t uid;
	size_t len=HOST_NAME_MAX;

	host=(char *)malloc(len);

	/*Get username*/
	uid = getuid();
	passwd = getpwuid(uid);

	/*Get hostname*/
	gethostname(host,len);

	printf("\n\n************Welcome to RSHELL***********\n\n");
	printf("User: %s\nHost: %s\n\n",passwd->pw_name,host);

    while(1){
    	if (pInfo)
    	{
    		free(pInfo);
    		pInfo = (parseInfo *)NULL;
    	}
    	pInfo = (parseInfo *)malloc(sizeof(parseInfo));
    	if(cmdLine){
    		free(cmdLine);
    		cmdLine = (char *)NULL;
    	}
    	cmdLine = readline(printPrompt());
       	if(cmdLine && *cmdLine)
       		add_history(cmdLine);
	   	scan_string(cmdLine);
	   	parsingStatus = yyparse();
	   	/*Parsing Failed*/
	   	if(parsingStatus != 0){
	   		continue;
	   	}
	   	info = pInfo;
       	
       	builtInCommandId = isBuiltInCommand(info);
       	if(builtInCommandId != -1){
       		executeBuiltInCommand(info, builtInCommandId);
       	}
       	else{
	       	childPid = fork();
    	   	if (childPid == 0){
       			executeCommand(info); //calls execvp 
			}
			else{
				if(isBackgroundJob(info)){
					//record in list of background jobs
				}
				else{
					wait();
				}
			}
	   	}
    }
}