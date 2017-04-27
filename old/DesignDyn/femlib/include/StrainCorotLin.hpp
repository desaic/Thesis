#ifndef STRAINCOROTLIN_HPP
#define STRAINCOROTLIN_HPP
#include "StrainLin.hpp"

#include <Eigen/Dense>

class StrainCorotLin :public StrainLin{
public:
  StrainCorotLin();
  virtual float getEnergy(const Matrix3f & F);
  virtual Matrix3f getPK1(const Matrix3f & F);
  virtual Matrix3f getdPdx(const Matrix3f & F, const Matrix3f & dF);
  Eigen::MatrixXf EMatrix();
};

#endif
