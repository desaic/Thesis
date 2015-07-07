#include "ConfigFile.hpp"
#include "Render.hpp"
#include "World.hpp"
#include "Element.hpp"
#include "ElementQuad.hpp"
#include "ElementRegGrid.hpp"

#include "StepperGrad.hpp"
#include "StepperNewton.hpp"
#include "StepperNewtonDyn.hpp"

#include "MaterialQuad.hpp"
#include "StrainEneNeo.hpp"
#include "StrainLin.hpp"
#include "StrainCorotLin.hpp"
#include "UnitTests.hpp"
#include "Quadrature.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
void runTest()
{
  stiffnessTest(2);
	//forceTest(0);
//  system("pause");
}

void makeQuadGrid(ElementMesh * em, int nx, int ny, float meshScale = 1.0f)
{
  float dx = meshScale / ny;
  for(int ii = 0; ii<= nx; ii++){
    for(int jj = 0; jj<=ny; jj++){
      Vector3f v(ii*dx, jj*dx, 0);
      em->X.push_back(v);
    }
  }
  for(int ii = 0; ii<nx; ii++){
    for(int jj = 0; jj<ny; jj++){
      ElementQuad * e = new ElementQuad();
      em->e.push_back(e);
      int VIDX[4][2] = {{0,0},{0,1},{1,0},{1,1}};
      for(int kk =0; kk<e->nV(); kk++){
        int vi=ii+VIDX[kk][0];
        int vj=jj+VIDX[kk][1];
        (*e)[kk] = vi*(ny+1) + vj;
      }
    }
  }
  em->initArrays();
}

int main(int argc, char* argv[])
{
  const char * filename = "config.txt";
  if(argc>1){
    filename = argv[1];
  }
  ConfigFile conf;
  conf.load(filename);
  if (conf.getBool("test")){
    runTest();
    return 0;
  }
  int dim = 3;
  if(conf.hasOpt("dim") && conf.getInt("dim")==2){
    dim = 2;
  }

  //make materials
  std::vector<StrainEneNeo> ene(2);
  //std::vector<StrainCorotLin> ene(2);
  //std::vector<StrainLin> ene(2);
  ene[0].param[0] = 1e4;
  ene[0].param[1] = 1e5;
  ene[1].param[0] = 4e8;
  ene[1].param[1] = 1.5e9;
  std::vector<MaterialQuad> material(ene.size());
  for (unsigned int ii = 0; ii < material.size(); ii++){
    for (unsigned int jj = 0; jj < material[ii].e.size(); jj++){
      material[ii].e[jj] = &ene[ii];
    }
  }

  //inch to meter
  //  float meshScale = 0.0254;
  float meshScale = 1;

  ElementMesh * em = 0;
  Vector3f ff(100, 0, 0);
  if(dim == 2){
    for (unsigned int ii = 0; ii < material.size(); ii++){
      material[ii].q = &(Quadrature::Gauss2_2D);
      material[ii].e.resize(material[ii].q->x.size());
      for (unsigned int jj = 0; jj < material[ii].e.size(); jj++){
        material[ii].e[jj] = &ene[ii];
      }
    }

    em = new ElementMesh();
    em->dim = dim;
    int refine = 1;
    if (conf.hasOpt("refine")){
      refine = conf.getInt("refine");
    }
    int res = (int)std::pow(2, refine);
    int nx = res, ny=4*res;
    makeQuadGrid(em, nx, ny, meshScale);

    int topV[2] = {1,3};
    int botV[2] = {0,2};
    Vector3f force = (1.0/nx)*ff;
    for(int ii = 0; ii<nx; ii++){
      int ei = ii*ny+ny-1;
      for(int jj =0; jj<2; jj++){
        int vi = em->e[ei]->at(topV[jj]);
        em->fe[vi] += force;
      }
      ei = ii*ny;
      for(int jj =0; jj<2; jj++){
        int vi = em->e[ei]->at(botV[jj]);
//        em->fixed[vi] = true;
      }
    }

  }else if(conf.hasOpt("meshfile")){
    std::string meshfile = conf.getString("meshfile");
    std::ifstream in(meshfile);
    if(!in.good()){
      std::cout<<"Can't open fem mesh "<<meshfile<<"\n";
      return -1;
    }
    em = new ElementMesh();
    em->dim = dim;
    em->load(in, meshScale);

    //apply some forces
    float max = 0, min = 100;
    for(unsigned int ii =0 ; ii<em->x.size(); ii++){
      if(em->X[ii][1]>max){
        max = em->X[ii][1];
      }
      if(em->X[ii][1]<min){
        min = em->X[ii][1];
      }
    }
    float eps = 1e-4;
    for(unsigned int ii =0 ; ii<em->x.size(); ii++){
      if(em->X[ii][1]>max-eps && em->X[ii][0]>0.5){
        em->fe[ii] = Vector3f(0, -100.0, 0);
      }else if(em->X[ii][1]< min + eps ){
        em->fixed[ii] = true;
      }
    }

    for(unsigned int ii =0 ; ii<em->me.size(); ii++){
      em->me[ii] = 0;
    }

    in.close();
  }else{
    int refine = 1;
    if (conf.hasOpt("refine")){
      refine = conf.getInt("refine");
    }
    int res = (int)std::pow(2, refine);
    int nx = res, ny=4*res, nz=res;
    //int nx = 32, ny = 80, nz = 32;
    //int nx = 16, ny = 40, nz = 16;
    //per element pushing force
    ff = (1.0f / (nx*nz)) * ff;
    ElementRegGrid * grid = new ElementRegGrid(nx,ny,nz);
    em = grid;

    //assign some materials
    for (int ii = 0; ii < nx; ii++){
      for (int jj = 0; jj < ny; jj++){
        for (int kk = 0; kk < nz; kk++){
          int eidx = grid->GetEleInd(ii, jj, kk);

          //if (jj < ny / 2){
          //  em->me[eidx] = 1;
          //}
          //if ( (ii == 0) || ii == nx - 1 || kk == 0 || kk == nz - 1){
          //  em->me[eidx] = 1;
          //}
        }
      }
    }

    //add force and constraints
    for(int ii = 0;ii<nx;ii++){
      for(int jj =0;jj<nz;jj++){
        int eidx= grid->GetEleInd(ii,0,jj);
        std::cout<<ii<<" "<<jj<<" "<<eidx<<"\n";
        int aa[4] = {0,1,4,5};
        for(int kk = 0;kk<4;kk++){
          int vidx =em->e[eidx]->at(aa[kk]);
          std::cout<<vidx<<"\n";
          em->fixed[vidx] = 1;
        }

        eidx= grid->GetEleInd(ii,ny-1,jj);
        int bb[4] = {2,3,6,7};
        for(int kk = 0;kk<4;kk++){
          int vidx = em->e[eidx]->at(bb[kk]);
          em->fe[vidx] += ff;
        }
      }
    }
  }

  for(int ii =0 ; ii<material.size(); ii++){
    em->addMaterial(&material[ii]);
  }

  em->dt = 0.001;
  em->check();

  World * world = new World();
  bool renderOpt = true;

  if (conf.hasOpt("render") && conf.getBool("render") == false){
    renderOpt = false;
  }

  if (renderOpt){
    world->em.push_back(em);
  }

  Stepper * stepper = 0;
  int nSteps = 10000;

  if (conf.hasOpt("nSteps")){
    nSteps = conf.getInt("nSteps");
  }

  std::string stepperType = conf.getString("stepper");

  if (stepperType == "newton"){
    stepper = new StepperNewton();
  }else if (stepperType == "newtonDyn"){
    stepper = new StepperNewtonDyn();
  }
  else if (stepperType == "grad"){
    stepper = new StepperGrad();
  }

  if(stepper==0){
    std::cout<<"Error in config: need a time stepper\n";
    return -1;
  }

  stepper->nSteps = nSteps;
  stepper->init(em);
  stepper->launchThread();
  Render render;
  world->stepper = stepper;
  render.init(world);
  render.loop();
	return 0;
}
