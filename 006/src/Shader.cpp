#include "Shader.h"

Shader::Shader() {
  shaderID = 0;
  uniformModel = 0;
  uniformProjection = 0;
}

void Shader::createFromString(const char *vertexCode, const char *fragmentCode) {
  compileShader(vertexCode, fragmentCode);
}

void Shader::compileShader(const char *vertexCode, const char *fragmentCode) {
  shaderID = glCreateProgram();

  if (!shaderID) {
    printf("Error creating shader program!\n");
    return;
  }

  addShader(shaderID, vertexCode, GL_VERTEX_SHADER);
  addShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

  GLint result = 0;
  GLchar eLog[1024] = {0};

  glLinkProgram(shaderID);
  glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
  if (!result) {
    glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
    printf("Error linking program: '%s'\n", eLog);
    return;
  }

  glValidateProgram(shaderID);
  glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
  if (!result) {
    glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
    printf("Error validating program: '%s'\n", eLog);
    return;
  }

  uniformModel = glGetUniformLocation(shaderID, "model");
  uniformProjection = glGetUniformLocation(shaderID, "projection");
}

void Shader::addShader(GLuint theProgram, const char *shaderCode, GLenum shaderType) {
  GLuint theShader = glCreateShader(shaderType);

  const GLchar *theCode[1];
  theCode[0] = shaderCode;

  GLint codeLength[1];
  codeLength[0] = strlen(shaderCode);

  glShaderSource(theShader, 1, theCode, codeLength);
  glCompileShader(theShader);

  
  GLint result = 0;
  GLchar eLog[1024] = {0};

  glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
  if (!result) {
    glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
    printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
    return;
  }

  glAttachShader(theProgram, theShader);
}

GLuint Shader::getModelLocation() {
  return uniformModel;
}

GLuint Shader::getProjectionLocation() {
  return uniformProjection;
}

void Shader::useShader() {
  glUseProgram(shaderID);
}

void Shader::clearShader() {
  if (shaderID != 0) {
    glDeleteProgram(shaderID);
    shaderID = 0;
  }

  uniformModel = 0;
  uniformProjection = 0;
}

Shader::~Shader() {
  clearShader();
}
