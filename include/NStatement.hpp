/*
 * NStatement.hpp
 *
 *  Created on: Jun 26, 2013
 *      Author: desaic
 */

#ifndef NSTATEMENT_HPP_
#define NSTATEMENT_HPP_

#include "AstNode.hpp"

class NStatement : public AstNode {
public:
  NStatement();
  NStatement(const AstType & _type);
};

#endif /* NSTATEMENT_HPP_ */
