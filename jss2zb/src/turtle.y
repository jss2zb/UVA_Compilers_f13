
%{
#include <stdio.h>
#include "symtab.h"
%}

%union { int i; node *n; double d;}

%token GO TURN VAR JUMP
%token FOR STEP TO DO
%token COPEN CCLOSE
%token IF ELSE THEN WHILE PROCEDURE PCALL PARAM
%token SIN COS SQRT
%token <d> FLOAT
%token <n> ID               
%token <i> NUMBER       
%token SEMICOLON PLUS MINUS TIMES DIV OPEN CLOSE ASSIGN 
%token LESSTHAN GREATERTHAN LESSTHANEQUALS GREATERTHANEQUALS EQUALS NOTEQUALS CURLOPEN CURLCLOSE

/*%type <n> decl
%type <n> decllist*/

%%
program: head decllist stmtlist tail;

head: { printf("%%!PS Adobe\n"
               "\n"
	       "newpath\n 0 0 moveto\n"
	       );
      };

tail: { printf("closepath \nstroke\n"); };

decllist: ;
decllist: decllist decl;

decl: VAR ID SEMICOLON { printf("/tlt%s 0 def\n",$2->symbol);};


stmtlist: ;
stmtlist: stmtlist stmt ;

stmt: ID ASSIGN expr SEMICOLON {printf("/tlt%s exch store\n",$1->symbol);};
stmt: GO expr SEMICOLON {printf("0 rlineto\n");};
stmt: JUMP expr SEMICOLON {printf("0 rmoveto\n");};
stmt: TURN expr SEMICOLON {printf("rotate\n");};

stmt: FOR ID ASSIGN expr 
          STEP expr
	  TO expr
	  DO {printf("{ /tlt%s exch store\n",$2->symbol);} 
	     stmt {printf("} for\n");};

stmt: COPEN stmtlist CCLOSE; 

stmt: PCALL ID params SEMICOLON {printf("%s",$2->symbol);};	 
 
stmt: IFBLOCK {printf(" if\n");};
stmt: IFBLOCK ELSEBLOCK;

stmt: WHILE {printf("\n{ ");} 
          OPEN compare CLOSE {printf("{} {exit} ifelse\n");}
          CURLOPEN
          stmtlist CURLCLOSE {printf("} loop\n");};

stmt: PROCEDURE ID {printf("/proc%s {",$2->symbol);} 
          CURLOPEN stmtlist CURLCLOSE {printf("} def\n");};

params: atomic params;
params: ;
 
IFBLOCK:  IF OPEN compare CLOSE
              CURLOPEN {printf("\n{");}
              stmtlist CURLCLOSE {printf("}");};

IFBLOCK: IF OPEN compare CLOSE
              THEN CURLOPEN {printf("{");}
              stmtlist CURLCLOSE {printf("}");};

ELSEBLOCK: ELSE CURLOPEN {printf("{");}
              stmtlist CURLCLOSE {printf("} ifelse\n");};

compare: expr EQUALS expr {printf("eq ");};
compare: expr NOTEQUALS expr {printf("ne ");};
compare: expr GREATERTHAN expr {printf("gt ");};
compare: expr GREATERTHANEQUALS expr {printf("ge ");};
compare: expr LESSTHAN expr {printf("lt ");};
compare: expr LESSTHANEQUALS expr {printf("le ");};

expr: expr PLUS term { printf("add ");};
expr: expr MINUS term { printf("sub ");};
expr: term;

term: term TIMES factor { printf("mul ");};
term: term DIV factor { printf("div ");};
term: factor;

factor: MINUS atomic { printf("neg ");};
factor: PLUS atomic;
factor: SIN factor { printf("sin ");};
factor: COS factor { printf("cos ");};
factor: SQRT factor { printf("sqrt ");};
factor: atomic;

atomic: OPEN expr CLOSE;
atomic: NUMBER {printf("%d ",$1);};
atomic: FLOAT {printf("%f ",$1);};
atomic: ID {printf("tlt%s ", $1->symbol);};
atomic: PARAM {};

%%
int yyerror(char *msg)
{  fprintf(stderr,"Error: %s\n",msg);
   return 0;
}

int main(void)
{   yyparse();
    return 0;
}

