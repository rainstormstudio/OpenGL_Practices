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
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "Model.h"
//#include "Timer.h"

// Window dimensions
const float toRadians = 3.1415926f / 180.0f;

Window mainWindow;
//Timer *timer;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

Texture brickTexture;
Texture steelTexture;
Texture concreteTexture;
Texture floorTexture;

Material shinyMaterial;
Material dullMaterial;

Model tie_fighter;
Model x_wing;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
GLfloat lastFrameTime = 0.0f;

// Vertex Shader
static const char *vShader = "shaders/basics.vsh";

// Fragment Shader
static const char *fShader = "shaders/basics.fsh";

void calcAverageNormals(unsigned int *indices, unsigned int indiceCount, GLfloat *vertices,
			unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset) {
  for (size_t i = 0; i < indiceCount; i += 3) {
    unsigned int in0 = indices[i + 0] * vLength;
    unsigned int in1 = indices[i + 1] * vLength;
    unsigned int in2 = indices[i + 2] * vLength;
    glm::vec3 v1(vertices[in1 + 0] - vertices[in0 + 0],
		 vertices[in1 + 1] - vertices[in0 + 1],
		 vertices[in1 + 2] - vertices[in0 + 2]);
    glm::vec3 v2(vertices[in2 + 0] - vertices[in0 + 0],
		 vertices[in2 + 1] - vertices[in0 + 1],
		 vertices[in2 + 2] - vertices[in0 + 2]);
    glm::vec3 normal = cross(v1, v2);
    normal = glm::normalize(normal);

    in0 += normalOffset;
    in1 += normalOffset;
    in2 += normalOffset;
    vertices[in0 + 0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
    vertices[in1 + 0] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
    vertices[in2 + 0] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
  }
  for (size_t i = 0; i < verticeCount / vLength; i ++) {
    unsigned int nOffset = i * vLength + normalOffset;
    glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
    vec = glm::normalize(vec);
    vertices[nOffset + 0] = vec.x;
    vertices[nOffset + 1] = vec.y;
    vertices[nOffset + 2] = vec.z;
  }
}

void createObjects() {
  unsigned int floorIndices[] = {
				 0, 2, 1,
				 1, 2, 3
  };
  GLfloat floorVertices[] = {
			     -20.0f, 0.0f, -20.0f,   0.0f, 0.0f,    0.0f, -1.0f, 0.0f,
			     20.0f, 0.0f, -20.0f,    20.0f, 0.0f,   0.0f, -1.0f, 0.0f,
			     -20.0f, 0.0f, 20.0f,    0.0f, 20.0f,   0.0f, -1.0f, 0.0f,
			     20.0f, 0.0f, 20.0f,     20.0f, 20.0f,  0.0f, -1.0f, 0.0f
  };
  
  unsigned int indices[] = {
			    0, 3, 1,
			    1, 3, 2,
			    2, 3, 0,
			    0, 1, 2
  };
  GLfloat vertices[] = {
			// x     y     z        u     v       nx    ny    nz
			-1.0f, -1.0f, -0.6f,    0.0f, 0.0f,   0.0f, 0.0f, 0.0f, 
			0.0f, -1.0f, 1.0f,      0.5f, 0.0f,   0.0f, 0.0f, 0.0f, 
			1.0f, -1.0f, -0.6f,     1.0f, 0.0f,   0.0f, 0.0f, 0.0f, 
			0.0f, 1.0f, 0.0f,       0.5f, 1.0f,   0.0f, 0.0f, 0.0f
  };
  calcAverageNormals(indices, 12, vertices, 32, 8, 5);
  
  unsigned int cubeIndices[] = {
				0, 1, 2,
				2, 1, 3,
				6, 7, 5,
				6, 5, 4,

				9, 15, 11,
				15, 9, 13,
				8, 10, 14,
				12, 8, 14,

				20, 17, 16,
				20, 21, 17,
				18, 19, 22,
				22, 19, 23
  };
  GLfloat cubeVertices[] = {
			    -1.0, -1.0,  1.0,    0.0f, 0.0f,   0.0f, 0.0f, 0.0f, 
			    -1.0, 1.0,  1.0,     0.0f, 1.0f,   0.0f, 0.0f, 0.0f, 
			    1.0,  -1.0,  1.0,    1.0f, 0.0f,   0.0f, 0.0f, 0.0f, 
			    1.0,  1.0,  1.0,     1.0f, 1.0f,   0.0f, 0.0f, 0.0f, 
			    -1.0, -1.0, -1.0,    1.0f, 0.0f,   0.0f, 0.0f, 0.0f, 
			    -1.0, 1.0, -1.0,     1.0f, 1.0f,   0.0f, 0.0f, 0.0f, 
			    1.0,  -1.0, -1.0,    0.0f, 0.0f,   0.0f, 0.0f, 0.0f, 
			    1.0,  1.0, -1.0,     0.0f, 1.0f,   0.0f, 0.0f, 0.0f, 

			    -1.0, -1.0,  1.0,    0.0f, 1.0f,   0.0f, 0.0f, 0.0f, 
			    -1.0, 1.0,  1.0,     0.0f, 0.0f,   0.0f, 0.0f, 0.0f, 
			    1.0,  -1.0,  1.0,    1.0f, 1.0f,   0.0f, 0.0f, 0.0f, 
			    1.0,  1.0,  1.0,     1.0f, 0.0f,   0.0f, 0.0f, 0.0f, 
			    -1.0, -1.0, -1.0,    0.0f, 0.0f,   0.0f, 0.0f, 0.0f, 
			    -1.0, 1.0, -1.0,     0.0f, 1.0f,   0.0f, 0.0f, 0.0f, 
			    1.0,  -1.0, -1.0,    1.0f, 0.0f,   0.0f, 0.0f, 0.0f, 
			    1.0,  1.0, -1.0,     1.0f, 1.0f,   0.0f, 0.0f, 0.0f,
			    
			    -1.0, -1.0,  1.0,    0.0f, 0.0f,   0.0f, 0.0f, 0.0f, 
			    -1.0, 1.0,  1.0,     0.0f, 1.0f,   0.0f, 0.0f, 0.0f, 
			    1.0,  -1.0,  1.0,    1.0f, 0.0f,   0.0f, 0.0f, 0.0f, 
			    1.0,  1.0,  1.0,     1.0f, 1.0f,   0.0f, 0.0f, 0.0f, 
			    -1.0, -1.0, -1.0,    1.0f, 0.0f,   0.0f, 0.0f, 0.0f, 
			    -1.0, 1.0, -1.0,     1.0f, 1.0f,   0.0f, 0.0f, 0.0f, 
			    1.0,  -1.0, -1.0,    0.0f, 0.0f,   0.0f, 0.0f, 0.0f, 
			    1.0,  1.0, -1.0,     0.0f, 1.0f,   0.0f, 0.0f, 0.0f
  };
  calcAverageNormals(cubeIndices, 36, cubeVertices, 192, 8, 5);

  Mesh *obj0 = new Mesh();
  obj0->createMesh(floorVertices, floorIndices, 32, 6);
  meshList.push_back(obj0);
  
  Mesh *obj1 = new Mesh();
  obj1->createMesh(vertices, indices, 32, 12);
  meshList.push_back(obj1);

  Mesh *obj2 = new Mesh();
  obj2->createMesh(vertices, indices, 32, 12);
  meshList.push_back(obj2);
  
  Mesh *cube1 = new Mesh();
  cube1->createMesh(cubeVertices, cubeIndices, 192, 36);
  meshList.push_back(cube1);

  Mesh *cube2 = new Mesh();
  cube2->createMesh(cubeVertices, cubeIndices, 192, 36);
  meshList.push_back(cube2);

  Mesh *cube3 = new Mesh();
  cube3->createMesh(cubeVertices, cubeIndices, 192, 36);
  meshList.push_back(cube3);
  
  Mesh *cube4 = new Mesh();
  cube4->createMesh(cubeVertices, cubeIndices, 192, 36);
  meshList.push_back(cube4);
  
  Mesh *cube5 = new Mesh();
  cube5->createMesh(cubeVertices, cubeIndices, 192, 36);
  meshList.push_back(cube5);
  
  Mesh *cube6 = new Mesh();
  cube6->createMesh(cubeVertices, cubeIndices, 192, 36);
  meshList.push_back(cube6);
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
  //timer = new Timer();

  createObjects();
  createShaders();

  camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.1f);

  brickTexture = Texture("textures/brick.png");
  brickTexture.loadTextureAlpha();
  steelTexture = Texture("textures/steel.png");
  steelTexture.loadTextureAlpha();
  concreteTexture = Texture("textures/clay-pixel.png");
  concreteTexture.loadTextureAlpha();
  floorTexture = Texture("textures/floor.png");
  floorTexture.loadTextureAlpha();

  shinyMaterial = Material(4.0f, 256);
  dullMaterial = Material(0.3f, 4);

  tie_fighter = Model();
  tie_fighter.loadModel("models/TIE-fighter.obj");
  x_wing = Model();
  x_wing.loadModel("models/x-wing.obj");
  
  mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
  			       0.2f, 0.2f,
  			       2.0f, -1.0f, -2.0f);
  
  unsigned int pointLightCount = 0;
  pointLights[0] = PointLight(0.0f, 0.0f, 1.0f,
			      0.1f, 0.4f,
			      4.0f, 0.0f, 0.0f,
			      0.3f, 0.2f, 0.1f);
  pointLightCount ++;
  
  pointLights[1] = PointLight(0.0f, 1.0f, 0.0f,
			      0.1f, 1.0f,
			      -4.0f, 2.0f, 0.0f,
			      0.3f, 0.2f, 0.1f);
  pointLightCount ++;

  unsigned int spotLightCount = 0;
  spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
			    0.0f, 2.0f,
			    0.0f, 0.0f, 0.0f,
			    0.0f, -1.0f, 0.0f,
			    0.3f, 0.2f, 0.1f,
			    20.0f);
  spotLightCount ++;
  
  GLuint uniformProjection = 0;
  GLuint uniformModel = 0;
  GLuint uniformView = 0;
  GLuint uniformEyePosition = 0;
  GLuint uniformSpecularIntensity = 0;
  GLuint uniformShininess = 0;
  glm::mat4 projection = glm::perspective(45.0f,
					  mainWindow.getBufferWidth() /
					  mainWindow.getBufferHeight(),
					  0.1f, 100.0f);
  
  // loop until window closed
  while (!mainWindow.getShouldClose()) {
    GLfloat currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;
    //timer->update();
    //timer->frameControl();
    
    // Get and handle user input events
    glfwPollEvents();

    camera.keyControl(mainWindow.getKeys(), deltaTime);
    camera.mouseControl(mainWindow.getXchange(), mainWindow.getYchange());

    // clear window
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderList[0].useShader();
    uniformModel = shaderList[0].getModelLocation();
    uniformProjection = shaderList[0].getProjectionLocation();
    uniformView = shaderList[0].getViewLocation();
    uniformEyePosition = shaderList[0].getEyePositionLocation();
    uniformSpecularIntensity = shaderList[0].getSpecularIntensityLocation();
    uniformShininess = shaderList[0].getShininessLocation();

    glm::vec3 lowerLight = camera.getCameraPosition();
    lowerLight.y -= 0.3f;
    spotLights[0].setFlash(lowerLight, camera.getCameraDirection());
    
    shaderList[0].setDirectionalLight(&mainLight);
    shaderList[0].setPointLights(pointLights, pointLightCount);
    shaderList[0].setSpotLights(spotLights, spotLightCount);
    //    mainLight.useLight(uniformAmbientIntensity, uniformAmbientColor,
    //		       uniformDiffuseIntensity, uniformDirection);
    
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateView()));
    glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
    
    glm::mat4 model(1.0);
    
    model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    floorTexture.useTexture();
    dullMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
    meshList[0]->renderMesh();
    
    model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
    // model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    // model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    brickTexture.useTexture();
    dullMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
    // meshList[1]->renderMesh();

    model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));
    // model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    // model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));    
    concreteTexture.useTexture();
    shinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
    // meshList[2]->renderMesh();

    model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    brickTexture.useTexture();
    dullMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
    meshList[3]->renderMesh();

    model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, -10.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    brickTexture.useTexture();
    dullMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);    
    meshList[4]->renderMesh();

    model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    steelTexture.useTexture();
    shinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);    
    meshList[5]->renderMesh();

    model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(4.0f, 0.0f, -8.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    brickTexture.useTexture();
    dullMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);    
    meshList[6]->renderMesh();

    model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(4.0f, 0.0f, -6.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    brickTexture.useTexture();
    dullMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);    
    meshList[7]->renderMesh();

    model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(4.0f, 0.0f, -4.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    brickTexture.useTexture();
    dullMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);    
    meshList[8]->renderMesh();

    model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(-7.0f, 0.0f, 10.0f));
    model = glm::scale(model, glm::vec3(0.06f, 0.06f, 0.06f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    shinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
    x_wing.renderModel();

    glUseProgram(0);

    mainWindow.swapBuffers();
  }
  
  return 0;
}
