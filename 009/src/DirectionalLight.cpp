#include "DirectionalLight.h"
#include <glm/gtc/matrix_transform.hpp>

DirectionalLight::DirectionalLight() : Light() {
  direction = glm::vec3(0.0f, -1.0f, 0.0f);
  lightProj = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 20.0f);
}

DirectionalLight::DirectionalLight(GLfloat shadowWidth, GLfloat shadowHeight,
				   GLfloat red, GLfloat green, GLfloat blue,
				   GLfloat aIntensity, GLfloat dIntensity,
				   GLfloat xDir, GLfloat yDir, GLfloat zDir)
  : Light(shadowWidth, shadowHeight, red, green, blue, aIntensity, dIntensity) {
  direction = glm::vec3(xDir, yDir, zDir);
  lightProj = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 20.0f);
}

void DirectionalLight::useLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
		     GLuint diffuseIntensityLocation, GLuint directionLocation) {
  glUniform3f(ambientColorLocation, color.x, color.y, color.z);
  glUniform1f(ambientIntensityLocation, ambientIntensity);

  glUniform3f(directionLocation, direction.x, direction.y, direction.z);
  glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}

glm::mat4 DirectionalLight::calculateLightTransform() {
  return lightProj * glm::lookAt(-direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

DirectionalLight::~DirectionalLight() {
  
}
