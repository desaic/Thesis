/*
 * NInteger.hpp
 *
 *  Created on: Jun 27, 2013
 *      Author: desaic
 */

#ifndef NINTEGER_HPP_
#define NINTEGER_HPP_

#include "NExpression.hpp"
namespace llvm{
class Value;
}
class CodeGenContext;
class NExpression;
class NInteger : public NExpression {
public:
    int value;
  NInteger(int value);
    virtual llvm::Value* codeGen(CodeGenContext& context);
    virtual ~NInteger();
};

#endif /* NINTEGER_HPP_ */
