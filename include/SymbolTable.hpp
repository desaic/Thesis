/*
 * SymbolTable.hpp
 *
 *  Created on: Jun 26, 2013
 *      Author: desaic
 */

#ifndef SYMBOLTABLE_HPP_
#define SYMBOLTABLE_HPP_
#include <map>
#include <string>
#include <vector>
class AstNode;
typedef std::map<std::string, AstNode * > SymbolMap;
struct SymbolTable
{
  std::vector<SymbolMap> symbol;
  void incScope();
  void decScope();
  AstNode * find(const std::string & name);
  void addLocalSymbol(const std::string & name, AstNode * node);
};

#endif /* SYMBOLTABLE_HPP_ */
