# RSHELL
A Basic Linux Shell Implementation

<b>Implemented Internal Commands</b>
- cd
- exit
- help
- history

<b>Features</b>
- Tab completion and command prompt editing   [GNU Readline Library]
- Command History
- Up and down arrows to scroll through the history  [GNU Readline Library]
- Parsing using LEX and YACC tools    [FLEX & BISON]
- Background Jobs
- I/O Redirection

<b>Requirements</b>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
sudo apt-get install flex
sudo apt-get install bison
sudo apt-get install libreadline6 libreadline6-dev
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

<b>How to compile</b>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
gcc shell.c -o rshell -lreadline
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
