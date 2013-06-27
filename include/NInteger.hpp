/*
 * NInteger.hpp
 *
 *  Created on: Jun 27, 2013
 *      Author: desaic
 */

#ifndef NINTEGER_HPP_
#define NINTEGER_HPP_

#include "NExpression.hpp"
class NInteger : public NExpression {
public:
    int value;
  NInteger(int value) :
    NExpression(AstType(AstType::AST_INT)),
    value(value) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

#endif /* NINTEGER_HPP_ */
