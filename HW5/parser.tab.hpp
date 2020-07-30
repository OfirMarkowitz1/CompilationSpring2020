/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     VOID = 258,
     INT = 259,
     BYTE = 260,
     B = 261,
     BOOL = 262,
     TRUE = 263,
     FALSE = 264,
     RETURN = 265,
     IF = 266,
     WHILE = 267,
     BREAK = 268,
     CONTINUE = 269,
     STATIC = 270,
     COMMA = 271,
     ID = 272,
     NUM = 273,
     STRING = 274,
     LBRACE = 275,
     RBRACE = 276,
     LPAREN = 277,
     RPAREN = 278,
     SC = 279,
     COND_WITHOUT_ELSE = 280,
     ELSE = 281,
     ASSIGN = 282,
     OR = 283,
     AND = 284,
     EQUALITY = 285,
     RELATIONAL = 286,
     ADDITIVE = 287,
     MULTIPLICATIVE = 288,
     NOT = 289
   };
#endif
/* Tokens.  */
#define VOID 258
#define INT 259
#define BYTE 260
#define B 261
#define BOOL 262
#define TRUE 263
#define FALSE 264
#define RETURN 265
#define IF 266
#define WHILE 267
#define BREAK 268
#define CONTINUE 269
#define STATIC 270
#define COMMA 271
#define ID 272
#define NUM 273
#define STRING 274
#define LBRACE 275
#define RBRACE 276
#define LPAREN 277
#define RPAREN 278
#define SC 279
#define COND_WITHOUT_ELSE 280
#define ELSE 281
#define ASSIGN 282
#define OR 283
#define AND 284
#define EQUALITY 285
#define RELATIONAL 286
#define ADDITIVE 287
#define MULTIPLICATIVE 288
#define NOT 289




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

