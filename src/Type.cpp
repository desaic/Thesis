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
#include <llvm/Value.h>
#include <llvm/InstrTypes.h>
#include <llvm/Instructions.h>

std::string AstType::toString()const
{
  switch(typeId){
  case AST_INT:
    return "int";
  case AST_INT64:
    return "int64";
  case AST_FLOAT:
    return "float";
  case AST_DOUBLE:
    return "double";
  case AST_VOID:
    return "void";
  case AST_INVALID:
    return "invalidType";
  default:
    return "undefType";
  }
}

llvm::Value * cast(const AstType * src, const AstType * dst, llvm::Value * S)
{
  llvm::Value * castInst = NULL;
  llvm::Type * targetTy = dst->getLLVMType();
  switch(src->getId()){
  case AstType::AST_INT:
  case AstType::AST_INT64:
    switch(dst->getId()){
    case AstType::AST_INT:
    case AstType::AST_INT64:
      castInst = llvm::CastInst::CreateIntegerCast(S, targetTy, true);
      break;
    case AstType::AST_FLOAT:
    case AstType::AST_DOUBLE:
      castInst = new llvm::SIToFPInst(S,targetTy);
      break;
    }
    break;
  case AstType::AST_FLOAT:
  case AstType::AST_DOUBLE:
    switch(dst->getId()){
    case AstType::AST_FLOAT:
    case AstType::AST_DOUBLE:
      castInst = llvm::CastInst::CreateFPCast(S,targetTy);
      break;
    case AstType::AST_INT:
    case AstType::AST_INT64:
      castInst = new llvm::FPToSIInst(S,targetTy);
      break;
    }
    break;
  }
  return castInst;
}


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
typeId(AST_INVALID)
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

