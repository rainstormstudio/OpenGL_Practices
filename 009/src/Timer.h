#pragma once
#include "constants.h"
#include <math.h>
#include <chrono>
#include <thread>
using namespace std::chrono;

class Timer {
private:
  double deltaTime;
  double sleepTime;
  system_clock::time_point lastTime;
  system_clock::time_point currentTime;
public:
  Timer();

  double getDeltaTime();
  void update();
  void sleep(int milli);
  void sleep(double sec);
  double frameControl();
  double currentFPS();
  
  ~Timer();
};
