#ifndef GLHEADER_HPP
#define GLHEADER_HPP

#undef USE_SHADER

#ifdef __APPLE__
//#include <GL/glew.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#elif __linux
//#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <GL/freeglut.h>
#include <glfw3.h>
#elif _WIN32
#define NOMINMAX //Stop errors with std::max
#include<windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "glfw3.h"
#endif ///includes

#endif ///glheader.hpp

