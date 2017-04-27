#ifndef ELEMENTQUAD_HPP
#define ELEMENTQUAD_HPP
#include "Element.hpp"

#include <Eigen/Dense>

class ElementQuad:public Element
{
public:
  ElementQuad();
  ElementQuad(const ElementQuad & e);

  int nF() const override{ return 1; }

    std::vector<std::array<int,2> > getEdges();

  ///@brief natural coordinate for a point in reference space
  Vector3f natCoord(const Vector3f & p, const std::vector<Vector3f> & X);
  
  std::vector<float> shapeFun(const Vector3f & p)const;

  Vector3f shapeFunGrad(int ii, const Vector3f & xx,
    const std::vector<Vector3f> & X) const;

  float getVol(const std::vector<Vector3f> & X);

};
#endif
