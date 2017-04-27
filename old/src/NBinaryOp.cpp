/*
 * NBinaryOp.cpp
 *
 *  Created on: Jun 24, 2013
 *      Author: desaic
 */

#include "NBinaryOp.hpp"

#include <llvm/InstrTypes.h>
#include <llvm/Instructions.h>
#include <llvm/Value.h>
#include "AstType.hpp"
#include <iostream>
#include <vector>
using namespace llvm;
#include "Ast.hpp"
#include "parser.hpp"
std::string op2String(int op){
  switch(op){
  case ADD:
    return std::string("+");
  case SUB:
    return std::string("-");
  case MUL:
    return std::string("*");
  case DIV:
    return std::string("/");
  case EQ:
    return std::string("==");
  case NEQ:
    return std::string("!=");
  case GT:
    return std::string(">");
  case LT:
    return std::string("<");
  case GEQ:
    return std::string(">=");
  case LEQ:
    return std::string("<=");
  }
  return std::string("unknown");
}
///@brief lower indices are converted to higher indices
///@return -1 if don't know how to convert
int typeOrder(const int typeId)
{
  switch(typeId){
  case AstType::AST_BOOL:
    return 10;
  case AstType::AST_INT:
    return 20;
  case AstType::AST_INT64:
    return 30;
  case AstType::AST_FLOAT:
    return 40;
  case AstType::AST_DOUBLE:
    return 50;
  default:
    return -1;
  }
}

AstType getCommonType(const std::vector<const AstType*> & args)
{
  int maxIdx=-1;
  int commonType=0;
  for(unsigned int ii = 0;ii<args.size();ii++){
    int ty = args[ii]->getId();
    int idx = typeOrder(ty);
    if(maxIdx<idx){
      commonType = ty;
      maxIdx = idx;
    }
  }
  return AstType(commonType);
}

NBinaryOp::~NBinaryOp()
{}

NBinaryOp::NBinaryOp(NExpression* _lhs, int op, NExpression* _rhs) :
        lhs(_lhs), rhs(_rhs), op(op)
{updateType();}

void
NBinaryOp::updateType()
{
  if(type.getId()!=AstType::AST_INVALID){
    return;
  }
  if(isComparison()){
    type= AstType::AST_BOOL;
    return;
  }
  std::vector<const AstType *> args(2);
  args[0] = &lhs->getType();
  args[1] = &rhs->getType();
  if(args[0]==0 || args[1]==0){
    std::cout<<"Error: Unknow operand types\n";
    return;
  }
  type = getCommonType(args);
}

Value* NBinaryOp::genCmpInst(
    const AstType & operandTy,
    llvm::Value * vals[],CodeGenContext& context)
{
  llvm::Value * ret = NULL;
  switch(op){
  case EQ:
    switch(operandTy.getId()){
    case AstType::AST_BOOL:
    case AstType::AST_INT:
    case AstType::AST_INT64:
      ret=context.builder.CreateICmpEQ(vals[0],vals[1]);
      break;
    case AstType::AST_FLOAT:
    case AstType::AST_DOUBLE:
      ret=context.builder.CreateFCmpOEQ(vals[0],vals[1]);
      break;
    }
    break;
  case NEQ:
    switch (operandTy.getId()) {
    case AstType::AST_BOOL:
    case AstType::AST_INT:
    case AstType::AST_INT64:
      ret=context.builder.CreateICmpNE(vals[0], vals[1]);
      break;
    case AstType::AST_FLOAT:
    case AstType::AST_DOUBLE:
      ret=context.builder.CreateFCmpUNE(vals[0], vals[1]);
      break;
    }
    break;
  case GT:
    switch (operandTy.getId()) {
    case AstType::AST_INT:
    case AstType::AST_INT64:
      ret=context.builder.CreateICmpSGT(vals[0], vals[1]);
      break;
    case AstType::AST_FLOAT:
    case AstType::AST_DOUBLE:
      ret=context.builder.CreateFCmpOGT(vals[0], vals[1]);
      break;
    }
    break;
  case GEQ:
    switch (operandTy.getId()) {
    case AstType::AST_INT:
    case AstType::AST_INT64:
      ret = context.builder.CreateICmpSGE(vals[0], vals[1]);
      break;
    case AstType::AST_FLOAT:
    case AstType::AST_DOUBLE:
      ret = context.builder.CreateFCmpOGE(vals[0], vals[1]);
      break;
    }
    break;
  case LT:
    switch (operandTy.getId()) {
    case AstType::AST_INT:
    case AstType::AST_INT64:
      ret = context.builder.CreateICmpSLT(vals[0], vals[1]);
      break;
    case AstType::AST_FLOAT:
    case AstType::AST_DOUBLE:
      ret = context.builder.CreateFCmpOLT(vals[0], vals[1]);
      break;
    }
    break;
  case LEQ:
    switch (operandTy.getId()) {
    case AstType::AST_INT:
    case AstType::AST_INT64:
      ret = context.builder.CreateICmpSLE(vals[0], vals[1]);
      break;
    case AstType::AST_FLOAT:
    case AstType::AST_DOUBLE:
      ret = context.builder.CreateFCmpOLE(vals[0], vals[1]);
      break;
    }
    break;
  }
  if(ret == NULL){
    std::cout<<"Error: Undefined comparison operator "<<
        op2String(op)<< " for type "<<
        type.toString();
  }
  return ret;
}


Value* NBinaryOp::codeGen(CodeGenContext& context)
{
  std::cout << "Creating binary operation " <<
      op2String(op) << std::endl;

  getType();

  std::vector<const AstType *> args(2);
  args[0] = &lhs->getType();
  args[1] = &rhs->getType();
  if (args[0] == 0 || args[1] == 0) {
    std::cout << "Error: Unknow operand types\n";
  }
  AstType commonTy = getCommonType(args);
  if(commonTy.getId() == AstType::AST_INVALID){
    std::cout<<"Error: Binary op type undefined\n";
    return NULL;
  }

  Value * vals[2];
  vals[0] = lhs->codeGen(context);
  vals[1] = rhs->codeGen(context);
  const AstType * types[2];
  types[0] = &lhs->getType();
  types[1] = &rhs->getType();
  for(size_t ii = 0;ii<2;ii++){
    if(types[ii]->getId()!=commonTy.getId()){
      vals[ii] = cast(types[ii],&commonTy, vals[ii],context);
    }
  }
  
  if(isArithmetic()){
    return genBinOp(vals,types,context);
  }

  if(isComparison()){
    return genCmpInst(commonTy, vals,context);
  }

  /* TODO logic operations*/
  return NULL;
}

bool NBinaryOp::isComparison()
{
  switch(op){
  case EQ:
  case NEQ:
  case GT:
  case GEQ:
  case LT:
  case LEQ:
    return true;
  }
  return false;
}

Value * NBinaryOp::genBinOp(
    Value * vals[]
    , const AstType * types[]
    ,CodeGenContext& context)
{
  int instr=-1;
    switch (op) {
    case ADD:
      switch (type.getId()) {
      case AstType::AST_FLOAT:
      case AstType::AST_DOUBLE:
        instr = Instruction::FAdd;
        break;
      case AstType::AST_BOOL:
      case AstType::AST_INT:
      case AstType::AST_INT64:
        instr = Instruction::Add;
        break;
      }
      break;
    case SUB:
      switch (type.getId()) {
      case AstType::AST_FLOAT:
      case AstType::AST_DOUBLE:
        instr = Instruction::FSub;
        break;
      case AstType::AST_BOOL:
      case AstType::AST_INT:
      case AstType::AST_INT64:
        instr = Instruction::Sub;
        break;
      }
      break;
    case MUL:
      switch (type.getId()) {
      case AstType::AST_FLOAT:
      case AstType::AST_DOUBLE:
        instr = Instruction::FMul;
        break;
      case AstType::AST_BOOL:
      case AstType::AST_INT:
      case AstType::AST_INT64:
        instr = Instruction::Mul;
        break;
      }
      break;
    case DIV:
      switch (type.getId()) {
      case AstType::AST_FLOAT:
      case AstType::AST_DOUBLE:
        instr = Instruction::FDiv;
        break;
      case AstType::AST_INT:
      case AstType::AST_INT64:
        instr = Instruction::SDiv;
        break;
      }
      break;
    default:
      break;
    }
    if(instr==-1){
      std::cout << "Error: Unknown arithmetic operator " <<
          op2String(op) << " for types "
          << types[0]->toString() << " and " << types[1]->toString()<< "\n";
        return NULL;
    }
    return context.builder.CreateBinOp((Instruction::BinaryOps)instr, vals[0], vals[1], "");
}

bool NBinaryOp::isArithmetic()
{
  switch(op){
  case ADD:
  case SUB:
  case MUL:
  case DIV:
    return true;
  }
  return false;
}
