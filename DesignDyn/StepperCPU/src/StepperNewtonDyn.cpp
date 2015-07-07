#include "StepperNewtonDyn.hpp"
#include "ElementMesh.hpp"
#include "ArrayUtil.hpp"
#include "femError.hpp"

#include "Timer.hpp"
#include "Eigen/Sparse"
#include "pcg_solver.h"
#include <fstream>
#include <assert.h>

StepperNewtonDyn::StepperNewtonDyn():dx_tol(1e-5f),h(0.005f)
,frameCnt(0)
{
  m_Init = false;
}

void StepperNewtonDyn::init(ElementMesh * _m)
{
  m = _m;
  m->computeMass();
  h=m->dt;
}

int StepperNewtonDyn::oneStep()
{
  std::vector<Vector3f> force = m->getForce();

  float E = m->getEnergy();
  int dim = m->dim;
  int ndof = dim*(int)m->x.size();
  std::vector<float> bb(ndof);

  for(unsigned int ii =0 ; ii<force.size(); ii++){
//    add gravity
    force[ii] += m->mass[ii] * m->G;
    force[ii] = h*h*force[ii] + h*m->mass[ii]*m->v[ii];
  }

  compute_dx_sparse(m, force, std::vector<bool>(0), bb);
  std::vector<bool> collide(m->x.size(),false);
  bool hasCollision = false;
  //hard-coded collision detection.
  for(unsigned int ii = 0; ii<m->x.size(); ii++){
    if( bb[dim*ii+1] + m->x[ii][1] < 0 ){
      collide[ii] = true;
      hasCollision = true;
    }
  }
  if(hasCollision){
    compute_dx_sparse(m, force, collide, bb);
  }

  for(unsigned int ii =0; ii<force.size(); ii++){
    for(int jj =0 ; jj<dim; jj++){
      m->x[ii][jj] += bb[dim*ii+jj];
      m->v[ii][jj] = (1.0/h) * bb[dim*ii+jj];
    }
  }

  if(frameCnt*h>=0.05){
    for(unsigned int ii =0; ii<force.size(); ii++){
      m->fe[ii] = Vector3f(0,0,0);
      m->fixed[ii] = false;
    }
  }

  frameCnt++;
  return 0;
}

float StepperNewtonDyn::compute_dx_sparse(ElementMesh * iMesh,
                                          const std::vector<Vector3f> &iForces,
                                          const std::vector<bool> & collide,
                                          std::vector<float> &bb)
{

  bool triangular = false;
  if (!m_Init){
    iMesh->stiffnessPattern(m_I, m_J, triangular);
    int ncol = m_I.size()-1;
    mat.n = ncol;
    mat.index.resize(ncol);
    mat.value.resize(ncol);
    for (int col=0; col<ncol;col++){
      int nEntry = m_I[col+1] - m_I[col];
      mat.index[col].resize(nEntry);
      mat.value[col].resize(nEntry);
      for (int i=m_I[col]; i<m_I[col+1]; i++){
        int row = m_J[i];
        mat.index[col][ i - m_I[col] ] = row;
      }
    }
    m_Init = true;
  }

  int dim = iMesh->dim;
  int ndof = dim * iMesh->x.size();

  PCGSolver <double> solver;

  assert(iMesh && ndof==bb.size());

  std::vector<float> Kvalues;
  Timer timer;
  timer.start();
  iMesh->getStiffnessSparse(Kvalues, triangular, true);
  timer.end();
  std::cout<<"Assembly time "<< timer.getSeconds()<<"\n";

  timer.start();

  for(unsigned int ii = 0;ii<m->x.size(); ii++){
    for(int jj = 0;jj<dim;jj++){
      int row = dim*ii + jj;
      if(m->fixed[ii]){
        bb[ row ] = 0;
      }else{
        bb[ row ] = iForces[ii][jj];
      }
    }
  }

  int nrow = ndof;
  int ncol = ndof;

  std::vector<double> rhs(nrow);
  std::vector<double> x(nrow,0);

  for (int col=0; col<ncol;col++){
    rhs[col] = bb[col];

    for (int i=m_I[col]; i<m_I[col+1]; i++){
      int row = m_J[i];
      if(triangular && col>row){
        continue;
      }
      Kvalues[i] *= h*h;
      int vrow = row/dim;
      if(row == col){
        Kvalues[i] += m->mass[vrow];
      }
      mat.value[col][ i-m_I[col] ] = Kvalues[i];
    }
  }

  timer.end();
  std::cout<<"Copy data time "<< timer.getSeconds()<<"\n";
  timer.start();

  //collision correction
  if(collide.size() == iMesh->x.size()){
    for (int col=0; col<ncol;col++){
      int vcol = col/dim;
      if(collide[vcol]){
        rhs[col] = 0;
      }
      for (int i=m_I[col]; i<m_I[col+1]; i++){
       int row = m_J[i];
       if(triangular && col>row){
          continue;
       }
       int vrow = row/dim;
       if(collide[vrow] || collide [vcol]){
         if(row == col){
           std::cout<<mat.value[col][ i-m_I[col] ]<<"\n";
         }else{
           mat.value[col][ i-m_I[col] ]=0;
         }
       }
      }
    }
  }

  timer.end();
  std::cout<<"Copy collision time "<< timer.getSeconds()<<"\n";

  timer.start();
  ///@TODO add call to sparse solver
  double residual=0;
  int iters = 0;
  bool ret = solver.solve(mat, rhs, x,residual, iters);
  std::cout<< "pcg ret " <<ret<<" iters " << iters << " residual "<<residual<<"\n";
  timer.end();

  std::cout<<"Solver time "<< timer.getSeconds()<<"\n";

  for(int ii = 0;ii<x.size();ii++){
    bb[ii] = x[ii];
  }

  return 0;
}
