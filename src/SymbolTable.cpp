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
  symbol.push_back(SymbolMap());
}

void SymbolTable::decScope(){
  symbol.pop_back();
}

AstNode *
SymbolTable::find(const std::string & name)
{
  for (int ii = (int) symbol.size() - 1; ii >= 0; ii--) {
    if (symbol[ii].find(name) != symbol[ii].end()) {
      return symbol[ii][name];
    }
  }
  return NULL;
}

void
SymbolTable::addLocalSymbol(const std::string & name, AstNode * node)
{
#ifdef DEBUG
  if(symbol.size()==0) {
    std::cout<<"Error: Uninitialized symbol table\n";
  }
#endif
  symbol.back()[name] = node;
}

