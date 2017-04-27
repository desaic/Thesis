#ifndef MATRIXX_HPP
#define MATRIXX_HPP
#include <iostream>

template <typename T>
void copyArray(const T* src, T*dst, int size)
{
  for(int ii = 0;ii<size;ii++){
    dst[ii] = src[ii];
  }
}

template<typename ValueType>
struct MatrixX
{
  MatrixX() :M(0), mm(0), nn(0){}

  MatrixX(int _m, int _n) : M(0), mm(_m), nn(_n){ allocate(_m, _n); }

  MatrixX(const MatrixX & ma) : M(0), mm(ma.mm), nn(ma.nn){
    allocate(mm, nn);
    copyArray(ma.M, M, mm*nn);
  }

  void fill(ValueType val){
    int nEntry = mm*nn;
    for (int ii = 0; ii<nEntry; ii++){
      M[ii] = val;
    }
  }

  MatrixX & operator=(const MatrixX & ma){
    if (mm * nn != ma.mm * ma.nn && M != 0){
      delete[]M;
      M = 0;
    }
    mm = ma.mm;
    nn = ma.nn;
    if (M == 0){
      M = new ValueType[mm*nn];
    }
    copyArray(ma.M, M, mm*nn);
    return *this;
  }

  void allocate(int _m, int _n){
    if (mm * nn != _m * _n && M != 0){
      delete[]M;
      M = 0;
    }
    nn = _n;
    mm = _m;
    if (M == 0){
      M = new ValueType[mm*nn];
    }
  }

  void resize(int _m, int _n)
  {
    if (M == 0){
      allocate(_m, _n);
      return;
    }
    if (mm == _m && nn == _n){
      return;
    }

    ValueType * Mold = M;
    M = new ValueType[_m*_n];
    for (int ii = 0; ii<_m; ii++){
      for (int jj = 0; jj<_n; jj++){
        if (ii<mm && jj<nn){
          M[ii*_n + jj] = Mold[ii*nn + jj];
        }
        else{
          M[ii*_n + jj] = 0;
        }
      }
    }
    mm = _m;
    nn = _n;
    delete[] Mold;
  }

  int get1dIndex(int ii, int jj)const{
    return ii*nn + jj;
  }

  ValueType & operator()(int ii, int jj){
    return  M[get1dIndex(ii, jj)];
  }

  ValueType operator()(int ii, int jj)const{
    return  M[get1dIndex(ii, jj)];
  }

  ~MatrixX(){
    if (M != 0){
      delete[]M;
    }
  }

  void print(std::ostream & stream){
    for (int ii = 0; ii<mm; ii++){
      for (int jj = 0; jj<nn; jj++){
        stream << operator ()(ii, jj) << " ";
      }
      stream << "\n";
    }
    stream << "\n";
  }

  ValueType * M;
  int mm, nn;
};

template<typename T1, typename T2>
void addMat(const T1 & src, T2 & dst, int m, int n)
{
  for (int ii = 0; ii<m; ii++){
    for (int jj = 0; jj<n; jj++){
      dst(ii, jj) += src(ii, jj);
    }
  }
}

template<typename T1, typename T2>
void addScaleMat(const T1 & src, T2 & dst, float s, int m, int n)
{
  for (int ii = 0; ii<m; ii++){
    for (int jj = 0; jj<n; jj++){
      dst(ii, jj) += s * src(ii, jj);
    }
  }
}


template<typename T1, typename T2>
void addSubMat(const T1 & src, T2 & dst, int si1, int sj1, int si2, int sj2,
                int m, int n)
{
  for(int ii = 0;ii<m;ii++){
    for(int jj = 0;jj<n;jj++){
      dst(si2 + ii, sj2 + jj) += src(si1+ii, sj1+jj);
    }
  }
}

template<typename T1, typename T2>
void copyMat(const T1 & src, T2& dst, int m, int n)
{
  for(int ii = 0;ii<m;ii++){
    for(int jj = 0;jj<n;jj++){
      dst(ii,jj) = src(ii,jj);
    }
  }
}

typedef MatrixX<double> MatrixXd;
typedef MatrixX<float> MatrixXf;
#endif
