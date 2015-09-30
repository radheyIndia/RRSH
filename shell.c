/**
 *  shell.c
 */
 #include <stdio.h>
 #include <string.h>
 #include <malloc.h>
 #include "parse.c"

 void printPrompt(){
 	printf("user@machine:");
 }

 char *readline(){
 	char *cmd;
 	cmd = (char *)malloc(sizeof(char)*256);
 	scanf("%[^\n]%*c",cmd);
 	return cmd;
 }
 int main(int argc, char **argv) {
 	char *cmdLine;
 	char *info;
    while(1) {
    	printPrompt();
       	cmdLine = readline();
       	parse(cmdLine);
//       	info  = parse(cmdLine);
  //     	print_info(info);
    }

    //free_info(info);
 }
