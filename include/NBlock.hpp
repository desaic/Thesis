/*
 * NBlock.hpp
 *
 *  Created on: Jun 26, 2013
 *      Author: desaic
 */

#ifndef NBLOCK_HPP_
#define NBLOCK_HPP_
#include "NExpression.hpp"
#include "NStatement.hpp"
#include <deque>
namespace llvm{
class Value;
}
typedef std::deque<NStatement*> StatementList;
class NBlock : public NExpression {
public:
    StatementList statements;
    NBlock() { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

#endif /* NBLOCK_HPP_ */
