/*
 * NStructDeclaration.hpp
 *
 *  Created on: Jul 2, 2013
 *      Author: desaic
 */

#ifndef NSTRUCTDECLARATION_HPP_
#define NSTRUCTDECLARATION_HPP_

#include "NStatement.hpp"
#include <string>
class NStructDeclaration: public NStatement
{
public:
  NStructDeclaration();
  NStructDeclaration(const std::string & _name);
  virtual ~NStructDeclaration();
  static NStructDeclaration * FindOrCreate(const NIdentifier & id);
  void setFields(const VariableList & _fields);
  std::string getName();
private:
  std::string name;
  VariableList fields;
};

#endif /* NSTRUCTDECLARATION_HPP_ */
