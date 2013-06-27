/*
 * NBool.hpp
 *
 *  Created on: Jun 27, 2013
 *      Author: desaic
 */

#ifndef NBOOL_HPP_
#define NBOOL_HPP_

#include "NExpression.hpp"
namespace llvm{
  class Value;
}
///@brief bool variable
class NBool: public NExpression
{
public:
  NBool();
  NBool(bool _val);
  virtual ~NBool();
  virtual llvm::Value* codeGen(CodeGenContext& context);
  bool val;
};

#endif /* NBOOL_HPP_ */
