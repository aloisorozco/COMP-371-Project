#define GLEW_STATIC 1  
#include <GL/glew.h>  

#include <GLFW/glfw3.h> 
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

// --- Global variables --- 
// Upper arm (model --> upArm acts as the root of the model)
extern glm::vec3 upArmScale;
extern glm::vec3 upArmPosition;
extern float upArmYAngle1;
extern float upArmYAngle2;
extern float upArmXAngle1;
extern float upArmXAngle2;
extern int whichRacket;

// Model positions
extern glm::vec3 racketPosition1;
extern glm::vec3 racketPosition2;

// Lower arm
extern float lowArmZAngle;
extern float lowArmXAngle;

// Racket handle (wrist)
extern float wristXAngle;
extern float wristYAngle;
extern float wristZAngle;

// Rendering of model
extern GLenum renderModeModel;
extern GLenum renderModeRacketGrid;

// Score count
extern bool p1Scored;
extern bool p2Scored;

// Tennis ball variables
extern glm::vec3 sphereAcceleration;
extern glm::vec3 sphereVelocity;
extern glm::vec3 spherePosition;

extern float sphereInitialYVelocity;

extern bool shouldRotateSphere;
extern bool isHittingNet;

extern int sphereBounceAfterHittingNetCount;

extern glm::mat4 iMat;

extern float lightAngle;

extern struct LightTexturedColoredVertex;

extern int toggleTexture;
extern int toggleShadows;
extern bool toggleDefaultLight;
extern bool toggleSpotlight;
extern bool toggleRadialLight;
extern bool useRadialCamera;

extern int sceneShaderProgram;
extern int shadowShaderProgram;

// Camera info
extern int m;
extern float worldXAngle;
extern float worldYAngle;
extern glm::vec3 cameraPosition;
extern glm::vec3 cameraLookAt;
extern glm::vec3 radialCameraPosition;
extern glm::vec3 radialCameraLookAt;
extern float cameraHorizontalAngle;
extern float cameraVerticalAngle;
extern float theta;
extern float phi;

extern int fall;

// --- Global functions --- 
extern void setWorldMatrix(int shaderProgram, glm::mat4 worldMatrix);
extern void setProjectionMatrix(int shaderProgram, glm::mat4 worldMatrix);
extern void setUniqueColor(int shaderProgram, float colorX, float colorY, float colorZ);
extern void setTexture(int sceneShaderProgram, GLuint textureID, int textureUnit, int useTexture);
extern void noTexture(int sceneShaderProgram);
extern void setMaterial(int sceneShaderProgram, float ambientLight, float diffuseLight, float specularLight, float alpha, int useShadows);
extern void setBlend(int sceneShaderProgram, float alphaBlending);
extern void score(bool p1Scored, bool p2Scored);
extern void drawSphere(glm::mat4 worldMatrix, int sphereVao, int sceneShaderProgram, std::vector<int> indices, GLuint tennisBallTextureID, glm::vec3 racketPosition1, glm::vec3 racketPosition2);