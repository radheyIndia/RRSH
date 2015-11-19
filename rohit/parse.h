#define MAX_VAR_NUM 10
#define PIPE_MAX_NUM 10
#define FILE_MAX_SIZE 40

struct commandType {
  char *command;
  char *VarList[MAX_VAR_NUM];
  int VarNum;
};

/* parsing information structure */
typedef struct {
  int   boolInfile;		       /* boolean value - infile specified */
  int   boolOutfile;		       /* boolean value - outfile specified */
  int   boolBackground;		       /* run the process in the background? */
  struct commandType CommArray[PIPE_MAX_NUM];
  int   pipeNum;
  char  *inFile;	       /* file to be piped from */
  char  *outFile;	       /* file to be piped into */
} parseInfo;