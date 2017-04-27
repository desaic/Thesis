#ifndef ELEMENTMESH_HPP
#define ELEMENTMESH_HPP
#include <vector>
#include "vecmath.h"
#include "BoundVec3.hpp"
#include "MatrixX.hpp"
#include <Eigen/Sparse>

class Element;
class Material;

class ElementMesh{
public:
  ///@brief elements will be deleted by destructor. Do not put same pointer in different meshes.
  ///Make copies if needed.
  std::vector<Element*>e;

  ///@brief deformed positions in t-1. used by dynamics(unit m)
//  std::vector<Vector3f>x0;

  ///@brief deformed positions (unit m)
  std::vector<Vector3f>x;
  
  ///@brief vertex positions at rest pose.
  std::vector<Vector3f>X;

  ///@brief velocity
  std::vector<Vector3f>v;

  ///@brief dimension of the mesh. either 2 or 3
  ///in 2D, the third coordinate is ignored/wasted.
  int dim;

  ///@brief time step (unit s)
  float dt;
  ///@brief gravitational constant
  Vector3f G;
  ///@TODO: change to per element density.
  ///@brief kg/m3
  float density;
  ///@brief lumped mass
  std::vector<float>mass;

  std::vector<Material*>m;
  
  ///@brief material for each element
  std::vector<int>me;
  
  ///@brief external forces applied to each dof.
  std::vector<Vector3f>fe;
  
  std::vector<int> fixed;

  std::vector<std::vector<Vector3f> > * u;

  ///@brief default constructor
  ElementMesh();
  
  ///@brief load a plain text hex fem mesh
  void load(std::istream & in, float scale=1.0f);

  ///@brief utility. call after initializing or changing X and e 
  ///X is copied to x;
  void initArrays();

  ///@brief add a material to the list of materials in this mesh
  void addMaterial(Material*_m);

  ///@brief for debug, check the size of members.
  int check();

  float getEnergy();
  float getEnergy(int eIdx);

  ///@brief computes internal forces only
  std::vector<Vector3f> getForce(int eIdx);
  std::vector<Vector3f> getForce();

  MatrixXf getStiffness(int eIdx);

  ///@param trig if true, return only the upper triangle of the symmetric matrix.
  void getStiffnessSparse(std::vector<float> &val, bool trig = false, bool constrained=false, bool iFixedRigid=false);

  Eigen::SparseMatrix<float> getStiffnessSparse(bool trig = false, bool constrained=false, bool iFixedRigid=false);

  ///@brief precompute lumped maasses.
  void computeMass();

  ///@param I row offsets. I.size() = matrix size + 1. I[size()-1]=number of non-zeros.
  ///@param J column indices.
  void stiffnessPattern(std::vector<int> & I, std::vector<int> & J, bool trig = false, bool iFixedRigid=false);

  MatrixXf getStiffness();

  virtual ~ElementMesh();
};


void getEleX(int ii, const ElementMesh * m, std::vector<Vector3f> &x);

void setEleX(int ii, ElementMesh * m, const std::vector<Vector3f> &x);

#endif
