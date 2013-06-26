/*
 * NReturn.cpp
 *
 *  Created on: Jun 21, 2013
 *      Author: desaic
 */

#include "NReturn.hpp"
#include <llvm/Instructions.h>
llvm::Value*
NReturn::codeGen(CodeGenContext& context)
{
  llvm::ReturnInst * retInst=0;
  std::cout<<"Status: Generating return instruction\n";
  if(expr==0){
    std::cout<<"Void return\n";
    retInst = llvm::ReturnInst::Create(llvm::getGlobalContext());
  }else{
    std::cout<<"Return something\n";
    retInst = llvm::ReturnInst::Create(
        llvm::getGlobalContext(),expr->codeGen(context),
        context.currentBlock());
  }
  return retInst;
}

NReturn::NReturn():
expr(0)
{}
NReturn::NReturn(NExpression * _expr):
    expr(_expr)
{}
NReturn::~NReturn()
{}
