/*
 * NFloat.hpp
 *
 *  Created on: Jun 27, 2013
 *      Author: desaic
 */

#ifndef NFLOAT_HPP_
#define NFLOAT_HPP_

class NFloat: public NExpression {
public:
  float value;
    NFloat(float value) :
      NExpression(AstType(AstType::AST_FLOAT)),
      value(value) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

#endif /* NFLOAT_HPP_ */
