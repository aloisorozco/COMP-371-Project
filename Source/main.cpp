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

// --- Global variables ---
// Identity matrix
glm::mat4 iMat = glm::mat4(1.0f);

// Light angle
float lightAngle = 0.0f;

// Upper arm (model --> upArm acts as the root of the model)
glm::vec3 upArmScale = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 upArmPosition = glm::vec3(0.0f, 0.0f, 0.0f);
float upArmYAngle1 = 0.0f;
float upArmYAngle2 = 0.0f;
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
float lowArmXAngle = 0.0f;

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

// Toggles
int toggleTexture = 1;
int toggleShadows = 1;
bool toggleDefaultLight = true;
bool toggleSpotlight = false;
bool toggleRadialLight = false;
bool useRadialCamera;
bool toggleGrid = false;

// Camera info
int m = 0;
float worldXAngle;
float worldYAngle;
glm::vec3 cameraPosition;
glm::vec3 cameraLookAt;
glm::vec3 radialCameraPosition;
glm::vec3 radialCameraLookAt;
float cameraHorizontalAngle = 90.0f;
float cameraVerticalAngle = -35.0f;
float theta = glm::radians(cameraHorizontalAngle);
float phi = glm::radians(cameraVerticalAngle);

// Temperature
int fall = -1;

// Tennis ball variables
glm::vec3 sphereAcceleration = glm::vec3(0);
glm::vec3 sphereVelocity = glm::vec3(0);
glm::vec3 spherePosition = glm::vec3(8.5f, 22.0f, 30.0f)
;
float sphereInitialYVelocity = 0.0f;

bool shouldRotateSphere = true;
bool isHittingNet = false;
int sphereBounceAfterHittingNetCount = 0;


// Rendering of model
GLenum renderModeModel = GL_TRIANGLES;
GLenum renderModeRacketGrid = GL_LINES;

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

// Defining that there is no texture used
void noTexture(int sceneShaderProgram) {
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

// Set an alpha value for the blending of textures
void setBlend(int sceneShaderProgram, float alphaBlending) {
    glUseProgram(sceneShaderProgram);
    GLuint alphaBlendLocation = glGetUniformLocation(sceneShaderProgram, "blendAlpha");
    glUniform1f(alphaBlendLocation, alphaBlending);
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
#endif

    // Compiling and linking shaders here
    sceneShaderProgram = compileAndLinkShaders("../Assets/Shaders/vertexSceneShader.glsl", "../Assets/Shaders/fragmentSceneShader.glsl");
    shadowShaderProgram = compileAndLinkShaders("../Assets/Shaders/vertexShadowShader.glsl", "../Assets/Shaders/fragmentShadowShader.glsl");

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
    radialCameraPosition = glm::vec3(-30.0f, 10.0f, 0.0f);
    radialCameraLookAt = glm::vec3(0.0f, 0.0f, 0.0f); // at origin of world
    glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);

    // Initializing the camera angle
    cameraLookAt = glm::vec3(cosf(phi) * cosf(theta), sinf(phi), -cosf(phi) * sinf(theta));

    // World matrix information (only changing the world's rotation, hence these variables)
    worldYAngle = 0.0f;
    worldXAngle = 0.0f;

    // Light position
    float lightPosX = 30.0f;
    float lightPosY = 83.0f;
    float lightPosZ = 1.0f;

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

    // --- CREATING VAOs ---
    // Creation of the vertex array objects
    int gridVao = createVertexArrayObject(gridArray, sizeof(gridArray));
    int racketGridVao = createVertexArrayObject(racketGridArray, sizeof(racketGridArray));
    int netGridVao = createVertexArrayObject(netGridArray, sizeof(netGridArray));

    // Creation of the VAOs with light and texture effects
    int cubeVao = createLightTexturedVertexArrayObject(cubeArray, sizeof(cubeArray));
    int cubeVaoRepeat = createLightTexturedVertexArrayObject(cubeArrayRepeat, sizeof(cubeArrayRepeat));

    std::vector<LightTexturedColoredVertex> vertices = generateSphereVertices(2.0f, 30, 30);
    std::vector<int> tennisIndices = generateSphereIndices(30, 30);
    std::vector<int> snowIndices = generateSphereIndices(10, 10);
    int sphereVao = createSphereVertexArrayObject(vertices.data(), vertices.size() * sizeof(LightTexturedColoredVertex), tennisIndices.data(), tennisIndices.size());

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

        if (useRadialCamera) {
            // Set initial view matrix for shader when using camera
            viewMatrix = glm::lookAt(radialCameraPosition,    // eye
                radialCameraLookAt,                      // center
                cameraUp);                              // up

            setViewMatrix(sceneShaderProgram, viewMatrix);

            glUseProgram(sceneShaderProgram);
            glUniform3fv(glGetUniformLocation(sceneShaderProgram, "view_position"), 1, value_ptr(radialCameraPosition));
            glUseProgram(0);
        }
        else {
            // Set initial view matrix for shader when using radialCamera
            viewMatrix = glm::lookAt(cameraPosition,    // eye
                cameraPosition + cameraLookAt,                    // center
                cameraUp);                                        // up

            setViewMatrix(sceneShaderProgram, viewMatrix);

            glUseProgram(sceneShaderProgram);
            glUniform3fv(glGetUniformLocation(sceneShaderProgram, "view_position"), 1, value_ptr(cameraPosition));
            glUseProgram(0);
        }

        // Initialize particles
        for (loop = 0; loop < MAX_PARTICLES; loop++) {
            initParticles(loop);
        }

        // Light parameters for default light
        glm::vec3 lightPosition = glm::vec3(lightPosX, lightPosY, lightPosZ);
        glm::vec3 lightFocus(0.0f, 0.0f, -1.0f);
        glm::vec3 lightDirection = normalize(lightFocus - lightPosition);

        float lightNearPlane = 0.01f;
        float lightFarPlane = 400.0f;

        glm::mat4 lightProjMatrix = glm::ortho(-65.0f, 65.0f, -65.0f, 65.0f, lightNearPlane, lightFarPlane);
        glm::mat4 lightViewMatrix = glm::lookAt(lightPosition, lightFocus, glm::vec3(0.0f, 1.0f, 0.0f));

        glUseProgram(sceneShaderProgram);

        GLuint lightIntensityLocation = glGetUniformLocation(sceneShaderProgram, "light_color");
        if (toggleDefaultLight) {
            glUniform3fv(lightIntensityLocation, 1, value_ptr(glm::vec3(1.0f)));
        }
        else {
            glUniform3fv(lightIntensityLocation, 1, value_ptr(glm::vec3(0.3f)));
        }

        glUniformMatrix4fv(glGetUniformLocation(sceneShaderProgram, "light_proj_view_matrix"), 1, GL_FALSE, &(lightProjMatrix * lightViewMatrix)[0][0]);
        glUniform1i(glGetUniformLocation(sceneShaderProgram, "light_near_plane"), lightNearPlane);
        glUniform1i(glGetUniformLocation(sceneShaderProgram, "light_far_plane"), lightFarPlane);
        glUniform3fv(glGetUniformLocation(sceneShaderProgram, "light_position"), 1, value_ptr(lightPosition));
        glUniform3fv(glGetUniformLocation(sceneShaderProgram, "light_direction"), 1, value_ptr(lightDirection));
        glUniformMatrix4fv(glGetUniformLocation(sceneShaderProgram, "worldMatrix"), 1, GL_FALSE, &worldMatrix[0][0]);

        // Light parameters for spotlight
        glm::vec3 spotlightPosition = glm::vec3(0.0f, 10.0f, 30.0f); // the location of the light in 3D space: fixed position
        glm::vec3 spotlightFocus(0.0f, 0.0f, -1.0f);  // the point in 3D space the light "looks" at
        glm::vec3 spotlightDirection = normalize(spotlightFocus - spotlightPosition);

        glUseProgram(sceneShaderProgram);
        glUniform3fv(glGetUniformLocation(sceneShaderProgram, "spotlight_color"), 1, value_ptr(glm::vec3(1.0f)));
        glUniform3fv(glGetUniformLocation(sceneShaderProgram, "spotlight_position"), 1, value_ptr(spotlightPosition));
        glUniform3fv(glGetUniformLocation(sceneShaderProgram, "spotlight_direction"), 1, value_ptr(spotlightDirection));

        // Light parameters for radial light
        glm::vec3 radialLightPosition = radialCameraPosition;
        glm::vec3 radialLightDirection = normalize(glm::vec3(0.0f, -0.01f, 0.0f) - radialCameraPosition);

        glUniform3fv(glGetUniformLocation(sceneShaderProgram, "radial_light_color"), 1, value_ptr(glm::vec3(1.0f)));
        glUniform3fv(glGetUniformLocation(sceneShaderProgram, "radial_light_position"), 1, value_ptr(radialCameraPosition));
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
        drawModelShadow(worldMatrix, racketGridVao, cubeVao, shadowShaderProgram, racketPosition1, upArmXAngle1, upArmYAngle1);
        drawModelShadow(worldMatrix, racketGridVao, cubeVao, shadowShaderProgram, racketPosition2, upArmXAngle2, upArmYAngle2);
        // Court
        drawCourtShadow(worldMatrix, cubeVao, shadowShaderProgram);
        // Stadium
        drawStadiumShadow(worldMatrix, cubeVao, shadowShaderProgram);
        // Scoreboard
        drawScoreboardShadow(worldMatrix, cubeVao, shadowShaderProgram);
        // Net
        drawNetShadow(worldMatrix, netGridVao, cubeVao, shadowShaderProgram);
        // Sphere
        drawSphereShadow(worldMatrix, sphereVao, shadowShaderProgram, tennisIndices);
        // Unbind geometry
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
        // Court
        drawCourt(worldMatrix, clayTextureID, grassTextureID, cubeVao, cubeVaoRepeat, sceneShaderProgram);
        // Net
        drawNet(worldMatrix, netGridVao, cubeVao, sceneShaderProgram);
        // Stadium
        drawStadium(worldMatrix, cubeVao, sceneShaderProgram);
        // Scoreboard
        drawScoreboard(worldMatrix, cubeVao, sceneShaderProgram, woodTextureID);
        // Sphere
        drawSphere(worldMatrix, sphereVao, sceneShaderProgram, tennisIndices, tennisBallTextureID, racketPosition1, racketPosition2);
        // Light Cube
        drawLightCube(worldMatrix, sceneShaderProgram, cubeVao, lightPosition);
        // Sky Box
        if (toggleDefaultLight) {
            drawSkyBox(worldMatrix, sceneShaderProgram, cubeVao, skyTextureID);
        }
        else {
            drawSkyBox(worldMatrix, sceneShaderProgram, cubeVao, starsTextureID);
        }
        // Model 1
        drawModel(worldMatrix, racketColor1, racketTextureID, racketGridVao, cubeVao, sceneShaderProgram, racketPosition1, upArmXAngle1, upArmYAngle1);
        // Model 2
        drawModel(worldMatrix, racketColor2, racketTextureID, racketGridVao, cubeVao, sceneShaderProgram, racketPosition2, upArmXAngle2, upArmYAngle2);
        // Temperatures
        if (fall == 0) 
        {
            drawRain(worldMatrix, sceneShaderProgram, cubeVao);
        }
        else if (fall == 2)
        {
            drawSnow(worldMatrix, sphereVao, sceneShaderProgram, snowIndices, woodTextureID);
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
            upArmYAngle1 = 0.0f;
            upArmYAngle2 = 0.0f;
            lowArmXAngle = 0.0f;
            lowArmZAngle = 0.0f;
            wristXAngle = 0.0f;
            wristYAngle = 0.0f;
            wristZAngle = 0.0f;
            upArmScale = glm::vec3(1.0f);
        }

        // Rotate world up
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) // move up
        {
            worldXAngle -= 0.3f;
        }

        // Rotate world down
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) // move up
        {
            worldXAngle += 0.3f;
        }

        // Rotate world left
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) // move up
        {
            worldYAngle += 0.3f;
        }

        // Rotate world right
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) // move up
        {
            worldYAngle -= 0.3f;
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
            lightPosX += 0.5; 
        }
        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) // move left
        {
            lightPosX -= 0.5;
        }


        glfwSetKeyCallback(window, key_callback);

        // Change camera 
        glm::mat4 viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);

        setViewMatrix(sceneShaderProgram, viewMatrix);
    }

    // Shutdown GLFW
    glfwTerminate();

    return 0;
}
