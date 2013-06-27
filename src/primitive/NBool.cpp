/*
 * NBool.cpp
 *
 *  Created on: Jun 27, 2013
 *      Author: desaic
 */

#include "NBool.hpp"
#include "AstType.hpp"
#include <iostream>
llvm::Value* NBool::codeGen(CodeGenContext& context)
{
  std::cout << "Creating bool: " << val << "\n";
  return ConstantInt::get(Type::getInt32Ty(getGlobalContext()), value);
}

NBool::NBool(bool _val):
NExpression(AstType::AST_BOOL),val(_val)
{}

NBool::NBool():val(false)
{}

NBool::~NBool()
{}

