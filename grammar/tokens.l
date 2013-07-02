%{
#include <string>
#include "Ast.hpp"
#include "LexWrapper.hpp"
#include "parser.hpp"
#define TOKEN(t) (yylval->token = t)
/* Update location each time a rule is matched. */
#define YY_USER_ACTION *yylloc += yyleng;
%}

%option c++
%option yyclass="LexWrapper"

%x COMMENT

digit           [[:digit:]]
id              [[:alpha:]_][_[:alnum:]]*
line_comment    "//"[^\n]*
space           [ \f\r\t\v]

string          \"(\\.|[^\\"])*\" 
integer         {digit}+
double          ({digit}+(((\.{digit}+)?[Ee][+-]?{digit}+)|(\.{digit}*)))|(\.{digit}+)
float           {double}f

%%
"void"                  { return TOKEN(VOID); }
"bool"                  { return TOKEN(BOOL); }
"double"                { return TOKEN(DOUBLE); }
"float"                 { return TOKEN(FLOAT); }
"int"                   { return TOKEN(INT); }
"return"                { return TOKEN(RETURN); }

"true"                  { yylval->boolVal = true; return TOKEN(BOOLLITERAL); }
"false"                 { yylval->boolVal = false; return TOKEN(BOOLLITERAL); }

"if"                    { return TOKEN(IF); }
"else"                  { return TOKEN(ELSE); }
"for"                   { return TOKEN(FOR); }

"="                     return TOKEN(ASSIGN);
"=="                    return TOKEN(EQ);
"!="                    return TOKEN(NEQ);
"<"                     return TOKEN(LT);
"<="                    return TOKEN(LEQ);
">"                     return TOKEN(GT);
">="                    return TOKEN(GEQ);
"+"                     return TOKEN(ADD);
"-"                     return TOKEN(SUB);
"*"                     return TOKEN(MUL);
"/"                     return TOKEN(DIV);
"&&"                    { return TOKEN(AND); }
"||"                    { return TOKEN(OR);  }
"^"                     { return TOKEN(XOR); }
"!"                     { return TOKEN(NOT); }
 /* Punctuation */
"("             { return '('; }
")"             { return ')'; }
";"             { return ';'; }
","             { return ','; }
"."             { return '.'; }
"{"             { return '{'; }
"}"             { return '}'; }
"["             { return '['; }
"]"             { return ']'; }

{id}            { yylval->string = new std::string(yytext,yyleng); return ID; }
{string}        { 
                  yylval->string = new std::string(yytext,yyleng-2);
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
                

 /* Comments */
{line_comment}  { yylloc->Skip(); }
"/*"            { BEGIN(COMMENT); yylloc->Skip(); }

 /* Whitespace */
{space}         { yylloc->Skip(); }
\n+             { yylloc->Newline(yyleng); yylloc->Skip(); }

 /* Comments */
<COMMENT>{
"*/"            { BEGIN(INITIAL); yylloc->Skip(); }
"*"             { yylloc->Skip(); }
[^*\n]+         { yylloc->Skip(); }
\n+             { yylloc->Newline(yyleng); yylloc->Skip(); }
}

%%
