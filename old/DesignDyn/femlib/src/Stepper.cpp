#include "Stepper.hpp"
#include "Timer.hpp"

#include <atomic>
#include <iostream>

Stepper::Stepper() :nSteps(100), m(0)
{}

Stepper::~Stepper(){}

void runSim(ElementMesh * m, Stepper * stepper)
{
  Timer t;
  for (int iter = 0; iter < stepper->nSteps; iter++){
    std::cout << "iter: " << iter <<"\n";
    std::unique_lock<std::mutex> lck(stepper->mtx);
    while (stepper->state == Stepper::PAUSE){
      stepper->cv.wait(lck);
    }
    if (stepper->state == Stepper::SINGLE){
      stepper->state = Stepper::PAUSE;
    }
    lck.unlock();
    
    t.start();
    int ret = stepper->oneStep();
    t.end();
    float duration = t.getSeconds();
    std::cout << "time: " << duration << "\n";
    if (ret < 0){
      break;
    }
  }
  std::cout << "Exit stepper loop\n";
}

void Stepper::launchThread()
{
  state = PAUSE;
  thread = std::thread(runSim, m, this);
}

void Stepper::init(ElementMesh * _m)
{
  m = _m;
}

int Stepper::step()
{
  Timer t;
  for (int ii = 0; ii < nSteps; ii++){
    t.start();
    int ret = oneStep();
    t.end();
    float duration = t.getSeconds();
    std::cout << ii << " time: " << duration << "\n";
    if (ret < 0){
      return ret;
    }
    else if (ret==1)
    {
      std::cout << "Failed!" << std::endl;
      return ret;
    }
  }
  return 0;
}
