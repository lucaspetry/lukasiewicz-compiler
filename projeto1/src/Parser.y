%{
#include "SymbolTable.h"
#include "SyntaxTree.h"
#include "TreeNode.h"
#include <iostream>

SymbolTable SYMBOL_TABLE;
SyntaxTree* SYNTAX_TREE;
extern int yylex();
extern void yyerror(const char* s, ...);
%}

%define parse.trace

%code requires {
    class TreeNode;
    class SyntaxTree;
}

/* yylval == %union
 * union informs the different ways we can store data
 */
%union {
    TreeNode* node;
    SyntaxTree* syntaxTree;
    int integer;
    char *id;
}

/* token defines our terminal symbols (tokens).
 */
%token <integer> T_INT
%token <id> T_ID
%token T_PLUS T_TIMES T_MINUS T_DIVIDE T_OPEN_PAR T_CLOSING_PAR T_NL T_ATT T T_TYPE_INT T_COMMA

/* type defines the type of our nonterminal symbols.
 * Types should match the names used in the union.
 * Example: %type<node> expr
 */
%type <syntaxTree> lines program
%type <node> line expr declar

/* Operator precedence for mathematical operators
 * The latest it is listed, the highest the precedence
 * left, right, nonassoc
 */
%left T_PLUS T_MINUS
%left T_TIMES T_DIVIDE
%nonassoc U_MINUS error

/* Starting rule
 */
%start program

%%

program:
    lines { SYNTAX_TREE = $1; }
    ;

lines:
    line { $$ = new SyntaxTree(); if($1 != NULL) $$->pushBackLine($1); }
    | line lines { if($1 != NULL) $2->pushBackLine($1); }
    | lines error T_NL { yyerrok; }
    ;

line:
    T_NL { $$ = NULL; }
    | T_TYPE_INT declar { $$ = 0; }
    | T_ID T_ATT expr { $$ = 0; }
    ;

expr:
    T_INT { $$ = new Integer($1); }
    | T_ID { $$ = new Variable($1, NULL); }
    | T_MINUS expr %prec U_MINUS { $$ = new BinaryOperation($1, BinaryOperation::PLUS, $3); }
    | expr T_PLUS expr { $$ = new BinaryOperation($1, BinaryOperation::PLUS, $3); }
    | expr T_MINUS expr { $$ = new BinaryOperation($1, BinaryOperation::MINUS, $3); }
    | expr T_TIMES expr { $$ = new BinaryOperation($1, BinaryOperation::TIMES, $3); }
    | expr T_DIVIDE expr { $$ = new BinaryOperation($1, BinaryOperation::DIVIDE, $3); }
    | T_OPEN_PAR expr T_CLOSING_PAR { $$ = $2; }
    ;

declar:
    T_ID T_COMMA declar
    | T_ID { $$ = new Variable($1, NULL); }
    | T_ID T_ATT expr T_COMMA declar
    | T_ID T_ATT expr { $$ = new BinaryOperation($1, BinaryOperation::ASSIGN, $3); }
    ;

%%
