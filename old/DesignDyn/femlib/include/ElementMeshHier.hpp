#ifndef ELEMENTMESHHIER_HPP
#define ELEMENTMESHHIER_HPP
#include <vector>
#include "vecmath.h"
#include "BoundVec3.hpp"
#include "MatrixX.hpp"
class Element;
class Material;
class ElementMesh;
struct QuadPt;

///@brief Assumes materials are MaterialQuad. 
///For each element, all quadrature points uses the first material stored in MaterialQuad.
class ElementMeshHier{
public:
  ///@brief Each mesh correspond to a level. 0 is the fine level.
  ///Materials, constrains and forces from the fine level are used.
  std::vector<ElementMesh*>m;
  ///@brief a list of quadrature points;
  std::vector<QuadPt> quadpt;
  ElementMeshHier();
  ///@brief collect quadrature points for one element
  std::vector<int> getQuadIdx(int level, int eIdx);
  ///@brief construct nLevel additional levels given m[0].
  void buildHier(int nLevel);

  ///@param qidx index of quadrature point
  void updateDefGrad(int level, int qidx);
  void updateDefGrad(int level);
  float getEnergy(int level = 0);
  float getEnergy(int level, int eIdx);

  ///@brief computes internal forces only
  std::vector<Vector3f> getForce(int level, int eIdx);
  std::vector<Vector3f> getForce(int level);

  void getStiffnessPattern(int level, std::vector<int> & I, std::vector<int> & J, bool trig);
  void getStiffness(int level, std::vector<float> &val, bool trig, bool constrained);
  ///@brief stiffness block between dofs at the same level.
  MatrixXf getStiffness(int level, int eIdx);
  ///@brief stiffness contributed by a quadrature point.
  ///@param qIdx assumed to be in the range of number of quadrature points for that element.
  MatrixXf getStiffness(int level, int eIdx, int qIdx);
  void defGradProd(int level, int qi, Matrix3f & Fl, Matrix3f & Fr);

  virtual ~ElementMeshHier();
};

#endif
