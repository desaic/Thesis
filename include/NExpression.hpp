/*
 * NExpression.h
 *
 *  Created on: Jun 21, 2013
 *      Author: desaic
 */

#ifndef NEXPRESSION_H_
#define NEXPRESSION_H_

#include "AstNode.hpp"
class NExpression : public AstNode {
public:
  NExpression();
  NExpression(const AstType & _type);
  NExpression(const NExpression&);
  virtual ~NExpression();
};

#endif /* NEXPRESSION_H_ */
