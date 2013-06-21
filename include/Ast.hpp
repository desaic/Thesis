#ifndef AST_HPP
#define AST_HPP
#include <deque>
#include <iostream>
#include <vector>
#include <llvm/Value.h>
#include "AstNode.hpp"
#include "NExpression.hpp"
#include "Type.hpp"
class CodeGenContext;
class NStatement;
class NVariableDeclaration;

typedef std::deque<NStatement*> StatementList;
typedef std::deque<NExpression*> ExpressionList;
typedef std::deque<NVariableDeclaration*> VariableList;

class NStatement : public AstNode {
};

class NInteger : public NExpression {
public:
    int value;
    NInteger(int value) : value(value) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NDouble : public NExpression {
public:
    double value;
    NDouble(double value) : value(value) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NFloat: public NExpression {
public:
  float value;
    NFloat(float value) : value(value) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
};
/*
class NString: public NExpression{
public:
  std::string * value;

};
*/
class NIdentifier : public NExpression {
public:
    std::string name;
    NIdentifier(const std::string& name) : name(name) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NMethodCall : public NExpression {
public:
    const NIdentifier& id;
    ExpressionList arguments;
    NMethodCall(const NIdentifier& id, ExpressionList& arguments) :
        id(id), arguments(arguments) { }
    NMethodCall(const NIdentifier& id) : id(id) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NBinaryOperator : public NExpression {
public:
    NExpression& lhs;
    NExpression& rhs;
    int op;
    NBinaryOperator(NExpression& _lhs, int op, NExpression& _rhs) :
        lhs(_lhs), rhs(_rhs), op(op) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NAssignment : public NExpression {
public:
    NIdentifier& lhs;
    NExpression& rhs;
    NAssignment(NIdentifier& lhs, NExpression& rhs) : 
        lhs(lhs), rhs(rhs) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NBlock : public NExpression {
public:
    StatementList statements;
    NBlock() { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NExpressionStatement : public NStatement {
public:
    NExpression& expression;
    NExpressionStatement(NExpression& expression) : 
        expression(expression) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NVariableDeclaration : public NStatement {
public:
  AstType * type;
  NIdentifier& id;
  NExpression *assignmentExpr;
  NVariableDeclaration(AstType * _type, NIdentifier& id) :
      type(_type), id(id), assignmentExpr(0)
  {
  }
  NVariableDeclaration(AstType * _type, NIdentifier& id,
      NExpression *assignmentExpr) :
      type(_type), id(id), assignmentExpr(assignmentExpr)
  {
  }
  virtual llvm::Value* codeGen(CodeGenContext& context);
  virtual ~NVariableDeclaration()
  {
    if (type != 0) {
      delete type;
    }
  }
};

class NFunctionDeclaration : public NStatement {
public:
  AstType * type;
    const NIdentifier& id;
    VariableList arguments;
    NBlock& block;
    NFunctionDeclaration(AstType * _type, const NIdentifier& id,
        const VariableList& arguments, NBlock& block) :
          type(_type),id(id), arguments(arguments), block(block) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
  virtual ~NFunctionDeclaration()
  {
    if (type != 0) {
      delete type;
    }
  }

};

#endif
