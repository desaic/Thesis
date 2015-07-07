#ifndef WORLDHPP
#define WORLDHPP
#include <vector>
class ElementMesh;
class Stepper;
class World{
public:
  World() :stepper(0){}
  std::vector<ElementMesh*> em;
  Stepper * stepper;
};
#endif