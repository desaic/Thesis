#include "LexWrapper.hpp"
#include "ParserWrapper.hpp"
#include "Ast.hpp"
#include "parser.hpp"

int yyFlexLexer::yywrap() { return 1; }

LexWrapper::LexWrapper(ParserWrapper* parser) :yyFlexLexer(0,0), _parser(parser)
{
}
