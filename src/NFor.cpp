/*
 * NFor.cpp
 *
 *  Created on: Jul 2, 2013
 *      Author: desaic
 */

#include "NStatement.hpp"
#include "NFor.hpp"
#include "codegen.h"
#include <llvm/Value.h>

llvm::Value*
NFor::codeGen(CodeGenContext& context)
{
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

