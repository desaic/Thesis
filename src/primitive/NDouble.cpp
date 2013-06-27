/*
 * NDouble.cpp
 *
 *  Created on: Jun 27, 2013
 *      Author: desaic
 */

#include "NExpression.hpp"
#include "NDouble.hpp"
#include <llvm/Value.h>
#include "codegen.h"
#include <llvm/Type.h>
#include <iostream>

llvm::Value* NDouble::codeGen(CodeGenContext& context)
{
  std::cout << "Creating double: " << value << std::endl;
  return llvm::ConstantFP::get(
      llvm::Type::getDoubleTy(llvm::getGlobalContext()), value);
}
NDouble:: NDouble(double _value):
      NExpression(AstType(AstType::AST_DOUBLE)),
      value(_value) { }

NDouble::~NDouble()
{}

