/******************************************************************* 
  Name: Jonathan             NetID: jh4482
  Course: CSE 4714              
  Part 2: Parsing
  Programming Environment: Visual Studio Code with WSL Ubuntu 20.04 using g++
  Purpose of File: Contains the rules for the regular expressions to test.

*******************************************************************/
%option yylineno
%option noyywrap
%{
#include "lexer.h"
int line_number = 1;

%}

%%

 /* Regex for punctuation */
;                { return TOK_SEMICOLON; }
:                { return TOK_COLON; }
\(               { return TOK_OPENPAREN; }
\)               { return TOK_CLOSEPAREN; }
\{               { return TOK_OPENBRACE; }
\}               { return TOK_CLOSEBRACE; }

 /* Regex for operators */
\+              { return TOK_PLUS; }
\-              { return TOK_MINUS; }
\*              { return TOK_MULTIPLY; }
\/              { return TOK_DIVIDE; }
:=              { return TOK_ASSIGN; }
=               { return TOK_EQUALTO; }
\<              { return TOK_LESSTHAN; }
\>              { return TOK_GREATERTHAN; }
\<\>            { return TOK_NOTEQUALTO; }
MOD             { return TOK_MOD; }
NOT             { return TOK_NOT; }
OR              { return TOK_OR; }
AND             { return TOK_AND; }

 /* Regex for keyword */
BEGIN           { return TOK_BEGIN; }
BREAK           { return TOK_BREAK; }
CONTINUE        { return TOK_CONTINUE; }
DOWNTO          { return TOK_DOWNTO; }
ELSE            { return TOK_ELSE; }
END             { return TOK_END; }
FOR             { return TOK_FOR; }
IF              { return TOK_IF; }
LET             { return TOK_LET; }
PROGRAM         { return TOK_PROGRAM; }
READ            { return TOK_READ; }
THEN            { return TOK_THEN; }
TO              { return TOK_TO; }
VAR             { return TOK_VAR; }
WHILE           { return TOK_WHILE; }
WRITE           { return TOK_WRITE; }

 /* Regex for specifiers */
INTEGER         { return TOK_INTEGER; }
REAL            { return TOK_REAL; }

 /* Regex for abstractions */
[0-9]+                                                              { return TOK_INTLIT; }
[0-9]+\.[0-9]+                                                       { return TOK_FLOATLIT; }
[A-Z][A-Z0-9]*                                                      { if (yyleng > 8)  return TOK_UNKNOWN;
                                                                      else return TOK_IDENT; }
"'"[ A-Z0-9\t\n\+\*\/\<\>\;\:\=\(\)\{\}\-]*"'"                      { if (yyleng > 82)  return TOK_UNKNOWN;
                                                                      else { return TOK_STRINGLIT;}  }
                                                       

 /* Count lines */
\n      { ++line_number; }

[ \t\r\n]+   /*nop*/

 /* Found an unknown character */

.         { return TOK_UNKNOWN; }

 /* Recognize end of file */

<<EOF>>   { return TOK_EOF; }


