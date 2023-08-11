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
extern float upArmYAngle[];
extern float upArmXAngle1;
extern float upArmXAngle2;
extern int whichRacket;

// Model positions
extern glm::vec3 racketPosition1;
extern glm::vec3 racketPosition2;

// Lower arm
extern float lowArmZAngle;
extern float lowArmXAngle[];

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
extern bool canStartPoint;
extern bool isP1sTurnToServe;
extern bool canStartRacketAnimation;
extern int playerRacketIndex;
extern int racketDirectionMultiplier;
extern float racketDirectionAngle;
extern int botRacketDirectionMultiplier;
extern float botRacketDirectionAngle;

extern glm::mat4 iMat;

extern float lightAngle;

extern struct LightTexturedColoredVertex;

extern int toggleTexture;
extern int toggleShadows;
extern bool toggleDefaultLight;
extern bool toggleSpotlight;
extern bool toggleRadialLight;
extern bool useRadialCamera;
extern bool isSimulation;
extern bool useCamera1;

extern int sceneShaderProgram;
extern int shadowShaderProgram;
extern int objShaderProgram;

// Camera info
extern int m;
extern float worldXAngle;
extern float worldYAngle;
extern glm::vec3 cameraPosition;
extern glm::vec3 cameraLookAt;
extern glm::vec3 cameraPosition1;
extern glm::vec3 cameraLookAtCenter;
extern glm::vec3 radialCameraPosition;
extern glm::vec3 radialCameraLookAt;
extern float cameraHorizontalAngle;
extern float cameraVerticalAngle;
extern float theta;
extern float phi;

//Move speed
extern float botSpeed;

extern int fall;

// --- Global functions --- 
extern void setWorldMatrix(int shaderProgram, glm::mat4 worldMatrix);
extern void setProjectionMatrix(int shaderProgram, glm::mat4 worldMatrix);
extern void setUniqueColor(int shaderProgram, float colorX, float colorY, float colorZ);
extern void setTexture(int sceneShaderProgram, GLuint textureID, int textureUnit, int useTexture);
extern void setObjTexture(int sceneShaderProgram, GLuint textureID, int useTexture);
extern void setObjColor(int shaderProgram, glm::vec3 color);
extern void noTexture(int sceneShaderProgram);
extern void setMaterial(int sceneShaderProgram, float ambientLight, float diffuseLight, float specularLight, float alpha, int useShadows);
extern void setBlend(int sceneShaderProgram, float alphaBlending);
extern void score(bool p1Scored, bool p2Scored);
extern void startPoint();