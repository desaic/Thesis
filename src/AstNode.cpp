/*
 * AstNode.cpp
 *
 *  Created on: Jun 21, 2013
 *      Author: desaic
 */

#include "AstNode.hpp"
SymbolTable AstNode::symbol;
AstNode::AstNode()
{}

AstNode::~AstNode() {}
llvm::Value*
AstNode::codeGen(CodeGenContext&)
{
  return 0;
}
