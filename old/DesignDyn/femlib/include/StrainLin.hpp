#ifndef STRAIN_LIN_HPP
#define STRAIN_LIN_HPP
#include "StrainEne.hpp"
#include <vecmath.h>
#include <Eigen/Dense>
class StrainLin :public StrainEne{
public:
	StrainLin();
  virtual float getEnergy(const Matrix3f & F);
  virtual Matrix3f getPK1(const Matrix3f & F);
  virtual Matrix3f getdPdx(const Matrix3f & F, const Matrix3f & dF);

	virtual Eigen::MatrixXf EMatrix();
};

#endif
