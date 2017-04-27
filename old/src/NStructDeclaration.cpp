/*
 * NStructDeclaration.cpp
 *
 *  Created on: Jul 2, 2013
 *      Author: desaic
 */
#include "NStatement.hpp"
#include "NIdentifier.hpp"
#include "NStructDeclaration.hpp"
#include "codegen.h"
#include <iostream>
llvm::Value*
NStructDeclaration::codeGen(CodeGenContext& context)
{

  return NULL;
}

std::string
NStructDeclaration::getName()
{
  return name;
}

NStructDeclaration *
NStructDeclaration::FindOrCreate(const NIdentifier & id)
{
  AstNode * node = symbol.findStruct(id.name);
  NStructDeclaration * structNode = dynamic_cast<NStructDeclaration*>(node);
  if(structNode == NULL){
    node =new NStructDeclaration(id.name);
    symbol.addLocalStruct(id.name, structNode );
    return structNode;
  }
  if(structNode == 0){
    std::cout<<"Error: unknown struct "<<  id.name<<".\n";
  }
  return structNode;
}

void NStructDeclaration::setFields(const VariableList & _fields)
{
  //cannot have empty struct body
  if(fields.size()>0){
    std::cout<<"Error: struct already defined\n";
    return;
  }
  fields = _fields;
}

NStructDeclaration::NStructDeclaration(const std::string & _name):
  name(_name)
{}

NStructDeclaration::NStructDeclaration()
{}

NStructDeclaration::~NStructDeclaration()
{}

