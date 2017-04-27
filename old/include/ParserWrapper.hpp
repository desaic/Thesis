#ifndef PARSER_WRAPPER_HPP
#define PARSER_WRAPPER_HPP
#include "LexWrapper.hpp"
#include <iostream>
class ParserWrapper
{
public:
  ParserWrapper();
  ParserWrapper(const char * filename);
  ~ParserWrapper();
  
  LexWrapper * GetLex(){
    return &_lex;
  }
  
  int Parse(std::istream *stream, const char *filename);
  const char * GetFilename(){return _filename;}
private:
  LexWrapper _lex;
  const char * _filename;
};
#endif
