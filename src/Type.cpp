/*
 * Type.cpp
 *
 *  Created on: Jun 18, 2013
 *      Author: desaic
 */

#include "Type.hpp"
#include <llvm/LLVMContext.h>
#include <llvm/Type.h>
#include <llvm/DerivedTypes.h>
int AstType::getId()const
{
  return typeId;
}

AstType &
AstType::operator=(const AstType& ty)
{
  typeId = ty.typeId;
  return *this;
}

AstType::AstType(const AstType & ty):
typeId(ty.typeId)
{}

void AstType::set(int id)
{
  typeId = id;
}

AstType::AstType():
typeId(AST_VOID)
{}

AstType::AstType(int _id):
    typeId(_id)
{}

AstType::~AstType()
{}

llvm::Type *AstType::getLLVMType()const
{

  switch (typeId) {
  case AstType::AST_VOID:
    return llvm::Type::getVoidTy(llvm::getGlobalContext());
  case AstType::AST_FLOAT:
    return llvm::Type::getFloatTy(llvm::getGlobalContext());
  case AstType::AST_DOUBLE:
    return llvm::Type::getDoubleTy(llvm::getGlobalContext());
  case AstType::AST_INT:
    return llvm::Type::getInt32Ty(llvm::getGlobalContext());
  case AstType::AST_INT64:
    return llvm::Type::getInt64Ty(llvm::getGlobalContext());
    //case struct todo
  }
  return llvm::Type::getVoidTy(llvm::getGlobalContext());
}

