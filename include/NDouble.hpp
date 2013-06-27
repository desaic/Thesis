/*
 * NDouble.hpp
 *
 *  Created on: Jun 27, 2013
 *      Author: desaic
 */

#ifndef NDOUBLE_HPP_
#define NDOUBLE_HPP_

class NDouble : public NExpression {
public:
    double value;
    NDouble(double value) :
      NExpression(AstType(AstType::AST_DOUBLE)),
      value(value) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

#endif /* NDOUBLE_HPP_ */
