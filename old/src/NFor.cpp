/*
 * NFor.cpp
 *
 *  Created on: Jul 2, 2013
 *      Author: desaic
 */

#include "NStatement.hpp"
#include "NExpression.hpp"
#include "NBlock.hpp"
#include "NFor.hpp"
#include "codegen.h"
#include <llvm/Value.h>
#include <iostream>
llvm::Value*
NFor::codeGen(CodeGenContext& context)
{
  //generate initializer
  if(expr[0]!=NULL){
    expr[0]->codeGen(context);
  }
  llvm::Function * fun = context.currentBlock()->getParent();
  llvm::BasicBlock * condBB =llvm::BasicBlock::Create(llvm::getGlobalContext(),
        "forcond",fun);
  context.builder.CreateBr(condBB);
  context.pushBlock(condBB);

  //generate condition
  NExpression * cond = expr[1];
  llvm::Value * condVal;
  if (cond == NULL) {
    //no loop condition means loop forever
    condVal = llvm::ConstantInt::getTrue(llvm::getGlobalContext());
  }
  else {
    condVal = cond->codeGen(context);
    if (cond->getType().getId() != AstType::AST_BOOL) {
      AstType boolType(AstType::AST_BOOL);
      condVal = cast(&(cond->getType()), &boolType, condVal, context);
      if (condVal == NULL) {
        std::cout << "Error: cannot cast from " << cond->getType().toString()
            << " to bool.\n";
        return NULL;
      }
    }
  }

  llvm::BasicBlock * loopBB =llvm::BasicBlock::Create(llvm::getGlobalContext(),
        "forloop",fun);
  llvm::BasicBlock * endBB = llvm::BasicBlock::Create(llvm::getGlobalContext(),
      "endfor", fun);
  context.builder.CreateCondBr(condVal, loopBB, endBB);

  context.pushBlock(loopBB );

  if(block!=NULL){
    block->codeGen(context);
  }
  if(expr[2]!=0){
    expr[2]->codeGen(context);
  }
  context.builder.CreateBr(condBB);
  context.popBlock();   //loopBB
  context.popBlock();   //condBB

  context.builder.SetInsertPoint(endBB);
  return NULL;
}

NFor::NFor(NExpression * init, NExpression * cond,
      NExpression * end,NBlock * b):
expr{init,cond,end},block(b)
{}

NFor::NFor():
    expr{0,0,0},block(0)
{
}

NFor::~NFor()
{
}

