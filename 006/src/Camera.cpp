#include "Camera.h"

Camera::Camera() {
  
}

Camera::Camera(glm::vec3 initPosition, glm::vec3 initUp, GLfloat initYaw, GLfloat initPitch, GLfloat initMoveSpeed, GLfloat initTurnSpeed) {
  position = initPosition;
  worldUp = initUp;
  yaw = initYaw;
  pitch = initPitch;
  front = glm::vec3(0.0f, 0.0f, -1.0f);

  moveSpeed = initMoveSpeed;
  turnSpeed = initTurnSpeed;

  update();
}

void Camera::keyControl(bool *keys) {
  if (keys[GLFW_KEY_W]) {
    position += front * moveSpeed;
  }
  if (keys[GLFW_KEY_S]) {
    position -= front * moveSpeed;
  }
  if (keys[GLFW_KEY_A]) {
    position -= right * moveSpeed;
  }
  if (keys[GLFW_KEY_D]) {
    position += right * moveSpeed;
  }
}

glm::mat4 Camera::calculateView() {
  return glm::lookAt(position, position + front, up);
}

void Camera::update() {
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  front = glm::normalize(front);

  right = glm::normalize(glm::cross(front, worldUp));
  up = glm::normalize(glm::cross(right, front));
}

Camera::~Camera() {
  
}
