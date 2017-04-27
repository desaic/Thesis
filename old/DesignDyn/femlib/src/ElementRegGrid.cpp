/*
 * ElementRegGrid.cpp
 *
 *  Created on: Aug 29, 2013
 *      Author: desaic
 */
#include "Element.hpp"
#include "ElementRegGrid.hpp"
#include "ElementHex.hpp"
#include "util.h"
#include <set>

//node index
#define IX(ii,jj,kk) ( (ii) * NY * NZ + (jj) * NZ + kk)
bool naive = false;
///vertex positions for single element
int oneEleV[8][3] =
{ {0,0,0},
  {0,0,1},
  {0,1,0},
  {0,1,1},
  {1,0,0},
  {1,0,1},
  {1,1,0},
  {1,1,1}
};

int ElementRegGrid::GetVertInd(int ii , int jj, int kk)
{
  int vidx =  ii * (ny+1) * (nz+1)  + jj * (nz+1) + kk;
  if(vertIdx.size()==0){
    return vidx;
  }
  return vertIdx[vidx];
}

int
ElementRegGrid::GetEleInd(int ii , int jj, int kk)const
{
  if(eleIdx.size()==0){
    return toArrInd(ii,jj,kk);
  }
  return eleIdx[toArrInd(ii,jj,kk)];
}

int
ElementRegGrid::GetEleInd(const Vector3f & p)
{
  Vector3f lp = p - origin;
  int ii = (int)( lp[0]/dx);
  int jj = (int)( lp[1]/dx);
  int kk = (int)( lp[2]/dx);
//  ii = clamp(ii, 0, nx - 1);
//  jj = clamp(jj, 0, ny - 1);
//  kk = clamp(kk, 0, nz - 1);
  return GetEleInd(ii,jj,kk);
}

int
ElementRegGrid::GetEleInd_clamp(const Vector3f & p)
{
  Vector3f lp = p - origin;
  int ii = (int)( lp[0]/dx);
  int jj = (int)( lp[1]/dx);
  int kk = (int)( lp[2]/dx);
  ii = clamp(ii, 0, nx - 1);
  jj = clamp(jj, 0, ny - 1);
  kk = clamp(kk, 0, nz - 1);
  return GetEleInd(ii,jj,kk);
}

void
ElementRegGrid::rmEmptyVert()
{
  vertIdx.resize(X.size());
  std::fill(vertIdx.begin(), vertIdx.end(), -1);
  std::vector<Vector3f> newNodes;
  int vCnt =0;
  for(unsigned int ii = 0;ii<e.size(); ii++){
    Element * ele = e[ii];
    
    for(int jj = 0;jj<ele->nV();jj++){
      int vi = ele->at(jj);
      if(vertIdx[vi]<0){
        newNodes.push_back(X[vi]);
        vertIdx[vi]=vCnt;
        vCnt++;
      }
      vi = vertIdx[vi];
      (*ele)[jj] = vi;
    }
  }
  X=newNodes;
  x=X;
}

void
ElementRegGrid::deleteEle()
{
  for(auto ii = 0;ii< e.size();ii++){
    delete e[ii];
  }
}

void ElementRegGrid::allocate()
{
  for(auto ii = 0;ii< e.size();ii++){
    delete e[ii];
  }
  e.clear();
  X.clear();
  int maxn = std::max(nx, ny);
  maxn = std::max(maxn, nz);
  dx = 1.0f / maxn;
  //number of vertices is 1 more than grid cells in each dimension
  int NY = ny + 1;
  int NZ = nz + 1;
  for (int ii = 0; ii <= nx; ii++) {
    for (int jj = 0; jj <= ny; jj++) {
      for (int kk = 0; kk <= nz; kk++) {
        X.push_back(dx * Vector3f((float)ii, (float)jj, (float)kk));
      }
    }
  }
  for (int ii = 0; ii < nx; ii++) {
    for (int jj = 0; jj < ny; jj++) {
      for (int kk = 0; kk < nz; kk++) {
        std::vector<int> indices(8);
        ElementHex * ele = new ElementHex();
        for(int ll =0;ll<8;ll++){
          (*ele)[ll] = IX(ii+oneEleV[ll][0],
              jj+oneEleV[ll][1],
              kk+oneEleV[ll][2]);
        }
        e.push_back(ele);
      }
    }
  }
  initArrays();
}

ElementRegGrid::ElementRegGrid(int _nx , int _ny , int _nz )
  :nx(_nx),ny(_ny),nz(_nz)
{
  if (nx>0 && ny>0 && nz > 0){
    allocate();
  }
}

void
ElementRegGrid::resize(int _nx, int _ny, int _nz)
{
  nx = _nx;
  ny = _ny;
  nz = _nz;
  allocate();  
}

ElementRegGrid::~ElementRegGrid()
{
  //elements are deleted in base class ElementMesh.
}
