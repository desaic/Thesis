#ifndef ELEMENT_HIER_HPP
#define ELEMENT_HIER_HPP

#include "ElementHex.hpp"

#include <vector>

class ElementRegGrid;
class ElementMesh;
class ElementHier:public ElementHex
{
public:
  
  ElementHier();
  ElementHier(const ElementHex& e);
  ///@brief index of parent coarser element if any.
  int parent;
  ///@brief indices of children. For fine elements, they are indices of quadrature points.
  ///Built by ElementMeshHier.
  std::vector<int> children;
  ///@brief natural coordinates of vertices in parent element.
  std::vector<Vector3f> Xn;
  ///@param p natural coordinates.
  Vector3f getDisp(const Matrix3f & Fc, const Vector3f & p);

};

///@brief replace ElementHex with ElementHier
void replaceElementHex(ElementRegGrid * grid);
///@bug grid assumed to have even dimensions for the moment.
///@param grid assumes to contain elements of type ElementCoarse
ElementRegGrid* coarsen(const ElementRegGrid * grid);

#endif
