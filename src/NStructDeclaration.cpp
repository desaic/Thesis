/*
 * NStructDeclaration.cpp
 *
 *  Created on: Jul 2, 2013
 *      Author: desaic
 */

#include "NStructDeclaration.hpp"

std::string
NStructDeclaration::getName()
{
  return name;
}

NStructDeclaration *
NStructDeclaration::FindOrCreate(const NIdentifier & id)
{
  Symbol ss = symbol.findStruct(id.name);
  if(ss.value == NULL){
    NStructDeclaration * structNode =new NStructDeclaration(id.name);
    symbol.addLocalStruct(id.name, structNode );
    return structNode;
  }
  NStructDeclaration * structNode = dynamic_cast<NStructDeclaration*>(ss.value);
  if(structNode == 0){
    std::cout<<"Error: Struct symbols contain a non-struct.\n";
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

