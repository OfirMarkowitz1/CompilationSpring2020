%{

#include <stdlib.h>
#include "Node.hpp"
#include "parser.tab.hpp"
#include "hw3_output.hpp"

using namespace std;

void exitWithError();

void setRelopNode();

void setBinopNode();

void setStringNode();

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
not                                                     return NOT;
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
==|!=                                                   setRelopNode(); return EQUALITY;
\<|\>|\<=|\>=                                           setRelopNode(); return RELATIONAL;
\+|\-                                                   setBinopNode(); return ADDITIVE;
\*|\/                                                   setBinopNode(); return MULTIPLICATIVE;
[a-zA-Z][a-zA-Z0-9]*                                    yylval = make_shared<IdentifierNode>(yylineno, yytext); return ID;
0|[1-9][0-9]*                                           yylval = make_shared<NumNode>(yylineno, atoi(yytext)); return NUM;
\"([^\n\r\"\\]|\\[rnt"\\])+\"                           setStringNode(); return STRING;
[ \t\n\r]                                               ;
\/\/[^\r\n]*[ \r|\n|\r\n]?                              ;
.                                                       exitWithError();

%%

void exitWithError()
{
    output::errorLex(yylineno);
    exit(1);
}

void setRelopNode()
{
	yylval = make_shared<RelopNode>(yylineno, yytext);
}

void setBinopNode()
{
	yylval = make_shared<BinopNode>(yylineno, yytext[0]);
}

void setStringNode()
{
	string str(yytext);

	str = str.substr(1, str.size() - 2);

	yylval = make_shared<StringNode>(yylineno, str);
}
