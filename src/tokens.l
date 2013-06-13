%{
#include <string>
#include "node.h"
#include "parser.hpp"
#define TOKEN(t) (yylval.token = t)
extern "C" int yywrap() { }

/* Update location each time a rule is matched. */
#define YY_USER_ACTION *yylloc += yyleng;
%}
digit           [[:digit:]]
id              [[:alpha:]_][_[:alnum:]]*
line_comment    "//"[^\n]*
space           [ \f\r\t\v]

string          \"(\\.|[^\\"])*\" 
integer         {digit}+
double          ({digit}+(((\.{digit}+)?[Ee][+-]?{digit}+)|(\.{digit}*)))|(\.{digit}+)
float           {double}f
%%

{id}            { yylval->strVal = strdup(yytext); return ID; }
{string}        { 
                  yylval->strVal = strdup(yytext+1);  
                  yylval->strVal[strlen(yylval->strVal)-1]='\0'; 
                  return STRLITERAL; 
                }
{integer}       { yylval->intVal = atoi(yytext); return INTLITERAL; }
{float}         { 
                  char *tmp = strdup(yytext);
                  tmp[strlen(tmp)-1] = '\0';
                  yylval->floatVal = static_cast<float>(atof(tmp)); 
                  free(tmp);
                  return FLOATLITERAL; 
                }
{double}        { 
                  yylval->doubleVal = atof(yytext); 
                  return DOUBLELITERAL; 
                }
                
[ \t\n]                 ;

"="                     return TOKEN(TEQUAL);
"=="                    return TOKEN(TCEQ);
"!="                    return TOKEN(TCNE);
"<"                     return TOKEN(TCLT);
"<="                    return TOKEN(TCLE);
">"                     return TOKEN(TCGT);
">="                    return TOKEN(TCGE);
"("                     return TOKEN(TLPAREN);
")"                     return TOKEN(TRPAREN);
"{"                     return TOKEN(TLBRACE);
"}"                     return TOKEN(TRBRACE);
"."                     return TOKEN(TDOT);
","                     return TOKEN(TCOMMA);
"+"                     return TOKEN(TPLUS);
"-"                     return TOKEN(TMINUS);
"*"                     return TOKEN(TMUL);
"/"                     return TOKEN(TDIV);
.                       printf("Unknown token!\n"); yyterminate();

%%



