#pragma once

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

#include "DirectionalLight.h"
#include "PointLight.h"

class Shader {
public:
  Shader();

  void createFromString(const char *vertexCode, const char *fragmentCode);
  void createFromFiles(const char *vertexLocation, const char *fragmentLocation);

  std::string readFile(const char *fileLocation);
  
  GLuint getProjectionLocation();
  GLuint getModelLocation();
  GLuint getViewLocation();
  GLuint getEyePosition();
  GLuint getAmbientIntensityLocation();
  GLuint getAmbientColorLocation();
  GLuint getDiffuseIntensityLocation();
  GLuint getDirectionLocation();
  GLuint getSpecularIntensityLocation();
  GLuint getShininessLocation();

  void setDirectionalLight(DirectionalLight *dLight);

  void useShader();
  void clearShader();

  ~Shader();
private:
  int pointLightCount;
  
  GLuint shaderID, uniformProjection, uniformModel, uniformView, uniformEyePosition, 
    uniformSpecularIntensity, uniformShininess;

  struct {
    GLuint uniformColor;
    GLuint uniformAmbientIntensity;
    GLuint uniformDiffuseIntensity;
    GLuint uniformDirection;
  } uniformDirectionalLight;

  void compileShader(const char *vertexCode, const char *fragmentCode);
  void addShader(GLuint theProgram, const char *shaderCode, GLenum shaderType);
};
