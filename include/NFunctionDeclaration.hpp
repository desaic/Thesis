/*
 * NFunctionDeclaration.hpp
 *
 *  Created on: Jun 26, 2013
 *      Author: desaic
 */

#ifndef NFUNCTIONDECLARATION_HPP_
#define NFUNCTIONDECLARATION_HPP_

#include "NStatement.hpp"
#include <deque>

class AstType;
class NIdentifier;
class NVariableDeclaration;
typedef std::deque<NVariableDeclaration*> VariableList;

class NFunctionDeclaration : public NStatement {
public:
  const NIdentifier& id;
  VariableList arguments;
  NBlock& block;
  NFunctionDeclaration(const AstType & _type, NIdentifier& id,
      const VariableList& arguments, NBlock& block);
  virtual llvm::Value* codeGen(CodeGenContext& context);
  virtual ~NFunctionDeclaration();
};

#endif /* NFUNCTIONDECLARATION_HPP_ */
