#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <limits.h>
#include <signal.h>
#include <fcntl.h>
#include <termios.h>
#include <pwd.h>
#include <time.h>

#include <sys/types.h>
#include <sys/wait.h>

#include <readline/readline.h>
#include <readline/history.h>

static pid_t MY_PID;
static pid_t MY_PGID;
static int MY_TERMINAL;
static struct termios MY_TMODES;


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

void init(){
	MY_PID = getpid();
	MY_TERMINAL = STDIN_FILENO;
	if (isatty(MY_TERMINAL)){
		MY_PGID = getpgrp();
		while(tcgetpgrp(MY_TERMINAL) != MY_PGID)
			kill(MY_PID, SIGTTIN);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGTTOU, SIG_IGN);
		signal(SIGTTIN, SIG_IGN);
		signal(SIGTSTP, SIG_IGN);
		signal(SIGINT, SIG_IGN);
//		signal(SIGCHLD, &signalHandler_child);

		setpgid(MY_PID, MY_PID);
		MY_PGID = getpgrp();
		if(MY_PID != MY_PGID){
			printf("Error: the shell is not process group leader\n");
			exit(EXIT_FAILURE);
		}
		if(tcsetpgrp(MY_TERMINAL, MY_PGID) == -1)
			tcgetattr(MY_TERMINAL, &MY_TMODES);
	}
	else{
		printf("Could not make shell interactive.\n");
		exit(EXIT_FAILURE);
	}
}
void welcomeScreen(){
	char *host;
	struct passwd *passwd;
	uid_t uid;
	size_t len = HOST_NAME_MAX;

	host=(char *)malloc(len);

	/*Get username*/
	uid = getuid();
	passwd = getpwuid(uid);

	/*Get hostname*/
	gethostname(host,len);
	printf("rshell initiated.\n");
	printf("User: %s\nHost: %s\n\n",passwd->pw_name,host);	
	free(host);
}

int main(int argc, char **argv) {
	char *cmdLine;
	int builtInCommandId, parsingStatus;
	parseInfo *info;
	pid_t childPid;
	time_t clk;
    cmdLine = NULL;
	/*Initialize the shell*/
	init();
	/*Print the welcome screen*/
	welcomeScreen();
    while(1){
    	if (pInfo){
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
       		clk = time(NULL);
       		add_history_time(ctime(&clk));
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