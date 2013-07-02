/*
 * NStructDeclaration.hpp
 *
 *  Created on: Jul 2, 2013
 *      Author: desaic
 */

#ifndef NSTRUCTDECLARATION_HPP_
#define NSTRUCTDECLARATION_HPP_
#include "VariableList.hpp"

class NStatement;
class CodeGenContext;
namespace llvm{
class Value;
}

#include <string>
class NStructDeclaration: public NStatement
{
public:
  NStructDeclaration();
  NStructDeclaration(const std::string & _name);
  virtual ~NStructDeclaration();
  llvm::Value* codeGen(CodeGenContext& context);
  static NStructDeclaration * FindOrCreate(const NIdentifier & id);
  void setFields(const VariableList & _fields);
  std::string getName();
private:
  std::string name;
  VariableList fields;
};

#endif /* NSTRUCTDECLARATION_HPP_ */
