#ifndef ARRAYUTIL_HPP
#define ARRAYUTIL_HPP
#include <vector>
#include "vecmath.h"

void BBox(const std::vector<Vector3f >& v,
    Vector3f & mn, Vector3f & mx);

template<typename T>
void add(std::vector<T> & dst, const std::vector<T> & src)
{
  for(unsigned int ii = 0;ii<dst.size();ii++){
    dst[ii] += src[ii];
  }
}

template<typename T>
std::vector<T> mul(float f, const std::vector<T> & src)
{
  std::vector<T> prod(src.size());
  for(unsigned int ii = 0;ii<src.size();ii++){
    prod[ii] = f*src[ii];
  }
  return prod;
}

template<typename T>
void addmul(std::vector<T> & dst, float f, const std::vector<T> & src)
{
  for(unsigned int ii = 0;ii<src.size();ii++){
    dst[ii] += f*src[ii];
  }
}
#endif // ARRAYUTIL_HPP
