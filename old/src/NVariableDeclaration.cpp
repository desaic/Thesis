/*
 * NVariableDeclaration.cpp
 *
 *  Created on: Jun 26, 2013
 *      Author: desaic
 */

#include "NStatement.hpp"
#include "Ast.hpp"
#include "NVariableDeclaration.hpp"
#include "NIdentifier.hpp"

#include <llvm/Value.h>
#include <llvm/Instructions.h>
#include <iostream>
void NVariableDeclaration::addSymbol()
{
  symbol.addLocalVar(id.name, this);
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
  std::cout << "Creating variable declaration " << type.toString() <<
      " "<<id.name<<"\n";
  llvm::AllocaInst *alloc =
      context.builder.CreateAlloca(type.getLLVMType(), 0, id.name.c_str());
  context.locals()[id.name] = Symbol(this,alloc);
  if (assignmentExpr != NULL) {
    NAssignment assn(id, *assignmentExpr);
    assn.codeGen(context);
  }
  return alloc;
}


NVariableDeclaration::~NVariableDeclaration()
{}
