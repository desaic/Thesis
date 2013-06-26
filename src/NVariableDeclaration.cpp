/*
 * NVariableDeclaration.cpp
 *
 *  Created on: Jun 26, 2013
 *      Author: desaic
 */

#include "NVariableDeclaration.hpp"
#include <llvm/Value.h>
#include <llvm/Instructions.h>
void NVariableDeclaration::addSymbol()
{
  symbol.addLocalSymbol(id.name, this);
}

NVariableDeclaration::NVariableDeclaration(const AstType & _type, NIdentifier& id) :
NStatement(_type), id(id), assignmentExpr(0)
{
  addSymbol();
  id.updateType();
}

NVariableDeclaration::NVariableDeclaration(const AstType &_type, NIdentifier& id,
    NExpression *assignmentExpr) :
NStatement(_type), id(id), assignmentExpr(assignmentExpr)
{
  addSymbol();
  id.updateType();
}

llvm::Value*
NVariableDeclaration::codeGen(CodeGenContext& context)
{
  std::cout << "Creating variable declaration " << type.getId() << "\n";
  llvm::AllocaInst *alloc =
      new llvm::AllocaInst(type.getLLVMType(), id.name.c_str(), context.currentBlock());
  context.locals()[id.name] = Symbol(this,alloc);
  if (assignmentExpr != NULL) {
    NAssignment assn(id, *assignmentExpr);
    assn.codeGen(context);
  }
  return alloc;
}


NVariableDeclaration::~NVariableDeclaration()
{}
