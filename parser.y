%pure_parser
%locations
%parse-param {ParserWrapper* parser}
%lex-param   {ParserWrapper* parser}
%error-verbose

%{
#include "ParserWrapper.h"
#include <stdio.h>

#undef yylex
#define yylex parser->GetLexer()->Lex

void yyerror(UtSourceRange* range, PrsWrapParser* parser, const char* msg)
{
    parser->Log()->WriteWhere(UtLog::UT_ERROR, range->GetStart(), msg);
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
#include "TempStructures.h"
#include "OpenFL/Ir/Exps.h"
#include "OpenFL/Ir/Inst.h"
#include "OpenFL/Ir/Stmts.h"
#include "OpenFL/Ir/Var.h"
#include "OpenFL/Ir/VarDecl.h"
#include "OpenFL/Ir/Program.h"
#include "OpenFL/Ir/ProgramInst.h"
#include "OpenFL/Ir/FuncParam.h"
#include "OpenFL/Ir/FuncPrototype.h"
#include "OpenFL/Ir/Function.h"
#include "OpenFL/Ir/Fablet.h"
#include "OpenFL/Ir/Struct.h"
#include "OpenFL/Ir/EntryFunction.h"
#include "OpenFL/Ir/ElemAccess.h"
#include "OpenFL/Operators/Info.h"
#include "OpenFL/Utility/SourcePos.h"
#include "OpenFL/Utility/Types.h"
%}

%{
#include "OpenFL/Parser/WrapParser.h"
%}


%{
    #include "Ast.h"
    NBlock *programBlock; /* the top level root node of our final AST */
    #include <stdio.h>
    extern int yylex();
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
    int token;
}

/* Define our terminal symbols (tokens). This should
   match our tokens.l lex file. We also define the node type
   they represent.
 */
%token <string> TIDENTIFIER TINTEGER TDOUBLE
%token <token> TCEQ TCNE TCLT TCLE TCGT TCGE TEQUAL
%token <token> TLPAREN TRPAREN TLBRACE TRBRACE TCOMMA TDOT
%token <token> TPLUS TMINUS TMUL TDIV

/* Define the type of node our nonterminal symbols represent.
   The types refer to the %union declaration above. Ex: when
   we call an ident (defined by union type ident) we are really
   calling an (NIdentifier*). It makes the compiler happy.
 */
%type <ident> ident
%type <expr> numeric expr 
%type <varvec> func_decl_args
%type <exprvec> call_args
%type <block> program stmts block
%type <stmt> stmt var_decl func_decl
%type <token> comparison

/* Operator precedence for mathematical operators */
%left TPLUS TMINUS
%left TMUL TDIV

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

block : TLBRACE stmts TRBRACE { $$ = $2; }
      | TLBRACE TRBRACE { $$ = new NBlock(); }
      ;

var_decl : ident ident { $$ = new NVariableDeclaration(*$1, *$2); }
         | ident ident TEQUAL expr { $$ = new NVariableDeclaration(*$1, *$2, $4); }
         ;
        
func_decl : ident ident TLPAREN func_decl_args TRPAREN block 
            { $$ = new NFunctionDeclaration(*$1, *$2, *$4, *$6); delete $4; }
          ;
    
func_decl_args : /*blank*/  { $$ = new VariableList(); }
          | var_decl { $$ = new VariableList(); $$->push_back($<var_decl>1); }
          | func_decl_args TCOMMA var_decl { $1->push_back($<var_decl>3); }
          ;

ident : TIDENTIFIER { $$ = new NIdentifier(*$1); delete $1; }
      ;

numeric : TINTEGER { $$ = new NInteger(atol($1->c_str())); delete $1; }
        | TDOUBLE { $$ = new NDouble(atof($1->c_str())); delete $1; }
        ;
    
expr : ident TEQUAL expr { $$ = new NAssignment(*$<ident>1, *$3); }
     | ident TLPAREN call_args TRPAREN { $$ = new NMethodCall(*$1, *$3); delete $3; }
     | ident { $<ident>$ = $1; }
     | numeric
     | expr comparison expr { $$ = new NBinaryOperator(*$1, $2, *$3); }
     | TLPAREN expr TRPAREN { $$ = $2; }
     ;
    
call_args : /*blank*/  { $$ = new ExpressionList(); }
          | expr { $$ = new ExpressionList(); $$->push_back($1); }
          | call_args TCOMMA expr  { $1->push_back($3); }
          ;

comparison : TCEQ | TCNE | TCLT | TCLE | TCGT | TCGE 
           | TPLUS | TMINUS | TMUL | TDIV
           ;

%%
