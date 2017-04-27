/*
 * NMethodCall.cpp
 *
 *  Created on: Jun 26, 2013
 *      Author: desaic
 */

#include "NFunctionCall.hpp"
#include "NIdentifier.hpp"
#include "NFunctionDeclaration.hpp"
#include <llvm/Value.h>
#include <llvm/Function.h>
#include <iostream>
llvm::Value*
NFunctionCall::codeGen(CodeGenContext& context)
{
  llvm::Function *function = context.module->getFunction(id.name.c_str());
  if (function == NULL) {
    std::cerr << "no such function " << id.name << std::endl;
    return NULL;
  }
  std::vector<llvm::Value*> args;
  ExpressionList::const_iterator it;
  for (it = arguments.begin(); it != arguments.end(); it++) {
    args.push_back((**it).codeGen(context));
  }
  llvm::CallInst *call = context.builder.CreateCall(function, args, "");
  std::cout << "Creating method call: " << id.name << std::endl;
  return call;
}

NFunctionCall::NFunctionCall(const NIdentifier& id, ExpressionList& arguments) :
        id(id), arguments(arguments)
{
  updateType();
}

NFunctionCall::NFunctionCall(const NIdentifier& id) : id(id)
{
  updateType();
}

void NFunctionCall::updateType()
{
  AstNode * declaration = symbol.findFun(id.name);

  if(declaration==NULL){
    std::cout<<"Error: function "<<id.name<<"undeclared\n";
    return;
  }

  NFunctionDeclaration * func =dynamic_cast<NFunctionDeclaration*> (declaration);
  if(func == 0){
    std::cout<<"Error: " << id.name<<"is not a function. Should not happen.\n";
    return;
  }
  type = func->getType();
}
