#include "Voxel.hpp"
#include <stdlib.h>
#include <iostream>
#include <fstream>
int main(int argc ,char * argv[])
{
  int voxRes = 160;
  if(argc<2){
    std::cout<<argv[0]<<" meshfile(obj format) [voxel resolution:int]\n";
    return 0;
  }
  if(argc>2){
    voxRes = atoi(argv[2]);
  }
  std::cout<<"Voxel grid resolution: "<<voxRes<<"\n";
  const char * meshfile = argv[1];
  std::cout<<"Load mesh "<<meshfile<<"\n";
  Voxel voxel(meshfile , voxRes);
  int vx = voxel.grid.nx;
  int vy = voxel.grid.ny;
  int vz = voxel.grid.nz;
  std::ofstream out("vox_out.txt");
  if(!out.good()){
    std::cout<<"Cannot open output file\n";
    return -1;
  }

  for(int ii = 0;ii<vx;ii++){
    for(int jj = 0;jj<vy;jj++){
      for(int kk = 0;kk<vz;kk++){
        out<<voxel.grid.read(ii,jj,kk)<<" ";
      }
      out<<"\n";
    }
    out<<"\n";
  }
  voxel.saveObj("voxel_out.obj");
  voxel.computeConnectivity();
  voxel.saveConnectivity("fem.txt");
  return 0;
}
