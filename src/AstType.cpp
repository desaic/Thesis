/*
 * Type.cpp
 *
 *  Created on: Jun 18, 2013
 *      Author: desaic
 */

#include "AstType.hpp"
#include "codegen.h"
#include <llvm/LLVMContext.h>
#include <llvm/Type.h>
#include <llvm/DerivedTypes.h>
#include <llvm/Value.h>
#include <llvm/Constants.h>
#include <llvm/InstrTypes.h>
#include <llvm/Instructions.h>
#include <iostream>

bool AstType::isAnyInt()const
{
  return typeId == AST_INT || typeId == AST_INT64;
}

bool AstType::isAnyFloat()const
{
  return typeId == AST_FLOAT || typeId == AST_DOUBLE;
}

std::string AstType::toString() const
{
  switch (typeId) {
  case AST_VOID:
    return "void";
  case AST_BOOL:
    return "bool";
  case AST_DOUBLE:
    return "double";
  case AST_FLOAT:
    return "float";
  case AST_INT:
    return "int";
  case AST_INT64:
    return "int64";
  case AST_INVALID:
    return "invalidType";
  default:
    return "undefType";
  }
}

llvm::Value * castIntToBool(llvm::Value * S, CodeGenContext & context)
{
  llvm::Value * zero = llvm::ConstantInt::get(S->getType(), 0);
  llvm::Value * inst = context.builder.CreateICmpNE(S, zero,"int2bool_cmp");
  return inst;
}

llvm::Value * castFPToBool(llvm::Value * S, CodeGenContext & context)
{
  llvm::Value * zero = llvm::ConstantFP::get(S->getType(), 0.0f);
  llvm::Value * inst = context.builder.CreateFCmpUNE(S, zero,"float2bool_cmp");
  return inst;
}

bool canCast(const AstType * src, const AstType * dst)
{
  //unimplemented
  return true;
}

llvm::Value * cast(const AstType * src, const AstType * dst, llvm::Value * S,
   CodeGenContext & context)
{
  llvm::Value * castInst = NULL;
  llvm::Type * targetTy = dst->getLLVMType();
  std::cout << "Status: Cast from " << src->toString() << " to "
      << dst->toString() << "\n";

  if(src->isAnyInt()){
    if(dst->isAnyInt()){
      return context.builder.CreateSExtOrTrunc(S, (llvm::IntegerType*)targetTy,"int2int");
    }
    if(dst->isAnyFloat()){
      return context.builder.CreateSIToFP(S, targetTy,"int2float");
    }
  }

  if (src->isAnyFloat()) {
    if (dst->isAnyFloat()) {
      return context.builder.CreateFPCast(S,targetTy,"float2float");
    }
    if (dst->isAnyInt()) {
      return context.builder.CreateFPToSI(S,targetTy,"float2int");
    }
  }

  switch (src->getId()) {
  case AstType::AST_BOOL:
    switch (dst->getId()) {
    case AstType::AST_INT:
    case AstType::AST_INT64:
      castInst = context.builder.CreateZExt(S,targetTy,"bool2int");
      break;
    case AstType::AST_FLOAT:
    case AstType::AST_DOUBLE:
      castInst = context.builder.CreateUIToFP(S, targetTy, "bool2float");
      break;
    }
    break;
  case AstType::AST_INT:
  case AstType::AST_INT64:
    switch (dst->getId()) {
    case AstType::AST_BOOL:
      castInst = castIntToBool(S, context);
      break;
    }
    break;
  case AstType::AST_FLOAT:
  case AstType::AST_DOUBLE:
    switch (dst->getId()) {
    case AstType::AST_BOOL:
      castInst = castFPToBool(S, context);
      break;
    }
    break;
  }
  if (castInst == 0) {
    std::cout << "Status: cast failed.\n";
  }
  return castInst;
}

int AstType::getId() const
{
  return typeId;
}

AstType &
AstType::operator=(const AstType& ty)
{
  typeId = ty.typeId;
  return *this;
}

AstType::AstType(const AstType & ty) :
    typeId(ty.typeId)
{
}

void AstType::set(int id)
{
  typeId = id;
}

AstType::AstType() :
    typeId(AST_INVALID)
{
}

AstType::AstType(int _id) :
    typeId(_id)
{
}

AstType::~AstType()
{
}

llvm::Type *AstType::getLLVMType() const
{

  switch (typeId) {
  case AstType::AST_VOID:
    return llvm::Type::getVoidTy(llvm::getGlobalContext());
  case AstType::AST_BOOL:
    return llvm::Type::getInt1Ty(llvm::getGlobalContext());
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

