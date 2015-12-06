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

	if(info->CommArray[0].VarNum == 0){
		//No flags
		if(list){
			while(list[i]){
				printf("%s \n", list[i]->line);
				i++;
			}
		}
	}
	else{
		if(strcmp(info->CommArray[0].VarList[0], "-t") == 0){
			if(list){
				while(list[i]){
					printf("%s \t %s \n", list[i]->line, list[i]->timestamp);
					i++;
				}
			}
		}
		else{
			printf("Invalid option. Please type \n help history \n to see more options.\n");
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
	HIST_ENTRY **list = NULL;
	FILE *fp;
	int i=0;
	
	printf("Initializing shell termination...\n");	
	list = history_list();
	if(list){
		fp = fopen("files/history.txt","a");
		if(!fp){
			printf("Error in saving command history.\n");
		}
		else{
			while(list[i]){
				fprintf(fp,"%s \n %s \n", list[i]->line, list[i]->timestamp);
				i++;
			}
			fclose(fp);
		}
	}
	printf("Shell Terminated.\n\n");
	exit(0);
}

int rshell_kill(parseInfo *info){}

int rshell_help(parseInfo *info){
	FILE *fp;
	char ch;
	char *file_name;
	if(info->CommArray[0].VarNum == 0){
		//No flags
		fp = fopen("help/help_index.txt","r");
		if(!fp){
			printf("No help index found.\n");
		}
		else{
			ch = fgetc(fp);
			while(ch != EOF){
				printf("%c", ch);
				ch = fgetc(fp);
			}
			fclose(fp);
		}
	}
	else{
		file_name = (char *)malloc(20);
		sprintf(file_name, "help/%s.txt",info->CommArray[0].VarList[0]);
		fp = fopen(file_name, "r");
		if(!fp){
			printf("No help file found.\n");
		}
		else{
			ch = fgetc(fp);
			while(ch != EOF){
				printf("%c", ch);
				ch = fgetc(fp);
			}
			fclose(fp);
		}
		free(file_name);
	}
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
