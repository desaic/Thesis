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
#include <Type.hpp>
#include <iostream>
#include <vector>
using namespace llvm;
#include "Ast.hpp"
#include "parser.hpp"

///@brief lower indices are converted to higher indices
///@return -1 if don't know how to convert
int typeOrder(const int typeId)
{
  switch(typeId){
  case AstType::AST_INT:
    return 0;
  case AstType::AST_INT64:
    return 1;
  case AstType::AST_FLOAT:
    return 2;
  case AstType::AST_DOUBLE:
    return 3;
  default:
    return -1;
  }
}

AstType * getCommonType(const std::vector<const AstType*> & args)
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
  return new AstType(commonType);
}

NBinaryOp::~NBinaryOp()
{}

NBinaryOp::NBinaryOp(NExpression& _lhs, int op, NExpression& _rhs) :
        lhs(_lhs), rhs(_rhs), op(op)
{}

Value * cast(const AstType * src, const AstType * dst, Value * S)
{
  Value * castInst = NULL;
  Type * targetTy = dst->getLLVMType();
  switch(src->getId()){
  case AstType::AST_INT:
  case AstType::AST_INT64:
    switch(dst->getId()){
    case AstType::AST_INT:
    case AstType::AST_INT64:
      castInst = CastInst::CreateIntegerCast(S, targetTy, true);
      break;
    case AstType::AST_FLOAT:
    case AstType::AST_DOUBLE:
      castInst = new SIToFPInst(S,targetTy);
      break;
    }
    break;
  case AstType::AST_FLOAT:
  case AstType::AST_DOUBLE:
    switch(dst->getId()){
    case AstType::AST_FLOAT:
    case AstType::AST_DOUBLE:
      castInst = CastInst::CreateFPCast(S,targetTy);
      break;
    case AstType::AST_INT:
    case AstType::AST_INT64:
      castInst = new FPToSIInst(S,targetTy);
      break;
    }
    break;
  }
  return castInst;
}

const AstType *
NBinaryOp::getType()
{
  if(type!=0){
    return type;
  }
  std::vector<const AstType *> args(2);
  args[0] = lhs.getType();
  args[1] = rhs.getType();
  if(args[0]==0 || args[1]==0){
    std::cout<<"Error: Unknow operand types\n";
    return type;
  }
  type = getCommonType(args);
  return type;
}

Value* NBinaryOp::codeGen(CodeGenContext& context)
{
  std::cout << "Creating binary operation " << op << std::endl;
  getType();
  if(type==0){
    std::cout<<"Error: Binary op type undefined\n";
    return NULL;
  }
  Value * vals[2];
  Value * operands[2];
  vals[0] = lhs.codeGen(context);
  vals[1] = rhs.codeGen(context);
  const AstType * types[2];
  types[0] = lhs.getType();
  types[1] = rhs.getType();
  for(size_t ii = 0;ii<2;ii++){
    if(types[ii]->getId()!=type->getId()){
      operands[ii] = cast(types[ii],type, vals[ii]);
    }
  }
  
  int instr=-1;
  switch (op) {
  case ADD:
    switch (type->getId()) {
    case AstType::AST_FLOAT:
    case AstType::AST_DOUBLE:
      instr = Instruction::FAdd;
      break;
    case AstType::AST_INT:
    case AstType::AST_INT64:
      instr = Instruction::Add;
      break;
    }
    break;
  case SUB:
    switch (type->getId()) {
    case AstType::AST_FLOAT:
    case AstType::AST_DOUBLE:
      instr = Instruction::FSub;
      break;
    case AstType::AST_INT:
    case AstType::AST_INT64:
      instr = Instruction::Sub;
      break;
    }
    break;
  case MUL:
    switch (type->getId()) {
    case AstType::AST_FLOAT:
    case AstType::AST_DOUBLE:
      instr = Instruction::FMul;
      break;
    case AstType::AST_INT:
    case AstType::AST_INT64:
      instr = Instruction::Mul;
      break;
    }
    break;
  case DIV:
    switch (type->getId()) {
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
    /* TODO comparison */
  default:
    break;
  }
  if(instr==-1){
    std::cout << "Error: Unknown operator" << op << " for types "
          << types[0]->getId() << " and " << types[1]->getId() << "\n";
      return NULL;
  }
  return BinaryOperator::Create((Instruction::BinaryOps)instr, operands[0],
    operands[1], "", context.currentBlock());
}
