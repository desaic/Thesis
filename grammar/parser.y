%pure_parser
%locations
%parse-param {ParserWrapper* parser}
%lex-param   {ParserWrapper* parser}
%error-verbose

%{
/**C declarations*/
#include "ParserWrapper.hpp"
#include <stdio.h>
#include "Ast.hpp"
#include "NBinaryOp.hpp"
#include "NIfStatement.hpp"
#include "NReturn.hpp"
#include "NVariableDeclaration.hpp"
NBlock *programBlock; /* the top level root node of our final AST */

#undef yylex
#define yylex parser->GetLex()->Lex

void yyerror(TextRange* range, ParserWrapper* parser, const char* msg)
{
    std::cout<<"Error: "<<msg<<"\n";
    const TextPos& pos = range->GetStart();
    const TextPos& pos1 = range->GetEnd();
    std::cout<<"Between Line "<<pos.GetLine()<<" Column "<<pos.GetColumn()<<"\n";
    std::cout<<"And Line "<<pos1.GetLine()<<" Column "<<pos1.GetColumn()<<"\n";
}

/* Combine first and last locations if possible.  Otherwise the location
   follows the previous location. */
#define YYLLOC_DEFAULT(Current, Rhs, N)                             \
        N?                                                          \
            (Current).Combine( YYRHSLOC(Rhs, 1), YYRHSLOC(Rhs, N) ):\
            (Current).Follow( YYRHSLOC(Rhs, 0) )    
%}
/**Bison declarations*/
/* Represents the many different ways we can access our data */
%union {
    AstNode *node;
    NBlock *block;
    NExpression *expr;
    NStatement *stmt;
    NIdentifier *ident;
    NVariableDeclaration *var_decl;
    std::deque<NVariableDeclaration*> *varvec;
    std::deque<NExpression*> *exprvec;
    std::string *string;
    AstType * type;
    int intVal;
    float floatVal;
    double doubleVal;
    int token;
    bool boolVal;
}

/* Define our terminal symbols
 */
%token <string> ID STRLITERAL
%token <intVal> INTLITERAL
%token <doubleVal> DOUBLELITERAL
%token <floatVal> FLOATLITERAL
%token <boolVal> BOOLLITERAL
%token VOID BOOL DOUBLE FLOAT INT 
%token ASSIGN RETURN TRUE FALSE
%token <token> EQ NEQ LT LEQ GT GEQ
%token <token> ADD SUB MUL DIV
%token <token> IF ELSE
/**Non-terminals*/
%type <ident> ident
%type <expr> LiteralExp expr 
%type <expr> BinExpr
%type <varvec> func_decl_args
%type <exprvec> call_args
%type <block> program stmts block
%type <var_decl> var_decl
%type <stmt> stmt func_decl ReturnStmt IfStmt
%type <type> BuiltinType Type

/* Operator precedence*/
%left '.'
%right ASSIGN ADDASN SUBASN
%left OR
%left AND
%left XOR
%left NEQ EQ
%left LT LEQ GT GEQ 
%left ADD SUB NOT
%left MUL DIV MOD
/*start state*/
%start program
%initial-action {
    @$ = TextRange(parser->GetFilename());
}

%%
/**Grammar Rules*/

program : stmts { programBlock = $1; }
        ;
        
stmts : { $$ = new NBlock();}
      | stmt stmts {$$=$2; $$->statements.push_front($1); }
      ;

stmt : var_decl ';' {$$ = $1;}
     | func_decl
     | expr ';'{ $$ = new NExpressionStatement(*$1); }
     | ReturnStmt ';'
     | IfStmt
     ;

IfStmt : IF '(' expr ')' block            {$$ = new NIfStatement($3, $5);}
       | IF '(' expr ')' block ELSE block {$$ = new NIfStatement($3, $5, $7);}
      
     
block : '{' stmts '}' { $$ = $2; }
//      | '{' '}' { $$ = new NBlock(); }
      ;

var_decl : Type ident { $$ = new NVariableDeclaration(*$1, *$2); delete $1;}
         | Type ident ASSIGN expr { $$ = new NVariableDeclaration(*$1, *$2, $4); delete $1;}
         ;
        
func_decl : Type ident '(' func_decl_args ')' block 
            { $$ = new NFunctionDeclaration(*$1, *$2, *$4, *$6); delete $1; delete $4; }
          ;
    
func_decl_args : /*blank*/  { $$ = new VariableList(); }
          | var_decl { $$ = new VariableList(); $$->push_front($1); }
          | var_decl ',' func_decl_args   { $$=$3; $$->push_front($1); }
          ;

ReturnStmt
    : RETURN expr        { $$ = new NReturn($2); }
    | RETURN            { $$ = new NReturn(NULL); }
    ;
          
ident : ID { $$ = new NIdentifier(*$1); delete $1; }
      ;

LiteralExp : INTLITERAL { $$ = new NInteger($1); }
        | DOUBLELITERAL { $$ = new NDouble($1); }
        | FLOATLITERAL { $$ = new NFloat($1); }
        | BOOLLITERAL {$$ = new NBool($1);}
        ;
    
expr : ident ASSIGN expr { $$ = new NAssignment(*$<ident>1, *$3); }
     | ident '(' call_args ')' { $$ = new NFunctionCall(*$1, *$3); delete $3; }
     | ident { $<ident>$ = $1; }
     | LiteralExp
     | BinExpr
     | '(' expr ')' { $$ = $2; }
     ;
BinExpr : expr ADD expr { $$ = new NBinaryOp($1, $2, $3); }
        | expr SUB expr { $$ = new NBinaryOp($1, $2, $3); }
        | expr MUL expr { $$ = new NBinaryOp($1, $2, $3); }
        | expr DIV expr { $$ = new NBinaryOp($1, $2, $3); }
        | expr EQ expr { $$ = new NBinaryOp ($1, $2, $3); }
        | expr NEQ expr { $$ = new NBinaryOp($1, $2, $3); }
        | expr LT expr { $$ = new NBinaryOp ($1, $2, $3); }
        | expr LEQ expr { $$ = new NBinaryOp($1, $2, $3); }
        | expr GT expr { $$ = new NBinaryOp ($1, $2, $3); }
        | expr GEQ expr { $$ = new NBinaryOp($1, $2, $3); }
call_args : /*blank*/  { $$ = new ExpressionList(); }
          | expr { $$ = new ExpressionList(); $$->push_front($1); }
          | expr ',' call_args { $$=$3; $$->push_front($1); }
          ;

Type : BuiltinType  /*Default action $$=$1*/
     | ID           {$$ = new StructType($1); delete $1;}
     ;

BuiltinType : INT      {$$ = new AstType(AstType::AST_INT);}
            | BOOL    {$$ = new AstType(AstType::AST_BOOL);}
            | FLOAT    {$$ = new AstType(AstType::AST_FLOAT);}
            | DOUBLE   {$$ = new AstType(AstType::AST_DOUBLE);}
            | VOID     {$$ = new AstType(AstType::AST_VOID);}
            ;
%%
/**Additional c code*/