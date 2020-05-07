#pragma once
#include <GL/glew.h>
#include "stb_image.h"

class Texture {
public:
  Texture();
  Texture(const char *fileLoc);

  bool loadTexture();
  bool loadTextureAlpha();
  void useTexture();
  void clearTexture();
  
  ~Texture();

private:
  GLuint textureID;
  int width, height, bitDepth;
  const char *fileLocation;
};
