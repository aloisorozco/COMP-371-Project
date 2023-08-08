#define GLEW_STATIC 1  
#include <GL/glew.h>  

#include <GLFW/glfw3.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

#include "common.h"


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
    glm::mat4 courtRightModelMatrix = glm::translate(iMat, glm::vec3(15.0f, -0.5f, 0.0f)) * glm::scale(iMat, glm::vec3(2.0f, 0.1f, 74.0f));
    courtRightModelMatrix = worldMatrix * courtRightModelMatrix;
    setWorldMatrix(sceneShaderProgram, courtRightModelMatrix);

    // Drawing the court line right
    setMaterial(sceneShaderProgram, 0.3f, 0.6f, 0.1f, 10.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 1.0f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Court line left model matrix
    glm::mat4 courtLeftModelMatrix = glm::translate(iMat, glm::vec3(-15.0f, -0.5f, 0.0f)) * glm::scale(iMat, glm::vec3(2.0f, 0.1f, 74.0f));
    courtLeftModelMatrix = worldMatrix * courtLeftModelMatrix;
    setWorldMatrix(sceneShaderProgram, courtLeftModelMatrix);

    // Drawing the court line left
    setMaterial(sceneShaderProgram, 0.3f, 0.6f, 0.1f, 10.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 1.0f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Court very bottom line model matrix
    glm::mat4 courtBottom1ModelMatrix = glm::translate(iMat, glm::vec3(0.0f, -0.5f, 36.0f)) * glm::scale(iMat, glm::vec3(32.0f, 0.1f, 2.0f));
    courtBottom1ModelMatrix = worldMatrix * courtBottom1ModelMatrix;
    setWorldMatrix(sceneShaderProgram, courtBottom1ModelMatrix);

    // Drawing the court very bottom line
    setMaterial(sceneShaderProgram, 0.3f, 0.6f, 0.1f, 10.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 1.0f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Court very top line model matrix
    glm::mat4 courtTop1ModelMatrix = glm::translate(iMat, glm::vec3(0.0f, -0.5f, -36.0f)) * glm::scale(iMat, glm::vec3(32.0f, 0.1f, 2.0f));
    courtTop1ModelMatrix = worldMatrix * courtTop1ModelMatrix;
    setWorldMatrix(sceneShaderProgram, courtTop1ModelMatrix);

    // Drawing the court very top line
    setMaterial(sceneShaderProgram, 0.3f, 0.6f, 0.1f, 10.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 1.0f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Court close bottom line model matrix
    glm::mat4 courtBottom2ModelMatrix = glm::translate(iMat, glm::vec3(0.0f, -0.5f, 26.0f)) * glm::scale(iMat, glm::vec3(32.0f, 0.1f, 2.0f));
    courtBottom2ModelMatrix = worldMatrix * courtBottom2ModelMatrix;
    setWorldMatrix(sceneShaderProgram, courtBottom2ModelMatrix);

    // Drawing the court close bottom line
    setMaterial(sceneShaderProgram, 0.3f, 0.6f, 0.1f, 10.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 1.0f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Court close top line model matrix
    glm::mat4 courtTop2ModelMatrix = glm::translate(iMat, glm::vec3(0.0f, -0.5f, -26.0f)) * glm::scale(iMat, glm::vec3(32.0f, 0.1f, 2.0f));
    courtTop2ModelMatrix = worldMatrix * courtTop2ModelMatrix;
    setWorldMatrix(sceneShaderProgram, courtTop2ModelMatrix);

    // Drawing the court close top line
    setMaterial(sceneShaderProgram, 0.3f, 0.6f, 0.1f, 10.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 1.0f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Court middle line model matrix
    glm::mat4 courtMiddleModelMatrix = glm::translate(iMat, glm::vec3(0.0f, -0.5f, 0.0f)) * glm::scale(iMat, glm::vec3(2.0f, 0.1f, 50.0f));
    courtMiddleModelMatrix = worldMatrix * courtMiddleModelMatrix;
    setWorldMatrix(sceneShaderProgram, courtMiddleModelMatrix);

    // Drawing the court middle line
    setMaterial(sceneShaderProgram, 0.3f, 0.6f, 0.1f, 10.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 1.0f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Court base model matrix
    glm::mat4 courtModelMatrix = glm::translate(iMat, glm::vec3(0.0f, -1.0f, 0.0f)) * glm::scale(iMat, glm::vec3(36.0f, 0.5f, 78.0f));
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
    glm::mat4 backgroundGroundModelMatrix = glm::translate(iMat, glm::vec3(0.0f, -2.0f, 0.0f)) * glm::scale(iMat, glm::vec3(300.0f, 0.5f, 300.0f));
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
    glm::mat4 netRightModelMatrix = glm::translate(iMat, glm::vec3(17.0f, 5.0f, 0.0f)) * glm::scale(iMat, glm::vec3(2.0f, 12.0f, 2.0f));
    netRightModelMatrix = worldMatrix * netRightModelMatrix;
    setWorldMatrix(sceneShaderProgram, netRightModelMatrix);

    // Drawing the net right post
    setMaterial(sceneShaderProgram, 0.3f, 0.3f, 1.0f, 20.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 0.161f, 0.396f, 0.0235f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Net left post model matrix
    glm::mat4 netLeftModelMatrix = glm::translate(iMat, glm::vec3(-17.0f, 5.0f, 0.0f)) * glm::scale(iMat, glm::vec3(2.0f, 12.0f, 2.0f));
    netLeftModelMatrix = worldMatrix * netLeftModelMatrix;
    setWorldMatrix(sceneShaderProgram, netLeftModelMatrix);

    // Drawing the net left post
    setMaterial(sceneShaderProgram, 0.3f, 0.3f, 1.0f, 20.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 0.161f, 0.396f, 0.0235f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Net top model matrix
    glm::mat4 netTopModelMatrix = glm::translate(iMat, glm::vec3(0.0f, 10.0f, 0.0f)) * glm::scale(iMat, glm::vec3(32.0f, 2.0f, 0.2f));
    netTopModelMatrix = worldMatrix * netTopModelMatrix;
    setWorldMatrix(sceneShaderProgram, netTopModelMatrix);

    // Drawing the net top
    setMaterial(sceneShaderProgram, 0.4f, 0.8f, 0.1f, 10.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 0.94f, 0.94f, 0.94f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Net bottom model matrix
    glm::mat4 netBottomModelMatrix = glm::translate(iMat, glm::vec3(0.0f, 3.0f, 0.0f)) * glm::scale(iMat, glm::vec3(32.0f, 0.5f, 0.2f));
    netBottomModelMatrix = worldMatrix * netBottomModelMatrix;
    setWorldMatrix(sceneShaderProgram, netBottomModelMatrix);

    noTexture(sceneShaderProgram);

    // Drawing the net bottom
    setMaterial(sceneShaderProgram, 0.4f, 0.8f, 0.1f, 10.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 0.22f, 0.22f, 0.22f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Net middle model matrix
    glm::mat4 netMiddleModelMatrix = glm::translate(iMat, glm::vec3(0.0f, 7.0f, 0.0f)) * glm::scale(iMat, glm::vec3(1.0f, 8.0f, 0.2f));
    netMiddleModelMatrix = worldMatrix * netMiddleModelMatrix;
    setWorldMatrix(sceneShaderProgram, netMiddleModelMatrix);

    // Drawing the net middle
    setMaterial(sceneShaderProgram, 0.4f, 0.8f, 0.1f, 10.0f, toggleShadows);
    setUniqueColor(sceneShaderProgram, 0.94f, 0.94f, 0.94f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Net grid model matrix
    glm::mat4 netGridModelMatrix = glm::translate(iMat, glm::vec3(0.0f, 7.0f, 0.0f)) * glm::scale(iMat, glm::vec3(17.0f, 3.8f, 1.0f));
    netGridModelMatrix = worldMatrix * netGridModelMatrix;
    setWorldMatrix(sceneShaderProgram, netGridModelMatrix);

    // Drawing the net grid
    glBindVertexArray(netGridVao);
    setUniqueColor(sceneShaderProgram, 1.0f, 1.0f, 1.0f); // For grid, set uniqueColor to neutral color (white)
    setMaterial(sceneShaderProgram, 1.0f, 0.0f, 0.0f, 0.0f, toggleShadows);
    glDrawArrays(GL_LINES, 0, 260);

    glBindVertexArray(0);
}


void drawStadium(glm::mat4 worldMatrix, int cubeVao, int sceneShaderProgram) 
{
    noTexture(sceneShaderProgram);
    setMaterial(sceneShaderProgram, 0.4f, 0.8f, 0.1f, 10.0f, toggleShadows);
    glBindVertexArray(cubeVao);

    // Far wall model matrix
    glm::mat4 farWallModelMatrix = glm::translate(iMat, glm::vec3(0.0f, 5.0f, -65.0f)) * glm::scale(iMat, glm::vec3(160.0f, 13.0f, 10.0f));
    farWallModelMatrix = worldMatrix * farWallModelMatrix;
    setWorldMatrix(sceneShaderProgram, farWallModelMatrix);

    // Drawing the far wall
    setUniqueColor(sceneShaderProgram, 0.039f, 0.378f, 0.039f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Near wall model matrix
    glm::mat4 nearWallModelMatrix = glm::translate(iMat, glm::vec3(0.0f, 5.0f, 65.0f)) * glm::scale(iMat, glm::vec3(160.0f, 13.0f, 10.0f));
    nearWallModelMatrix = worldMatrix * nearWallModelMatrix;
    setWorldMatrix(sceneShaderProgram, nearWallModelMatrix);

    // Drawing the near wall
    setUniqueColor(sceneShaderProgram, 0.039f, 0.378f, 0.039f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

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

    // Right wall model matrix
    glm::mat4 rightWallModelMatrix = glm::translate(iMat, glm::vec3(85.0f, 13.0f, 0.0f)) * glm::scale(iMat, glm::vec3(10.0f, 29.0f, 120.0f));
    rightWallModelMatrix = worldMatrix * rightWallModelMatrix;
    setWorldMatrix(sceneShaderProgram, rightWallModelMatrix);

    // Drawing the right wall
    setUniqueColor(sceneShaderProgram, 0.039f, 0.378f, 0.039f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Left wall model matrix
    glm::mat4 leftWallModelMatrix = glm::translate(iMat, glm::vec3(-85.0f, 13.0f, 0.0f)) * glm::scale(iMat, glm::vec3(10.0f, 29.0f, 120.0f));
    leftWallModelMatrix = worldMatrix * leftWallModelMatrix;
    setWorldMatrix(sceneShaderProgram, leftWallModelMatrix);

    // Drawing the left wall
    setUniqueColor(sceneShaderProgram, 0.039f, 0.378f, 0.039f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Corner 1 model matrix (near right)
    glm::mat4 corner1ModelMatrix = glm::translate(iMat, glm::vec3(85.0f, 9.0f, 65.0f)) * glm::scale(iMat, glm::vec3(10.0f, 21.0f, 10.0f));
    corner1ModelMatrix = worldMatrix * corner1ModelMatrix;
    setWorldMatrix(sceneShaderProgram, corner1ModelMatrix);

    // Drawing the corner 1
    setUniqueColor(sceneShaderProgram, 0.039f, 0.378f, 0.039f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Corner 2 model matrix (near left)
    glm::mat4 corner2ModelMatrix = glm::translate(iMat, glm::vec3(-85.0f, 9.0f, 65.0f)) * glm::scale(iMat, glm::vec3(10.0f, 21.0f, 10.0f));
    corner2ModelMatrix = worldMatrix * corner2ModelMatrix;
    setWorldMatrix(sceneShaderProgram, corner2ModelMatrix);

    // Drawing the corner 2
    setUniqueColor(sceneShaderProgram, 0.039f, 0.378f, 0.039f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Corner 3 model matrix (far right)
    glm::mat4 corner3ModelMatrix = glm::translate(iMat, glm::vec3(85.0f, 9.0f, -65.0f)) * glm::scale(iMat, glm::vec3(10.0f, 21.0f, 10.0f));
    corner3ModelMatrix = worldMatrix * corner3ModelMatrix;
    setWorldMatrix(sceneShaderProgram, corner3ModelMatrix);

    // Drawing the corner 3
    setUniqueColor(sceneShaderProgram, 0.039f, 0.378f, 0.039f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Corner 4 model matrix (far left)
    glm::mat4 corner4ModelMatrix = glm::translate(iMat, glm::vec3(-85.0f, 9.0f, -65.0f)) * glm::scale(iMat, glm::vec3(10.0f, 21.0f, 10.0f));
    corner4ModelMatrix = worldMatrix * corner4ModelMatrix;
    setWorldMatrix(sceneShaderProgram, corner4ModelMatrix);

    // Drawing the corner 4
    setUniqueColor(sceneShaderProgram, 0.039f, 0.378f, 0.039f);
    glDrawArrays(GL_TRIANGLES, 0, 36);
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

void drawSkyBox(glm::mat4 worldMatrix, int sceneShaderProgram, int cubeVao, int skyId) {
    // We want to see the inside of the sky box
    glCullFace(GL_FRONT);
    // Sky box matrix
    glm::mat4 cubeWorldMatrix = glm::translate(iMat, glm::vec3(0.0f, 0.0f, 0.0f)) * glm::scale(iMat, glm::vec3(300.0f, 300.0f, 300.0f));
    cubeWorldMatrix = worldMatrix * cubeWorldMatrix;

    GLuint skyBoxLocation = glGetUniformLocation(sceneShaderProgram, "isSkyBox");
    glUniform1i(skyBoxLocation, 1);

    // Drawing the sky box
    glBindVertexArray(cubeVao);
    setWorldMatrix(sceneShaderProgram, cubeWorldMatrix);
    setUniqueColor(sceneShaderProgram, 0.294f, 0.6745f, 0.925f);
    setTexture(sceneShaderProgram, skyId, 0, toggleTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glUniform1i(skyBoxLocation, 0);
    glCullFace(GL_BACK);
}

