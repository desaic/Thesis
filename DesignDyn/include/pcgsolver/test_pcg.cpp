#include <iostream>
#include <fstream>
#include <algorithm>
#include <utility>
#include "pcg_solver.h"

const int STENCIL_SIZE=4;

int varIdx(int row, int col, int N)
{
  return row*N+col;
}

bool inBound(int row, int col, int N)
{
  return (row>=0 && col>=0 && row<N && col<N);
}

template <class T1, class T2>
struct CmpPairFirst{
  bool operator()(const std::pair<T1, T2> & p1, const std::pair<T1, T2> & p2){
    return p1.first<p2.first;
  }
};

typedef std::pair<unsigned int, double> ColVal;

int main(int argc, char * argv[])
{
  int gridSize = 10;
  int matSize = gridSize * gridSize;

  //only works with double
  SparseMatrixd mat(matSize);
  std::vector<double> rhs(matSize,0);
  CmpPairFirst<unsigned int, double> cmp;
  int nbr[STENCIL_SIZE][2] = {{-1,0},{0,-1},{0,1},{1,0}};
  //make a 2d laplace matrix on regular grid
  //row
  for(int ii = 0; ii<gridSize; ii++){
    //col 
    for(int jj = 0; jj<gridSize; jj++){
      int centerIdx = varIdx(ii, jj, gridSize);
      double centerVal = 0;
      std::vector<ColVal> pairs;
      int nbrCnt = 0;
      for(int nn = 0; nn<STENCIL_SIZE; nn++){
        int ni = ii + nbr[nn][0];
        int nj = jj + nbr[nn][1];
        if(inBound(ni, nj, gridSize)){
          int nbrIdx = varIdx( ni, nj, gridSize);
          nbrCnt++;
          pairs.push_back(ColVal(nbrIdx, -1));
        }
      }
      centerVal = nbrCnt;
      pairs.push_back(ColVal(centerIdx, centerVal));
      std::sort(pairs.begin(), pairs.end(), cmp);
      std::vector<unsigned int> indices(pairs.size());
      std::vector<double> values (pairs.size());
      for(unsigned int nn = 0; nn<pairs.size(); nn++){
        indices[nn] = pairs[nn].first;
        values[nn] = pairs[nn].second;
      }
      mat.add_sparse_row(centerIdx, indices, values);
      float dx = 1.0/gridSize;
      rhs[centerIdx] = dx*ii*dx*ii + dx*jj;
    }
  }
  unsigned int lastIdx = varIdx(gridSize-1, gridSize-1, gridSize);
  mat.symmetric_remove_row_and_column(lastIdx);
  mat.set_element(lastIdx, lastIdx, 1);
  rhs[lastIdx] = 0;
  mat.write_matlab(std::cout, "K");
  
  PCGSolver <double> solver;
  solver.set_solver_parameters(1e-12, 1000);
  double residual;
  int iters;
  std::vector<double> result(matSize,0);
  
  solver.solve(mat, rhs, result, residual, iters);
  
  for(unsigned int ii =0 ; ii<rhs.size(); ii++){
    std::cout<<rhs[ii]<<"\n";
  }
  for(unsigned int ii =0 ; ii<result.size(); ii++){
    std::cout<<result[ii]<<"\n";
  }
  std::cout<<"Residual: "<<residual<<"\n";
  std::cout<<"Iterations: "<<iters<<"\n";
  return 0;
}
