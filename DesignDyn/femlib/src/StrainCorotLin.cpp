#include "StrainCorotLin.hpp"
#include <Eigen/Dense>
#include <vector>

#include <iostream>

std::vector<Eigen::Matrix3f> initEp3();
//3D Levi-Civita symbol
std::vector<Eigen::Matrix3f> Ep3=initEp3();

std::vector<Eigen::Matrix3f> initEp3()
{
  std::vector<Eigen::Matrix3f> Ep3(3, Eigen::Matrix3f::Zero());
  Ep3[0](1, 2) = 1;
  Ep3[0](2, 1) = -1;

  Ep3[1](0, 2) = -1;
  Ep3[1](2, 0) = 1;

  Ep3[2](0, 1) = 1;
  Ep3[2](1, 0) = -1;

  return Ep3;
}

float dot(Eigen::Matrix3f m1, Eigen::Matrix3f m2){
  float prod = 0;
  for (int ii = 0; ii < 3; ii++){
    for (int jj = 0; jj < 3; jj++){
      prod += m1(ii, jj) * m2(ii, jj);
    }
  }
  return prod;
}

Eigen::Matrix3f toEigen(const Matrix3f & F)
{
  Eigen::Matrix3f m;
  for (int ii = 0; ii < 3; ii++){
    for (int jj = 0; jj < 3; jj++){
      m(ii, jj) = F(ii, jj);
    }
  }
  return m;
}

Matrix3f toVecmath(const Eigen::Matrix3f & F)
{
  Matrix3f m;
  for (int ii = 0; ii < 3; ii++){
    for (int jj = 0; jj < 3; jj++){
      m(ii, jj) = F(ii, jj);
    }
  }
  return m;
}

StrainCorotLin::StrainCorotLin()
{
  param.resize(2);
  param[0] = 1;
  param[1] = 10;
}

float StrainCorotLin::getEnergy(const Matrix3f & F)
{
  Eigen::Matrix3f m = toEigen(F);
  Eigen::JacobiSVD<Eigen::Matrix3f> svd(m);
  Eigen::Vector3f Sigma = svd.singularValues() - Eigen::Vector3f(1,1,1);
  float mu = param[0];
  float lambda = param[1];
  Eigen::Matrix3f I = Eigen::Matrix3f::Identity();
  float t = Sigma[0] + Sigma[1] + Sigma[2];
  float Psi = mu*(Sigma[0] * Sigma[0] + Sigma[1] * Sigma[1] + Sigma[2] * Sigma[2]) + 0.5f * lambda * t * t;
  return Psi;
}

Matrix3f StrainCorotLin::getPK1(const Matrix3f & F)
{
  Eigen::Matrix3f m = toEigen(F);
  Eigen::JacobiSVD<Eigen::Matrix3f> svd(m, Eigen::ComputeFullU | Eigen::ComputeFullV);
  Eigen::Matrix3f U = svd.matrixU();
  Eigen::Matrix3f V = svd.matrixV();
  Eigen::Matrix3f R = U * V.transpose();
  float mu = param[0];
  float lambda = param[1];
  Eigen::Matrix3f I = Eigen::Matrix3f::Identity();
  Eigen::Matrix3f P = 2*mu*(m-R) + lambda * (R.transpose() * m - I).trace() * R;

  Matrix3f ret = toVecmath(P);
  return ret;
}

Eigen::Matrix3f crossProdMat(const Eigen::Vector3f & v)
{
  Eigen::Matrix3f A = Eigen::Matrix3f::Zero();
  A(0, 1) = -v[2];
  A(0, 2) = v[1];
  A(1, 0) = v[2];
  A(1, 2) = -v[0];
  A(2, 0) = -v[1];
  A(2, 1) = v[0];
  return A;
}

Matrix3f StrainCorotLin::getdPdx(const Matrix3f & m, const Matrix3f & dm)
{
  Eigen::Matrix3f F  = toEigen(m);
  Eigen::Matrix3f dF = toEigen(dm);
  Eigen::JacobiSVD<Eigen::Matrix3f> svd(F, Eigen::ComputeFullU | Eigen::ComputeFullV);
  Eigen::Matrix3f U = svd.matrixU();
  Eigen::Matrix3f V = svd.matrixV();
  Eigen::Matrix3f R = U * V.transpose();
  Eigen::Matrix3f W = R.transpose()*dF;
  Eigen::Matrix3f Sigma = svd.singularValues().asDiagonal();
  Eigen::Matrix3f S = V*Sigma*V.transpose();
  Eigen::Vector3f w;
  Eigen::Matrix3f I = Eigen::Matrix3f::Identity();
  w[0] = W(1, 2) - W(2, 1);
  w[1] = W(2, 0) - W(0, 2);
  w[2] = W(0, 1) - W(1, 0);
  Eigen::Matrix3f SI = (S.trace()*I - S).inverse();
  Eigen::Matrix3f dR = -R*crossProdMat(SI*w);
  //debug dR computation
  //float h = 0.01;
  //Eigen::Matrix3f F1 = F + h*dF;
  //svd.compute(F1, Eigen::ComputeFullU | Eigen::ComputeFullV);
  //U = svd.matrixU();
  //V = svd.matrixV();
  //Eigen::Matrix3f R1 = U * V.transpose();
  //R1 -= R;
  //R1 = (1/h)*R1;
  //std::cout << dR << "\n" << R1 << "\n\n";

  Eigen::Matrix3f dP;
  float mu = param[0];
  float lambda = param[1];
  dP = 2 * mu*dF + lambda*W.trace()*R +(lambda*(S - I).trace() - 2 * mu)*dR;
  return toVecmath(dP);
}

Eigen::MatrixXf StrainCorotLin::EMatrix()
{
  Eigen::MatrixXf E = Eigen::MatrixXf::Zero(6, 6);
  float G = param[0];
  float l = param[1];
  float e = G*(3 * l + 2 * G) / (l + G);
  float nu = l / (2 * (l + G));
  float c = e / ((1 + nu)*(1 - 2 * nu));
  for (int ii = 0; ii < 3; ii++){
    for (int jj = 0; jj < 3; jj++){
      E(ii, jj) = nu;
    }
    E(ii, ii) = 1 - nu;
    E(3 + ii, 3 + ii) = 0.5f - nu;
  }
  return c*E;
}
