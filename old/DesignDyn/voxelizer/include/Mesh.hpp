#ifndef MESH_H
#define MESH_H

#include <map>
#include <vector>
#include <fstream>
#include <vecmath.h>
#include "vec.h"
class Mesh{
public:
  std::vector<Vector3f>v;
  std::vector<Vector3f>n;
  std::vector<Vector2f>tex;
  std::vector<Vec3i>texId;
  ///@brief triangles
  std::vector<Vec3i>t;

  Mesh();
  Mesh(const std::vector<Vector3f>&_v,
      const std::vector<Vec3i>&_t);
  Mesh(const Vector3f * _v, const Vec3i * _t);
  
  Mesh(const char * filename,bool normalize);
  void load_mesh(const char * filename, bool normalize=true);
  void save(const char * filename);
  void save(std::ostream &out, std::vector<Vector3f> *vert=0);
  void load(std::istream &in);
  void read_ply(std::istream & f);
  void read_obj(std::istream &f);

  void save_obj(const char * filename);
  void load_tex(const char * filename);
  
  void compute_norm();
  void rescale();
  void append(const Mesh & m);
  Mesh & operator= (const Mesh& m);
  virtual void update();
};
void makeCube(Mesh & m, const Vector3f & mn,
    const Vector3f mx);
///@brief cube [0,1]^3
extern Mesh UNIT_CUBE;
void BBox(const Mesh & m, Vector3f & mn,
    Vector3f & mx);

void BBox(const std::vector<Vector3f> & v, Vector3f & mn,
    Vector3f & mx);

bool ptInBox(const Vector3f & mn,
    const Vector3f mx, const Vector3f & x);
void adjlist(const Mesh & m, std::vector<std::vector<int> > & adjMat);

#endif
