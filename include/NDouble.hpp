/*
 * NDouble.hpp
 *
 *  Created on: Jun 27, 2013
 *      Author: desaic
 */

#ifndef NDOUBLE_HPP_
#define NDOUBLE_HPP_

class NExpression;
class CodeGenContext;

namespace llvm{
  class Value;
}

class NDouble : public NExpression {
public:
    double value;
    NDouble(double value);
    virtual llvm::Value* codeGen(CodeGenContext& context);
    virtual ~NDouble();
};

#endif /* NDOUBLE_HPP_ */
