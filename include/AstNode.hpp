/*
 * AstNode.hpp
 *
 *  Created on: Jun 21, 2013
 *      Author: desaic
 */

#ifndef ASTNODE_HPP_
#define ASTNODE_HPP_
#include <llvm/Value.h>
#include "Type.hpp"
#include "codegen.h"
#include "TextRange.hpp"
#include "SymbolTable.hpp"
class AstType;
///@brief Base class for all intermediate representation.
///@param context: all code share the same context.
class AstNode {
public:
  AstNode();
  AstNode(int astTypeId);
  AstNode(const AstType & type);
  AstType type;
  virtual const AstType & getType() const;
  virtual ~AstNode();
  virtual llvm::Value* codeGen(CodeGenContext& context);
  ///@brief used for error reporting.
  TextRange range;
  static SymbolTable symbol;
};

#endif /* ASTNODE_HPP_ */
