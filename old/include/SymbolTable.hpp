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
  std::vector<SymbolMap> vars;
  std::vector<SymbolMap> funs;
  std::vector<SymbolMap> structs;
  void incScope();
  void decScope();
  AstNode * findVar(const std::string & name);
  AstNode * findFun(const std::string & name);
  AstNode * findStruct(const std::string & name);
  void addLocalVar(const std::string & name, AstNode * node);
  void addLocalFun(const std::string & name, AstNode * node);
  void addLocalStruct(const std::string & name, AstNode * node);
};

#endif /* SYMBOLTABLE_HPP_ */
