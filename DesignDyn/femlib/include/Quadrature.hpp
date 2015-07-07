#ifndef QUADRATURE_HPP
#define QUADRATURE_HPP
#include <vector>
#include "vecmath.h"
class Quadrature
{
public:
  std::vector<Vector3f> x;
  std::vector<float> w;
  Quadrature();
  
  static const Quadrature Gauss2_2D;
  static const Quadrature Gauss4_2D;

  static const Quadrature Gauss2;
  static const Quadrature Uniform4;
};

#endif