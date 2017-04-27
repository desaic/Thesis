#include "StrainLin.hpp"

StrainLin::StrainLin()
{
	param.resize(2);
	param[0] = 1;
	param[1] = 10;
}

float StrainLin::getEnergy(const Matrix3f & F)
{
	Matrix3f I = Matrix3f::identity();
	Matrix3f eps = 0.5*(F + F.transposed()) - I;
	float t = eps.trace();
	float Psi = param[0]*eps.norm2() + 0.5f*param[1] * t*t;
	return Psi;
}

Matrix3f StrainLin::getPK1(const Matrix3f & F)
{
	float mu = param[0];
	float lambda = param[1];
	Matrix3f I = Matrix3f::identity();
	Matrix3f PP = mu*(F + F.transposed()-2*I) + lambda * (F.trace()-3) * I;
	return PP;
}

Matrix3f StrainLin::getdPdx(const Matrix3f & F, const Matrix3f & dF)
{
	Matrix3f dP = Matrix3f();
	float mu = param[0];
	float lambda = param[1];
	Matrix3f I = Matrix3f::identity();
	dP = mu * (dF + dF.transposed()) + lambda * dF.trace() * I;
	return dP;
}

Eigen::MatrixXf StrainLin::EMatrix()
{
	Eigen::MatrixXf E=Eigen::MatrixXf::Zero(6,6);
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
