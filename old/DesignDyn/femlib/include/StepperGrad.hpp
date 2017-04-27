#ifndef STEPPERGRAD_HPP
#define STEPPERGRAD_HPP
#include "Stepper.hpp"
///@brief very slow example time stepper
class StepperGrad:public Stepper
{
public:
  StepperGrad();
  int oneStep();
  float h;
  ///@brief tolerance for sum of force length squared.
  float force_L2tol;
};
#endif