#ifndef AST_HPP
#define AST_HPP
#include <deque>
#include <iostream>
#include <vector>
#include <llvm/Value.h>
#include "AstNode.hpp"
#include "NExpression.hpp"
#include "NIdentifier.hpp"
#include "NFunctionCall.hpp"
#include "NBlock.hpp"
#include "NFunctionDeclaration.hpp"
class CodeGenContext;

class NInteger : public NExpression {
public:
    int value;
  NInteger(int value) :
    NExpression(AstType(AstType::AST_INT)),
    value(value) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NDouble : public NExpression {
public:
    double value;
    NDouble(double value) :
      NExpression(AstType(AstType::AST_DOUBLE)),
      value(value) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NFloat: public NExpression {
public:
  float value;
    NFloat(float value) :
      NExpression(AstType(AstType::AST_FLOAT)),
      value(value) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
};
/*
class NString: public NExpression{
public:
  std::string * value;

};
*/

class NAssignment : public NExpression {
public:
    NIdentifier& lhs;
    NExpression& rhs;
    NAssignment(NIdentifier& lhs, NExpression& rhs) : 
        lhs(lhs), rhs(rhs) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NExpressionStatement : public NStatement {
public:
    NExpression& expression;
    NExpressionStatement(NExpression& expression) : 
        expression(expression) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

#endif
