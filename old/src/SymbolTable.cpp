/*
 * SymbolTable.cpp
 *
 *  Created on: Jun 26, 2013
 *      Author: desaic
 */

#include "SymbolTable.hpp"
#include <iostream>
#include "AstNode.hpp"
void
SymbolTable::incScope()
{
  vars.push_back(SymbolMap());
  funs.push_back(SymbolMap());
  structs.push_back(SymbolMap());
}

void SymbolTable::decScope(){
  vars.pop_back();
  funs.pop_back();
  structs.pop_back();
}

AstNode *
SymbolTable::findVar(const std::string & name)
{
  for (int ii = (int) vars.size() - 1; ii >= 0; ii--) {
    if (vars[ii].find(name) != vars[ii].end()) {
      return vars[ii][name];
    }
  }
  return NULL;
}

void
SymbolTable::addLocalVar(const std::string & name, AstNode * node)
{
#ifdef DEBUG
  if(vars.size()==0) {
    std::cout<<"Error: Uninitialized symbol table\n";
  }
#endif
  vars.back()[name] = node;
}

AstNode *
SymbolTable::findFun(const std::string & name)
{
  for (int ii = (int) funs.size() - 1; ii >= 0; ii--) {
    if (funs[ii].find(name) != funs[ii].end()) {
      return funs[ii][name];
    }
  }
  return NULL;
}

void
SymbolTable::addLocalFun(const std::string & name, AstNode * node)
{
#ifdef DEBUG
  if(funs.size()==0) {
    std::cout<<"Error: Uninitialized symbol table\n";
  }
#endif
  funs.back()[name] = node;
}

AstNode *
SymbolTable::findStruct(const std::string & name)
{
  for (int ii = (int) structs.size() - 1; ii >= 0; ii--) {
    if (structs[ii].find(name) != structs[ii].end()) {
      return structs[ii][name];
    }
  }
  return NULL;
}

void
SymbolTable::addLocalStruct(const std::string & name, AstNode * node)
{
#ifdef DEBUG
  if(structs.size()==0) {
    std::cout<<"Error: Uninitialized symbol table\n";
  }
#endif
  structs.back()[name] = node;
}

