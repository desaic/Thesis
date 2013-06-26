/*
 * NVariableDeclaration.cpp
 *
 *  Created on: Jun 26, 2013
 *      Author: desaic
 */

#include "NVariableDeclaration.hpp"
#include <llvm/Value.h>
#include <llvm/Instructions.h>
NVariableDeclaration::NVariableDeclaration(const AstType & _type, NIdentifier& id) :
type(new AstType(_type)), id(id), assignmentExpr(0)
{
}

NVariableDeclaration::NVariableDeclaration(const AstType &_type, NIdentifier& id,
    NExpression *assignmentExpr) :
type(new AstType(_type)), id(id), assignmentExpr(assignmentExpr)
{
}

llvm::Value*
NVariableDeclaration::codeGen(CodeGenContext& context)
{
  std::cout << "Creating variable declaration " << type->typeId << std::endl;
  llvm::AllocaInst *alloc =
      new llvm::AllocaInst(type->getLLVMType(), id.name.c_str(), context.currentBlock());
  context.locals()[id.name] = Symbol(this,alloc);
  if (assignmentExpr != NULL) {
    NAssignment assn(id, *assignmentExpr);
    assn.codeGen(context);
  }
  return alloc;
}


NVariableDeclaration::~NVariableDeclaration()
{
  if(type != NULL) {
    delete type;
    type = 0;
  }
}
