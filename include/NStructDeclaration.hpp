/*
 * NStructDeclaration.hpp
 *
 *  Created on: Jul 2, 2013
 *      Author: desaic
 */

#ifndef NSTRUCTDECLARATION_HPP_
#define NSTRUCTDECLARATION_HPP_

#include "NStatement.hpp"

class NStructDeclaration: public NStatement
{
public:
  NStructDeclaration();
  virtual ~NStructDeclaration();
  static NStructDeclaration * FindOrCreate(const NIdentifier & id);
private:
  NBlock * body;
};

#endif /* NSTRUCTDECLARATION_HPP_ */
