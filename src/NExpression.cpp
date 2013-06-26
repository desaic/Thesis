/*
 * NExpression.cpp
 *
 *  Created on: Jun 21, 2013
 *      Author: desaic
 */

#include "NExpression.hpp"

const AstType *
NExpression::getType() const
{
  return type;
}

NExpression::NExpression(const AstType & _type)
  :type(new AstType(_type))
{
}

NExpression::NExpression(const NExpression&expr)
:type(new AstType(*(expr.getType())))
{
}

NExpression::NExpression() :
    type(0)
{
}

NExpression::~NExpression()
{
  if (type != 0) {
    delete type;
  }
}
