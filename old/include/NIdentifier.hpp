/*
 * NIdentifier.hpp
 *
 *  Created on: Jun 26, 2013
 *      Author: desaic
 */

#ifndef NIDENTIFIER_HPP_
#define NIDENTIFIER_HPP_

#include "NExpression.hpp"
class AstType;
#include <string>
class NIdentifier : public NExpression {
public:
    std::string name;
    NIdentifier(const std::string& name);
    virtual llvm::Value* codeGen(CodeGenContext& context);
    void updateType();
};

#endif /* NIDENTIFIER_HPP_ */
