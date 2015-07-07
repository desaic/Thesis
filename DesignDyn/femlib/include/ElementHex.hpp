#ifndef ELEMENTHEX_HPP
#define ELEMENTHEX_HPP
#include "Element.hpp"

#include <Eigen/Dense>
extern const int facen[][3];
extern const int faces[][4];
class ElementHex:public Element
{
public:
  ElementHex();
  ElementHex(const ElementHex & e);
  int nV() const override{ return 8; }

  int nF() const override{ return 6; }


  ///@brief natural coordinate for a point in reference space
  Vector3f natCoord(const Vector3f & p, const std::vector<Vector3f> & X);
  
  std::vector<float> shapeFun(const Vector3f & p)const;

  Vector3f shapeFunGrad(int ii, const Vector3f & xx,
    const std::vector<Vector3f> & X) const;

  std::vector<std::array<int,2> > getEdges();
  float getVol(const std::vector<Vector3f> & X);

  Vector3f facePt(int fi, const Vector3f & x);
  ///@param fi face index.
  Eigen::MatrixXf NMatrix(int fi);
  ///@brief shape function matrix.
  Eigen::MatrixXf HMatrix(const Vector3f & xx);
  //specific for linear strain (F+F^T).
  Eigen::MatrixXf BMatrix(const Vector3f & xx, const std::vector<Vector3f>X);
};
#endif
