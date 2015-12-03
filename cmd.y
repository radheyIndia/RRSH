%{
#include "parse.h"

parseInfo *pInfo;
int i=0;

/*Handle Errors*/
void yyerror(const char *str){}


int yywrap(){return 1;}
%}

%token _SPACE _DELIM RIO LIO PIPE BG

%union{	int number; char *string;}

%token <string> CMD
%token <string> ARG
%type <string> args
%type <string> arg
%type <number> io

%start commands

%%
commands: command{
			pInfo->CommArray[0].VarNum = i;
			i = 0;
			return 0;
			}
	| commands _DELIM command{return 0;};
command: CMD{
			pInfo->CommArray[0].command = strdup($1);
			pInfo->boolBackground = 0;
			}
	| CMD _SPACE BG{
			pInfo->CommArray[0].command = strdup($1);
			pInfo->boolBackground = 1;
			}
	| CMD _SPACE args{
			pInfo->CommArray[0].command = strdup($1);
			pInfo->boolBackground = 0;
			} 
	| CMD _SPACE args _SPACE BG{
			pInfo->CommArray[0].command = strdup($1);
			pInfo->boolBackground = 1;
			} 
	| CMD _SPACE io _SPACE arg{
			pInfo->CommArray[0].command = strdup($1);
			if(pInfo->boolInfile)
				pInfo->inFile = strdup($5);
			else
				pInfo->outFile = strdup($5);
			pInfo->boolBackground = 0;
			}
	| CMD _SPACE io _SPACE arg _SPACE BG{
			pInfo->CommArray[0].command = strdup($1);
			if(pInfo->boolInfile)
				pInfo->inFile = strdup($5);
			else
				pInfo->outFile = strdup($5);
			pInfo->boolBackground = 1;
			};
args: args _SPACE arg{
			pInfo->CommArray[0].VarList[i++] = strdup($1);
			}
	| arg{
			pInfo->CommArray[0].VarList[i++] = strdup($1);
			};
arg: CMD{$$ = $1;}
 	| ARG{$$ = $1;};
io: RIO{pInfo->boolOutfile = 1; pInfo->boolInfile = 0;}
	| LIO{pInfo->boolInfile = 1; pInfo->boolOutfile = 0;};
%%
void scan_string(const char* str){
	yy_switch_to_buffer(yy_scan_string(str));
}