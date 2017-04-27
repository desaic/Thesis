#include "MatTable.hpp"
#include "FileUtil.hpp"
#include <stdio.h>
#include <string>
#include <sstream>

#ifdef _MSC_VER
#define snprintf sprintf_s
#endif
const int bufSize=64;

MatTable::MatTable()
{}

int comb2Int(int * mats, int nMat, int nVox)
{
  int idx = 0;
  int pow = 1;
  for(int ii = nVox-1;ii>=0;ii--){
    idx += mats[ii] * pow;
    pow *= nMat;
  }
  return idx;
}

void int2Comb(int * mats, int idx, int nMat, int nVox)
{
  for(int ii = nVox-1;ii>=0;ii--){
    mats[ii] = idx % nMat;
    idx/=nMat;
  }
}

///@brief Parse a string into a sequence of ints.
void string2Comb(const std::string ss, int * mats, int nVox)
{
  std::string str=ss.substr(0,ss.find('.'));
  char cc;
  std::stringstream sst(str);
  for(int ii = 0;ii<nVox;ii++){
    sst>>mats[ii];
    sst>>cc;
  }
}

int MatTable::fine2Coarse(int * fineMats, int lvl)const
{
  if(lvl==3){
    int matIdx = comb2Int(fineMats,l[lvl].nMat, 8);
    int ctrIdx = l[lvl][matIdx];
    return ctrIdx;
  }else if(lvl==6){
    int matZY[2][2];
    int matZ[2];
    int mats[2];
    int matIdx;
    for(int zi = 0; zi<2; zi++){
      for(int yi =0; yi<2; yi++){
        int pair[2]={2*yi+zi, 2*yi+zi+4};
        mats[0]=fineMats[pair[0]];
        mats[1]=fineMats[pair[1]];
        matIdx=comb2Int(mats, l[lvl-2].nMat, l[lvl-2].nVox);
        matZY[zi][yi]=l[lvl-2][matIdx];
      }
    }
    for(int zi = 0;zi<2;zi++){
      matIdx=comb2Int(matZY[zi], l[lvl-1].nMat, l[lvl-1].nVox);
      matZ[zi] = l[lvl-1][matIdx];
    }
    matIdx=comb2Int(matZ,l[lvl].nMat,l[lvl].nVox);
    int ctrIdx=l[lvl][matIdx];
    return ctrIdx;
  }
  return 0;
}

void MatTable::coarse2Fine(int * fineMats, int coarseCtr, int lvl) const
{
  if(lvl==3){
    return centerMats(coarseCtr,fineMats,lvl);
  }else{
    int mats[2];
    centerMats(coarseCtr, mats, lvl);
    return coarse2FineMat(fineMats,mats,lvl);
  }
}

void
MatTable::coarse2Fine(int * fineMats, int * coarseMats, int lvl)const
{
  //in x y z order
  int axis = (lvl+2)%3;
  int idx =0 ;
  int coarseMatZY[2][2];
  int coarseMatZYX[2][2][2];

  switch(axis){
  case 0:
    for(int ii = 0;ii<8;ii++){
      fineMats[ii] = coarseMats[ii/4];
    }
    break;
  case 1:
    for(int ii =0;ii<2;ii++){
      centerMats(coarseMats[ii],coarseMatZY[ii],lvl-1);
    }
    for(int xi=0;xi<2;xi++){
      for(int yi = 0;yi<2;yi++){
        for(int zi = 0;zi<2;zi++){
          fineMats[idx] = coarseMatZY[yi][xi];
          idx++;
        }
      }
    }
    break;
  case 2:
    for(int ii =0;ii<2;ii++){
      centerMats(coarseMats[ii],coarseMatZY[ii],lvl-1);
    }
    for(int ii =0;ii<2;ii++){
      for(int jj =0; jj<2; jj++){
        centerMats(coarseMatZY[ii][jj],coarseMatZYX[ii][jj],lvl-2);
//        std::cout<<coarseMatZYX[ii][jj][0]<<","<<coarseMatZYX[ii][jj][1]<<"  ";
      }
//      std::cout<<"\n";
    }
    int idx =0 ;
    for(int xi=0;xi<2;xi++){
      for(int yi = 0;yi<2;yi++){
        for(int zi = 0;zi<2;zi++){
          fineMats[idx] = coarseMatZYX[zi][yi][xi];
          idx++;
        }
      }
    }
    break;
  }
}

char *
MatTable::paramFileName(int lvl, int * mats)
{
  const int bufSize=64;
  char filename[bufSize];
  char dirname[bufSize];
  snprintf(dirname,bufSize,"../../mat/param/%d/",lvl);
  int len = 0;
  int nVox = l[lvl].nVox;
  for(int ii =0;ii<nVox;ii++){
    len += snprintf(filename+len,bufSize-len,"%d_",mats[ii]);
  }
  len--;
  filename[len]='\0';
  char * infile = new char[bufSize];
  snprintf(infile,bufSize,"%s%s.txt",dirname,filename);
  return infile;
}

void MatTable::loadTable(int lvl, std::istream & in)
{
  int nComb= (int)std::pow(l[lvl].nMat, l[lvl].nVox);
  int nLines;
  in>>nLines;
  in>>l[lvl].nCenter;
  l[lvl].ctrIdx.resize(l[lvl].nCenter);
  if(nComb!=nLines){
    std::cout<<"warning: table size differ from number of combinations\n";
    std::cout<<nLines<<" "<<nComb<<"\n";
  }
  l[lvl].cl.resize(nComb);
  std::string comb,center;
  int cc;
  int * mats = new int[l[lvl].nVox];
  for(int ii = 0;ii<nLines;ii++){
    in>>comb>>cc>>center;
    string2Comb(comb,mats, l[lvl].nVox);
    int combIdx=comb2Int(mats, l[lvl].nMat, l[lvl].nVox);
    l[lvl][combIdx] = cc;

    string2Comb(center, mats, l[lvl].nVox);
    combIdx=comb2Int(mats, l[lvl].nMat, l[lvl].nVox);
    l[lvl].ctrIdx[cc] = combIdx;
  }
  delete []mats;
}

void MatTable::loadLevel(int lvl, const char * dirname)
{
  char filename[bufSize];
  snprintf(filename,bufSize,"%s/%d.txt",dirname,lvl);
  FileUtilIn in(filename);
  loadTable(lvl,in.in);
  in.close();
}

void MatTable::saveClusterCenter(const char *filename, int level)
{
  FileUtilOut out(filename);
  out.out<<l[level].nCenter<<" "<<l[level].nVox<<"\n";
  int * mats=new int[l[level].nVox];
  for(int ii = 0;ii<l[level].nCenter; ii++){
    int2Comb(mats,l[level].ctrIdx[ii],
             l[level].nMat,l[level].nVox);
    for(int jj = 0;jj<l[level].nVox;jj++){
      if(jj>0){
        out.out<<" ";
      }
      out.out<<mats[jj];
    }
    out.out<<"\n";
  }
  delete []mats;
  out.close();
}

std::string readLineNoComment(std::istream & in)
{
  std::string ss;
  while(1){
    std::getline(in,ss);
    if(ss[0]!='#'){
      break;
    }
    if(in.eof()){
      break;
    }
  }
//  std::cout<<ss<<"\n";
  return ss;
}

void
MatTable::loadMetaInfo(const char*filename)
{
  FileUtilIn in(filename);
  std::string token;
  std::stringstream sst;
  std::string line;

  line=readLineNoComment(in.in);
  sst.clear();
  sst.str(line);
  sst>>token;
  sst>>nTab;
  for(int ii =0;ii<nTab;ii++){
    line=readLineNoComment(in.in);
    sst.clear();
    sst.str(line);
    int lvl;
    sst>>lvl;
    if(l.size()<lvl+1){
      l.resize(lvl+1);
    }
    sst>>l[lvl].nMat;
    sst>>l[lvl].nVox;
  }

  in.close();
}

void
MatTable::centerMats(int ctr, int * mats, int lvl)const
{
  int idx = l[lvl].ctrIdx[ctr];
  int2Comb(mats,idx, l[lvl].nMat, l[lvl].nVox);
}

int MatTable::findCenter(int * mats, int lvl)
{
  int combIdx=comb2Int(mats, l[lvl].nMat, l[lvl].nVox);
  return l[lvl][combIdx];
}
