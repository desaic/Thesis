#include "LexWrapper.hpp"
#include "ParserWrapper.hpp"
#include "Parser.hpp"

int yyFlexLexer::yywrap() { return 1; }

LexWrapper::LexWrapper(ParserWrapper* parser) : _parser(parser)
{
}