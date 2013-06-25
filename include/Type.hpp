/*
 * Type.hpp
 *
 *  Created on: Jun 18, 2013
 *      Author: desaic
 */

#ifndef TYPE_HPP_
#define TYPE_HPP_
#include <string>
namespace llvm{
class Type;
};
///@brief a class representing primitive types,
///arrays, and structs
class AstType
{
public:
  enum AstTypeEnum{
    AST_VOID = 0,
    AST_DOUBLE,
    AST_FLOAT,
    AST_INT,
    AST_INT64,
    AST_UNIMP
  };
  AstType();
  AstType(int _id);
  AstType(const AstType & ty);
  AstType & operator=(const AstType& ty);
  int getId()const;
  void set(int id);
  int typeId;
  virtual ~AstType();
  llvm::Type * getLLVMType() const ;
};

///@brief not implemented
class StructType: public AstType
{
public:
  std::string name;
  StructType():AstType(AST_UNIMP){}
  StructType(std::string * _name):
    AstType(AST_UNIMP),name(*_name){}
};

#endif /* TYPE_HPP_ */
