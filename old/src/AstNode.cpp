/*
 * AstNode.cpp
 *
 *  Created on: Jun 21, 2013
 *      Author: desaic
 */

#include "AstNode.hpp"
SymbolTable AstNode::symbol;

const AstType & AstNode::getType() const
{
  return type;
}

AstNode::AstNode()
{}

AstNode::AstNode(int astType):
    type(astType)
{}

AstNode::AstNode(const AstType & _type):
  type(_type)
{}

AstNode::~AstNode() {}

llvm::Value*
AstNode::codeGen(CodeGenContext&)
{
  return 0;
}
