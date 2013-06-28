/*
 * NFunctionDeclaration.cpp
 *
 *  Created on: Jun 26, 2013
 *      Author: desaic
 */

#include "NFunctionDeclaration.hpp"
#include "NVariableDeclaration.hpp"
#include "AstNode.hpp"
#include "AstType.hpp"
#include <llvm/Function.h>
#include <llvm/Type.h>
#include <llvm/Value.h>
#include <vector>
llvm::Value* NFunctionDeclaration::codeGen(CodeGenContext& context)
{
  std::vector< llvm::Type*> argTypes;
  VariableList::const_iterator it;
  for (it = arguments.begin(); it != arguments.end(); it++) {
    argTypes.push_back((**it).type.getLLVMType());
  }
  llvm::FunctionType *ftype = llvm::FunctionType::get
      (type.getLLVMType(), argTypes, false);

  llvm::Function *function = llvm::Function::Create
      (ftype, llvm::GlobalValue::InternalLinkage, id.name.c_str(), context.module);
  symbol.incScope();
  it = arguments.begin();
  for(llvm::Function::arg_iterator ai = function->arg_begin();
      ai != function->arg_end();ai++,it++){
    ai->setName((*it)->id.name);
    symbol.addLocalSymbol((*it)->id.name, (*it));
  }

  llvm::BasicBlock *bblock = llvm::BasicBlock::Create
      (llvm::getGlobalContext(), "entry", function);
  context.pushBlock(bblock);

  it = arguments.begin();
  for(llvm::Function::arg_iterator ai = function->arg_begin();
        ai != function->arg_end();ai++,it++){
    llvm::AllocaInst *alloc =
          context.builder.CreateAlloca((*it)->type.getLLVMType());
    context.builder.CreateStore(&(*ai), alloc);
    context.locals()[(*it)->id.name] = Symbol((*it),alloc);
  }

  block.codeGen(context);
  context.popBlock();
  std::cout << "Creating function: " << id.name << std::endl;
  symbol.decScope();
  return function;
}


NFunctionDeclaration::NFunctionDeclaration(const AstType & _type,
    NIdentifier& id, const VariableList& arguments, NBlock& block) :
    NStatement(_type), id(id), arguments(arguments), block(block)
{
  symbol.addLocalSymbol(id.name, this);
  id.updateType();
}

NFunctionDeclaration:: ~NFunctionDeclaration()
{}
