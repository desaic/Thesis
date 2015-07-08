#include "ElementQuad.hpp"
#include <iostream>

const int quadEdges[4][2] =
{ {0,1},
  {1,3},
  {3,2},
  {2,0}};

const int sw[4][3] =
{{ -1,-1},
 { -1, 1},
 {  1,-1},
 {  1, 1}};

Vector3f ElementQuad::natCoord(const Vector3f & p, const std::vector<Vector3f> & X)
{
  Vector3f n = p - X[at(0)];
  float size = (X[at(3)][0] - X[at(0)][0]);
  n = (2.0 / size) * n - Vector3f(1,1,1);
  return n;
}

std::vector<float>
ElementQuad::shapeFun(const Vector3f & p)const
{
  std::vector<float> weights(nV(), 0);
  for(int ii = 0;ii<nV();ii++){
    weights[ii] = (1.0f/4) * (1+sw[ii][0]*p[0])*(1+sw[ii][1]*p[1]);
  }
  return weights;
}

Vector3f
ElementQuad::shapeFunGrad(int ii, const Vector3f & xx,
                                 const std::vector<Vector3f> & X) const
{
  Vector3f size=2*(X[at(3)] - X[at(0)]);
//  std::cout<<size[0]<<"\n";

  Vector3f grad;
  size[0] = 1.0f/(size[0]);
  size[1] = 1.0f/(size[1]);
  grad[0] = sw[ii][0] * size[0] * (1 + sw[ii][1] * xx[1]);
  grad[1] = sw[ii][1] * size[1] * (1 + sw[ii][0] * xx[0]);
  grad[2] = 0;
  return grad;
}

std::vector<std::array<int,2> >
ElementQuad::getEdges()
{
  int nEdge = 4;
  std::vector<std::array<int,2> >  edges(nEdge);
  for(int ii=0; ii<nEdge; ii++){
    edges[ii][0] = quadEdges[ii][0];
    edges[ii][1] = quadEdges[ii][1];
  }
  return edges;
}

float ElementQuad::getVol(const std::vector<Vector3f> & X)
{
  Vector3f size = X[at(3)] - X[at(0)];
  float vol = size[0] * size[1];
  return vol;
}

ElementQuad::ElementQuad():Element(4)
{}

ElementQuad::ElementQuad(const ElementQuad & e) :Element(e)
{}
