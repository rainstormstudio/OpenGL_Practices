#define STB_IMAGE_IMPLREMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "constants.h"
#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"

// Window dimensions
const float toRadians = 3.1415926f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

Texture brickTexture;
Texture clayTexture;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
GLfloat lastFrameTime = 0.0f;

// Vertex Shader
static const char *vShader = "shaders/basics.vsh";

// Fragment Shader
static const char *fShader = "shaders/basics.fsh";

void createObjects() {
  unsigned int indices[] = {
			    0, 3, 1,
			    1, 3, 2,
			    2, 3, 0,
			    0, 1, 2
  };
  GLfloat vertices[] = {
			// x     y     z        u     v
			-1.0f, -1.0f, 0.0f,    0.0f, 0.0f, 
			0.0f, -1.0f, 1.0f,     0.5f, 0.0f,
			1.0f, -1.0f, 0.0f,     1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,      0.5f, 1.0f
  };
  unsigned int cubeIndices[] = {
				0, 1, 2,
				2, 1, 3,
				3, 2, 6,
				6, 3, 7,
				7, 6, 5,
				5, 6, 4,
				4, 5, 1,
				1, 4, 0,

				9, 11, 15,
				15, 9, 13,
				8, 10, 14,
				14, 8, 12
  };
  GLfloat cubeVertices[] = {
			    -1.0, -1.0,  1.0,    0.0f, 0.0f, 
			    -1.0, 1.0,  1.0,     0.0f, 1.0f,
			    1.0,  -1.0,  1.0,    1.0f, 0.0f,
			    1.0,  1.0,  1.0,     1.0f, 1.0f,
			    -1.0, -1.0, -1.0,    1.0f, 0.0f,
			    -1.0, 1.0, -1.0,     1.0f, 1.0f,
			    1.0,  -1.0, -1.0,    0.0f, 0.0f,
			    1.0,  1.0, -1.0,     0.0f, 1.0f,

			    -1.0, -1.0,  1.0,    0.0f, 1.0f, 
			    -1.0, 1.0,  1.0,     0.0f, 0.0f,
			    1.0,  -1.0,  1.0,    1.0f, 1.0f,
			    1.0,  1.0,  1.0,     1.0f, 0.0f,
			    -1.0, -1.0, -1.0,    0.0f, 0.0f,
			    -1.0, 1.0, -1.0,     0.0f, 1.0f,
			    1.0,  -1.0, -1.0,    1.0f, 0.0f,
			    1.0,  1.0, -1.0,     1.0f, 1.0f,
			    };

  Mesh *obj1 = new Mesh();
  obj1->createMesh(vertices, indices, 20, 12);
  meshList.push_back(obj1);

  Mesh *obj2 = new Mesh();
  obj2->createMesh(vertices, indices, 20, 12);
  meshList.push_back(obj2);

  Mesh *cube = new Mesh();
  cube->createMesh(cubeVertices, cubeIndices, 80, 36);
  meshList.push_back(cube);
}

void createShaders() {
  Shader *shader1 = new Shader();
  shader1->createFromFiles(vShader, fShader);
  shaderList.push_back(*shader1);
}

int main(int argc, char *argv[])
{
  mainWindow = Window(SCREEN_WIDTH, SCREEN_HEIGHT);
  mainWindow.initialize();

  createObjects();
  createShaders();

  camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.1f);

  brickTexture = Texture("textures/brick.png");
  brickTexture.loadTexture();
  clayTexture = Texture("textures/clay.png");
  clayTexture.loadTexture();
  
  GLuint uniformProjection = 0;
  GLuint uniformModel = 0;
  GLuint uniformView = 0;
  glm::mat4 projection = glm::perspective(45.0f,
					  mainWindow.getBufferWidth() /
					  mainWindow.getBufferHeight(),
					  0.1f, 100.0f);

  // loop until window closed
  while (!mainWindow.getShouldClose()) {
    GLfloat currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;
    
    // Get and handle user input events
    glfwPollEvents();

    camera.keyControl(mainWindow.getKeys(), deltaTime);
    camera.mouseControl(mainWindow.getXchange(), mainWindow.getYchange());

    // clear window
    glClearColor(0.0f, 0.6f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderList[0].useShader();
    uniformModel = shaderList[0].getModelLocation();
    uniformProjection = shaderList[0].getProjectionLocation();
    uniformView = shaderList[0].getViewLocation();
    
    glm::mat4 model(1.0);

    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
    // model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateView()));
    brickTexture.useTexture();
    meshList[0]->renderMesh();

    model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
    // model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    clayTexture.useTexture();
    meshList[1]->renderMesh();

    model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    brickTexture.useTexture();
    meshList[2]->renderMesh();
    
    glUseProgram(0);

    if (currentTime - lastFrameTime >= 1.0f / FPS) {
      mainWindow.swapBuffers();
      lastFrameTime = currentTime;
    }
    lastTime = currentTime;
  }
  
  return 0;
}
