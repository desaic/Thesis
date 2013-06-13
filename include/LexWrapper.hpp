#ifndef LEX_WRAPPER_HPP
#define LEX_WRAPPER_HPP
///@brief Put flex lexer in a class.
#include "TextRange.hpp"
class ParserWrapper;

union YYSTYPE;
#define YYLTYPE TextRange

typedef int Token;      // We use "int" instead of "enum yytokentype"

// Tell flex to put yylex code in a method of Lexer.
#undef YY_DECL
#define	YY_DECL \
    Token LexWrapper::Lex(YYSTYPE *yylval, YYLTYPE *yylloc, void *parser)

// Include the lexer base class, which isn't properly guarded by an #ifdef.
#ifndef __FLEX_LEXER_H
#include <FlexLexer.h>
#endif

class LexWrapper
{
public:
  LexWrapper::LexWrapper(ParserWrapper* parser);
   Token Lex(YYSTYPE *yylval, YYLTYPE *yylloc, void *parser);
private:
  ParserWrapper * _parser;
};

#endif
