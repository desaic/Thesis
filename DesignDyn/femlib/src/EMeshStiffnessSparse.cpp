#include "ElementMesh.hpp"
#include "Element.hpp"
#include "Eigen/Sparse"
#include "MatrixX.hpp"

#include "Timer.hpp"

typedef Eigen::Triplet<float> Tripletf;

///@brief add rows to K and b to constrain 6 degrees of freedom.
///Only works for 3D
void fixRigid(Eigen::SparseMatrix<float> & K,  ElementMesh * mesh)
{
  int row = K.rows();
  K.conservativeResize(row + 6, K.cols()+6);
  Eigen::SparseMatrix<float> KConstraint(row + 6,row + 6);

  Eigen::Vector3f center = Eigen::Vector3f::Zero();
  for(int ii = 0;ii<mesh->x.size();ii++){
    center += (Eigen::Vector3f) mesh->x[ii];
  }
  center /= mesh->x.size();
  float cScale = 1000;
  std::vector<Tripletf> triplets;
  for(int ii = 0;ii<mesh->x.size();ii++){
    int col = mesh->dim*ii;
    //relative position to center
    Eigen::Vector3f rel = (Eigen::Vector3f) mesh->x[ii] - center;
    //cross product matrix
    Eigen::Matrix3f c;
    c <<       0, -rel[2],  rel[1],
          rel[2],       0, -rel[0],
         -rel[1],  rel[0],       0;
    c = -cScale * c;
    for(int kk = 0; kk<3; kk++){
      triplets.push_back( Tripletf(row + kk+3,   col + kk, -cScale ));
//      triplets.push_back( Tripf(col + kk  , row + kk+3, cScale ));
      for(int ll = 0; ll<3;ll++){
        triplets.push_back( Tripletf(row + kk, col + ll, c(kk,ll)) );
//        triplets.push_back( Tripf(col + ll, row + kk, c(kk,ll)) );
      }
    }
  }
  for(int ii = 0;ii<6;ii++){
    triplets.push_back(Tripletf(row+ii, row + ii,0.f));
  }
  KConstraint.setFromTriplets(triplets.begin(), triplets.end());
  K += KConstraint;
}

void ElementMesh::getStiffnessSparse(std::vector<float> &val, bool trig, bool constrained, bool iFixedRigid)
{
  int N = dim * (int)x.size();
  std::vector<Tripletf> coef;
  Eigen::SparseMatrix<float> Ksparse(N,N);

  for(unsigned int ii = 0;ii<e.size();ii++){
    Element * ele = e[ii];
    int nV = ele->nV();
    MatrixXf K  = getStiffness(ii);
    for(int jj = 0; jj<nV; jj++){
      int vj = ele->at(jj);
      for(int kk = 0; kk<nV; kk++){
        int vk = ele->at(kk);
        for(int dim1=0; dim1<dim; dim1++){
          for(int dim2=0; dim2<dim; dim2++){
            if(trig && (dim*vk+dim2 > dim*vj+dim1)) {
              continue;
            }
            float val = K(dim * jj + dim1, dim * kk + dim2);
            if (constrained){
              if (fixed[vk] || fixed[vj]){
                val = 0;
//                if (vj == vk && dim1 == dim2){
//                  val = 100;
//                }
              }
            }
            //if (vk == vj && dim1 == dim2){
            //  val += 1;
            //}
            Tripletf triple(dim * vj + dim1, dim * vk + dim2, val);
            coef.push_back(triple);
          }
        }
      }
    }
  }

  Ksparse.setFromTriplets(coef.begin(), coef.end());

  if (iFixedRigid){
    fixRigid(Ksparse, this);
  }

  for(int ii = 0; ii<Ksparse.rows(); ii++){
    for (Eigen::SparseMatrix<float>::InnerIterator it(Ksparse, ii); it; ++it){
     val.push_back(it.value());
   }
  }
}

Eigen::SparseMatrix<float>
ElementMesh::getStiffnessSparse(bool trig, bool constrained, bool iFixedRigid)
{
  int N = dim * (int)x.size();
  std::vector<Tripletf> coef;
  Eigen::SparseMatrix<float> Ksparse(N,N);
  for(unsigned int ii = 0;ii<e.size();ii++){
    Element * ele = e[ii];
    int nV = ele->nV();
    MatrixXf K  = getStiffness(ii);
    for(int jj = 0; jj<nV; jj++){
      int vj = ele->at(jj);
      for(int kk = 0; kk<nV; kk++){
        int vk = ele->at(kk);
        for(int dim1= 0 ;dim1<dim; dim1++){
          for(int dim2= 0 ;dim2<dim; dim2++){
            if(trig && (dim*vk+dim2 > dim*vj+dim1)) {
              continue;
            }
            float val = K(dim * jj + dim1, dim * kk + dim2);
            if (constrained){
              if (fixed[vk] || fixed[vj]){
                val = 0;
//                if (vj == vk && dim1 == dim2){
//                  val = 1;
//                }
              }
            }
            //if (vk == vj && dim1 == dim2){
            //  val += 1;
            //}
            Tripletf triple(dim * vj + dim1, dim * vk + dim2, val);
//            if(dim * vj + dim1 == dim * vk + dim2){
//              std::cout<<val<<"\n";
//            }
            coef.push_back(triple);
          }
        }
      }
    }
  }
  Ksparse.setFromTriplets(coef.begin(), coef.end());
  if (iFixedRigid){
    fixRigid(Ksparse, this);
  }
//  std::cout<<Ksparse;
  return Ksparse;
}

void ElementMesh::stiffnessPattern(std::vector<int> & I, std::vector<int> & J,
   bool trig, bool iFixedRigid)
{
  int N = dim * (int)x.size();
  std::vector<Tripletf> coef;
  Eigen::SparseMatrix<float> Ksparse(N,N);

  for(unsigned int ii = 0;ii<e.size();ii++){
    Element * ele = e[ii];
    for(int jj = 0; jj<ele->nV(); jj++){
      int vj = ele->at(jj);
      for(int kk = 0; kk<ele->nV(); kk++){
        int vk = ele->at(kk);
        for(int dim1= 0 ;dim1<dim; dim1++){
          for(int dim2= 0 ;dim2<dim; dim2++){
            if(trig&&(dim*vk+dim2 > dim*vj+dim1)){
              continue;
            }
            Tripletf triple(dim*vj+dim1,dim*vk+dim2,1);
            coef.push_back(triple);
          }
        }
      }
    }
  }

  Ksparse.setFromTriplets(coef.begin(), coef.end());
  if (iFixedRigid){
    fixRigid(Ksparse, this);
  }

  I.push_back(0);
  for(int ii = 0; ii<Ksparse.cols(); ii++){
    for (Eigen::SparseMatrix<float>::InnerIterator it(Ksparse, ii); it; ++it){
     J.push_back(it.row());
   }
   I.push_back((int)J.size());
  }
}
