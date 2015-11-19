%{
	#include	"y.tab.h"
%}
CMD			[a-zA-z][a-zA-z0-9]*
ARG1 		[\-\/%_.$!@#^&a-zA-z0-9]+
ARG2		[\"|\'][\-\/%_.$!@#^& a-zA-z0-9]+[\"|\']

%%
[ ]*			{return _SPACE;}
[;]				{return _DELIM;}
[>]				{return RIO;}
[<]				{return LIO;}
[|]				{return PIPE;}
{CMD}			{yylval.string = strdup(yytext); return CMD;}
{ARG1}			{yylval.string = strdup(yytext); return ARG;}
{ARG2}			{yylval.string = strdup(yytext); return ARG;}
.				;
%%