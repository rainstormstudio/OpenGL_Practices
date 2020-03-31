#include "Timer.h"

Timer::Timer() {
  lastTime = system_clock::now();
  deltaTime = 0.0f;
  currentTime = system_clock::now();
}

void Timer::update() {
  currentTime = system_clock::now();
  duration<double, std::milli> delta = currentTime - lastTime;
  deltaTime = delta.count() / 1000.0f;
  lastTime = currentTime;
}

double Timer::getDeltaTime() {
  return deltaTime;
}

void Timer::sleep(int milli) {
  std::this_thread::sleep_for(milliseconds(milli));
}

void Timer::sleep(double sec) {
  int temp = round(sec * 1000.0f);
  std::this_thread::sleep_for(milliseconds(temp));
}

double Timer::frameControl() {
  currentTime = system_clock::now();
  if (deltaTime < 1.0f / FPS) {
    sleep(1.0f / FPS - deltaTime);
  }
  lastTime = currentTime;
  currentTime = system_clock::now();
  duration<double, std::milli> delta = currentTime - lastTime;  
  sleepTime = delta.count() / 1000.0f;
  return sleepTime;
}

double Timer::currentFPS() {
  return (1.0f / (deltaTime + sleepTime));
}

Timer::~Timer() {
  
}
