#include "StepperNewton.hpp"
#include "ElementMesh.hpp"
#include "MatrixX.hpp"
#include "ArrayUtil.hpp"
#include "femError.hpp"

#include "Eigen/Sparse"

#include <fstream>
#include <assert.h>

StepperNewton::StepperNewton():dense(true),dx_tol(1e-5f),h(1.0f)
  ,rmRigid(false)
{
  m_Init = false;
  dense = false;
}

int StepperNewton::oneStep()
{
  std::vector<Vector3f> force = m->getForce();
  float E = m->getEnergy();
 
  int ndof = 3*(int)m->x.size();
  std::vector<float> bb(ndof);

  if (dense)
  {
    compute_dx_dense(m, force, rmRigid, bb);
  }
  else
  {
    compute_dx_sparse(m, force, rmRigid, bb);
  }

  double totalMag = 0;
  for(int ii = 0;ii<ndof;ii++){
    totalMag += std::abs(bb[ii]);
  }

  for(unsigned int ii = 0;ii<m->x.size(); ii++){
    for(int jj = 0;jj<3;jj++){
      force[ii][jj] = (float)bb[3*ii+jj];
    }
  }
  //line search
  std::vector<Vector3f> x0 = m->x;
  float E1=E;
  while(1){
    if(totalMag * h<dx_tol){
      break;
    }
    m->x=x0;
    addmul(m->x, h, force);
    E1 = m->getEnergy();
    if(E1>E || fem_error){
      fem_error = 0;
      h = 0.5f* h;
      if(h<1e-12){
        break;
      }
    }else{
     // h=1.1f*h;
      break;
    }
  }
  if (std::abs(E - E1) < 1e-3f){
    return -1;
  }
  std::cout << E << "\n";
  return 0;
}

///@brief add rows to K and b to constrain 6 degrees of freedom.
///@param K size is #DOF + 6
void fixRigid(MatrixXf & K, float * b,
        ElementMesh * mesh)
{
  int row = K.mm-6;
  for(int ii =0; ii<6; ii++){
    b[row + ii] = 0;
  }

  Vector3f center(0,0,0);
  for(int ii = 0;ii<mesh->x.size();ii++){
    center += mesh->x[ii];
  }
  center /= (float)mesh->x.size();
  float cscale = 1000;
  for(int ii = 0;ii<mesh->x.size();ii++){
    int col = 3*ii;
    //relative position to center
    Vector3f rel = mesh->x[ii] - center;
    //cross product matrix
    Matrix3f c (0, -rel[2],  rel[1],
             rel[2],       0, -rel[0],
            -rel[1],  rel[0],       0);

    c = cscale * c;
    for(int jj=0; jj<3; jj++){
      for(int kk = 0; kk<3; kk++){
        K(row+jj, col+kk) = c(jj,kk);
        K(col+kk, row+jj) = c(jj,kk);
      }
    }

    for(int jj=0; jj<3; jj++){
      K(row+3+jj, col+jj) = 1;
      K(col+jj, row+3+jj) = 1;
    }
  }
}

float StepperNewton::compute_dx_dense(ElementMesh * iMesh, const std::vector<Vector3f> &iForces, bool iRmRigid, std::vector<float> &bb)
{
  int ndof = bb.size();
  assert(iMesh && 3*iMesh->x.size()==ndof);

  MatrixXf K = iMesh->getStiffness();
   
  for(unsigned int ii = 0;ii<m->x.size(); ii++){
    for(int jj = 0;jj<3;jj++){
      int row = 3*ii + jj;
      //damping, better condition number
  //    K(row,row) += 100;
      if(m->fixed[ii]){
        bb[ row ] = 0;
        for(int kk = 0;kk<ndof;kk++){
          K(row,kk) = 0;
          K(row,row) = 100;
        }
      }else{
        bb[ row ] = iForces[ii][jj];
      }
    }
  }
  if(iRmRigid){
    K.resize(ndof + 6, ndof+6);
    bb.resize(ndof+6);
    fixRigid(K,&(bb[0]),m);
  }
  ///@TODO add call to dense linear solver

  return 0;
}

float StepperNewton::compute_dx_sparse(ElementMesh * iMesh, const std::vector<Vector3f> &iForces, bool iRmRigid, std::vector<float> &bb)
{
  bool triangular = true;

  if (!m_Init)
  {
    m_I.clear();
    m_J.clear();
    iMesh->stiffnessPattern(m_I, m_J, triangular, iRmRigid);

    m_Init = true;
  }

  int ndof = bb.size();
  assert(iMesh && 3*iMesh->x.size()==bb.size());

  std::vector<float> Kvalues;
  iMesh->getStiffnessSparse(Kvalues, triangular, true, iRmRigid);
   
  for(unsigned int ii = 0;ii<m->x.size(); ii++){
    for(int jj = 0;jj<3;jj++){
      int row = 3*ii + jj;
      if(m->fixed[ii]){
        bb[ row ] = 0;
      }else{
        bb[ row ] = iForces[ii][jj];
      }
    }
  }

  int nrow = ndof;
  int ncol = ndof;
  std::vector<int> ia(nrow+1),ja;
  std::vector<double> val ;
  std::vector<double> rhs(nrow);
  std::vector<double> x(nrow,0);
  int indVal = 0;
  //starting index of a row
  //only upper triangle is needed
  for (int col=0; col<ncol;col++){
    rhs[col] = bb[col];
    ia[col] = indVal;

    //for (Eigen::SparseMatrix<float>::InnerIterator it(K, col); it; ++it){
    //    int row = it.row();
    for (int i=m_I[col]; i<m_I[col+1]; i++)
    {
      int row = m_J[indVal];
      if(triangular && col>row)
      {
        continue;
      }
      ja.push_back(row);
      val.push_back(Kvalues[indVal]);
      indVal++;
    }
  }
  ia[nrow] = indVal;

  ///@TODO add call to sparse solver

//  std::cout<< "sparse solve " << status << "\n";

  for(int ii = 0;ii<x.size();ii++){
    bb[ii] = x[ii];
  }

  return 0;
}
