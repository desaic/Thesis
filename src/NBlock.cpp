/*
 * NBlock.cpp
 *
 *  Created on: Jun 26, 2013
 *      Author: desaic
 */

#include "NBlock.hpp"
#include <llvm/Value.h>
#include <iostream>
#include <typeinfo> ///@todo need to replace with node types
llvm::Value* NBlock::codeGen(CodeGenContext& context)
{
  StatementList::const_iterator it;
  llvm::Value *last = NULL;
  for (it = statements.begin(); it != statements.end(); it++) {
    std::cout << "Generating code for " << typeid(**it).name() << std::endl;
    last = (**it).codeGen(context);
  }
  std::cout << "Creating block" << std::endl;
  return last;
}
