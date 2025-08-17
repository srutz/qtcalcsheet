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
     NUMBER = 258,
     STRING = 259,
     CELL_REF = 260,
     FUNCTION_NAME = 261,
     LPAREN = 262,
     RPAREN = 263,
     SEMICOLON = 264,
     COMMA = 265,
     COLON = 266,
     PLUS = 267,
     MINUS = 268,
     MULTIPLY = 269,
     DIVIDE = 270,
     POWER = 271,
     EQ = 272,
     NE = 273,
     LT = 274,
     LE = 275,
     GT = 276,
     GE = 277,
     CONCAT = 278,
     NEWLINE = 279,
     UPLUS = 280,
     UMINUS = 281
   };
#endif
/* Tokens.  */
#define NUMBER 258
#define STRING 259
#define CELL_REF 260
#define FUNCTION_NAME 261
#define LPAREN 262
#define RPAREN 263
#define SEMICOLON 264
#define COMMA 265
#define COLON 266
#define PLUS 267
#define MINUS 268
#define MULTIPLY 269
#define DIVIDE 270
#define POWER 271
#define EQ 272
#define NE 273
#define LT 274
#define LE 275
#define GT 276
#define GE 277
#define CONCAT 278
#define NEWLINE 279
#define UPLUS 280
#define UMINUS 281




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 93 "/Users/sr/work/qt/qtcalcsheet/src/formula/formulagrammer.y"
{
    string* str;
    shared_ptr<ExcelNode>* node;
    vector<shared_ptr<ExcelNode>>* nodeList;
}
/* Line 1529 of yacc.c.  */
#line 107 "/Users/sr/work/qt/qtcalcsheet/src/formula/formulagrammer.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

