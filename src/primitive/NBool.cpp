/*
 * NBool.cpp
 *
 *  Created on: Jun 27, 2013
 *      Author: desaic
 */

#include "NBool.hpp"
#include "AstType.hpp"
#include <llvm/Value.h>
#include <llvm/Constants.h>
#include <llvm/Type.h>
#include <iostream>
llvm::Value* NBool::codeGen(CodeGenContext& context)
{
  std::cout << "Creating bool: " << val << "\n";
  return llvm::ConstantInt::get(llvm::Type::getInt1Ty(llvm::getGlobalContext()), val);
}

NBool::NBool(bool _val):
NExpression(AstType::AST_BOOL),val(_val)
{}

NBool::NBool():val(false)
{}

NBool::~NBool()
{}

