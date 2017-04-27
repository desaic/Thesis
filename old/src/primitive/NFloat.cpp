/*
 * NFloat.cpp
 *
 *  Created on: Jun 27, 2013
 *      Author: desaic
 */

#include "NExpression.hpp"
#include "NFloat.hpp"
#include "codegen.h"
#include <llvm/Value.h>
#include <llvm/Type.h>
#include <iostream>
llvm::Value* NFloat::codeGen(CodeGenContext& context)
{
  std::cout << "Creating float: " << value << std::endl;
  return llvm::ConstantFP::get(
      llvm::Type::getFloatTy(llvm::getGlobalContext()), value);
}

NFloat::NFloat(float value) :
      NExpression(AstType(AstType::AST_FLOAT)),
      value(value) { }

NFloat::~NFloat()
{}

