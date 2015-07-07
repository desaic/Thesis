#ifndef STEPPER_HPP
#define STEPPER_HPP
#include <thread>
#include <mutex>
#include <condition_variable>

class ElementMesh;

class Stepper{
public:
  enum StepperState{PAUSE, SINGLE, ALL};
  
  Stepper();
  virtual void init(ElementMesh * _m);
  virtual int step();
  virtual int oneStep() = 0;
  virtual ~Stepper();
  
  void launchThread();
  void notify(StepperState s);
  
  int nSteps;
  ElementMesh * m;

  StepperState state;
  std::thread thread;
  std::mutex mtx;
  std::condition_variable cv;
};

#endif