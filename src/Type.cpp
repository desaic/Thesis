/*
 * Type.cpp
 *
 *  Created on: Jun 18, 2013
 *      Author: desaic
 */

#include "Type.hpp"

AstType &
AstType::operator=(const AstType& ty)
{
  typeId = ty.typeId;
  return *this;
}

AstType::AstType(const AstType & ty):
typeId(ty.typeId)
{}

void AstType::set(int id)
{
  typeId = id;
}

AstType::AstType():
typeId(AST_VOID)
{}

AstType::AstType(int _id):
    typeId(_id)
{}

AstType::~AstType()
{}
