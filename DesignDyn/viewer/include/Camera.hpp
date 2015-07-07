#ifndef CAMERA_HPP
#define CAMERA_HPP
#include <vecmath.h>
#include <math.h>
const float MAXY = 0.49f * 3.141593f;
const float MAXXZ = 2*3.141593f;
#define NKEYS 6
struct Camera{
  Camera():angle_xz(0),angle_y(0){
    for (int ii=0;ii<3;ii++){
      eye[ii]=0.0f;
      at[ii]=0.0f;
      up[ii] = 0.0f;
    }
    at[2]=-1.0f;
    at[1] = 0.5f;
    eye[2]= -2.0f;
    eye[1]=0.5f;
    up[1] = 1.0f;

    for(int ii= 0;ii<NKEYS;ii++){
      keyhold[ii] = false;
    }
  }
  float angle_xz, angle_y;
  void update(){
    Vector3f viewDir ( -sin(angle_xz),0,cos(angle_xz));
    if(angle_y > MAXY){
      angle_y = MAXY;
    }
    if(angle_y< -MAXY){
      angle_y = -MAXY;
    }

    if(angle_xz >  MAXXZ){
      angle_xz -=  MAXXZ;
    }
    if(angle_xz < -MAXXZ){
      angle_xz +=  MAXXZ;
    }

    Vector3f yaxis = Vector3f(0,1,0);
    Vector3f right = Vector3f::cross(viewDir,yaxis);
    viewDir = cos(angle_y)*viewDir + sin(angle_y)*yaxis;
    at = eye + viewDir;
    up = Vector3f::cross(right, viewDir);
  }

  Vector3f eye;
  Vector3f at;
  Vector3f up;

  //if keys are held
  //W S A D R F
  bool keyhold[NKEYS];
};

#endif
