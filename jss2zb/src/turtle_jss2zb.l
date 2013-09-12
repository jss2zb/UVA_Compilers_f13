%{
#include "symtab.h"
#include "token.h"
%}

IDs [a-z]+[a-z0-9]*
PARAMETER ([a-z]+[a-z0-9],)*[a-z]+[a-z0-9]

%%

\/\/.*          ;
[[:space:]]     ;

GO              return GO;
TURN            return TURN; 
VAR             return VAR;
JUMP            return JUMP;
FOR             return FOR;
STEP            return STEP;
TO              return TO;
DO              return DO;
BEGIN           return COPEN;
END             return CCLOSE;

IF              return IF;
ELSE            return ELSE;
THEN            return THEN;
WHILE           return WHILE;


PROCEDURE       return PROCEDURE;

sin             return SIN;
cos             return COS;
sqrt            return SQRT;

;               return SEMICOLON;
\+              return PLUS;
-               return MINUS;
\*              return TIMES;
\/              return DIV;
\(              return OPEN;
\)              return CLOSE;
\>              return GREATERTHAN;
\<              return LESSTHAN;
\,              return COMMA;
=               return ASSIGN;

{IDs}              { yylval.n = lookup(yytext); 
                        if (yylval.n==NULL)
                               yylval.n = insert(yytext);
                        return ID;
                 }
    
[0-9]+\.[0-9]+  yylval.d = atof(yytext);  return FLOAT;
[0-9]+          yylval.i = atoi(yytext);  return NUMBER;

[0-9]+[a-z]+ fprintf(stderr,"Invalid Identifier %c (%2X) \n",yytext[0],yytext[0]);  
.               fprintf(stderr,"Unexpected character %c (%2X)\n",yytext[0],yytext[0]);

%%
