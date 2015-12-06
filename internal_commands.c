int rshell_cd(parseInfo *info){
	int temp;
	if(info->CommArray[0].VarList[0]){		
		temp = chdir(info->CommArray[0].VarList[0]);
		if(temp == -1){
			printf("Error: Invalid directory name.\n");
		}
	}
	else
		printf("Error: Invalid directory name.\n");
}

int rshell_history(parseInfo *info){
	HIST_ENTRY **list = NULL;
	char *cmd = NULL;
	int i=0;
	list = history_list();
	
	if(list){
		while(list[i]){
			printf("%s \t %s \n", list[i]->timestamp, list[i]->line);
			i++;
		}
	}
}

int rshell_jobs(parseInfo *info){
	int i=0;
	int status, result;
	if(bg_jobs_count == 0){
		return 0;
	}
	else{
		while(i<bg_jobs_count){
			result = waitpid(background_jobs[i].pid, &status, WNOHANG);
			if(result == -1){
				printf("Error in getting status.\n");
			}else if(result == 0){
				printf("%d\n", background_jobs[i].pid);
			}else if(result == background_jobs[i].pid){
				printf("Finished\n");
			}
		}
	}
}

int rshell_exit(parseInfo *info){
	printf("Initializing shell termination...\n");
	//free_info(info);
	printf("Shell Terminated.\n\n");
	exit(0);
}

int rshell_kill(parseInfo *info){}

int rshell_help(parseInfo *info){
	printf("RSHELL HELP INDEX\n");
	printf("Command\t\tSyntax\t\tDescription\n");
	printf("cd\t| cd <directory>\t| To change the current directory\n");
	printf("history\t| history\t| To view a list of previously used commands\n");
	printf("jobs\t| cd jobs\t| To view a list of active jobs\n");
	printf("exit\t| cd exit\t| To  exit the shell\n");

}

int rshell_which(parseInfo *info){
	printf("We are in\n");
	if (info->CommArray[0].VarNum > 0){
		printf("%s\n", getenv(info->CommArray[0].VarList[0]));
	}
}
int rshell_pushd(parseInfo *info){}
int rshell_popd(parseInfo *info){}
int rshell_alias(parseInfo *info){}
int rshell_fg(parseInfo *info){}
int rshell_bg(parseInfo *info){}
int rshell_printenv(parseInfo *info){}
int rshell_setenv(parseInfo *info){}
