#define MAX_VAR_NUM 10
#define PIPE_MAX_NUM 10
#define FILE_MAX_SIZE 40
#define MAXLINE 81

/*Single command structure*/
struct commandType {
  char *command;                /*Command*/
  char *VarList[MAX_VAR_NUM];   /*Arguments String Array*/
  int VarNum;                   /*Arguments count*/
};

/* parsing information structure */
typedef struct {
  int   boolInfile;          /* boolean value - infile specified */
  int   boolOutfile;           /* boolean value - outfile specified */
  int   boolBackground;          /* run the process in the background? */
  struct commandType CommArray[PIPE_MAX_NUM];
  int   pipeNum;
  char  inFile[FILE_MAX_SIZE];         /* file to be piped from */
  char  outFile[FILE_MAX_SIZE];        /* file to be piped into */
} parseInfo;

void init_info(parseInfo *p) {
  p->boolInfile = 0;
  p->boolOutfile = 0;
  p->boolBackground = 0;
  p->pipeNum = 0;
  p->inFile[0] = '\0';
  p->outFile[0] = '\0';
}

/* parse a single command */
void parse_command(char * cmdline, struct commandType *comm) {
  char *cmd;
  int i=0;
  comm->command = (char *)malloc(MAXLINE);
  comm->VarNum = 0;
  cmd = strtok(cmdline, " ");
  while(cmd != NULL)
  {
    if(i==0){
      //command
      strcpy(comm->command, cmd);
    }
    else{
      //args
      comm->VarList[comm->VarNum] = (char *)malloc(MAXLINE);
      strcpy(comm->VarList[comm->VarNum], cmd);
      comm->VarNum++;
    }
    cmd = strtok(NULL, " ");
    i++;
  }
}

/*   parse commandline for space separated commands */
parseInfo *parse (char *cmdline) {
  parseInfo *Result;
  char command[MAXLINE];
  int com_pos = -1;
  
  if (cmdline[-1] == '\n' && cmdline[-1] == '\0')
    return NULL;
  
  Result = (parseInfo *)malloc(sizeof(parseInfo));
  init_info(Result);
  parse_command(cmdline, &Result->CommArray[Result->pipeNum]);
  
  return Result;
}

/* prints out parse struct */
void print_info (parseInfo *info) {
  int i=0;
  printf("command: %s\n", info->CommArray[0].command);
  while(i < info->CommArray[0].VarNum)
  {
    printf("arg%d: %s\n",i+1,info->CommArray[0].VarList[i]);
    i++;
  }
}  

/*  free memory used in parseInfo */
void free_info (parseInfo *info) {
  int i=0,j;
  while(i<PIPE_MAX_NUM){
    free(info->CommArray[i].command);
    j=0;
    while(j<info->CommArray[i].VarNum){
      free(info->CommArray[i].VarList[j]);
      j++;
    }
    i++;
  }
  free(info);
}