/*
 * NExpression.h
 *
 *  Created on: Jun 21, 2013
 *      Author: desaic
 */

#ifndef NEXPRESSION_H_
#define NEXPRESSION_H_

#include "Type.hpp"
#include "AstNode.hpp"

class NExpression : public AstNode {
public:
  virtual const AstType * getType() const;
  AstType * type;
  NExpression();
  NExpression(const AstType & _type);
  NExpression(const NExpression&);
  virtual ~NExpression();
};

#endif /* NEXPRESSION_H_ */
