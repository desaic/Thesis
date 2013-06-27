/*
 * NMethodCall.hpp
 *
 *  Created on: Jun 26, 2013
 *      Author: desaic
 */

#ifndef NFUNCTIONCALL_HPP_
#define NFUNCTIONCALL_HPP_
#include <deque>
#include "NExpression.hpp"
class NIdentifier;

namespace llvm{
class Value;
}
typedef std::deque<NExpression*> ExpressionList;

class NFunctionCall : public NExpression {
public:
    const NIdentifier& id;
    ExpressionList arguments;
    NFunctionCall(const NIdentifier& id, ExpressionList& arguments);
    NFunctionCall(const NIdentifier& id) ;
    virtual llvm::Value* codeGen(CodeGenContext& context);
private:
    void updateType();
};

#endif /* NMETHODCALL_HPP_ */
