#ifndef STRAINENENEO_HPP
#define STRAINENENEO_HPP
#include "StrainEne.hpp"
#include <vecmath.h>

class StrainEneNeo:public StrainEne
{
public:
  StrainEneNeo();
  float getEnergy(const Matrix3f & F);
  Matrix3f getPK1(const Matrix3f & F);
  Matrix3f getdPdx(const Matrix3f & F,const Matrix3f & dF);
  
};
#endif