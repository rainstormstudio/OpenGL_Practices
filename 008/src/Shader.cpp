#include "Shader.h"

Shader::Shader() {
  shaderID = 0;
  uniformModel = 0;
  uniformProjection = 0;
}

void Shader::createFromString(const char *vertexCode, const char *fragmentCode) {
  compileShader(vertexCode, fragmentCode);
}

void Shader::createFromFiles(const char *vertexLocation, const char *fragmentLocation) {
  std::string vertexString = readFile(vertexLocation);
  std::string fragmentString = readFile(fragmentLocation);
  const char *vertexCode = vertexString.c_str();
  const char *fragmentCode = fragmentString.c_str();

  compileShader(vertexCode, fragmentCode);
}

std::string Shader::readFile(const char *fileLocation) {
  std::string content;
  std::ifstream fileStream(fileLocation, std::ios::in);

  if (!fileStream.is_open()) {
    printf("Failed to read %s! File doesn't exists.", fileLocation);
    return "";
  }

  std::string line = "";
  while (!fileStream.eof()) {
    std::getline(fileStream, line);
    content.append(line + "\n");
  }

  fileStream.close();
  return content;
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
  uniformView = glGetUniformLocation(shaderID, "view");
  uniformDirectionalLight.uniformColor = glGetUniformLocation(shaderID, "directionalLight.color");
  uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.ambientIntensity");
  uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
  uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.diffuseIntensity");
  uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
  uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
  uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");
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

GLuint Shader::getViewLocation() {
  return uniformView;
}

GLuint Shader::getEyePosition() {
  return uniformEyePosition;
}

GLuint Shader::getAmbientColorLocation() {
  return uniformDirectionalLight.uniformColor;
}

GLuint Shader::getAmbientIntensityLocation() {
  return uniformDirectionalLight.uniformAmbientIntensity;
}

GLuint Shader::getDiffuseIntensityLocation() {
  return uniformDirectionalLight.uniformDiffuseIntensity;
}

GLuint Shader::getDirectionLocation() {
  return uniformDirectionalLight.uniformDirection;
}

GLuint Shader::getSpecularIntensityLocation() {
  return uniformSpecularIntensity;
}

GLuint Shader::getShininessLocation() {
  return uniformShininess;
}

void Shader::setDirectionalLight(DirectionalLight *dLight) {
  dLight->useLight(uniformDirectionalLight.uniformAmbientIntensity,
		   uniformDirectionalLight.uniformColor,
		   uniformDirectionalLight.uniformDiffuseIntensity,
		   uniformDirectionalLight.uniformDirection);
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
