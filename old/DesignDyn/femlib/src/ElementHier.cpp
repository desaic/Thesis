#include "Element.hpp"
#include "ElementHier.hpp"
#include "ElementRegGrid.hpp"

extern int oneEleV [8][3];

ElementHier::ElementHier(const ElementHex& e) :ElementHex(e), parent(-1)
{}

//replace element hex with elementCoarse
void replaceElementHex(ElementRegGrid * grid)
{
  for (unsigned int ii = 0; ii < grid->e.size(); ii++){
    ElementHex * ele = (ElementHex*)grid->e[ii];
    ElementHier * ec = new ElementHier(*ele);
    delete ele;
    grid->e[ii] = ec;
  }
}

ElementRegGrid * coarsen(const ElementRegGrid * grid)
{
  int nx = grid->nx / 2;
  int ny = grid->ny / 2;
  int nz = grid->nz / 2;
  ElementRegGrid * coarsem = new ElementRegGrid();
  coarsem->nx = nx;
  coarsem->ny = ny;
  coarsem->nz = nz;
  coarsem->e.resize(nx*ny*nz, 0);
  int idx = 0;
  std::vector<int> coarseVidx(grid->X.size(), -1);
  
  for(int ii = 0; ii<nx; ii++){
    for(int jj = 0; jj<ny; jj++){
      for(int kk = 0; kk<nz; kk++){
        ElementHier * ec = new ElementHier();
        for(int ll = 0; ll<8; ll++){
          int fineIdx = grid->GetEleInd(2 * ii + oneEleV[ll][0],
            2 * jj + oneEleV[ll][1], 2 * kk + oneEleV[ll][2]);
          ElementHier *fine = (ElementHier *)grid->e[fineIdx];
          fine->parent = idx;
          int fineVidx = grid->e[fineIdx]->at(ll);
          int vc = coarseVidx[fineVidx];
          if (vc < 0){
            vc = (int)coarsem->X.size();
            coarseVidx[fineVidx] = vc;
            coarsem->X.push_back(grid->X[fineVidx]);
          }
          (*ec)[ll] = vc;
        }
        coarsem->e[idx] = ec;
        idx++;
      }
    }
  }
  coarsem->initArrays();

  //set natural coordinate of fine element in coarse element
  for (unsigned int ii = 0; ii<grid->e.size(); ii++){
    ElementHier * fine = (ElementHier *)grid->e[ii];
    ElementHier * ec = (ElementHier*)coarsem->e[fine->parent];
    fine->Xn.resize(8);
    for (int jj = 0; jj < 8; jj++){
      fine->Xn[jj] = ec->natCoord(grid->X[fine->at(jj)], coarsem->X);
    }
  }
  return coarsem;
}

ElementHier::ElementHier() :parent(-1)
{}
