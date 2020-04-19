{\rtf1\ansi\ansicpg1252\cocoartf2512
\cocoatextscaling0\cocoaplatform0{\fonttbl\f0\fmodern\fcharset0 Courier;}
{\colortbl;\red255\green255\blue255;\red0\green0\blue0;}
{\*\expandedcolortbl;;\cssrgb\c0\c0\c0;}
\paperw11900\paperh16840\margl1440\margr1440\vieww10800\viewh8400\viewkind0
\deftab720
\pard\pardeftab720\partightenfactor0

\f0\fs24 \cf2 \expnd0\expndtw0\kerning0
\outl0\strokewidth0 \strokec2 #include <iostream>\
#include "output.hpp"\
#include <sstream>\
\
using namespace std;\
\
const string output::rules[] = \{\
	"Program -> Funcs",\
	"Funcs -> epsilon",\
	"Funcs -> FuncDecl Funcs",\
	"FuncDecl -> RetType ID LPAREN Formals RPAREN LBRACE Statements RBRACE",\
	"RetType -> Type",\
	"RetType ->  VOID",\
	"Formals -> epsilon",\
	"Formals -> FormalsList",\
	"FormalsList -> FormalDecl",\
	"FormalsList -> FormalDecl COMMA FormalsList",\
	"FormalDecl -> Type ID",\
	"Statements -> Statement",\
	"Statements -> Statements Statement",\
	"Statement -> LBRACE Statements RBRACE",\
	"Statement -> Type ID SC",\
	"Statement -> Type ID ASSIGN Exp SC",\
	"Statement -> ID ASSIGN Exp SC",\
	"Statement -> Call SC",\
	"Statement -> RETURN SC",\
	"Statement -> RETURN Exp SC",\
	"Statement -> IF LPAREN Exp RPAREN Statement",\
	"Statement -> IF LPAREN Exp RPAREN Statement ELSE Statement",\
	"Statement -> WHILE LPAREN Exp RPAREN Statement",\
	"Statement -> WHILE LPAREN Exp RPAREN Statement ELSE Statement",\
	"Statement -> BREAK SC",\
	"Statement -> CONTINUE SC",\
	"Call -> ID LPAREN ExpList RPAREN",\
	"Call -> ID LPAREN RPAREN",\
	"ExpList -> Exp",\
	"ExpList -> Exp COMMA ExpList",\
	"Type -> INT",\
	"Type -> BYTE",\
	"Type -> BOOL",\
	"Exp -> LPAREN Exp RPAREN",\
	"Exp -> Exp BINOP Exp",\
	"Exp -> ID",\
	"Exp -> Call",\
	"Exp -> NUM",\
	"Exp -> NUM B",\
	"Exp -> STRING",\
	"Exp -> TRUE",\
	"Exp -> FALSE",\
	"Exp -> NOT Exp",\
	"Exp -> Exp AND Exp",\
	"Exp -> Exp OR Exp",\
	"Exp -> Exp RELOP Exp",\
\};\
\
void output::printProductionRule(int ruleno) \{\
	cout << ruleno << ": " << output::rules[ruleno-1] << endl;\
\}\
\
void output::errorLex(int lineno)\{\
    cout << "line " << lineno << ": " << "lexical error" << endl;\
\}\
\
void output::errorSyn(int lineno)\{\
    cout << "line " << lineno << ": " << "syntax error" << endl;\
\}\
}