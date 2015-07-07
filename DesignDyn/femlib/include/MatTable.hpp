#ifndef MATTABLE_HPP
#define MATTABLE_HPP

#include <vector>
#include <iostream>

///@brief Information of a level.
struct MatLevelInfo
{
  ///@brief Index for centroids.
  std::vector<int> ctrIdx;
  int lvl;
  ///@brief number of centers in each level.
  int nCenter;
  ///@brief number of base materials at this level.
  int nMat;
  ///@brief number of voxels merged in each level.
  int nVox;
  ///@brief cluster index for each material combination.
  std::vector<int> cl;
  MatLevelInfo():lvl(0),nCenter(0),nVox(0){}
  int & operator[](int idx){
    return cl[idx];
  }
  int operator[](int idx)const{
    return cl[idx];
  }
};

class MatTable
{
public:

  MatTable();

  void loadMetaInfo(const char*filename);
  void loadLevel(int lvl, const char * dirname);
  void loadTable(int lvl, std::istream & in);
  void saveClusterCenter(const char * filename, int level);
  char * paramFileName(int lvl, int * mats);

  ///@param nm Total number of materials.
  ///@param nv Number of voxels.
  int findCenter(int * mats, int lvl);

  ///@brief the set of materials for a centroid.
  void centerMats(int ctr, int * mats, int lvl) const;

  ///@brief given a pair of coarse materials, assign fine materials
  ///to a 2x2x2 grid.
  void coarse2FineMat(int * fineMats, int * coarseMats, int lvl) const;

  ///@brief Given a centroid index, find the set of fine materials.
  ///Only work for lvl=3 and 6.
  void coarse2Fine(int * fineMats, int coarseCtr, int lvl) const;

  int fine2Coarse(int * fineMats, int lvl)const;

  void coarse2Fine(int * fineMats, int * coarseMats, int lvl)const;

  int nTab;
  std::vector<MatLevelInfo> l;
};

///@brief compute the index of a material combination.
int comb2Int(int * mats, int nMat, int nVox);

///@brief compute material combination given the index.
void int2Comb(int * mats, int idx, int nMat, int nVox);

#endif // MATTABLE_HPP
