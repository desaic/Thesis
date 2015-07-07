#ifndef QUADPT_HPP
#define QUADPT_HPP

#include <vector>
#include <vecmath.h>
///@brief quantities stored in a quadrature point
struct QuadPt{
  ///@brief deformation gradient at each level
  std::vector<Matrix3f> F;
  ///@brief quadrature weight at each level
  std::vector<float> w;
  ///@brief natural coordinate at each level
  std::vector<Vector3f> X;
  ///@brief element indices
  std::vector<int> ei;
};

#endif