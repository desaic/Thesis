/*
 * Render.cpp
 *
 *  Created on: Aug 20, 2013
 *      Author: desaic
 */
#include "ArrayUtil.hpp"
#include "Element.hpp"
#include "ElementMesh.hpp"
#include "glheader.hpp"
#include "Render.hpp"
#include "World.hpp"
#include "Stepper.hpp"
#include "vecmath.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <chrono>
#include <atomic>
const float epsilon = 0.01f;
Render * render;
//mouse state
bool captureMouse = false;
static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  Stepper * stepper = render->getStepper();
  std::unique_lock<std::mutex> lck(stepper->mtx, std::defer_lock);
  if(action == GLFW_PRESS){
    switch(key){
    case GLFW_KEY_W:
      render->camera.keyhold[0]=true;
      break;
    case GLFW_KEY_S:
      render->camera.keyhold[1]=true;
      break;
    case GLFW_KEY_A:
      render->camera.keyhold[2]=true;
      break;
    case GLFW_KEY_D:
      render->camera.keyhold[3]=true;
      break;
    case GLFW_KEY_R:
      render->camera.keyhold[4]=true;
      break;
    case GLFW_KEY_F:
      render->camera.keyhold[5]=true;
      break;
    }
  }else if(action==GLFW_RELEASE){
    switch(key){
    case GLFW_KEY_ESCAPE:
      glfwSetWindowShouldClose(window, GL_TRUE);
      break;    
    case GLFW_KEY_W:
      render->camera.keyhold[0]=false;
      break;
    case GLFW_KEY_S:
      render->camera.keyhold[1]=false;
      break;
    case GLFW_KEY_A:
      render->camera.keyhold[2]=false;
      break;
    case GLFW_KEY_D:
      render->camera.keyhold[3]=false;
      break;
    case GLFW_KEY_R:
      render->camera.keyhold[4]=false;
      break;
    case GLFW_KEY_F:
      render->camera.keyhold[5]=false;
      break;

    case GLFW_KEY_P:
      lck.lock();
      stepper->state = Stepper::PAUSE;
      lck.unlock();
      break;
    case GLFW_KEY_LEFT_BRACKET:
      lck.lock();
      if (stepper->state == Stepper::PAUSE){
        stepper->state = Stepper::SINGLE;
        stepper->cv.notify_one();
      }
      else{
        stepper->state = Stepper::SINGLE;
        lck.unlock();
      }
      break;
    case GLFW_KEY_RIGHT_BRACKET:
      lck.lock();
      if (stepper->state == Stepper::PAUSE){
        stepper->state = Stepper::ALL;
        stepper->cv.notify_one();
      }
      else{
        stepper->state = Stepper::ALL;
        lck.unlock();
      }
      break;
    }
  }
}

Stepper * Render::getStepper()
{ 
  return world->stepper;
}

void Render::moveCamera(float dt)
{
  Vector3f viewDir = camera.at - render->camera.eye;
  Vector3f up = camera.up;
  Vector3f right = Vector3f::cross(viewDir,up);
  right[1] = 0;
  viewDir[1] = 0;

  if(camera.keyhold[0]){
    camera.eye += viewDir * dt * camSpeed;
    camera.at  += viewDir * dt * camSpeed;
  }
  if(camera.keyhold[1]){
    camera.eye -= viewDir * dt * camSpeed;
    camera.at  -= viewDir * dt * camSpeed;
  }
  if(camera.keyhold[2]){
    camera.eye -= right * dt * camSpeed;
    camera.at  -= right * dt * camSpeed;
  }
  if(camera.keyhold[3]){
    camera.eye += right * dt * camSpeed;
    camera.at  += right * dt * camSpeed;
  }
  if(camera.keyhold[4]){
    if(camera.eye[1]<2){
      camera.eye[1] += dt * camSpeed;
      camera.at[1]  += dt * camSpeed;
    }
  }
  if(camera.keyhold[5]){
    if(camera.eye[1]>0){
      camera.eye[1] -= dt * camSpeed;
      camera.at[1]  -= dt * camSpeed;
    }
  }
}

void mouseButtonFun(GLFWwindow *window , int button, int action, int mods)
{
  switch(button){
  case GLFW_MOUSE_BUTTON_LEFT:
    if(action == GLFW_RELEASE){
      captureMouse = !captureMouse;
      if(captureMouse){
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glfwSetCursorPos(window,width/2, height/2);
      }
    }
    break;
  }
}

void mousePosFun(GLFWwindow *window , double xpos, double ypos)
{
  if(captureMouse){
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    double dx =  xpos - width/2;
    double dy = -ypos + height/2;
    glfwSetCursorPos(window,width/2, height/2);
    render->camera.angle_xz += (float)(dx * render->xRotSpeed);
    render->camera.angle_y  += (float)(dy * render->yRotSpeed);
    render->camera.update();
  }
}

void Render::drawEle(int eidx, ElementMesh * m)
{
  Element * ele = m->e[eidx];
  std::vector<std::array<int , 2> > edges = ele->getEdges();
  glColor3f(0.4f, 0.5f, 0.9f);
  glDisable(GL_LIGHTING);
  glBegin(GL_LINES);
  for(unsigned int ii = 0;ii<edges.size();ii++){
    int vidx = (*ele)[edges[ii][0]];
    if(vidx>=m->x.size()){
      std::cout<<m->x.size()<<"\n";
    }
    Vector3f v = m->x[vidx];
    glVertex3f(v[0],v[1],v[2]);
    v = m->x[(*ele)[edges[ii][1]]];
    glVertex3f(v[0],v[1],v[2]);
  }

  if (m->u != 0){

    glColor3f(0.2f, 0.7f, 0.1f);
    for (unsigned int ii = 0; ii < edges.size(); ii++){
      Vector3f v = (*(m->u))[eidx][edges[ii][0]];
      glVertex3f(v[0], v[1], v[2]);
      v = (*(m->u))[eidx][edges[ii][1]];
      glVertex3f(v[0], v[1], v[2]);
    }
  }

  glEnable(GL_LIGHTING);
  glEnd();
}

void Render::drawEleMesh(ElementMesh * eMesh)
{
  for(unsigned int ii = 0;ii<eMesh->e.size();ii++){
    drawEle(ii,eMesh);
  }
}

void Render::draw()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(camera.eye[0], camera.eye[1], camera.eye[2],
    camera.at[0], camera.at[1], camera.at[2],
    camera.up[0],camera.up[1],camera.up[2]);

  for(unsigned int ii = 0;ii<world->em.size();ii++){
    drawEleMesh(world->em[ii]);
  }

  GLfloat floorCol[4] =
  { 1, 1, 1, 1 };
  glEnable(GL_LIGHTING);

  GLfloat position[] = { 2.0f, 2, 2, 1.0f };
  GLfloat position1[] = { -1.0f, -1, -1, 1.0f };
  glLightfv(GL_LIGHT0, GL_POSITION, position);
  glLightfv(GL_LIGHT1, GL_POSITION, position1);

  glMaterialfv(GL_FRONT, GL_SPECULAR, floorCol);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, floorCol);
  GLfloat s = 10;
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &s);
  
  glBegin(GL_TRIANGLE_STRIP);
  glNormal3f(0, 1, 0);
  glVertex3f( -1.0f, -.5f , -1.0f);
  glVertex3f( -1.0f, -.5f,  1.0f );
  glVertex3f(  1.0f, -.5f , -1.0f);
  glVertex3f(  1.0f, -.5f ,  1.0f);
  glEnd();

  glfwSwapBuffers(window);
}

int Render::loop()
{
  std::chrono::milliseconds::rep t0, t1, dt;
  auto duration = std::chrono::system_clock::now().time_since_epoch();
  t0 = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

  while (!glfwWindowShouldClose(window))
  {
    draw();
    glfwPollEvents();

    duration = std::chrono::system_clock::now().time_since_epoch();
    t1 = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    dt = t1 - t0;

    moveCamera((double)dt);
    t0 = t1;
  }
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

void window_size(GLFWwindow* window, int width, int height)
{
  float ratio;
  ratio = width / (float) height;
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(40.0, ratio, 0.1, 20.0);
}

void
Render::init(World * _world)
{
  glfwSetErrorCallback(error_callback);
  if (!glfwInit()){
    std::cout<<"Cannot init glfw\n";
    return;
  }
  glfwWindowHint(GLFW_DEPTH_BITS,32);
  window = glfwCreateWindow(800, 600, "Fem Viewer", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    std::cout<<"Cannot create window\n";
    return;
  }
  world = _world;
  render = this;
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback( window,mousePosFun );
  glfwSetMouseButtonCallback( window,mouseButtonFun );
  glfwSetWindowSizeCallback(window, window_size);

  float ratio;
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  ratio = width / (float) height;
  glViewport(0, 0, width, height);
  glClearColor(0.95f, 0.95f, 0.95f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(40.0, ratio, 0.1, 20.0);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_DEPTH_TEST);

  GLfloat white[]={1.0f, 1.0f, 1.0f, 1.0f};
  GLfloat grey[] ={0.3f, 0.3f, 0.3f, 1.0f};

  glLightfv (GL_LIGHT1, GL_DIFFUSE, white);
  glLightfv (GL_LIGHT1, GL_SPECULAR, white);
  glLightfv (GL_LIGHT0, GL_AMBIENT, grey);
}

Render::Render():world(0),anim(false),xRotSpeed(0.004f),
  yRotSpeed(0.004f),camSpeed(0.001f)
{}

Render::~Render()
{}
