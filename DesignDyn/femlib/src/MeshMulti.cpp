#include "MeshMulti.hpp"
#include "ElementHex.hpp"

Vector3f MeshMulti::getDisp(const HIndex & ind, const Vector3f & p)
{
  Vector3f u(0,0,0);
  Matrix3f Fprod = Matrix3f::identity();
  for(int level = ind.level; level>=0; level--){
    Element * ele = (*this)[ind];
    u += Fprod * ele->getDisp(p, Xh[level], xh[level]);
    Matrix3f F = ele->defGrad(p,Xh[level], xh[level]);
    Fprod = F * Fprod;
  }
  return u;
}

Matrix3f MeshMulti::defGrad(const HIndex & ind, const Vector3f & p)
{
  Matrix3f Fprod = Matrix3f::identity();
  for(int level = ind.level; level>=0; level--){
    Element * ele = (*this)[ind];
    Matrix3f F = ele->defGrad(p,Xh[level], xh[level]);
    Fprod = F * Fprod;
  }
  return Fprod;
}


