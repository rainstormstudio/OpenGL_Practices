#pragma once

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

class Shader {
public:
  Shader();

  void createFromString(const char *vertexCode, const char *fragmentCode);
  GLuint getProjectionLocation();
  GLuint getModelLocation();

  void useShader();
  void clearShader();

  ~Shader();
private:
  GLuint shaderID, uniformProjection, uniformModel;

  void compileShader(const char *vertexCode, const char *fragmentCode);
  void addShader(GLuint theProgram, const char *shaderCode, GLenum shaderType);
};
