#include "LexWrapper.hpp"
#include "ParserWrapper.hpp"
#include "parser.hpp"

int yyFlexLexer::yywrap() { return 1; }

LexWrapper::LexWrapper(ParserWrapper* parser) : _parser(parser)
{
}
