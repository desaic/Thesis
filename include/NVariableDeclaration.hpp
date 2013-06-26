/*
 * NVariableDeclaration.hpp
 *
 *  Created on: Jun 26, 2013
 *      Author: desaic
 */

#ifndef NVARIABLEDECLARATION_HPP_
#define NVARIABLEDECLARATION_HPP_

#include "Ast.hpp"

namespace llvm
{
class Value;
}
class NVariableDeclaration : public NStatement {
public:
  AstType * type;
  NIdentifier& id;
  NExpression *assignmentExpr;

  NVariableDeclaration(const AstType & _type, NIdentifier& id);
  NVariableDeclaration(const AstType &_type, NIdentifier& id,
      NExpression *assignmentExpr);
  virtual llvm::Value* codeGen(CodeGenContext& context);
  virtual ~NVariableDeclaration();
};
#endif /* NVARIABLEDECLARATION_HPP_ */
