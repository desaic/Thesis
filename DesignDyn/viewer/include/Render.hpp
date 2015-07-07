/*
 * Render.hpp
 *
 *  Created on: Aug 20, 2013
 *      Author: desaic
 */

#ifndef RENDER_HPP_
#define RENDER_HPP_
#include <vector>
#include "vecmath.h"
#include "Camera.hpp"
struct GLFWwindow;
class ElementHex;
class World;
class Element;
class ElementMesh;
class Stepper;
class Render
{
public:
  Render();
  void init(World * world);
  int loop();
  void draw();
  void drawEle(int eidx, ElementMesh * eMesh);
  void drawEleMesh(ElementMesh * eMesh);
  void moveCamera(float dt);
  Stepper * getStepper();
  virtual ~Render();
  bool anim;
  Camera camera;
  GLFWwindow* window;
  
  std::vector<Vector3f> matColor;

  //how fast to rotate in x and y axis
  float xRotSpeed, yRotSpeed;
  float camSpeed;
private:
  ///@brief Render does not own this pointer.
  World * world;
};

#endif /* RENDER_HPP_ */
