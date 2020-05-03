%{

#include <stdlib.h>
#include "parser.tab.hpp"
#include "output.hpp"

void exitWithError();

%}

%option yylineno
%option noyywrap

%%

void                                                    return VOID;
int                                                     return INT;
byte                                                    return BYTE;
b                                                       return B;
bool                                                    return BOOL;
and                                                     return AND;
or                                                      return OR;
true                                                    return TRUE;
false                                                   return FALSE;
return                                                  return RETURN;
if                                                      return IF;
else                                                    return ELSE;
while                                                   return WHILE;
break                                                   return BREAK;
continue                                                return CONTINUE;
;                                                       return SC;
,                                                       return COMMA;
\(                                                      return LPAREN;
\)                                                      return RPAREN;
\{                                                      return LBRACE;
\}                                                      return RBRACE;
=                                                       return ASSIGN;
==|!=|<|>|<=|>=                                         return RELOP;
\+|\-|\*|\/                                             return BINOP;
[a-zA-Z][a-zA-Z0-9]*                                    return ID;
0|[1-9][0-9]*                                           return NUM;
\"([^\n\r\"\\]|\\[rnt"\\])+\"                           return STRING;
[ \t\n\r]                                               ;
\/\/[^\r\n]*[ \r|\n|\r\n]?                              ;
.                                                       exitWithError();

%%

void exitWithError()
{
    output::errorLex(yylineno);
    exit(1);
}
