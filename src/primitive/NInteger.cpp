/*
 * NInteger.cpp
 *
 *  Created on: Jun 27, 2013
 *      Author: desaic
 */
#include "NExpression.hpp"
#include "NInteger.hpp"
#include "codegen.h"
#include <llvm/Value.h>
#include <llvm/Type.h>
#include <iostream>

llvm::Value*
NInteger::codeGen(CodeGenContext& context)
{
  std::cout << "Creating integer: " << value << "\n";
  return llvm::ConstantInt::get(
      llvm::Type::getInt32Ty(llvm::getGlobalContext()), value, true);
}

NInteger::NInteger(int value) :
    NExpression(AstType(AstType::AST_INT)),
    value(value) { }

NInteger::~NInteger()
{}

