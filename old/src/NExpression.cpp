/*
 * NExpression.cpp
 *
 *  Created on: Jun 21, 2013
 *      Author: desaic
 */

#include "NExpression.hpp"
NExpression::NExpression(const AstType & _type)
  :AstNode(_type)
{
}

NExpression::NExpression(const NExpression&expr)
:AstNode(expr.getType())
{}

NExpression::NExpression(){
}

NExpression::~NExpression()
{}
