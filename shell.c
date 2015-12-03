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
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAX_BG_JOBS 100
struct process{
	int pid;
	char *pname;
};
typedef struct process ps;

ps background_jobs[MAX_BG_JOBS];
int bg_jobs_count = 0;

#include "lex.yy.c"
#include "y.tab.c"
#include "internal_commands.c"
#include "helpers.c"

int main(int argc, char **argv) {
	char *cmdLine, *host;
	int builtInCommandId, parsingStatus;
	parseInfo *info;
	struct passwd *passwd;
	uid_t uid;
	size_t len=HOST_NAME_MAX;
	pid_t childPid;

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
       	if(cmdLine && *cmdLine){
       		add_history(cmdLine);
       		recordHistory(cmdLine);
       	}
	   	scan_string(cmdLine);
	   	parsingStatus = yyparse();
	   	/*Parsing Failed*/
	   	if(parsingStatus != 0 || pInfo == NULL){
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
				if(info->boolBackground == 1){
					printf("background process started: \n");
					addBgProcess(childPid, cmdLine);
				}
				else{
					waitpid(childPid, (int *)NULL, WUNTRACED | WCONTINUED);
				}
			}
	   	}
    }
}