#include "Light.h"

Light::Light() {
  color = glm::vec3(1.0f, 1.0f, 1.0f);
  ambientIntensity = 1.0f;
  diffuseIntensity = 0.0f;
}

Light::Light(GLfloat shadowWidth, GLfloat shadowHeight, GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity) {
  shadowMap = new ShadowMap();
  shadowMap->init(shadowWidth, shadowHeight);
  
  color = glm::vec3(red, green, blue);
  ambientIntensity = aIntensity;
  diffuseIntensity = dIntensity;
}

Light::~Light() {
  
}
