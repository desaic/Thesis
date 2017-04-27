/*
 * NFor.hpp
 *
 *  Created on: Jul 2, 2013
 *      Author: desaic
 */

#ifndef NFOR_HPP_
#define NFOR_HPP_

class NStatement;
class NExpression;
class NBlock;
class CodeGenContext;
namespace llvm
{
  class Value;
}

class NFor: public NStatement
{
public:
  NFor();
  NFor(NExpression * init, NExpression * cond,
      NExpression * end,NBlock * b);
  virtual ~NFor();
  virtual llvm::Value* codeGen(CodeGenContext& context);
private:
  NExpression * expr[3];
  NBlock * block;
};

#endif /* NFOR_HPP_ */
