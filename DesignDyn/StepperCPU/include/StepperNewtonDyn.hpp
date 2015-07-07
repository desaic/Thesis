#ifndef STEPPERNEWTONDYN_HPP
#define STEPPERNEWTONDYN_HPP
#include "Stepper.hpp"
#include <vector>
#include "Vector3f.h"
#include "sparse_matrix.h"
class StepperNewtonDyn:public Stepper
{
public:
  StepperNewtonDyn();
  virtual void init(ElementMesh * _m);
  int oneStep();

  float dx_tol;
  float h;

  int frameCnt;

private:
  ///@param collide. Pass in empty array if no collision
  float compute_dx_sparse(ElementMesh * iMesh, const std::vector<Vector3f> &iForces,
                          const std::vector<bool> &collide,
                          std::vector<float> &bb);
  SparseMatrixd mat;
  std::vector<int> m_I, m_J;
  bool m_Init;
};
#endif
