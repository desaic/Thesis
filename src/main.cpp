#include <fstream>
#include "codegen.h"
#include "Ast.hpp"
#include "ParserWrapper.hpp"

using namespace std;

extern "C" int yyparse (void);
extern NBlock* programBlock;

int main(int argc, char **argv)
{
    ParserWrapper parser;
    if(argc<2){
      std::cout<<"Need a filename\n";
      return 0;
    }
    char * filename = argv[1];
    std::ifstream  * stream = new std::ifstream();
    stream->open(filename);
    if(!stream->good()){
      std::cout<<"Cannot open file "<<filename<<"\n";
      return 0;
    }
    int status = parser.Parse(stream, filename);
    CodeGenContext context;
    switch(status){
    case 0:
      std::cout<<"Status: Syntax checked.\n";
      std::cout << programBlock << std::endl;
      context.generateCode(*programBlock);
      //context.runCode();
      context.saveLLVMIR("a.ll");
      break;
    case 1:
      std::cout<<"Error: Syntax.\n";
      break;
    case 2:
      std::cout<<"Error:Memory exhausted during parsing.\n";
      break;
    }

    return 0;
}

