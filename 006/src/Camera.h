#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Camera {
public:
  Camera();
  Camera(glm::vec3 initPosition, glm::vec3 initUp, GLfloat initYaw, GLfloat initPitch, GLfloat initMoveSpeed, GLfloat initTurnSpeed);

  void keyControl(bool *keys);

  glm::mat4 calculateView();
  
  ~Camera();
  
private:
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 worldUp;

  GLfloat yaw;
  GLfloat pitch;

  GLfloat moveSpeed;
  GLfloat turnSpeed;

  void update();
};
