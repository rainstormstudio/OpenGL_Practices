#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light {
public:
  Light();
  Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity);

  void useLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation);
  
  ~Light();
  
private:
  glm::vec3 color;
  GLfloat ambientIntensity;
};
