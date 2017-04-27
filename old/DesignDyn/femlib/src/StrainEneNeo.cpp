#include "StrainEneNeo.hpp"

StrainEneNeo::StrainEneNeo()
{
  param.resize(2);
  param[0] =1;
  param[1] =10;
}

float StrainEneNeo::getEnergy(const Matrix3f & F)
{
  float I1 = (F.transposed()*F).trace();
  float JJ = std::log(F.determinant());
  float mu = param[0],lambda=param[1];
  float Psi = (mu/2) * (I1-3) - mu*JJ + (lambda/2)*JJ*JJ;
  return (float)Psi;
}

Matrix3f StrainEneNeo::getPK1(const Matrix3f & F)
{
  float JJ = std::log(F.determinant());
  Matrix3f Finv = F.inverse();
  Finv.transpose();
  float mu = param[0],lambda=param[1];
  Matrix3f PP = mu*(F-Finv) + lambda*JJ*Finv;
  return PP;
}

Matrix3f StrainEneNeo::getdPdx(const Matrix3f & F,const Matrix3f & dF)
{
  Matrix3f dP = Matrix3f();
  float JJ = std::log(F.determinant());
  Matrix3f Finv = F.inverse();
  Matrix3f FinvT = Finv.transposed();
  float mu = param[0],lambda=param[1];
  dP = mu*dF;
  float c1 = mu-lambda * JJ;
  dP += c1 * FinvT*dF.transposed()*FinvT;
  dP += lambda*(Finv*dF).trace()*FinvT;
  return dP;
}
