/*
 * NReturn.hpp
 *
 *  Created on: Jun 21, 2013
 *      Author: desaic
 */

#ifndef NRETURN_HPP_
#define NRETURN_HPP_

#include "Ast.hpp"

class NReturn: public NStatement
{
public:
  NReturn();
  NReturn(NExpression * expr);
  virtual ~NReturn();
  virtual llvm::Value* codeGen(CodeGenContext& context);
private:
  NExpression * expr;
};

#endif /* NRETURN_HPP_ */
