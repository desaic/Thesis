%pure_parser
%locations
%parse-param {ParserWrapper* parser}
%lex-param   {ParserWrapper* parser}
%error-verbose

%{
#include "ParserWrapper.hpp"
#include <stdio.h>

#undef yylex
#define yylex parser->GetLex()->Lex

void yyerror(TextRange* range, ParserWrapper* parser, const char* msg)
{
    std::cout<<"Error: "<<msg<<"\n";
}

/* Combine first and last locations if possible.  Otherwise the location
   follows the previous location. */
#define YYLLOC_DEFAULT(Current, Rhs, N)                                 \
    do                                                                  \
        if (N)                                                          \
            (Current).Combine( YYRHSLOC(Rhs, 1), YYRHSLOC(Rhs, N) );    \
        else                                                            \
            (Current).Follow( YYRHSLOC(Rhs, 0) );                       \
    while (0)

%}

%{
    #include "Ast.hpp"
    NBlock *programBlock; /* the top level root node of our final AST */
    #include <stdio.h>
    void yyerror(const char *s) { printf("ERROR: %s\n", s); }
%}

/* Represents the many different ways we can access our data */
%union {
    AstNode *node;
    NBlock *block;
    NExpression *expr;
    NStatement *stmt;
    NIdentifier *ident;
    NVariableDeclaration *var_decl;
    std::vector<NVariableDeclaration*> *varvec;
    std::vector<NExpression*> *exprvec;
    std::string *string;
    int intVal;
    float floatVal;
    double doubleVal;
    int token;
}

/* Define our terminal symbols (tokens). This should
   match our tokens.l lex file. We also define the node type
   they represent.
 */
%token <string> ID STRLITERAL
%token <intVal> INTLITERAL
%token <doubleVal> DOUBLELITERAL
%token <floatVal> FLOATLITERAL
%token INT DOUBLE FLOAT ASSIGN
%token <token> EQ NEQ LT LEQ GT GEQ
%token <token> ADD SUB MUL DIV


/* Define the type of node our nonterminal symbols represent.
   The types refer to the %union declaration above. Ex: when
   we call an ident (defined by union type ident) we are really
   calling an (NIdentifier*). It makes the compiler happy.
 */
%type <ident> ident
%type <expr> LiteralExp expr 
%type <varvec> func_decl_args
%type <exprvec> call_args
%type <block> program stmts block
%type <stmt> stmt var_decl func_decl
%type <token> comparison

/* Operator precedence for mathematical operators */
%left '.'
%right ASSIGN ADDASN SUBASN
%left OR
%left AND
%left XOR
%left NEQ EQ
%left LT LEQ GT GEQ 
%left ADD SUB NOT
%left MUL DIV MOD

%start program

%%

program : stmts { programBlock = $1; }
        ;
        
stmts : stmt { $$ = new NBlock(); $$->statements.push_back($<stmt>1); }
      | stmts stmt { $1->statements.push_back($<stmt>2); }
      ;

stmt : var_decl | func_decl
     | expr { $$ = new NExpressionStatement(*$1); }
     ;

block : '{' stmts '}' { $$ = $2; }
      | '{' '}' { $$ = new NBlock(); }
      ;

var_decl : ident ident { $$ = new NVariableDeclaration(*$1, *$2); }
         | ident ident ASSIGN expr { $$ = new NVariableDeclaration(*$1, *$2, $4); }
         ;
        
func_decl : ident ident '(' func_decl_args ')' block 
            { $$ = new NFunctionDeclaration(*$1, *$2, *$4, *$6); delete $4; }
          ;
    
func_decl_args : /*blank*/  { $$ = new VariableList(); }
          | var_decl { $$ = new VariableList(); $$->push_back($<var_decl>1); }
          | func_decl_args ',' var_decl { $1->push_back($<var_decl>3); }
          ;

ident : ID { $$ = new NIdentifier(*$1); delete $1; }
      ;

LiteralExp : INTLITERAL { $$ = new NInteger($1); }
        | DOUBLELITERAL { $$ = new NDouble($1); }
        | FLOATLITERAL { $$ = new NFloat($1); }
        ;
    
expr : ident ASSIGN expr { $$ = new NAssignment(*$<ident>1, *$3); }
     | ident '(' call_args ')' { $$ = new NMethodCall(*$1, *$3); delete $3; }
     | ident { $<ident>$ = $1; }
     | LiteralExp
     | expr comparison expr { $$ = new NBinaryOperator(*$1, $2, *$3); }
     | '(' expr ')' { $$ = $2; }
     ;
    
call_args : /*blank*/  { $$ = new ExpressionList(); }
          | expr { $$ = new ExpressionList(); $$->push_back($1); }
          | call_args ',' expr  { $1->push_back($3); }
          ;

comparison : EQ | NEQ | LT | LEQ | GT | GEQ 
           | ADD | SUB | MUL | DIV
           ;

%%
