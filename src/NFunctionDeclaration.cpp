/*
 * NFunctionDeclaration.cpp
 *
 *  Created on: Jun 26, 2013
 *      Author: desaic
 */

#include "NFunctionDeclaration.hpp"
#include "NVariableDeclaration.hpp"
#include "AstNode.hpp"
#include "Type.hpp"
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
  llvm::BasicBlock *bblock = llvm::BasicBlock::Create
      (llvm::getGlobalContext(), "entry", function, 0);

  context.pushBlock(bblock);

  for (it = arguments.begin(); it != arguments.end(); it++) {
    (**it).codeGen(context);
  }

  block.codeGen(context);

  context.popBlock();
  std::cout << "Creating function: " << id.name << std::endl;
  return function;
}


NFunctionDeclaration::NFunctionDeclaration(const AstType & _type,
    const NIdentifier& id, const VariableList& arguments, NBlock& block) :
    NStatement(_type), id(id), arguments(arguments), block(block)
{
  symbol.addLocalSymbol(id.name, this);
}

NFunctionDeclaration:: ~NFunctionDeclaration()
{}
