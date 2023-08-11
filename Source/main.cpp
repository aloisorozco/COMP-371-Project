//
// COMP 371 Final Project
// Project of André Kasper Kolstad - 40210335
//            Mathieu La Brie - 40210809
//            Alois
//            Dom
//
// Framework created by Nicolas Bergeron on 20/06/2019.
//
// Inspired by the following tutorials:
// - https://learnopengl.com/Getting-started/Hello-Window
// - https://learnopengl.com/Getting-started/Hello-Triangle

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <irrKlang/include/irrKlang.h>
using namespace irrklang;

#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
                        // initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "common.h"
#include "draw.h"
#include "vertexArrays.h"
#include "keyCallBack.h"
#include "createVao.h"
#include "createParticles.h"
#include "loadShadersAndTextures.h"
#include "drawSphere.h"
#include "drawShadows.h"
#include "opponent_ai.h"

// --- Global variables ---
// Identity matrix
glm::mat4 iMat = glm::mat4(1.0f);

// Light angle
float lightAngle = 0.0f;

// Upper arm (model --> upArm acts as the root of the model)
glm::vec3 upArmScale = glm::vec3(0.5f);
glm::vec3 upArmPosition = glm::vec3(0.0f, 0.0f, 0.0f);
float upArmYAngle[] = { 0.0f, 0.0f };
float upArmXAngle1 = 0.0f;
float upArmXAngle2 = 0.0f;
int whichRacket;

// Model positions
glm::vec3 racketPosition1 = glm::vec3(-8.0f, 3.07f, -32.0f);
glm::vec3 racketPosition2 = glm::vec3(8.0f, 3.07f, 32.0f);

// Model colors
glm::vec3 racketColor1 = glm::vec3(0.835f, 0.192f, 0.278f);
glm::vec3 racketColor2 = glm::vec3(0.192f, 0.102f, 1.0f);

// Lower arm
float lowArmZAngle = 0.0f;
float lowArmXAngle[] = { 0.0f, 0.0f };

// Racket handle (wrist)
float wristXAngle = 0.0f;
float wristYAngle = 0.0f;
float wristZAngle = 0.0f;

// Score count
bool p1Scored;
bool p2Scored;

// Shader program
int sceneShaderProgram;
int shadowShaderProgram;
int objShaderProgram;

// Toggles
int toggleTexture = 1;
int toggleShadows = 1;
bool toggleDefaultLight = true;
bool toggleSpotlight = false;
bool toggleRadialLight = false;
bool toggleGrid = false;
bool useRadialCamera = true;
bool useCamera1;
bool toggleObj = true;

// Camera info
int m = 0;
float worldXAngle;
float worldYAngle;
glm::vec3 cameraPosition;
glm::vec3 cameraLookAt;
glm::vec3 cameraPosition1;
glm::vec3 cameraLookAtCenter;
glm::vec3 radialCameraPosition;
glm::vec3 radialCameraLookAt;
float cameraHorizontalAngle = 90.0f;
float cameraVerticalAngle = -35.0f;
float theta = glm::radians(cameraHorizontalAngle);
float phi = glm::radians(cameraVerticalAngle);

// Temperature
int fall = -1;

// Tennis ball variables
bool canStartPoint = true;
bool isP1sTurnToServe = true;
bool canStartRacketAnimation = false;
int playerRacketIndex = 1;
int racketDirectionMultiplier = 1;
float racketDirectionAngle = 0.0f;
int botRacketDirectionMultiplier = 1;
float botRacketDirectionAngle = 0.0f;

//Pi variable
float rotationAngle =0.0f;
float pi = (float)(M_PI);

// Rendering of model
GLenum renderModeModel = GL_TRIANGLES;
GLenum renderModeRacketGrid = GL_LINES;

// Sound engine
ISoundEngine* SoundEngine;

// Sounds
ISoundSource* musicSource;
ISoundSource* hitSource;
ISoundSource* shoeSource;
ISoundSource* shoe2Source;
ISoundSource* cheerSource;
ISoundSource* gameOverSimSource;
ISoundSource* gameOverSource;
ISoundSource* winSource;


// Setting projection matrix depending on shader program
void setProjectionMatrix(int shaderProgram, glm::mat4 projectionMatrix)
{
    glUseProgram(shaderProgram);
    GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
}

// Setting view matrix depending on shader program
void setViewMatrix(int shaderProgram, glm::mat4 viewMatrix)
{
    glUseProgram(shaderProgram);
    GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
}

// Setting world matrix depending on shader program
void setWorldMatrix(int shaderProgram, glm::mat4 worldMatrix)
{
    glUseProgram(shaderProgram);
    GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
}

// Setting a unique color to an object
void setUniqueColor(int shaderProgram, float colorX, float colorY, float colorZ)
{
    glUseProgram(shaderProgram);
    GLuint uniqueColorLocation = glGetUniformLocation(shaderProgram, "uniqueColor");
    glUniform3f(uniqueColorLocation, colorX, colorY, colorZ);
}

// Setting the texture of an object
void setTexture(int sceneShaderProgram, GLuint textureID, int isBlended, int useTexture)
{
    GLuint textureLocation = glGetUniformLocation(sceneShaderProgram, "textureSampler");
    glUniform1i(textureLocation, 1);

    GLuint isBlendedLocation = glGetUniformLocation(sceneShaderProgram, "isBlended");
    glUniform1i(isBlendedLocation, isBlended);

    GLuint useTextureLocation = glGetUniformLocation(sceneShaderProgram, "useTexture");
    glUniform1i(useTextureLocation, useTexture);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

// Setting textures for models in .obj files
void setObjTexture(int objShaderProgram, GLuint textureID, int useTexture)
{
    glUseProgram(objShaderProgram);
    GLuint textureLocation = glGetUniformLocation(objShaderProgram, "textureSampler");
    glUniform1i(textureLocation, 1);

    GLuint useTextureLocation = glGetUniformLocation(objShaderProgram, "useTexture");
    glUniform1i(useTextureLocation, useTexture);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUseProgram(0);
}

// Setting colour for models in .obj files
void setObjColor(int shaderProgram, vec3 color) 
{
    glUseProgram(shaderProgram);
    GLuint colorLocation = glGetUniformLocation(shaderProgram, "uniqueColor");
    glUniform3fv(colorLocation, 1, glm::value_ptr(color));
    glUseProgram(0);

}

// Defining that there is no texture used
void noTexture(int sceneShaderProgram) 
{
    GLuint useTextureLocation = glGetUniformLocation(sceneShaderProgram, "useTexture");
    glUniform1i(useTextureLocation, 0);
}

// Set the light properties, giving the illusion of a particular material
void setMaterial(int sceneShaderProgram, float ambientLight, float diffuseLight, float specularLight, float specularAlpha, int useShadows)
{
    glUseProgram(sceneShaderProgram);
    GLuint ambientLightLocation = glGetUniformLocation(sceneShaderProgram, "shading_ambient_strength");
    glUniform1f(ambientLightLocation, ambientLight);

    GLuint diffuseLightLocation = glGetUniformLocation(sceneShaderProgram, "shading_diffuse_strength");
    glUniform1f(diffuseLightLocation, diffuseLight);

    GLuint specularLightLocation = glGetUniformLocation(sceneShaderProgram, "shading_specular_strength");
    glUniform1f(specularLightLocation, specularLight);

    GLuint alphaLocation = glGetUniformLocation(sceneShaderProgram, "specularAlpha");
    glUniform1f(alphaLocation, specularAlpha);

    GLuint useShadowsLocation = glGetUniformLocation(sceneShaderProgram, "useShadows");
    glUniform1i(useShadowsLocation, useShadows);
}

// Set the light properties, giving the illusion of a particular material
void setObjMaterial(int objShaderProgram, float ambientLight, float diffuseLight, float specularLight, int useShadows)
{
    glUseProgram(objShaderProgram);
    GLuint ambientLightLocation = glGetUniformLocation(objShaderProgram, "shading_ambient_strength");
    glUniform1f(ambientLightLocation, ambientLight);

    GLuint diffuseLightLocation = glGetUniformLocation(objShaderProgram, "shading_diffuse_strength");
    glUniform1f(diffuseLightLocation, diffuseLight);

    GLuint specularLightLocation = glGetUniformLocation(objShaderProgram, "shading_specular_strength");
    glUniform1f(specularLightLocation, specularLight);

    GLuint useShadowsLocation = glGetUniformLocation(objShaderProgram, "useShadows");
    glUniform1i(useShadowsLocation, useShadows);
}

// Set an alpha value for the blending of textures
void setBlend(int sceneShaderProgram, float alphaBlending) {
    glUseProgram(sceneShaderProgram);
    GLuint alphaBlendLocation = glGetUniformLocation(sceneShaderProgram, "blendAlpha");
    glUniform1f(alphaBlendLocation, alphaBlending);
}

void setPositionX1(float xValue) {
    racketPosition1.x = xValue;
}
void setPositionX2(float xValue) {
    racketPosition2.x = xValue;
}

// Main function
int main(int argc, char* argv[])
{
    // Initialize GLFW and OpenGL version
    glfwInit();

#if defined(PLATFORM_OSX)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
    // On Windows, we set OpenGL version to 2.1, to support more hardware
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif

    // Create window and rendering context using GLFW, resolution is 1024x768
    GLFWwindow* window = glfwCreateWindow(1024, 768, "Comp371 - Final Project", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Sound engine
    SoundEngine = createIrrKlangDevice();


    // Start background music

    // Sound sources
    musicSource = SoundEngine->addSoundSourceFromFile("../Assets/Audio/homemadeSong.mp3");
    hitSource = SoundEngine->addSoundSourceFromFile("../Assets/Audio/hit.mp3");
    shoeSource = SoundEngine->addSoundSourceFromFile("../Assets/Audio/shoe.mp3");
    shoe2Source = SoundEngine->addSoundSourceFromFile("../Assets/Audio/shoe2.mp3");
    cheerSource = SoundEngine->addSoundSourceFromFile("../Assets/Audio/cheer.mp3");
    gameOverSimSource = SoundEngine->addSoundSourceFromFile("../Assets/Audio/gameoverSim.mp3");
    gameOverSource = SoundEngine->addSoundSourceFromFile("../Assets/Audio/gameover.mp3");
    winSource = SoundEngine->addSoundSourceFromFile("../Assets/Audio/win.mp3");

    // Setting volumes
    musicSource->setDefaultVolume(0.2f);
    hitSource->setDefaultVolume(0.2f);
    shoeSource->setDefaultVolume(0.1f);
    shoe2Source->setDefaultVolume(0.1f);
    cheerSource->setDefaultVolume(0.5f);
    gameOverSource->setDefaultVolume(0.5f);
    gameOverSimSource->setDefaultVolume(0.5f);
    winSource->setDefaultVolume(0.5f);


    // Start background music
    SoundEngine->play2D("../Assets/Audio/homemadeSong.mp3", true);

    // Load Textures
#if defined(PLATFORM_OSX)
    GLuint clayTextureID = loadTexture("../Assets/Textures/clay.jpg");
    GLuint racketTextureID = loadTexture("../Assets/Textures/racket.jpg");
    GLuint tennisBallTextureID = loadTexture("../Assets/Textures/tennisBall.jpg");
    GLuint skyTextureID = loadTexture("../Assets/Textures/sky.jpg");
    GLuint grassTextureID = loadTexture("../Assets/Textures/grass.jpg");
    GLuint metalTextureID = loadTexture("../Assets/Textures/metal.jpg");
    GLuint dirtyTextureID = loadTexture("../Assets/Textures/dirty.png");
    GLuint starsTextureID = loadTexture("../Assets/Textures/stars.jpg");
#else
    GLuint clayTextureID = loadTexture("../Assets/Textures/clay.jpg");
    GLuint racketTextureID = loadTexture("../Assets/Textures/racket.jpg");
    GLuint tennisBallTextureID = loadTexture("../Assets/Textures/tennisBall.jpg");
    GLuint skyTextureID = loadTexture("../Assets/Textures/sky.jpg");
    GLuint grassTextureID = loadTexture("../Assets/Textures/grass.jpg");
    GLuint starsTextureID = loadTexture("../Assets/Textures/stars.jpg");
    GLuint woodTextureID = loadTexture("../Assets/Textures/wood.jpg");
    GLuint cloudyTextureID = loadTexture("../Assets/Textures/cloudy.jpg");
    GLuint standTextureID = loadTexture("../Assets/Textures/stand2.jpg");
    GLuint wallTextureID = loadTexture("../Assets/Textures/wall.jpg");
    GLuint metalTextureID = loadTexture("../Assets/Textures/metal.jpg");
    GLuint moonTextureID = loadTexture("../Assets/Textures/moon.jpg");
    GLuint sunTextureID = loadTexture("../Assets/Textures/sun.jpg");
    GLuint leavesCubicTextureID = loadTexture("../Assets/Textures/leaves.jpg");
    GLuint trunkCubicTextureID = loadTexture("../Assets/Textures/trunk.jpg");
    GLuint leafTextureID = loadTexture("../Assets/Textures/leaves3.jpg");
    GLuint barkTextureID = loadTexture("../Assets/Textures/bark2.jpg");
#endif

    // Compiling and linking shaders here
    sceneShaderProgram = compileAndLinkShaders("../Assets/Shaders/vertexSceneShader.glsl", "../Assets/Shaders/fragmentSceneShader.glsl");
    shadowShaderProgram = compileAndLinkShaders("../Assets/Shaders/vertexShadowShader.glsl", "../Assets/Shaders/fragmentShadowShader.glsl");
    objShaderProgram = compileAndLinkShaders("../Assets/Shaders/scene_vertex.glsl", "../Assets/Shaders/scene_fragment.glsl");

    // Register the window resize callback function
    glfwSetFramebufferSizeCallback(window, windowResizeCallback);

    // Light blue background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Dimensions of the shadow texture
    const unsigned int DEPTH_MAP_TEXTURE_SIZE = 3072;
    GLuint depth_map_texture;
    // Get the texture
    glGenTextures(1, &depth_map_texture);
    // Bind the texture so the next glTex calls affect it
    glBindTexture(GL_TEXTURE_2D, depth_map_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, DEPTH_MAP_TEXTURE_SIZE, DEPTH_MAP_TEXTURE_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    // Set texture sampler parameters.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Variable storing index to framebuffer used for shadow mapping
    GLuint depth_map_fbo;
    // Get the framebuffer
    glGenFramebuffers(1, &depth_map_fbo);
    // Bind the framebuffer so the next glFramebuffer calls affect it
    glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_map_texture, 0);
    glDrawBuffer(GL_NONE);  // disable rendering colors, only write depth values

    // Camera parameters for view transform
    cameraPosition = glm::vec3(0.0f, 20.0f, 30.0f);
    cameraLookAt = glm::vec3(0.0f, 0.0f, 0.0f); // at origin of world
    cameraPosition1 = glm::vec3(0.0f);
    cameraLookAtCenter = glm::vec3(0.0f);
    radialCameraPosition = glm::vec3(0.0f, 70.0f, 30.0f);
    radialCameraLookAt = glm::vec3(0.0f, 0.0f, 0.0f); // at origin of world
    glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);

    // Initializing the camera angle
    cameraLookAt = glm::vec3(cosf(phi) * cosf(theta), sinf(phi), -cosf(phi) * sinf(theta));

    // World matrix information (only changing the world's rotation, hence these variables)
    worldYAngle = 0.0f;
    worldXAngle = 0.0f;

    // Light position
    vec3 lightPositionSun = vec3(30.0f, 83.0f, 1.0f);

    // Frame time
    float lastFrameTime = glfwGetTime();
    int lastMouseLeftState = GLFW_RELEASE;
    double lastMousePosX, lastMousePosY;
    glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);

    // Building of the grid
    glm::vec3 gridArray[808];
    for (int i = 0; i <= 800; i = i + 8) {

        // X plane
        gridArray[i] = glm::vec3(-1.0f + (i / 400.0), 0.0f, 1.0f);
        gridArray[i + 1] = glm::vec3(1.0f, 1.0f, 1.0f); // color
        gridArray[i + 2] = glm::vec3(-1.0f + (i / 400.0), 0.0f, -1.0f);
        gridArray[i + 3] = glm::vec3(1.0f, 1.0f, 1.0f); // color, repeated

        // Z plane
        gridArray[i + 4] = glm::vec3(1.0f, 0.0f, -1.0f + (i / 400.0));
        gridArray[i + 5] = glm::vec3(1.0f, 1.0f, 1.0f); // color, repeated
        gridArray[i + 6] = glm::vec3(-1.0f, 0.0f, -1.0f + (i / 400.0));
        gridArray[i + 7] = glm::vec3(1.0f, 1.0f, 1.0f); // color, repeated
    }

    // Building net grid
    glm::vec3 netGridArray[208];
    for (int i = 0; i <= 160; i += 4) {
        // X plane
        netGridArray[i] = glm::vec3(-1.0f + (i / 80.0f), 1.0f, 0.0f);
        netGridArray[i + 1] = glm::vec3(0.22f, 0.22f, 0.22f); // color
        netGridArray[i + 2] = glm::vec3(-1.0f + (i / 80.0f), -1.0f, 0.0f);
        netGridArray[i + 3] = glm::vec3(0.22f, 0.22f, 0.22f); // color, repeated
    }

    for (int i = 164; i <= 204; i += 4) {
        // Y plane
        netGridArray[i] = glm::vec3(1.0f, -1.0f + ((i - 164) / 20.0f), 0.0f);
        netGridArray[i + 1] = glm::vec3(0.22f, 0.22f, 0.22f); // color, repeated
        netGridArray[i + 2] = glm::vec3(-1.0f, -1.0f + ((i - 164) / 20.0f), 0.0f);
        netGridArray[i + 3] = glm::vec3(0.22f, 0.22f, 0.22f); // color, repeated
    }

    // --- SETTING UP MVP MATRIX ---
    // Instantiating the global world matrix
    glm::mat4 worldMatrix = glm::mat4(1.0f);
    glm::mat4 viewMatrix;


    // Set projection matrix for shader
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(70.0f),   // field of view in degrees
        1024.0f / 768.0f,                                                // aspect ratio
        0.01f, 400.0f);                                                  // near and far (near > 0)

    setProjectionMatrix(sceneShaderProgram, projectionMatrix);
    setProjectionMatrix(objShaderProgram, projectionMatrix);
    
    // --- CREATING VAOs ---
    // Creation of the vertex array objects
    int gridVao = createVertexArrayObject(gridArray, sizeof(gridArray));
    int racketGridVao = createVertexArrayObject(racketGridArray, sizeof(racketGridArray));
    int netGridVao = createVertexArrayObject(netGridArray, sizeof(netGridArray));

    // Creation of the VAOs with light and texture effects
    int cubeVao = createLightTexturedVertexArrayObject(cubeArray, sizeof(cubeArray));
    int cubeVaoRepeat = createLightTexturedVertexArrayObject(cubeArrayRepeat, sizeof(cubeArrayRepeat));

    std::vector<LightTexturedColoredVertex> vertices = generateSphereVertices(1.0f, 30, 30);
    std::vector<int> indices = generateSphereIndices(30, 30);
    int sphereVao = createSphereVertexArrayObject(vertices.data(), vertices.size() * sizeof(LightTexturedColoredVertex), indices.data(), indices.size());

    //Creation of models for the crowd
    // Source: https://skfb.ly/otLQE
    string modelPath = "../Assets/Models/matt.obj";                 //model file path
    int modelVertices;                                              //vertices in model
    GLuint modelVAO = createModelEBO(modelPath, modelVertices);     //creation of VAO

    //Creation of tree models
    //Source: https://free3d.com/3d-model/low-poly-tree-v1-146606.html
    string treePath = "../Assets/Models/LowPoly_Tree_v1.obj";
    int treeVertices;
    GLuint treeVAO = createModelEBO(treePath, treeVertices);

    // Enabling culling and depth test
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    // --- ENTERING MAIN LOOP ---
    while (!glfwWindowShouldClose(window))
    {
        // Getting time
        float dt = glfwGetTime() - lastFrameTime;
        lastFrameTime += dt;

        // Each frame, reset color and depth of each pixel to glClearColor and glClearDepth
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Creating a global world rotation/sacle matrix, which will be applied in world matrix
        glm::mat4 rotationYMatrix = glm::rotate(iMat, glm::radians(worldYAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 rotationXMatrix = glm::rotate(iMat, glm::radians(worldXAngle), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 scaleMatrix = glm::scale(iMat, glm::vec3(0.4f, 0.4f, 0.4f));
        worldMatrix = rotationXMatrix * rotationYMatrix * scaleMatrix;

        glUseProgram(sceneShaderProgram);

        if (useRadialCamera) {
            // Set initial view matrix for shader when using camera
            viewMatrix = glm::lookAt(radialCameraPosition,    // eye
                radialCameraLookAt,                      // center
                cameraUp);                              // up

            setViewMatrix(sceneShaderProgram, viewMatrix);
            setViewMatrix(objShaderProgram, viewMatrix);

            glUseProgram(sceneShaderProgram);
            glUniform3fv(glGetUniformLocation(sceneShaderProgram, "view_position"), 1, value_ptr(radialCameraPosition));
            glUseProgram(objShaderProgram);
            glUniform3fv(glGetUniformLocation(objShaderProgram, "view_position"), 1, value_ptr(radialCameraPosition));
            glUseProgram(0);
            glUseProgram(objShaderProgram);
            glUniform3fv(glGetUniformLocation(objShaderProgram, "view_position"), 1, value_ptr(radialCameraPosition));
            glUseProgram(0);

            glm::mat4 cameraRotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(dt * 1.8f), glm::vec3(0.0f, 1.0f, 0.0f));
            radialCameraPosition = glm::vec3(cameraRotationMatrix * glm::vec4(radialCameraPosition, 1.0f));
        }
        else if (useCamera1) {
            // Set initial view matrix for shader when using camera
            viewMatrix = glm::lookAt(cameraPosition1,    // eye
                cameraLookAtCenter,                      // center
                cameraUp);                              // up

            setViewMatrix(sceneShaderProgram, viewMatrix);
            setViewMatrix(objShaderProgram, viewMatrix);

            glUseProgram(sceneShaderProgram);
            glUniform3fv(glGetUniformLocation(sceneShaderProgram, "view_position"), 1, value_ptr(cameraPosition1));
            glUseProgram(objShaderProgram);
            glUniform3fv(glGetUniformLocation(objShaderProgram, "view_position"), 1, value_ptr(cameraPosition1));
        }
        else {
            // Set initial view matrix for shader when using radialCamera
            viewMatrix = glm::lookAt(cameraPosition,    // eye
                cameraPosition + cameraLookAt,                    // center
                cameraUp);                                        // up

            setViewMatrix(sceneShaderProgram, viewMatrix);
            setViewMatrix(objShaderProgram, viewMatrix);

            glUseProgram(sceneShaderProgram);
            glUniform3fv(glGetUniformLocation(sceneShaderProgram, "view_position"), 1, value_ptr(cameraPosition));
            glUseProgram(objShaderProgram);
            glUniform3fv(glGetUniformLocation(objShaderProgram, "view_position"), 1, value_ptr(cameraPosition));
        }

        // Initialize particles
        for (loop = 0; loop < MAX_PARTICLES; loop++) {
            initParticles(loop);
        }

        // Light rotation calculations
        rotationAngle += dt * 0.2f;
        float sunDistance = 160.0f;  // Adjust this value to set the desired distance
        float sunX = sunDistance * cos(rotationAngle);
        float sunY = sunDistance * sin(rotationAngle);
        float moonX = sunDistance * cos(rotationAngle + (float)(M_PI));
        float moonY = sunDistance * sin(rotationAngle + (float)(M_PI));


        // Rotating the light
        glUseProgram(sceneShaderProgram);
        GLuint lightIntensityLocation = glGetUniformLocation(sceneShaderProgram, "light_color");
        if (rotationAngle > 2 * (float)(M_PI))
        {
            rotationAngle -= 2 * (float)(M_PI);
        }
        else if (rotationAngle < -2 * (float)(M_PI))
        {
            rotationAngle += 2 * (float)(M_PI);
        }

        // Toggle spotlights at night
        if (rotationAngle > (15 * (float)(M_PI))/ 18 || rotationAngle < (3 * (float)(M_PI)) / 18) {
            glUniform1i(glGetUniformLocation(sceneShaderProgram, "useSpotlight"), true);
            glUniform1i(glGetUniformLocation(sceneShaderProgram, "useRadialLight"), true);
        }
        else {
            glUniform1i(glGetUniformLocation(sceneShaderProgram, "useSpotlight"), false);
            glUniform1i(glGetUniformLocation(sceneShaderProgram, "useRadialLight"), false);
        }

        // Night and day
        vec3 light_intensity;
        if (rotationAngle > (float)(M_PI)) {
            toggleDefaultLight = false;
            light_intensity = vec3(clamp(-sin(rotationAngle), 0.0f, 0.2f), clamp(-sin(rotationAngle), 0.0f, 0.2f), clamp(-sin(rotationAngle), 0.0f, 0.2f));
            glUseProgram(sceneShaderProgram);
            glUniform3fv(lightIntensityLocation, 1, value_ptr(light_intensity));
            glUniform3fv(glGetUniformLocation(sceneShaderProgram, "day_vector"), 1, value_ptr(vec3(-sin(rotationAngle))));

            glUniform1i(glGetUniformLocation(sceneShaderProgram, "useDefaultLight"), false);  
            glUseProgram(objShaderProgram);
            glUniform3fv(glGetUniformLocation(objShaderProgram, "light_color"), 1, value_ptr(clamp(light_intensity, 0.2f, 1.0f)));
            glUseProgram(sceneShaderProgram);
        }
        else {
            vec3 day_vector = vec3(sin(rotationAngle));
            light_intensity = vec3(clamp(sin(rotationAngle) * 1.4f, 0.0f, 1.0f), sin(rotationAngle), sin(rotationAngle));

            toggleDefaultLight = true;
            glUseProgram(sceneShaderProgram);
            glUniform3fv(lightIntensityLocation, 1, value_ptr(light_intensity));
            glUniform3fv(glGetUniformLocation(sceneShaderProgram, "day_vector"), 1, value_ptr(vec3(sin(rotationAngle))));
            glUniform1i(glGetUniformLocation(sceneShaderProgram, "useDefaultLight"), true);

            glUseProgram(objShaderProgram);
            glUniform3fv(glGetUniformLocation(objShaderProgram, "light_color"), 1, value_ptr(clamp(light_intensity, 0.2f, 1.0f)));
            glUseProgram(sceneShaderProgram);
        }

        // Create sun matrix with translation and rotation
        mat4 sun = glm::translate(glm::mat4(1.0f), glm::vec3(sunX, sunY, 0.0f));
        sun = glm::rotate(sun, rotationAngle, glm::vec3(0.0f, 0.0f, 1.0f));
        vec3 lightPositionSun = glm::vec3(sun[3]);

        // Light parameters for default light
        glm::vec3 lightFocus(0.0f, 0.0f, -1.0f);
        glm::vec3 lightDirection = normalize(lightFocus - lightPositionSun);


        // Create moon matrix with translation and rotation
        mat4 moon = glm::translate(glm::mat4(1.0f), glm::vec3(moonX, moonY, 0.0f));
        moon = glm::rotate(moon, (float)(rotationAngle + M_PI), glm::vec3(0.0f, 0.0f, 1.0f));
        vec3 lightPositionMoon = glm::vec3(moon[3]);

        vec3 moonDirection = normalize(lightFocus - lightPositionMoon);

        float lightNearPlane = 0.01f;
        float lightFarPlane = 400.0f;

        glm::mat4 lightProjMatrix = glm::ortho(-80.0f, 80.0f, -80.0f, 80.0f, lightNearPlane, lightFarPlane);
        glm::mat4 lightViewMatrix = glm::lookAt(lightPositionSun, lightFocus, glm::vec3(0.0f, 1.0f, 0.0f));

        // Light matrices
        if (toggleDefaultLight) {
            lightViewMatrix = glm::lookAt(lightPositionSun, lightFocus, glm::vec3(0.0f, 1.0f, 0.0f));
            glUseProgram(sceneShaderProgram);
            glUniform3fv(glGetUniformLocation(sceneShaderProgram, "light_position"), 1, value_ptr(lightPositionSun));
            glUniform3fv(glGetUniformLocation(sceneShaderProgram, "light_direction"), 1, value_ptr(lightDirection));

            glUseProgram(objShaderProgram);
            glUniform3fv(glGetUniformLocation(objShaderProgram, "light_position"), 1, value_ptr(lightPositionSun));
            glUniform3fv(glGetUniformLocation(objShaderProgram, "light_direction"), 1, value_ptr(lightDirection));
        }
        else {
            lightViewMatrix = glm::lookAt(lightPositionMoon, lightFocus, glm::vec3(0.0f, 1.0f, 0.0f));
            glUseProgram(sceneShaderProgram);
            glUniform3fv(glGetUniformLocation(sceneShaderProgram, "moon_position"), 1, value_ptr(lightPositionMoon));
            glUniform3fv(glGetUniformLocation(sceneShaderProgram, "moon_direction"), 1, value_ptr(moonDirection));

            glUseProgram(objShaderProgram);
            glUniform3fv(glGetUniformLocation(objShaderProgram, "moon_position"), 1, value_ptr(lightPositionMoon));
            glUniform3fv(glGetUniformLocation(objShaderProgram, "moon_direction"), 1, value_ptr(moonDirection));
        }
        glUseProgram(sceneShaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(sceneShaderProgram, "light_proj_view_matrix"), 1, GL_FALSE, &(lightProjMatrix* lightViewMatrix)[0][0]);
        glUniform1i(glGetUniformLocation(sceneShaderProgram, "light_near_plane"), lightNearPlane);
        glUniform1i(glGetUniformLocation(sceneShaderProgram, "light_far_plane"), lightFarPlane);
        glUniformMatrix4fv(glGetUniformLocation(sceneShaderProgram, "worldMatrix"), 1, GL_FALSE, &worldMatrix[0][0]);
        glUseProgram(objShaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(objShaderProgram, "light_proj_view_matrix"), 1, GL_FALSE, &(lightProjMatrix* lightViewMatrix)[0][0]);
        glUniform1i(glGetUniformLocation(objShaderProgram, "light_near_plane"), lightNearPlane);
        glUniform1i(glGetUniformLocation(objShaderProgram, "light_far_plane"), lightFarPlane);
        glUniformMatrix4fv(glGetUniformLocation(objShaderProgram, "worldMatrix"), 1, GL_FALSE, &worldMatrix[0][0]);


        // Light parameters for spotlights
        vec3 light_position = glm::vec3(80.0f, 80.0f, 60.0f); // the location of the light in 3D space: fixed position
        vec3 spotlightFocus = vec3(10.0f, 0.0f, 10.0f);  // the point in 3D space the light "looks" at
        vec3 spotlightDirection = normalize(spotlightFocus - light_position);

        glUseProgram(sceneShaderProgram);
        glUniform3fv(glGetUniformLocation(sceneShaderProgram, "spotlight_color"), 1, value_ptr(glm::vec3(1.0f)));
        glUniform3fv(glGetUniformLocation(sceneShaderProgram, "light_pos_1"), 1, value_ptr(light_position));
        glUniform3fv(glGetUniformLocation(sceneShaderProgram, "light_dir_1"), 1, value_ptr(spotlightDirection));

        light_position = glm::vec3(80.0f, 80.0f, -60.0f); // the location of the light in 3D space: fixed position
        spotlightFocus = vec3(10.0f, 0.0f, -10.0f);    // the point in 3D space the light "looks" at
        spotlightDirection = normalize(spotlightFocus - light_position);

        glUseProgram(sceneShaderProgram);
        glUniform3fv(glGetUniformLocation(sceneShaderProgram, "spotlight_color"), 1, value_ptr(glm::vec3(1.0f)));
        glUniform3fv(glGetUniformLocation(sceneShaderProgram, "light_pos_2"), 1, value_ptr(light_position));
        glUniform3fv(glGetUniformLocation(sceneShaderProgram, "light_dir_2"), 1, value_ptr(spotlightDirection));

        light_position = glm::vec3(-80.0f, 80.0f, -60.0f); // the location of the light in 3D space: fixed position
        spotlightFocus = vec3(-10.0f, 0.0f, -10.0f);    // the point in 3D space the light "looks" at
        spotlightDirection = normalize(spotlightFocus - light_position);

        glUseProgram(sceneShaderProgram);
        glUniform3fv(glGetUniformLocation(sceneShaderProgram, "spotlight_color"), 1, value_ptr(glm::vec3(1.0f)));
        glUniform3fv(glGetUniformLocation(sceneShaderProgram, "light_pos_3"), 1, value_ptr(light_position));
        glUniform3fv(glGetUniformLocation(sceneShaderProgram, "light_dir_3"), 1, value_ptr(spotlightDirection));

        light_position = glm::vec3(-80.0f, 80.0f, 60.0f); // the location of the light in 3D space: fixed position
        spotlightFocus = vec3(-10.0f, 0.0f, 10.0f);  // the point in 3D space the light "looks" at
        spotlightDirection = normalize(spotlightFocus - light_position);

        glUseProgram(sceneShaderProgram);
        glUniform3fv(glGetUniformLocation(sceneShaderProgram, "spotlight_color"), 1, value_ptr(glm::vec3(1.0f)));
        glUniform3fv(glGetUniformLocation(sceneShaderProgram, "light_pos_4"), 1, value_ptr(light_position));
        glUniform3fv(glGetUniformLocation(sceneShaderProgram, "light_dir_4"), 1, value_ptr(spotlightDirection));

        // Light parameters for radial light
        glm::vec3 radialLightPosition = glm::vec3(0.0f, 10.0f, 50.0f);
        glm::vec3 radialLightDirection = normalize(glm::vec3(0.0f, 20.0f, -100.0f) - radialLightPosition);

        glUniform3fv(glGetUniformLocation(sceneShaderProgram, "radial_light_color"), 1, value_ptr(glm::vec3(1.0f)));
        glUniform3fv(glGetUniformLocation(sceneShaderProgram, "radial_light_position"), 1, value_ptr(radialLightPosition));
        glUniform3fv(glGetUniformLocation(sceneShaderProgram, "radial_light_direction"), 1, value_ptr(radialLightDirection));
        glUseProgram(0);

        // Shadow parameters
        glUseProgram(shadowShaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(shadowShaderProgram, "lightProjection"), 1, GL_FALSE, &lightProjMatrix[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shadowShaderProgram, "lightView"), 1, GL_FALSE, &lightViewMatrix[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shadowShaderProgram, "worldMatrix"), 1, GL_FALSE, &worldMatrix[0][0]);


        // RENDER SHADOWS
        // Use proper shader
        glUseProgram(shadowShaderProgram);
        // Use proper image output size
        glViewport(0, 0, DEPTH_MAP_TEXTURE_SIZE, DEPTH_MAP_TEXTURE_SIZE);
        // Bind depth map texture as output framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
        // Clear depth data on the framebuffer
        glClear(GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        //// Grid and axis
        if (toggleGrid)
            drawGridAndAxisShadow(worldMatrix, cubeVao, gridVao, shadowShaderProgram);
        // Models
        drawModelShadow(worldMatrix, racketGridVao, cubeVao, shadowShaderProgram, racketPosition1, upArmXAngle1, 0);
        drawModelShadow(worldMatrix, racketGridVao, cubeVao, shadowShaderProgram, racketPosition2, upArmXAngle2, 1);
        // Court
        drawCourtShadow(worldMatrix, cubeVao, shadowShaderProgram);
        // Stadium
        drawStadiumShadow(worldMatrix, cubeVao, shadowShaderProgram);
        // Scoreboard
        drawScoreboardShadow(worldMatrix, cubeVao, shadowShaderProgram);
        // Net
        drawNetShadow(worldMatrix, netGridVao, cubeVao, shadowShaderProgram);
        // Sphere
        drawSphereShadow(worldMatrix, sphereVao, shadowShaderProgram, indices);
        
        if (toggleObj) {
            //Crowd
            drawCrowdShadows(worldMatrix, modelVAO, modelVertices, shadowShaderProgram);
            // Ball boys
            drawBallBoyShadows(worldMatrix, modelVAO, modelVertices, shadowShaderProgram);
            // Trees
            drawTreesShadow(worldMatrix, treeVAO, treeVertices, shadowShaderProgram);
        }
        else {
            // Trees
            drawTreesCubicShadow(worldMatrix, cubeVao, shadowShaderProgram);
        }
        glBindVertexArray(0);


        // RENDER SCENE
        // Use proper shader
        glUseProgram(sceneShaderProgram);
        // Use proper image output size
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        // Bind screen as output framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // Clear color and depth data on framebuffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Grid and axis
        if (toggleGrid)
            drawGridAndAxis(worldMatrix, cubeVao, gridVao, sceneShaderProgram);
        // Sky Box
        if (toggleDefaultLight) {
            if (fall == 0)
            {
                drawRain(worldMatrix, sceneShaderProgram, cubeVao);
                drawSkyBox(worldMatrix, sceneShaderProgram, sphereVao, cloudyTextureID, indices);
                glUniform3fv(glGetUniformLocation(sceneShaderProgram, "light_color"), 1, value_ptr(light_intensity * 0.7f));
            }
            else {
                drawSkyBox(worldMatrix, sceneShaderProgram, sphereVao, skyTextureID, indices);
                glUniform3fv(glGetUniformLocation(sceneShaderProgram, "light_color"), 1, value_ptr(glm::vec3(light_intensity)));
            }
        }
        else {
            drawSkyBox(worldMatrix, sceneShaderProgram, sphereVao, starsTextureID, indices);
        }
        // Court
        drawCourt(worldMatrix, clayTextureID, grassTextureID, cubeVao, cubeVaoRepeat, sceneShaderProgram);
        // Net
        drawNet(worldMatrix, netGridVao, cubeVao, sceneShaderProgram);
        // Stadium
        drawStadium(worldMatrix, cubeVao, cubeVaoRepeat, sceneShaderProgram, standTextureID, wallTextureID);
        // Scoreboard
        drawScoreboard(worldMatrix, cubeVao, sceneShaderProgram, woodTextureID);
        // Lights
        drawLights(worldMatrix, cubeVao, sceneShaderProgram, metalTextureID);
        // Sphere
        drawSphere(worldMatrix, sphereVao, sceneShaderProgram, indices, tennisBallTextureID, racketPosition1, racketPosition2);
        // Sun
        drawLightSphere(worldMatrix, sceneShaderProgram, sphereVao, lightPositionSun, indices, true, sunTextureID);
        // Moon
        drawLightSphere(worldMatrix, sceneShaderProgram, sphereVao, lightPositionMoon, indices, false, moonTextureID);
        // Model 1
        drawModel(worldMatrix, racketColor1, racketTextureID, racketGridVao, cubeVao, sceneShaderProgram, racketPosition1, upArmXAngle1, 0);
        // Model 2
        drawModel(worldMatrix, racketColor2, racketTextureID, racketGridVao, cubeVao, sceneShaderProgram, racketPosition2, upArmXAngle2, 1);

        if (toggleObj) {
            // Crowd
            drawCrowd(worldMatrix, modelVAO, modelVertices, objShaderProgram);
            // Ball Boys
            drawBallBoys(worldMatrix, modelVAO, modelVertices, objShaderProgram);
            //Trees
            drawTrees(worldMatrix, treeVAO, treeVertices, objShaderProgram, barkTextureID, leafTextureID);
        }
        else {
            // Trees
            drawTreesCubic(worldMatrix, cubeVao, sceneShaderProgram, trunkCubicTextureID, leavesCubicTextureID);
        }

        // Unbind geometry
        glBindVertexArray(0);

        // End frame
        glfwSwapBuffers(window);

        // Detect inputs
        glfwPollEvents();

        // Mouse movement variables
        double mousePosX, mousePosY;
        glfwGetCursorPos(window, &mousePosX, &mousePosY);
        double dx = mousePosX - lastMousePosX;
        double dy = mousePosY - lastMousePosY;
        lastMousePosX = mousePosX;
        lastMousePosY = mousePosY;
        const float cameraAngularSpeed = 40.0f;

        // -- EVENTS NOT IN KEY CALLBACK --
        // Quit the window
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Back to original world position and orientation, as well as cameraPosition/angles
        if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS)
        {
            // Changing world back to default
            worldXAngle = 0.0f;
            worldYAngle = 0.0f;
            rotationXMatrix = glm::rotate(iMat, glm::radians(worldXAngle), glm::vec3(1.0f, 0.0f, 0.0f));
            rotationYMatrix = glm::rotate(iMat, glm::radians(worldYAngle), glm::vec3(0.0f, 1.0f, 0.0f));

            // Re-setting angles to default
            theta = glm::radians(90.0f);
            phi = glm::radians(-35.0f);
            cameraLookAt = glm::vec3(cosf(phi) * cosf(theta), sinf(phi), -cosf(phi) * sinf(theta));

            // Re-setting camera zoom to default
            cameraPosition = glm::vec3(0.0f, 20.0f, 30.0f);
        }

        // Back to original model position
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        {
            upArmPosition = glm::vec3(0.0f);
            upArmXAngle1 = 0.0f;
            upArmXAngle2 = 0.0f;
            upArmYAngle[0] = 0.0f;
            upArmYAngle[1] = 0.0f;
            lowArmXAngle[0] = 0.0f;
            lowArmXAngle[1] = 0.0f;
            lowArmZAngle = 0.0f;
            wristXAngle = 0.0f;
            wristYAngle = 0.0f;
            wristZAngle = 0.0f;
            upArmScale = glm::vec3(1.0f);
        }

        // Pan the camera on the X axis
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
            cameraHorizontalAngle -= dx * cameraAngularSpeed * dt;

            if (cameraHorizontalAngle > 360)
            {
                cameraHorizontalAngle -= 360;
            }
            else if (cameraHorizontalAngle < -360)
            {
                cameraHorizontalAngle += 360;
            }

            theta = glm::radians(cameraHorizontalAngle);
            cameraLookAt = glm::vec3(cosf(phi) * cosf(theta), sinf(phi), -cosf(phi) * sinf(theta));
        }

        // Tilt the camera on the Y axis
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
        {
            cameraVerticalAngle -= dy * cameraAngularSpeed * dt;

            // Clamp vertical angle to [-85, 85] degrees
            cameraVerticalAngle = std::max(-85.0f, std::min(85.0f, cameraVerticalAngle));

            phi = glm::radians(cameraVerticalAngle);
            cameraLookAt = glm::vec3(cosf(phi) * cosf(theta), sinf(phi), -cosf(phi) * sinf(theta));
        }

        // Zoom in or out
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            cameraPosition = cameraPosition + normalize(cameraLookAt) * (glm::vec3(dx) * glm::vec3(0.2));
        }

        // Light translations
        if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) // move right
        {
            lightPositionSun.x += 0.5;
        }
        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) // move left
        {
            lightPositionSun.x -= 0.5;
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            if (!isSimulation) {
                setPositionX2(racketPosition2.x - 0.5f);

                if (spherePosition.z < -0.1f || sphereVelocity.z < 0.0f) {
                    racketDirectionMultiplier = -1;
                    racketDirectionAngle = 180.0f;
                }
            }
        }

        // Rotate model to the right
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            if (!isSimulation) {
                setPositionX2(racketPosition2.x + 0.5f);

                if (spherePosition.z < -0.1f || sphereVelocity.z < 0.0f) {
                    racketDirectionMultiplier = 1;
                    racketDirectionAngle = 0.0f;
                }
            }
        }

        glfwSetKeyCallback(window, key_callback);

        // Change camera
        glm::mat4 viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);

        setViewMatrix(sceneShaderProgram, viewMatrix);
        setViewMatrix(objShaderProgram, viewMatrix);
    }

    SoundEngine->drop();
    // Shutdown GLFW
    glfwTerminate();

    return 0;
}
