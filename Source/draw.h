#define GLEW_STATIC 1  
#include <GL/glew.h>  

#include <GLFW/glfw3.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

#include "common.h"
#include "scoreCalculations.h"

using namespace glm;
using namespace std;


// --- DRAWING AXIS ---
void drawGridAndAxis(glm::mat4 worldMatrix, int cubeVao, int gridVao, int sceneShaderProgram)
{
    noTexture(sceneShaderProgram);

    // --- GRID ---
    // Grid model matrix
    glm::mat4 gridModelMatrix = glm::translate(iMat, glm::vec3(0.0f, -1.0f, 0.0f)) * glm::scale(iMat, glm::vec3(50.0f, 1.0f, 50.0f));
    gridModelMatrix = worldMatrix * gridModelMatrix;
    setWorldMatrix(sceneShaderProgram, gridModelMatrix);

    // Drawing the grid
    glBindVertexArray(gridVao);
    setUniqueColor(sceneShaderProgram, 1.0f, 1.0f, 0.286f); // For grid, set uniqueColor to neutral color (white)
    setMaterial(sceneShaderProgram, 0.7f, 0.0f, 0.0f, 1.0f, 0);
    glDrawArrays(GL_LINES, 0, 404);
    glBindVertexArray(0);

    // --- AXIS ---
    glBindVertexArray(cubeVao);

    // -- X --
    // X-axis cube model matrix
    glm::mat4 xCubeModelMatrix = glm::translate(iMat, glm::vec3(2.5f, 0.0f, 0.25f)) * glm::scale(iMat, glm::vec3(5.0f, 0.5f, 0.5f));
    xCubeModelMatrix = worldMatrix * xCubeModelMatrix;
    setWorldMatrix(sceneShaderProgram, xCubeModelMatrix);

    setMaterial(sceneShaderProgram, 0.3f, 0.6f, 0.3f, 20.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 1.0f, 0.0f, 0.0f); // Set color to red

    // Drawing the X-axis cube
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // -- Y --
    // Y-axis cube model matrix
    glm::mat4 yCubeModelMatrix = glm::translate(iMat, glm::vec3(0.25f, 2.25f, 0.25f)) * glm::scale(iMat, glm::vec3(0.5f, 5.0f, 0.5f));
    yCubeModelMatrix = worldMatrix * yCubeModelMatrix;
    setWorldMatrix(sceneShaderProgram, yCubeModelMatrix);

    setMaterial(sceneShaderProgram, 0.3f, 0.6f, 0.3f, 20.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 0.0f, 1.0f, 0.0f); // Set color to green

    // Drawing the Y-axis cube
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // -- Z --
    // Z-axis cube model matrix
    glm::mat4 zCubeModelMatrix = glm::translate(iMat, glm::vec3(0.25f, 0.0f, 2.5f)) * glm::scale(iMat, glm::vec3(0.5f, 0.5f, 5.0f));
    zCubeModelMatrix = worldMatrix * zCubeModelMatrix;
    setWorldMatrix(sceneShaderProgram, zCubeModelMatrix);

    setMaterial(sceneShaderProgram, 0.3f, 0.6f, 0.3f, 20.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 0.0f, 0.0f, 1.0f); // Set color to blue

    // Drawing the Z-axis cube
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

// --- DRAWING MAIN MODEL ---
void drawModel(glm::mat4 worldMatrix, glm::vec3 racketColor, int racketTextureID, int racketGridVao, int cubeVao, int sceneShaderProgram, glm::vec3 upArmInitialPosition, float upArmXAngle, float upArmYAngle)
{
    glBindVertexArray(cubeVao);
    noTexture(sceneShaderProgram);

    // -- UPPER ARM --
    // Upper arm cube model matrix
    glm::mat4 upperArmTranslate = glm::translate(iMat, upArmInitialPosition + upArmPosition);
    glm::mat4 upperArmScale = glm::scale(iMat, glm::vec3(1.536f, 6.144f, 1.536f) * upArmScale);
    glm::mat4 upperArmInitialRotation = glm::rotate(iMat, glm::radians(-30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 upperArmRotation = glm::rotate(iMat, glm::radians(upArmXAngle), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(iMat, glm::radians(upArmYAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 upperArmModelMatrix = worldMatrix * upperArmTranslate * upperArmRotation * upperArmInitialRotation * upperArmScale;
    glm::mat4 upperArmHierarchy = upperArmTranslate * upperArmRotation;
    setWorldMatrix(sceneShaderProgram, upperArmModelMatrix);

    setMaterial(sceneShaderProgram, 0.3f, 0.6f, 0.3f, 10.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 1.0f, 0.835f, 0.498f);

    // Drawing the upper arm model cube
    glDrawArrays(renderModeModel, 0, 36);

    // -- LOWER ARM --
    // Lower arm cube model matrix
    glm::mat4 lowerArmTranslate = glm::translate(iMat, glm::vec3(1.4336f, 5.2736f, 0.0f) * upArmScale);
    glm::mat4 lowerArmScale = glm::scale(iMat, glm::vec3(1.536f, 6.144f, 1.536f) * upArmScale);
    glm::mat4 lowerArmRotation = glm::translate(iMat, glm::vec3(0.0f, -3.07f, 0.0f) * upArmScale) * glm::rotate(iMat, glm::radians(lowArmZAngle), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::rotate(iMat, glm::radians(lowArmXAngle), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::translate(iMat, glm::vec3(0.0f, 3.07f, 0.0f) * upArmScale);
    glm::mat4 lowerArmModelMatrix = worldMatrix * upperArmHierarchy * lowerArmTranslate * lowerArmRotation * lowerArmScale;
    glm::mat4 lowerArmHierarchy = lowerArmTranslate * lowerArmRotation;
    setWorldMatrix(sceneShaderProgram, lowerArmModelMatrix);

    setMaterial(sceneShaderProgram, 0.3f, 0.6f, 0.3f, 10.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 1.0f, 0.835f, 0.498f);

    // Drawing the lower arm model cube
    glDrawArrays(renderModeModel, 0, 36);

    // -- RACKET
    glBindVertexArray(cubeVao);
    setBlend(sceneShaderProgram, 0.4f);

    // -- RACKET HANDLE --
    // Racket handle cube model matrix
    glm::mat4 racketHandleTranslate = glm::translate(iMat, glm::vec3(0.0f, 4.6364f, 0.0f) * upArmScale);
    glm::mat4 racketHandleScale = glm::scale(iMat, glm::vec3(0.8f, 4.12f, 0.8f) * upArmScale);
    glm::mat4 wristRotation = glm::translate(iMat, glm::vec3(0.0f, -3.07, 0.0f) * upArmScale) * glm::rotate(iMat, glm::radians(wristZAngle), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::rotate(iMat, glm::radians(wristYAngle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(iMat, glm::radians(wristXAngle), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::translate(iMat, glm::vec3(0.0f, 3.07f, 0.0f) * upArmScale);
    glm::mat4 racketHandleModelMatrix = worldMatrix * upperArmHierarchy * lowerArmHierarchy * racketHandleTranslate * wristRotation * racketHandleScale;
    glm::mat4 wristHierarchy = racketHandleTranslate * wristRotation;
    setWorldMatrix(sceneShaderProgram, racketHandleModelMatrix);

    setTexture(sceneShaderProgram, racketTextureID, 1, toggleTexture);
    setMaterial(sceneShaderProgram, 0.3f, 0.5f, 1.0f, 20.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, racketColor.x, racketColor.y, racketColor.z);

    // Drawing the racket handle model cube
    glDrawArrays(renderModeModel, 0, 36);

    // -- RACKET BOTTOM RIGHT --
    // Racket bottom right cube model matrix
    glm::mat4 racketBottomRightTranslate = glm::translate(iMat, glm::vec3(1.1f, 3.09f, 0.0f) * upArmScale);
    glm::mat4 racketBottomRightScale = glm::scale(iMat, glm::vec3(0.8f, 4.12f, 0.8f) * upArmScale);
    glm::mat4 racketBottomRightInitialRotation = glm::rotate(iMat, glm::radians(-40.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 racketBottomRightModelMatrix = worldMatrix * upperArmHierarchy * lowerArmHierarchy * wristHierarchy * racketBottomRightTranslate * racketBottomRightInitialRotation * racketBottomRightScale;
    setWorldMatrix(sceneShaderProgram, racketBottomRightModelMatrix);

    setTexture(sceneShaderProgram, racketTextureID, 1, toggleTexture);
    setMaterial(sceneShaderProgram, 0.3f, 0.5f, 1.0f, 20.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 1.0f, 1.0f, 1.0f);

    // Drawing the racket bottom model cube
    glDrawArrays(renderModeModel, 0, 36);

    // -- RACKET BOTTOM LEFT --
    // Racket bottom left cube model matrix
    glm::mat4 racketBottomLeftTranslate = glm::translate(iMat, glm::vec3(-1.1f, 3.09f, 0.0f) * upArmScale);
    glm::mat4 racketBottomLeftScale = glm::scale(iMat, glm::vec3(0.8f, 4.12f, 0.8f) * upArmScale);
    glm::mat4 racketBottomLeftInitialRotation = glm::rotate(iMat, glm::radians(40.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 racketBottomLeftModelMatrix = worldMatrix * upperArmHierarchy * lowerArmHierarchy * wristHierarchy * racketBottomLeftTranslate * racketBottomLeftInitialRotation * racketBottomLeftScale;
    setWorldMatrix(sceneShaderProgram, racketBottomLeftModelMatrix);

    setTexture(sceneShaderProgram, racketTextureID, 1, toggleTexture);
    setMaterial(sceneShaderProgram, 0.3f, 0.5f, 1.0f, 20.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 1.0f, 1.0f, 1.0f);

    // Drawing the racket bottom left model cube
    glDrawArrays(renderModeModel, 0, 36);

    // -- RACKET RIGHT --
    // Racket right cube model matrix
    glm::mat4 racketRightTranslate = glm::translate(iMat, glm::vec3(2.318f, 7.63f, 0.0f) * upArmScale);
    glm::mat4 racketRightScale = glm::scale(iMat, glm::vec3(0.8f, 6.42f, 0.8f) * upArmScale);
    glm::mat4 racketRightModelMatrix = worldMatrix * upperArmHierarchy * lowerArmHierarchy * wristHierarchy * racketRightTranslate * racketRightScale;
    setWorldMatrix(sceneShaderProgram, racketRightModelMatrix);

    setTexture(sceneShaderProgram, racketTextureID, 1, toggleTexture);
    setMaterial(sceneShaderProgram, 0.3f, 0.5f, 1.0f, 20.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, racketColor.x, racketColor.y, racketColor.z);

    // Drawing the racket right model cube
    glDrawArrays(renderModeModel, 0, 36);

    // -- RACKET LEFT --
    // Racket left cube model matrix
    glm::mat4 racketLeftTranslate = glm::translate(iMat, glm::vec3(-2.318f, 7.63f, 0.0f) * upArmScale);
    glm::mat4 racketLeftScale = glm::scale(iMat, glm::vec3(0.8f, 6.42f, 0.8f) * upArmScale);
    glm::mat4 racketLeftModelMatrix = worldMatrix * upperArmHierarchy * lowerArmHierarchy * wristHierarchy * racketLeftTranslate * racketLeftScale;
    setWorldMatrix(sceneShaderProgram, racketLeftModelMatrix);

    setTexture(sceneShaderProgram, racketTextureID, 1, toggleTexture);
    setMaterial(sceneShaderProgram, 0.3f, 0.5f, 1.0f, 20.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, racketColor.x, racketColor.y, racketColor.z);

    // Drawing the racket left model cube
    glDrawArrays(renderModeModel, 0, 36);

    // -- RACKET TOP RIGHT --
    // Racket top right cube model matrix
    glm::mat4 racketTopRightTranslate = glm::translate(iMat, glm::vec3(1.87f, 11.19f, 0.0f) * upArmScale);
    glm::mat4 racketTopRightScale = glm::scale(iMat, glm::vec3(0.8f, 1.83f, 0.8f) * upArmScale);
    glm::mat4 racketTopRightInitialRotation = glm::rotate(iMat, glm::radians(40.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 racketTopRightModelMatrix = worldMatrix * upperArmHierarchy * lowerArmHierarchy * wristHierarchy * racketTopRightTranslate * racketTopRightInitialRotation * racketTopRightScale;
    setWorldMatrix(sceneShaderProgram, racketTopRightModelMatrix);

    setTexture(sceneShaderProgram, racketTextureID, 1, toggleTexture);
    setMaterial(sceneShaderProgram, 0.3f, 0.5f, 1.0f, 20.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 1.0f, 1.0f, 1.0f);

    // Drawing the racket bottom model cube
    glDrawArrays(renderModeModel, 0, 36);

    // -- RACKET TOP LEFT --
    // Racket top left cube model matrix
    glm::mat4 racketTopLeftTranslate = glm::translate(iMat, glm::vec3(-1.87f, 11.19f, 0.0f) * upArmScale);
    glm::mat4 racketTopLeftScale = glm::scale(iMat, glm::vec3(0.8f, 1.83f, 0.8f) * upArmScale);
    glm::mat4 racketTopLeftInitialRotation = glm::rotate(iMat, glm::radians(-40.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 racketTopLeftModelMatrix = worldMatrix * upperArmHierarchy * lowerArmHierarchy * wristHierarchy * racketTopLeftTranslate * racketTopLeftInitialRotation * racketTopLeftScale;
    setWorldMatrix(sceneShaderProgram, racketTopLeftModelMatrix);

    setTexture(sceneShaderProgram, racketTextureID, 1, toggleTexture);

    // Changing the racket top left cube color
    setUniqueColor(sceneShaderProgram, 1.0f, 1.0f, 1.0f);
    setMaterial(sceneShaderProgram, 0.3f, 0.5f, 1.0f, 20.0f, toggleShadows);
    glDrawArrays(renderModeModel, 0, 36);

    // -- RACKET TOP --
    // Racket top cube model matrix
    glm::mat4 racketTopTranslate = glm::translate(iMat, glm::vec3(0.0f, 11.83f, 0.0f) * upArmScale);
    glm::mat4 racketTopScale = glm::scale(iMat, glm::vec3(3.15f, 0.8f, 0.8f) * upArmScale);
    glm::mat4 racketTopModelMatrix = worldMatrix * upperArmHierarchy * lowerArmHierarchy * wristHierarchy * racketTopTranslate * racketTopScale;
    setWorldMatrix(sceneShaderProgram, racketTopModelMatrix);

    setTexture(sceneShaderProgram, racketTextureID, 1, toggleTexture);
    setMaterial(sceneShaderProgram, 0.3f, 0.5f, 1.0f, 20.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, racketColor.x, racketColor.y, racketColor.z);

    // Drawing the racket top model cube
    glDrawArrays(renderModeModel, 0, 36);

    // -- RACKET MIDDLE --
    // Racket middle cube model matrix
    glm::mat4 racketMiddleTranslate = glm::translate(iMat, glm::vec3(0.0f, 3.73f, 0.0f) * upArmScale);
    glm::mat4 racketMiddleScale = glm::scale(iMat, glm::vec3(3.15f, 0.5f, 0.5f) * upArmScale);
    glm::mat4 racketMiddleModelMatrix = worldMatrix * upperArmHierarchy * lowerArmHierarchy * wristHierarchy * racketMiddleTranslate * racketMiddleScale;
    setWorldMatrix(sceneShaderProgram, racketMiddleModelMatrix);

    setTexture(sceneShaderProgram, racketTextureID, 1, toggleTexture);
    setMaterial(sceneShaderProgram, 0.3f, 0.5f, 1.0f, 20.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, racketColor.x, racketColor.y, racketColor.z);

    // Drawing the racket middle model cube
    glDrawArrays(renderModeModel, 0, 36);

    //Exiting cube drawing :(
    glBindVertexArray(0);

    noTexture(sceneShaderProgram);

    // -- RACKET GRID --
    // Racket grid matrix
    glm::mat4 racketGridTranslate = glm::translate(iMat, glm::vec3(0.0f, 1.73f, 0.0f) * upArmScale);
    glm::mat4 racketGridScale = glm::scale(iMat, glm::vec3(1.0f, 1.3f, 1.0f) * upArmScale);
    glm::mat4 racketGridMatrix = worldMatrix * upperArmHierarchy * lowerArmHierarchy * wristHierarchy * racketGridTranslate * racketGridScale;
    setWorldMatrix(sceneShaderProgram, racketGridMatrix);

    // Drawing the racket grid
    glBindVertexArray(racketGridVao);
    setUniqueColor(sceneShaderProgram, 0.55f, 1.0f, 0.2f);
    setMaterial(sceneShaderProgram, 1.0f, 0.0f, 0.0f, 1.0f, toggleShadows);
    glDrawArrays(renderModeRacketGrid, 0, 40);
    glBindVertexArray(0);
}

void drawCourt(glm::mat4 worldMatrix, int clayTextureID, int grassTextureID, int cubeVao, int cubeVaoRepeat, int sceneShaderProgram)
{
    glBindVertexArray(cubeVao);

    noTexture(sceneShaderProgram);

    // Court line right model matrix
    glm::mat4 courtRightModelMatrix = glm::translate(iMat, glm::vec3(15.0f, -0.5f, 0.0f)) * glm::scale(iMat, glm::vec3(1.0f, 0.1f, 73.0f));
    courtRightModelMatrix = worldMatrix * courtRightModelMatrix;
    setWorldMatrix(sceneShaderProgram, courtRightModelMatrix);

    // Drawing the court line right
    setMaterial(sceneShaderProgram, 0.3f, 0.6f, 0.1f, 10.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 1.0f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Court line left model matrix
    glm::mat4 courtLeftModelMatrix = glm::translate(iMat, glm::vec3(-15.0f, -0.5f, 0.0f)) * glm::scale(iMat, glm::vec3(1.0f, 0.1f, 73.0f));
    courtLeftModelMatrix = worldMatrix * courtLeftModelMatrix;
    setWorldMatrix(sceneShaderProgram, courtLeftModelMatrix);

    // Drawing the court line left
    setMaterial(sceneShaderProgram, 0.3f, 0.6f, 0.1f, 10.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 1.0f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Court line outer right model matrix
    courtRightModelMatrix = glm::translate(iMat, glm::vec3(20.0f, -0.5f, 0.0f)) * glm::scale(iMat, glm::vec3(1.0f, 0.1f, 73.0f));
    courtRightModelMatrix = worldMatrix * courtRightModelMatrix;
    setWorldMatrix(sceneShaderProgram, courtRightModelMatrix);

    // Drawing the court line right outer
    setMaterial(sceneShaderProgram, 0.3f, 0.6f, 0.1f, 10.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 1.0f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Court line left outer model matrix
    courtLeftModelMatrix = glm::translate(iMat, glm::vec3(-20.0f, -0.5f, 0.0f)) * glm::scale(iMat, glm::vec3(1.0f, 0.1f, 73.0f));
    courtLeftModelMatrix = worldMatrix * courtLeftModelMatrix;
    setWorldMatrix(sceneShaderProgram, courtLeftModelMatrix);

    // Drawing the court line left outer
    setMaterial(sceneShaderProgram, 0.3f, 0.6f, 0.1f, 10.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 1.0f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Court very bottom line model matrix
    glm::mat4 courtBottom1ModelMatrix = glm::translate(iMat, glm::vec3(0.0f, -0.5f, 36.0f)) * glm::scale(iMat, glm::vec3(40.0f, 0.1f, 1.0f));
    courtBottom1ModelMatrix = worldMatrix * courtBottom1ModelMatrix;
    setWorldMatrix(sceneShaderProgram, courtBottom1ModelMatrix);

    // Drawing the court very bottom line
    setMaterial(sceneShaderProgram, 0.3f, 0.6f, 0.1f, 10.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 1.0f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Court very top line model matrix
    glm::mat4 courtTop1ModelMatrix = glm::translate(iMat, glm::vec3(0.0f, -0.5f, -36.0f)) * glm::scale(iMat, glm::vec3(40.0f, 0.1f, 1.0f));
    courtTop1ModelMatrix = worldMatrix * courtTop1ModelMatrix;
    setWorldMatrix(sceneShaderProgram, courtTop1ModelMatrix);

    // Drawing the court very top line
    setMaterial(sceneShaderProgram, 0.3f, 0.6f, 0.1f, 10.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 1.0f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Court close bottom line model matrix
    glm::mat4 courtBottom2ModelMatrix = glm::translate(iMat, glm::vec3(0.0f, -0.5f, 18.0f)) * glm::scale(iMat, glm::vec3(30.0f, 0.1f, 1.0f));
    courtBottom2ModelMatrix = worldMatrix * courtBottom2ModelMatrix;
    setWorldMatrix(sceneShaderProgram, courtBottom2ModelMatrix);

    // Drawing the court close bottom line
    setMaterial(sceneShaderProgram, 0.3f, 0.6f, 0.1f, 10.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 1.0f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Court close top line model matrix
    glm::mat4 courtTop2ModelMatrix = glm::translate(iMat, glm::vec3(0.0f, -0.5f, -18.0f)) * glm::scale(iMat, glm::vec3(30.0f, 0.1f, 1.0f));
    courtTop2ModelMatrix = worldMatrix * courtTop2ModelMatrix;
    setWorldMatrix(sceneShaderProgram, courtTop2ModelMatrix);

    // Drawing the court close top line
    setMaterial(sceneShaderProgram, 0.3f, 0.6f, 0.1f, 10.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 1.0f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Court middle line model matrix
    glm::mat4 courtMiddleModelMatrix = glm::translate(iMat, glm::vec3(0.0f, -0.5f, 0.0f)) * glm::scale(iMat, glm::vec3(1.0f, 0.1f, 36.0f));
    courtMiddleModelMatrix = worldMatrix * courtMiddleModelMatrix;
    setWorldMatrix(sceneShaderProgram, courtMiddleModelMatrix);

    // Drawing the court middle line
    setMaterial(sceneShaderProgram, 0.3f, 0.6f, 0.1f, 10.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 1.0f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Court line small middle close
    courtLeftModelMatrix = glm::translate(iMat, glm::vec3(0.0f, -0.5f, 35.0f)) * glm::scale(iMat, glm::vec3(1.0f, 0.1f, 2.0f));
    courtLeftModelMatrix = worldMatrix * courtLeftModelMatrix;
    setWorldMatrix(sceneShaderProgram, courtLeftModelMatrix);

    // Drawing the Court line small middle
    setMaterial(sceneShaderProgram, 0.3f, 0.6f, 0.1f, 10.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 1.0f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Court line small middle far
    courtLeftModelMatrix = glm::translate(iMat, glm::vec3(0.0f, -0.5f, -35.0f)) * glm::scale(iMat, glm::vec3(1.0f, 0.1f, 2.0f));
    courtLeftModelMatrix = worldMatrix * courtLeftModelMatrix;
    setWorldMatrix(sceneShaderProgram, courtLeftModelMatrix);

    // Drawing the Court line small middle
    setMaterial(sceneShaderProgram, 0.3f, 0.6f, 0.1f, 10.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 1.0f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Court base model matrix
    glm::mat4 courtModelMatrix = glm::translate(iMat, glm::vec3(0.0f, -1.0f, 0.0f)) * glm::scale(iMat, glm::vec3(46.0f, 0.5f, 78.0f));
    courtModelMatrix = worldMatrix * courtModelMatrix;
    setWorldMatrix(sceneShaderProgram, courtModelMatrix);

    setTexture(sceneShaderProgram, clayTextureID, 0, toggleTexture);
    setMaterial(sceneShaderProgram, 0.3f, 0.5f, 0.1f, 10.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 0.8f, 0.4f, 0.2f);

    // Drawing the court base
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);


    glBindVertexArray(cubeVaoRepeat);
    // Court ground model matrix
    glm::mat4 courtGroundModelMatrix = glm::translate(iMat, glm::vec3(0.0f, -1.5f, 0.0f)) * glm::scale(iMat, glm::vec3(100.0f, 0.5f, 120.0f));
    courtGroundModelMatrix = worldMatrix * courtGroundModelMatrix;
    setWorldMatrix(sceneShaderProgram, courtGroundModelMatrix);

    setBlend(sceneShaderProgram, 0.6f);
    setTexture(sceneShaderProgram, grassTextureID, 1, toggleTexture);
    setMaterial(sceneShaderProgram, 0.2f, 0.6f, 0.1f, 10.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 0.835f, 1.0f, 0.663f);

    // Drawing the court ground
    glDrawArrays(GL_TRIANGLES, 0, 36);


    // Background ground model matrix
    glm::mat4 backgroundGroundModelMatrix = glm::translate(iMat, glm::vec3(0.0f, -2.0f, 0.0f)) * glm::scale(iMat, glm::vec3(500.0f, 0.5f, 500.0f));
    backgroundGroundModelMatrix = worldMatrix * backgroundGroundModelMatrix;
    setWorldMatrix(sceneShaderProgram, backgroundGroundModelMatrix);

    setTexture(sceneShaderProgram, grassTextureID, 0, toggleTexture);
    setMaterial(sceneShaderProgram, 0.2f, 0.6f, 0.1f, 10.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 0.835f, 1.0f, 0.663f);

    // Drawing the court ground
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

}

void drawNet(glm::mat4 worldMatrix, int netGridVao, int cubeVao, int sceneShaderProgram)
{
    glBindVertexArray(cubeVao);
    noTexture(sceneShaderProgram);
    setBlend(sceneShaderProgram, 0.0f);

    // Net right post model matrix
    glm::mat4 netRightModelMatrix = glm::translate(iMat, glm::vec3(20.0f, 3.0f, 0.0f)) * glm::scale(iMat, glm::vec3(1.0f, 8.5f, 1.0f));
    netRightModelMatrix = worldMatrix * netRightModelMatrix;
    setWorldMatrix(sceneShaderProgram, netRightModelMatrix);

    // Drawing the net right post
    setMaterial(sceneShaderProgram, 0.3f, 0.3f, 1.0f, 20.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 0.161f, 0.396f, 0.0235f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Net left post model matrix
    glm::mat4 netLeftModelMatrix = glm::translate(iMat, glm::vec3(-20.0f, 3.0f, 0.0f)) * glm::scale(iMat, glm::vec3(1.0f, 8.5f, 1.0f));
    netLeftModelMatrix = worldMatrix * netLeftModelMatrix;
    setWorldMatrix(sceneShaderProgram, netLeftModelMatrix);

    // Drawing the net left post
    setMaterial(sceneShaderProgram, 0.3f, 0.3f, 1.0f, 20.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 0.161f, 0.396f, 0.0235f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Net top model matrix
    glm::mat4 netTopModelMatrix = glm::translate(iMat, glm::vec3(0.0f, 6.0f, 0.0f)) * glm::scale(iMat, glm::vec3(40.0f, 1.0f, 0.2f));
    netTopModelMatrix = worldMatrix * netTopModelMatrix;
    setWorldMatrix(sceneShaderProgram, netTopModelMatrix);

    // Drawing the net top
    setMaterial(sceneShaderProgram, 0.4f, 0.8f, 0.1f, 10.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 0.94f, 0.94f, 0.94f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Net bottom model matrix
    glm::mat4 netBottomModelMatrix = glm::translate(iMat, glm::vec3(0.0f, 1.5f, 0.0f)) * glm::scale(iMat, glm::vec3(40.0f, 0.5f, 0.2f));
    netBottomModelMatrix = worldMatrix * netBottomModelMatrix;
    setWorldMatrix(sceneShaderProgram, netBottomModelMatrix);

    noTexture(sceneShaderProgram);

    // Drawing the net bottom
    setMaterial(sceneShaderProgram, 0.4f, 0.8f, 0.1f, 10.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 0.22f, 0.22f, 0.22f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Net middle model matrix
    glm::mat4 netMiddleModelMatrix = glm::translate(iMat, glm::vec3(0.0f, 3.5f, 0.0f)) * glm::scale(iMat, glm::vec3(1.0f, 4.5f, 0.2f));
    netMiddleModelMatrix = worldMatrix * netMiddleModelMatrix;
    setWorldMatrix(sceneShaderProgram, netMiddleModelMatrix);

    // Drawing the net middle
    setMaterial(sceneShaderProgram, 0.4f, 0.8f, 0.1f, 10.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 0.94f, 0.94f, 0.94f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Net grid model matrix
    glm::mat4 netGridModelMatrix = glm::translate(iMat, glm::vec3(0.0f, 3.5f, 0.0f)) * glm::scale(iMat, glm::vec3(20.0f, 2.0f, 1.0f));
    netGridModelMatrix = worldMatrix * netGridModelMatrix;
    setWorldMatrix(sceneShaderProgram, netGridModelMatrix);

    // Drawing the net grid
    glBindVertexArray(netGridVao);
    setUniqueColor(sceneShaderProgram, 1.0f, 1.0f, 1.0f); // For grid, set uniqueColor to neutral color (white)
    setMaterial(sceneShaderProgram, 1.0f, 0.0f, 0.0f, 0.0f, toggleShadows);
    glDrawArrays(GL_LINES, 0, 260);

    glBindVertexArray(0);
}


void drawStadium(glm::mat4 worldMatrix, int cubeVao, int cubeVaoRepeat, int sceneShaderProgram, int standTextureID, int wallTextureID) 
{
    setTexture(sceneShaderProgram, wallTextureID, 0, toggleTexture);
    //noTexture(sceneShaderProgram);
    setMaterial(sceneShaderProgram, 0.4f, 0.8f, 0.1f, 10.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 0.039f, 0.378f, 0.239f);
    glBindVertexArray(cubeVao);

    // Far wall model matrix
    glm::mat4 farWallModelMatrix = glm::translate(iMat, glm::vec3(0.0f, 5.0f, -65.0f)) * glm::scale(iMat, glm::vec3(160.0f, 13.0f, 10.0f));
    farWallModelMatrix = worldMatrix * farWallModelMatrix;
    setWorldMatrix(sceneShaderProgram, farWallModelMatrix);

    // Drawing the far wall
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Near wall model matrix
    glm::mat4 nearWallModelMatrix = glm::translate(iMat, glm::vec3(0.0f, 5.0f, 65.0f)) * glm::scale(iMat, glm::vec3(160.0f, 13.0f, 10.0f));
    nearWallModelMatrix = worldMatrix * nearWallModelMatrix;
    setWorldMatrix(sceneShaderProgram, nearWallModelMatrix);

    // Drawing the near wall
    glDrawArrays(GL_TRIANGLES, 0, 36);


    // Right wall model matrix
    glm::mat4 rightWallModelMatrix = glm::translate(iMat, glm::vec3(85.0f, 13.0f, 0.0f)) * glm::scale(iMat, glm::vec3(10.0f, 29.0f, 120.0f));
    rightWallModelMatrix = worldMatrix * rightWallModelMatrix;
    setWorldMatrix(sceneShaderProgram, rightWallModelMatrix);

    // Drawing the right wall
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Left wall model matrix
    glm::mat4 leftWallModelMatrix = glm::translate(iMat, glm::vec3(-85.0f, 13.0f, 0.0f)) * glm::scale(iMat, glm::vec3(10.0f, 29.0f, 120.0f));
    leftWallModelMatrix = worldMatrix * leftWallModelMatrix;
    setWorldMatrix(sceneShaderProgram, leftWallModelMatrix);

    // Drawing the left wall
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Corner 1 model matrix (near right)
    glm::mat4 corner1ModelMatrix = glm::translate(iMat, glm::vec3(85.0f, 9.0f, 65.0f)) * glm::scale(iMat, glm::vec3(10.0f, 21.0f, 10.0f));
    corner1ModelMatrix = worldMatrix * corner1ModelMatrix;
    setWorldMatrix(sceneShaderProgram, corner1ModelMatrix);

    // Drawing the corner 1
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Corner 2 model matrix (near left)
    glm::mat4 corner2ModelMatrix = glm::translate(iMat, glm::vec3(-85.0f, 9.0f, 65.0f)) * glm::scale(iMat, glm::vec3(10.0f, 21.0f, 10.0f));
    corner2ModelMatrix = worldMatrix * corner2ModelMatrix;
    setWorldMatrix(sceneShaderProgram, corner2ModelMatrix);

    // Drawing the corner 2
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Corner 3 model matrix (far right)
    glm::mat4 corner3ModelMatrix = glm::translate(iMat, glm::vec3(85.0f, 9.0f, -65.0f)) * glm::scale(iMat, glm::vec3(10.0f, 21.0f, 10.0f));
    corner3ModelMatrix = worldMatrix * corner3ModelMatrix;
    setWorldMatrix(sceneShaderProgram, corner3ModelMatrix);

    // Drawing the corner 3
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Corner 4 model matrix (far left)
    glm::mat4 corner4ModelMatrix = glm::translate(iMat, glm::vec3(-85.0f, 9.0f, -65.0f)) * glm::scale(iMat, glm::vec3(10.0f, 21.0f, 10.0f));
    corner4ModelMatrix = worldMatrix * corner4ModelMatrix;
    setWorldMatrix(sceneShaderProgram, corner4ModelMatrix);

    // Drawing the corner 4
    glDrawArrays(GL_TRIANGLES, 0, 36);

    setTexture(sceneShaderProgram, standTextureID, 0, toggleTexture);
    setMaterial(sceneShaderProgram, 0.4f, 0.6f, 0.3f, 40.0f, toggleShadows);
    glBindVertexArray(cubeVaoRepeat);

    // Right stand 1 model matrix
    glm::mat4 rStand1ModelMatrix = glm::translate(iMat, glm::vec3(55.0f, 2.0f, 0.0f)) * glm::scale(iMat, glm::vec3(10.0f, 7.0f, 120.0f));
    rStand1ModelMatrix = worldMatrix * rStand1ModelMatrix;
    setWorldMatrix(sceneShaderProgram, rStand1ModelMatrix);

    // Drawing the right stand 1
    setUniqueColor(sceneShaderProgram, 0.8f, 0.8f, 0.8f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Right stand 2 model matrix
    glm::mat4 rStand2ModelMatrix = glm::translate(iMat, glm::vec3(65.0f, 5.0f, 0.0f)) * glm::scale(iMat, glm::vec3(10.0f, 13.0f, 120.0f));
    rStand2ModelMatrix = worldMatrix * rStand2ModelMatrix;
    setWorldMatrix(sceneShaderProgram, rStand2ModelMatrix);

    // Drawing the right stand 2
    setUniqueColor(sceneShaderProgram, 0.8f, 0.8f, 0.8f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Right stand 3 model matrix
    glm::mat4 rStand3ModelMatrix = glm::translate(iMat, glm::vec3(75.0f, 8.0f, 0.0f)) * glm::scale(iMat, glm::vec3(10.0f, 19.0f, 120.0f));
    rStand3ModelMatrix = worldMatrix * rStand3ModelMatrix;
    setWorldMatrix(sceneShaderProgram, rStand3ModelMatrix);

    // Drawing the right stand 3
    setUniqueColor(sceneShaderProgram, 0.8f, 0.8f, 0.8f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Left stand 1 model matrix
    glm::mat4 lStand1ModelMatrix = glm::translate(iMat, glm::vec3(-55.0f, 2.0f, 0.0f)) * glm::scale(iMat, glm::vec3(10.0f, 7.0f, 120.0f));
    lStand1ModelMatrix = worldMatrix * lStand1ModelMatrix;
    setWorldMatrix(sceneShaderProgram, lStand1ModelMatrix);

    // Drawing the left stand 1
    setUniqueColor(sceneShaderProgram, 0.8f, 0.8f, 0.8f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Left stand 2 model matrix
    glm::mat4 lStand2ModelMatrix = glm::translate(iMat, glm::vec3(-65.0f, 5.0f, 0.0f)) * glm::scale(iMat, glm::vec3(10.0f, 13.0f, 120.0f));
    lStand2ModelMatrix = worldMatrix * lStand2ModelMatrix;
    setWorldMatrix(sceneShaderProgram, lStand2ModelMatrix);

    // Drawing the left stand 2
    setUniqueColor(sceneShaderProgram, 0.8f, 0.8f, 0.8f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Left stand 3 model matrix
    glm::mat4 lStand3ModelMatrix = glm::translate(iMat, glm::vec3(-75.0f, 8.0f, 0.0f)) * glm::scale(iMat, glm::vec3(10.0f, 19.0f, 120.0f));
    lStand3ModelMatrix = worldMatrix * lStand3ModelMatrix;
    setWorldMatrix(sceneShaderProgram, lStand3ModelMatrix);

    // Drawing the left stand 3
    setUniqueColor(sceneShaderProgram, 0.8f, 0.8f, 0.8f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);

}


void drawLightCube(glm::mat4 worldMatrix, int sceneShaderProgram, int cubeVao, glm::vec3 lightPosition)
{
    noTexture(sceneShaderProgram);
    glBindVertexArray(cubeVao);

    // Light cube model matrix
    glm::mat4 lightCubeModelMatrix = glm::rotate(iMat, glm::radians(lightAngle), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::translate(iMat, lightPosition) *
        glm::scale(iMat, glm::vec3(1.0f, 1.0f, 1.0f));
    lightCubeModelMatrix = worldMatrix * lightCubeModelMatrix;
    setWorldMatrix(sceneShaderProgram, lightCubeModelMatrix);

    // Drawing the light cube
    setUniqueColor(sceneShaderProgram, 1.0f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawSkyBox(glm::mat4 worldMatrix, int sceneShaderProgram, int sphereVao, int skyId, std::vector<int> indices) {
    // We want to see the inside of the sky box
    glCullFace(GL_FRONT);
    // Sky box matrix
    glm::mat4 skyBoxWorldMatrix = glm::translate(iMat, glm::vec3(0.0f, -50.0f, 0.0f)) * glm::rotate(iMat, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(iMat, glm::vec3(220.0f, 220.0f, 220.0f));
    skyBoxWorldMatrix = worldMatrix * skyBoxWorldMatrix;

    GLuint skyBoxLocation = glGetUniformLocation(sceneShaderProgram, "isSkyBox");
    glUniform1i(skyBoxLocation, 1);

    // Drawing the sky box
    glBindVertexArray(sphereVao);
    setWorldMatrix(sceneShaderProgram, skyBoxWorldMatrix);
    setUniqueColor(sceneShaderProgram, 0.294f, 0.6745f, 0.925f);
    setTexture(sceneShaderProgram, skyId, 0, toggleTexture);
    //glDrawArrays(GL_TRIANGLES, 0, 36);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glUniform1i(skyBoxLocation, 0);
   glCullFace(GL_BACK);
}


// Drawing a number on the scoreboard
// IMPORTANT: activeNumber 11 and 12 represent A and d respectively***
void drawNumber(glm::mat4 worldMatrix, int cubeVao, int sceneShaderProgram, glm::vec3 initialPosition, glm::vec3 initialScale, int activeNumber) {
    noTexture(sceneShaderProgram);
    setMaterial(sceneShaderProgram, 0.4f, 0.8f, 0.1f, 10.0f, toggleShadows);
    glBindVertexArray(cubeVao);

    // Line 1 matrix (bottom)
    glm::mat4 line1ModelMatrix = glm::translate(iMat, (glm::vec3(0.0f, -5.0f, -68.5f) + initialPosition) * initialScale) * glm::scale(iMat, glm::vec3(5.0f, 1.0f, 1.0f) * initialScale);
    line1ModelMatrix = worldMatrix * line1ModelMatrix;
    setWorldMatrix(sceneShaderProgram, line1ModelMatrix);
    if (activeNumber == 0 || activeNumber == 2 || activeNumber == 3 || activeNumber == 5 || activeNumber == 6 || activeNumber == 8 || activeNumber == 12) {
        setUniqueColor(sceneShaderProgram, 1.0f, 0.98f, 0.313f);
    }
    else {
        setUniqueColor(sceneShaderProgram, 0.0f, 0.0f, 0.5f);
    }
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Line 2 matrix (middle)
    glm::mat4 line2ModelMatrix = glm::translate(iMat, (glm::vec3(0.0f, 0.0f, -68.5f) + initialPosition) * initialScale) * glm::scale(iMat, glm::vec3(5.0f, 1.0f, 1.0f) * initialScale);
    line2ModelMatrix = worldMatrix * line2ModelMatrix;
    setWorldMatrix(sceneShaderProgram, line2ModelMatrix);
    if (activeNumber == 2 || activeNumber == 3 || activeNumber == 4 || activeNumber == 5 || activeNumber == 6 || activeNumber == 8 || activeNumber == 9 || activeNumber == 11 || activeNumber == 12) {
        setUniqueColor(sceneShaderProgram, 1.0f, 0.98f, 0.313f);
    }
    else {
        setUniqueColor(sceneShaderProgram, 0.0f, 0.0f, 0.5f);
    }
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Line 3 matrix (top)
    glm::mat4 line3ModelMatrix = glm::translate(iMat, (glm::vec3(0.0f, 5.0f, -68.5f) + initialPosition) * initialScale) * glm::scale(iMat, glm::vec3(5.0f, 1.0f, 1.0f) * initialScale);
    line3ModelMatrix = worldMatrix * line3ModelMatrix;
    setWorldMatrix(sceneShaderProgram, line3ModelMatrix);
    if (activeNumber == 0 || activeNumber == 2 || activeNumber == 3 || activeNumber == 5 || activeNumber == 6 || activeNumber == 7 || activeNumber == 8 || activeNumber == 9 || activeNumber == 11) {
        setUniqueColor(sceneShaderProgram, 1.0f, 0.98f, 0.313f);
    }
    else {
        setUniqueColor(sceneShaderProgram, 0.0f, 0.0f, 0.5f);
    }
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Line 4 matrix (bottom left)
    glm::mat4 line4ModelMatrix = glm::translate(iMat, (glm::vec3(-3.0f, -2.5f, -68.5f) + initialPosition) * initialScale) * glm::scale(iMat, glm::vec3(1.0f, 5.0f, 1.0f) * initialScale);
    line4ModelMatrix = worldMatrix * line4ModelMatrix;
    setWorldMatrix(sceneShaderProgram, line4ModelMatrix);
    if (activeNumber == 0 || activeNumber == 2 || activeNumber == 6 || activeNumber == 8 || activeNumber == 11 || activeNumber == 12) {
        setUniqueColor(sceneShaderProgram, 1.0f, 0.98f, 0.313f);
    }
    else {
        setUniqueColor(sceneShaderProgram, 0.0f, 0.0f, 0.5f);
    }
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Line 5 matrix (bottom right)
    glm::mat4 line5ModelMatrix = glm::translate(iMat, (glm::vec3(3.0f, -2.5f, -68.5f) + initialPosition) * initialScale) * glm::scale(iMat, glm::vec3(1.0f, 5.0f, 1.0f) * initialScale);
    line5ModelMatrix = worldMatrix * line5ModelMatrix;
    setWorldMatrix(sceneShaderProgram, line5ModelMatrix);
    if (activeNumber == 0 || activeNumber == 1 || activeNumber == 3 || activeNumber == 4 || activeNumber == 5 || activeNumber == 6 || activeNumber == 7 || activeNumber == 8 || activeNumber == 9 || activeNumber == 11 || activeNumber == 12) {
        setUniqueColor(sceneShaderProgram, 1.0f, 0.98f, 0.313f);
    }
    else {
        setUniqueColor(sceneShaderProgram, 0.0f, 0.0f, 0.5f);
    }
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Line 6 matrix (top left)
    glm::mat4 line6ModelMatrix = glm::translate(iMat, (glm::vec3(-3.0f, 2.5f, -68.5f) + initialPosition) * initialScale) * glm::scale(iMat, glm::vec3(1.0f, 5.0f, 1.0f) * initialScale);
    line6ModelMatrix = worldMatrix * line6ModelMatrix;
    setWorldMatrix(sceneShaderProgram, line6ModelMatrix);
    if (activeNumber == 0 || activeNumber == 4 || activeNumber == 5 || activeNumber == 6 || activeNumber == 8 || activeNumber == 9 || activeNumber == 11) {
        setUniqueColor(sceneShaderProgram, 1.0f, 0.98f, 0.313f);
    }
    else {
        setUniqueColor(sceneShaderProgram, 0.0f, 0.0f, 0.5f);
    }
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Line 7 matrix (top right)
    glm::mat4 line7ModelMatrix = glm::translate(iMat, (glm::vec3(3.0f, 2.5f, -68.5f) + initialPosition) * initialScale) * glm::scale(iMat, glm::vec3(1.0f, 5.0f, 1.0f) * initialScale);
    line7ModelMatrix = worldMatrix * line7ModelMatrix;
    setWorldMatrix(sceneShaderProgram, line7ModelMatrix);
    if (activeNumber == 0 || activeNumber == 1 || activeNumber == 2 || activeNumber == 3 || activeNumber == 4 || activeNumber == 7 || activeNumber == 8 || activeNumber == 9 || activeNumber == 11 || activeNumber == 12) {
        setUniqueColor(sceneShaderProgram, 1.0f, 0.98f, 0.313f);
    }
    else {
        setUniqueColor(sceneShaderProgram, 0.0f, 0.0f, 0.5f);
    }
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);
}


void drawScoreboardParticle(glm::mat4 worldMatrix, int cubeVao, int sceneShaderProgram, float lastFrameTime)
{
    float particleYPosition = 21.0f;
    float particleXPosition = -39.0f;
    float particleVelocity = 200.0f;
    static float animationStartTime = lastFrameTime;
    float currentSpeed = (lastFrameTime - animationStartTime) * particleVelocity;
    float distanceY = 59.0f - 21.0f;
    float distanceX = 78.0f/* - abs(startingPositionX)*/;
    float animationTime = std::fmod(currentSpeed, distanceY);
    float animationProgress = animationTime / distanceY;


    // Calculate the particle's position based on animationProgress
    if (animationProgress <= 0.25f) {
        particleXPosition = -39.0f;
        particleYPosition = 21.0f + animationProgress * (distanceY * 4);
    }
    else if (animationProgress <= 0.5f) {
        particleYPosition = 59.0f;
        particleXPosition = -39.0f + (animationProgress - 0.25f) * (distanceX * 4);
    }
    else if (animationProgress <= 0.75f) {
        particleXPosition = 39.0f;
        particleYPosition = (21.0f + distanceY) - (animationProgress - 0.5f) * (distanceY * 4);
    }
    else {
        particleYPosition = 21.0f;
        particleXPosition = (-39.0f + distanceX) - (animationProgress - 0.75f) * (distanceX * 4);
    }

    glBindVertexArray(cubeVao);
    noTexture(sceneShaderProgram);

    // Particle model matrix
    glm::mat4 particleModelMatrix = glm::translate(iMat, glm::vec3(particleXPosition, particleYPosition, -68.5f)) * glm::scale(iMat, glm::vec3(2.0f, 2.0f, 1.0f));
    particleModelMatrix = worldMatrix * particleModelMatrix;
    setWorldMatrix(sceneShaderProgram, particleModelMatrix);

    // Drawing the particle
    setUniqueColor(sceneShaderProgram, 1.0f, 0.98f, 0.313f);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawScoreboard(glm::mat4 worldMatrix, int cubeVao, int sceneShaderProgram, int woodTextureID) {

    setTexture(sceneShaderProgram, woodTextureID, 0, toggleTexture);
    setMaterial(sceneShaderProgram, 0.4f, 0.8f, 0.1f, 10.0f, toggleShadows);
    glBindVertexArray(cubeVao);

    // Board model matrix
    glm::mat4 boardModelMatrix = glm::translate(iMat, glm::vec3(0.0f, 40.0f, -70.0f)) * glm::scale(iMat, glm::vec3(85.0f, 45.0f, 2.0f));
    boardModelMatrix = worldMatrix * boardModelMatrix;
    setWorldMatrix(sceneShaderProgram, boardModelMatrix);

    // Drawing the board
    setUniqueColor(sceneShaderProgram, 0.392f, 0.247f, 0.137f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Board leg 1 model matrix
    glm::mat4 boardLeg1ModelMatrix = glm::translate(iMat, glm::vec3(25.0f, 21.0f, -72.0f)) * glm::scale(iMat, glm::vec3(3.0f, 45.0f, 3.0f));
    boardLeg1ModelMatrix = worldMatrix * boardLeg1ModelMatrix;
    setWorldMatrix(sceneShaderProgram, boardLeg1ModelMatrix);

    // Drawing the board leg 1
    setUniqueColor(sceneShaderProgram, 0.392f, 0.247f, 0.137f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Board leg 2 model matrix
    glm::mat4 boardLeg2ModelMatrix = glm::translate(iMat, glm::vec3(-25.0f, 21.0f, -72.0f)) * glm::scale(iMat, glm::vec3(3.0f, 45.0f, 3.0f));
    boardLeg2ModelMatrix = worldMatrix * boardLeg2ModelMatrix;
    setWorldMatrix(sceneShaderProgram, boardLeg2ModelMatrix);

    // Drawing the board leg 2
    setUniqueColor(sceneShaderProgram, 0.392f, 0.247f, 0.137f);
    glDrawArrays(GL_TRIANGLES, 0, 36);


    noTexture(sceneShaderProgram);

    // Screen model matrix
    glm::mat4 screenModelMatrix = glm::translate(iMat, glm::vec3(0.0f, 40.0f, -69.0f)) * glm::scale(iMat, glm::vec3(80.0f, 40.0f, 1.0f));
    screenModelMatrix = worldMatrix * screenModelMatrix;
    setWorldMatrix(sceneShaderProgram, screenModelMatrix);

    // Drawing the screen
    setUniqueColor(sceneShaderProgram, 0.0f, 0.0f, 0.2f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Colon 1 model matrix
    glm::mat4 colon1ModelMatrix = glm::translate(iMat, glm::vec3(0.0f, 40.0f, -68.5f)) * glm::scale(iMat, glm::vec3(1.0f, 1.0f, 1.0f));
    colon1ModelMatrix = worldMatrix * colon1ModelMatrix;
    setWorldMatrix(sceneShaderProgram, colon1ModelMatrix);

    // Drawing the colon 1
    setUniqueColor(sceneShaderProgram, 1.0f, 0.98f, 0.313f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Colon 2 model matrix
    glm::mat4 colon2ModelMatrix = glm::translate(iMat, glm::vec3(0.0f, 35.0f, -68.5f)) * glm::scale(iMat, glm::vec3(1.0f, 1.0f, 1.0f));
    colon2ModelMatrix = worldMatrix * colon2ModelMatrix;
    setWorldMatrix(sceneShaderProgram, colon2ModelMatrix);

    // Drawing the colon 2
    setUniqueColor(sceneShaderProgram, 1.0f, 0.98f, 0.313f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);

    // Drawing point numbers
    drawNumber(worldMatrix, cubeVao, sceneShaderProgram, glm::vec3(-20.0f, 38.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), score1[0]);
    drawNumber(worldMatrix, cubeVao, sceneShaderProgram, glm::vec3(-10.0f, 38.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), score1[1]);
    drawNumber(worldMatrix, cubeVao, sceneShaderProgram, glm::vec3(10.0f, 38.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), score2[0]);
    drawNumber(worldMatrix, cubeVao, sceneShaderProgram, glm::vec3(20.0f, 38.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), score2[1]);

    // Drawing set numbers
    drawNumber(worldMatrix, cubeVao, sceneShaderProgram, glm::vec3(-25.0f, 86.0f, 0.0f), glm::vec3(0.6f, 0.6f, 1.0f), setScore1);
    drawNumber(worldMatrix, cubeVao, sceneShaderProgram, glm::vec3(25.0f, 86.0f, 0.0f), glm::vec3(0.6f, 0.6f, 1.0f), setScore2);

    // Particle for scoreboard
    drawScoreboardParticle(worldMatrix, cubeVao, sceneShaderProgram, glfwGetTime());
    drawScoreboardParticle(worldMatrix, cubeVao, sceneShaderProgram, glfwGetTime() + 0.5f);
    drawScoreboardParticle(worldMatrix, cubeVao, sceneShaderProgram, glfwGetTime() + 1.0f);
    drawScoreboardParticle(worldMatrix, cubeVao, sceneShaderProgram, glfwGetTime() + 1.5f);
    drawScoreboardParticle(worldMatrix, cubeVao, sceneShaderProgram, glfwGetTime() + 2.0f);
    drawScoreboardParticle(worldMatrix, cubeVao, sceneShaderProgram, glfwGetTime() + 2.5f);
    drawScoreboardParticle(worldMatrix, cubeVao, sceneShaderProgram, glfwGetTime() + 3.0f);
    drawScoreboardParticle(worldMatrix, cubeVao, sceneShaderProgram, glfwGetTime() + 3.5f);
    drawScoreboardParticle(worldMatrix, cubeVao, sceneShaderProgram, glfwGetTime() + 4.0f);
    drawScoreboardParticle(worldMatrix, cubeVao, sceneShaderProgram, glfwGetTime() + 4.5f);
    drawScoreboardParticle(worldMatrix, cubeVao, sceneShaderProgram, glfwGetTime() + 5.0f);
    drawScoreboardParticle(worldMatrix, cubeVao, sceneShaderProgram, glfwGetTime() + 5.5f);
    drawScoreboardParticle(worldMatrix, cubeVao, sceneShaderProgram, glfwGetTime() + 6.0f);
    drawScoreboardParticle(worldMatrix, cubeVao, sceneShaderProgram, glfwGetTime() + 6.5f);
    drawScoreboardParticle(worldMatrix, cubeVao, sceneShaderProgram, glfwGetTime() + 7.0f);
}



void drawLights(mat4 worldMatrix, int cubeVao, int shader) {

    glBindVertexArray(cubeVao);
    noTexture(shader);
    setUniqueColor(shader, 1.0f, 1.0f, 1.0f);

    // Right Back Post matrix
    mat4 postScaleMatrix = scale(iMat, glm::vec3(2.0f, 80.0f, 2.0f));
    mat4 postModelMatrix = translate(iMat, glm::vec3(85.0f, 40.0f, -65.0f)) * postScaleMatrix;
    postModelMatrix = worldMatrix * postModelMatrix;
    setWorldMatrix(shader, postModelMatrix);

    // Drawing the Right Back post
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //Right Front Post matrix
    postModelMatrix = translate(iMat, glm::vec3(85.0f, 40.0f, 65.0f)) * postScaleMatrix;
    postModelMatrix = worldMatrix * postModelMatrix;
    setWorldMatrix(shader, postModelMatrix);

    // Drawing the Right front post
    glDrawArrays(GL_TRIANGLES, 0, 36);


    //left back Post Matrix
    postModelMatrix = translate(iMat, glm::vec3(-85.0f, 40.0f, -65.0f)) * postScaleMatrix;
    postModelMatrix = worldMatrix * postModelMatrix;
    setWorldMatrix(shader, postModelMatrix);

    // Drawing the Left Back post
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // left front post matrix
    postModelMatrix = translate(iMat, glm::vec3(-85.0f, 40.0f, 65.0f)) * postScaleMatrix;
    postModelMatrix = worldMatrix * postModelMatrix;
    setWorldMatrix(shader, postModelMatrix);

    // Drawing the left front post
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // right back top light
    mat4 lightScaleMatrix = scale(iMat, glm::vec3(7.0f, 5.0f, 5.0f));
    mat4 lightRotationMatrix = rotate(iMat, radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * rotate(iMat,radians(45.0f),  glm::vec3(0.0f, 0.0f, 1.0f));
    mat4 lightModelMatrix = translate(iMat, glm::vec3(83.0f, 79.0f, -63.0f))* lightRotationMatrix * lightScaleMatrix;
    lightModelMatrix = worldMatrix * lightModelMatrix;
    setWorldMatrix(shader, lightModelMatrix);

    // Drawing the right back top light
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // right front top light
    lightRotationMatrix = rotate(iMat, radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * rotate(iMat, radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    lightModelMatrix = translate(iMat, glm::vec3(83.0f, 79.0f, 63.0f)) * lightRotationMatrix * lightScaleMatrix;
    lightModelMatrix = worldMatrix * lightModelMatrix;
    setWorldMatrix(shader, lightModelMatrix);

    // Drawing the right front top light
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // left back top light
    lightRotationMatrix = rotate(iMat, radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * rotate(iMat, radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    lightModelMatrix = translate(iMat, glm::vec3(-83.0f, 79.0f, -63.0f)) * lightRotationMatrix * lightScaleMatrix;
    lightModelMatrix = worldMatrix * lightModelMatrix;
    setWorldMatrix(shader, lightModelMatrix);

    // Drawing the left back top light
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // left front top light
    lightRotationMatrix = rotate(iMat, radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * rotate(iMat, radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    lightModelMatrix = translate(iMat, glm::vec3(-83.0f, 79.0f, 63.0f)) * lightRotationMatrix * lightScaleMatrix;
    lightModelMatrix = worldMatrix * lightModelMatrix;
    setWorldMatrix(shader, lightModelMatrix);

    // Drawing the left front top light
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
