#define GLEW_STATIC 1  
#include <GL/glew.h>  

#include <GLFW/glfw3.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

#include "common.h"


// --- DRAWING AXIS ---
void drawGridAndAxisShadow(glm::mat4 worldMatrix, int cubeVao, int gridVao, int shadowShaderProgram)
{
    glUseProgram(shadowShaderProgram);
    // --- GRID ---
    // Grid model matrix
    glm::mat4 gridModelMatrix = glm::translate(iMat, glm::vec3(0.0f, -1.0f, 0.0f)) * glm::scale(iMat, glm::vec3(50.0f, 1.0f, 50.0f));
    gridModelMatrix = worldMatrix * gridModelMatrix;
    setWorldMatrix(shadowShaderProgram, gridModelMatrix);

    // Drawing the grid
    glBindVertexArray(gridVao);
    glDrawArrays(GL_LINES, 0, 404);
    glBindVertexArray(0);

    // --- AXIS ---
    glBindVertexArray(cubeVao);
    // -- X --
    // X-axis cube model matrix
    glm::mat4 xCubeModelMatrix = glm::translate(iMat, glm::vec3(2.5f, 0.25f, 0.25f)) * glm::scale(iMat, glm::vec3(5.0f, 0.5f, 0.5f));
    xCubeModelMatrix = worldMatrix * xCubeModelMatrix;
    //(shadowShaderProgram, xCubeModelMatrix);

    // Drawing the X-axis cube
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // -- Y --
    // Y-axis cube model matrix
    glm::mat4 yCubeModelMatrix = glm::translate(iMat, glm::vec3(0.25f, 2.5f, 0.25f)) * glm::scale(iMat, glm::vec3(0.5f, 5.0f, 0.5f));
    yCubeModelMatrix = worldMatrix * yCubeModelMatrix;
    setWorldMatrix(shadowShaderProgram, yCubeModelMatrix);

    // Drawing the Y-axis cube
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // -- Z --
    // Z-axis cube model matrix
    glm::mat4 zCubeModelMatrix = glm::translate(iMat, glm::vec3(0.25f, 0.25f, 2.5f)) * glm::scale(iMat, glm::vec3(0.5f, 0.5f, 5.0f));
    zCubeModelMatrix = worldMatrix * zCubeModelMatrix;
    setWorldMatrix(shadowShaderProgram, zCubeModelMatrix);

    // Drawing the Z-axis cube
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

// --- DRAWING MAIN MODEL ---
void drawModelShadow(glm::mat4 worldMatrix, int racketGridVao, int cubeVao, int shadowShaderProgram, glm::vec3 upArmInitialPosition, float upArmXAngle, float upArmYAngle)
{
    glUseProgram(shadowShaderProgram);
    glBindVertexArray(cubeVao);

    // -- UPPER ARM --
    // Upper arm cube model matrix
    glm::mat4 upperArmTranslate = glm::translate(iMat, upArmInitialPosition + upArmPosition);
    glm::mat4 upperArmScale = glm::scale(iMat, glm::vec3(1.536f, 6.144f, 1.536f) * upArmScale);
    glm::mat4 upperArmInitialRotation = glm::rotate(iMat, glm::radians(-30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 upperArmRotation = glm::rotate(iMat, glm::radians(upArmXAngle), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(iMat, glm::radians(upArmYAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 upperArmModelMatrix = worldMatrix * upperArmTranslate * upperArmRotation * upperArmInitialRotation * upperArmScale;
    glm::mat4 upperArmHierarchy = upperArmTranslate * upperArmRotation;
    setWorldMatrix(shadowShaderProgram, upperArmModelMatrix);

    // Drawing the upper arm model cube
    glDrawArrays(renderModeModel, 0, 36);

    // -- LOWER ARM --
    // Lower arm cube model matrix
    glm::mat4 lowerArmTranslate = glm::translate(iMat, glm::vec3(1.4336f, 5.2736f, 0.0f) * upArmScale);
    glm::mat4 lowerArmScale = glm::scale(iMat, glm::vec3(1.536f, 6.144f, 1.536f) * upArmScale);
    glm::mat4 lowerArmRotation = glm::translate(iMat, glm::vec3(0.0f, -3.07f, 0.0f) * upArmScale) * glm::rotate(iMat, glm::radians(lowArmZAngle), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::rotate(iMat, glm::radians(lowArmXAngle), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::translate(iMat, glm::vec3(0.0f, 3.07f, 0.0f) * upArmScale);
    glm::mat4 lowerArmModelMatrix = worldMatrix * upperArmHierarchy * lowerArmTranslate * lowerArmRotation * lowerArmScale;
    glm::mat4 lowerArmHierarchy = lowerArmTranslate * lowerArmRotation;
    setWorldMatrix(shadowShaderProgram, lowerArmModelMatrix);

    // Drawing the lower arm model cube
    glDrawArrays(renderModeModel, 0, 36);

    // -- RACKET
    glBindVertexArray(cubeVao);

    // -- RACKET HANDLE --
    // Racket handle cube model matrix
    glm::mat4 racketHandleTranslate = glm::translate(iMat, glm::vec3(0.0f, 4.6364f, 0.0f) * upArmScale);
    glm::mat4 racketHandleScale = glm::scale(iMat, glm::vec3(0.8f, 4.12f, 0.8f) * upArmScale);
    glm::mat4 wristRotation = glm::translate(iMat, glm::vec3(0.0f, -3.07, 0.0f) * upArmScale) * glm::rotate(iMat, glm::radians(wristZAngle), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::rotate(iMat, glm::radians(wristYAngle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(iMat, glm::radians(wristXAngle), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::translate(iMat, glm::vec3(0.0f, 3.07f, 0.0f) * upArmScale);
    glm::mat4 racketHandleModelMatrix = worldMatrix * upperArmHierarchy * lowerArmHierarchy * racketHandleTranslate * wristRotation * racketHandleScale;
    glm::mat4 wristHierarchy = racketHandleTranslate * wristRotation;
    setWorldMatrix(shadowShaderProgram, racketHandleModelMatrix);

    // Drawing the racket handle model cube
    glDrawArrays(renderModeModel, 0, 36);

    // -- RACKET BOTTOM RIGHT --
    // Racket bottom right cube model matrix
    glm::mat4 racketBottomRightTranslate = glm::translate(iMat, glm::vec3(1.1f, 3.09f, 0.0f) * upArmScale);
    glm::mat4 racketBottomRightScale = glm::scale(iMat, glm::vec3(0.8f, 4.12f, 0.8f) * upArmScale);
    glm::mat4 racketBottomRightInitialRotation = glm::rotate(iMat, glm::radians(-40.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 racketBottomRightModelMatrix = worldMatrix * upperArmHierarchy * lowerArmHierarchy * wristHierarchy * racketBottomRightTranslate * racketBottomRightInitialRotation * racketBottomRightScale;
    setWorldMatrix(shadowShaderProgram, racketBottomRightModelMatrix);

    // Drawing the racket bottom model cube
    glDrawArrays(renderModeModel, 0, 36);

    // -- RACKET BOTTOM LEFT --
    // Racket bottom left cube model matrix
    glm::mat4 racketBottomLeftTranslate = glm::translate(iMat, glm::vec3(-1.1f, 3.09f, 0.0f) * upArmScale);
    glm::mat4 racketBottomLeftScale = glm::scale(iMat, glm::vec3(0.8f, 4.12f, 0.8f) * upArmScale);
    glm::mat4 racketBottomLeftInitialRotation = glm::rotate(iMat, glm::radians(40.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 racketBottomLeftModelMatrix = worldMatrix * upperArmHierarchy * lowerArmHierarchy * wristHierarchy * racketBottomLeftTranslate * racketBottomLeftInitialRotation * racketBottomLeftScale;
    setWorldMatrix(shadowShaderProgram, racketBottomLeftModelMatrix);

    // Drawing the racket bottom left model cube
    glDrawArrays(renderModeModel, 0, 36);

    // -- RACKET RIGHT --
    // Racket right cube model matrix
    glm::mat4 racketRightTranslate = glm::translate(iMat, glm::vec3(2.318f, 7.63f, 0.0f) * upArmScale);
    glm::mat4 racketRightScale = glm::scale(iMat, glm::vec3(0.8f, 6.42f, 0.8f) * upArmScale);
    glm::mat4 racketRightModelMatrix = worldMatrix * upperArmHierarchy * lowerArmHierarchy * wristHierarchy * racketRightTranslate * racketRightScale;
    setWorldMatrix(shadowShaderProgram, racketRightModelMatrix);

    // Drawing the racket right model cube
    glDrawArrays(renderModeModel, 0, 36);

    // -- RACKET LEFT --
    // Racket left cube model matrix
    glm::mat4 racketLeftTranslate = glm::translate(iMat, glm::vec3(-2.318f, 7.63f, 0.0f) * upArmScale);
    glm::mat4 racketLeftScale = glm::scale(iMat, glm::vec3(0.8f, 6.42f, 0.8f) * upArmScale);
    glm::mat4 racketLeftModelMatrix = worldMatrix * upperArmHierarchy * lowerArmHierarchy * wristHierarchy * racketLeftTranslate * racketLeftScale;
    setWorldMatrix(shadowShaderProgram, racketLeftModelMatrix);

    // Drawing the racket left model cube
    glDrawArrays(renderModeModel, 0, 36);

    // -- RACKET TOP RIGHT --
    // Racket top right cube model matrix
    glm::mat4 racketTopRightTranslate = glm::translate(iMat, glm::vec3(1.87f, 11.19f, 0.0f) * upArmScale);
    glm::mat4 racketTopRightScale = glm::scale(iMat, glm::vec3(0.8f, 1.83f, 0.8f) * upArmScale);
    glm::mat4 racketTopRightInitialRotation = glm::rotate(iMat, glm::radians(40.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 racketTopRightModelMatrix = worldMatrix * upperArmHierarchy * lowerArmHierarchy * wristHierarchy * racketTopRightTranslate * racketTopRightInitialRotation * racketTopRightScale;
    setWorldMatrix(shadowShaderProgram, racketTopRightModelMatrix);

    // Drawing the racket bottom model cube
    glDrawArrays(renderModeModel, 0, 36);

    // -- RACKET TOP LEFT --
    // Racket top left cube model matrix
    glm::mat4 racketTopLeftTranslate = glm::translate(iMat, glm::vec3(-1.87f, 11.19f, 0.0f) * upArmScale);
    glm::mat4 racketTopLeftScale = glm::scale(iMat, glm::vec3(0.8f, 1.83f, 0.8f) * upArmScale);
    glm::mat4 racketTopLeftInitialRotation = glm::rotate(iMat, glm::radians(-40.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 racketTopLeftModelMatrix = worldMatrix * upperArmHierarchy * lowerArmHierarchy * wristHierarchy * racketTopLeftTranslate * racketTopLeftInitialRotation * racketTopLeftScale;
    setWorldMatrix(shadowShaderProgram, racketTopLeftModelMatrix);

    // Drawing the racket top left model cube
    glDrawArrays(renderModeModel, 0, 36);

    // -- RACKET TOP --
    // Racket top cube model matrix
    glm::mat4 racketTopTranslate = glm::translate(iMat, glm::vec3(0.0f, 11.83f, 0.0f) * upArmScale);
    glm::mat4 racketTopScale = glm::scale(iMat, glm::vec3(3.15f, 0.8f, 0.8f) * upArmScale);
    glm::mat4 racketTopModelMatrix = worldMatrix * upperArmHierarchy * lowerArmHierarchy * wristHierarchy * racketTopTranslate * racketTopScale;
    setWorldMatrix(shadowShaderProgram, racketTopModelMatrix);

    // Drawing the racket top model cube
    glDrawArrays(renderModeModel, 0, 36);

    // -- RACKET MIDDLE --
    // Racket middle cube model matrix
    glm::mat4 racketMiddleTranslate = glm::translate(iMat, glm::vec3(0.0f, 3.73f, 0.0f) * upArmScale);
    glm::mat4 racketMiddleScale = glm::scale(iMat, glm::vec3(3.15f, 0.5f, 0.5f) * upArmScale);
    glm::mat4 racketMiddleModelMatrix = worldMatrix * upperArmHierarchy * lowerArmHierarchy * wristHierarchy * racketMiddleTranslate * racketMiddleScale;
    setWorldMatrix(shadowShaderProgram, racketMiddleModelMatrix);

    // Drawing the racket middle model cube
    glDrawArrays(renderModeModel, 0, 36);

    //Exiting cube drawing :(
    glBindVertexArray(0);

    // -- RACKET GRID --
    // Racket grid matrix
    glm::mat4 racketGridTranslate = glm::translate(iMat, glm::vec3(0.0f, 1.73f, 0.0f) * upArmScale);
    glm::mat4 racketGridScale = glm::scale(iMat, glm::vec3(1.0f, 1.3f, 1.0f) * upArmScale);
    glm::mat4 racketGridMatrix = worldMatrix * upperArmHierarchy * lowerArmHierarchy * wristHierarchy * racketGridTranslate * racketGridScale;
    setWorldMatrix(shadowShaderProgram, racketGridMatrix);

    // Drawing the racket grid
    glBindVertexArray(racketGridVao);
    glDrawArrays(renderModeRacketGrid, 0, 40);
    glBindVertexArray(0);
}

void drawCourtShadow(glm::mat4 worldMatrix, int cubeVao, int shadowShaderProgram)
{
    glUseProgram(shadowShaderProgram);
    glBindVertexArray(cubeVao);

    // Court base model matrix
    glm::mat4 courtModelMatrix = glm::translate(iMat, glm::vec3(0.0f, -1.0f, 0.0f)) * glm::scale(iMat, glm::vec3(36.0f, 0.5f, 78.0f));
    courtModelMatrix = worldMatrix * courtModelMatrix;
    setWorldMatrix(shadowShaderProgram, courtModelMatrix);

    // Drawing the court base
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    // Set shader back to color only
    glBindVertexArray(cubeVao);

    // Court line right model matrix
    glm::mat4 courtRightModelMatrix = glm::translate(iMat, glm::vec3(15.0f, -0.5f, 0.0f)) * glm::scale(iMat, glm::vec3(2.0f, 0.0f, 74.0f));
    courtRightModelMatrix = worldMatrix * courtRightModelMatrix;
    setWorldMatrix(shadowShaderProgram, courtRightModelMatrix);

    // Drawing the court line right
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Court line left model matrix
    glm::mat4 courtLeftModelMatrix = glm::translate(iMat, glm::vec3(-15.0f, -0.5f, 0.0f)) * glm::scale(iMat, glm::vec3(2.0f, 0.0f, 74.0f));
    courtLeftModelMatrix = worldMatrix * courtLeftModelMatrix;
    setWorldMatrix(shadowShaderProgram, courtLeftModelMatrix);

    // Drawing the court line left
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Court very bottom line model matrix
    glm::mat4 courtBottom1ModelMatrix = glm::translate(iMat, glm::vec3(0.0f, -0.5f, 36.0f)) * glm::scale(iMat, glm::vec3(32.0f, 0.0f, 2.0f));
    courtBottom1ModelMatrix = worldMatrix * courtBottom1ModelMatrix;
    setWorldMatrix(shadowShaderProgram, courtBottom1ModelMatrix);

    // Drawing the court very bottom line
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Court very top line model matrix
    glm::mat4 courtTop1ModelMatrix = glm::translate(iMat, glm::vec3(0.0f, -0.5f, -36.0f)) * glm::scale(iMat, glm::vec3(32.0f, 0.0f, 2.0f));
    courtTop1ModelMatrix = worldMatrix * courtTop1ModelMatrix;
    setWorldMatrix(shadowShaderProgram, courtTop1ModelMatrix);

    // Drawing the court very top line
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Court close bottom line model matrix
    glm::mat4 courtBottom2ModelMatrix = glm::translate(iMat, glm::vec3(0.0f, -0.5f, 26.0f)) * glm::scale(iMat, glm::vec3(32.0f, 0.0f, 2.0f));
    courtBottom2ModelMatrix = worldMatrix * courtBottom2ModelMatrix;
    setWorldMatrix(shadowShaderProgram, courtBottom2ModelMatrix);

    // Drawing the court close bottom line
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Court close top line model matrix
    glm::mat4 courtTop2ModelMatrix = glm::translate(iMat, glm::vec3(0.0f, -0.5f, -26.0f)) * glm::scale(iMat, glm::vec3(32.0f, 0.0f, 2.0f));
    courtTop2ModelMatrix = worldMatrix * courtTop2ModelMatrix;
    setWorldMatrix(shadowShaderProgram, courtTop2ModelMatrix);

    // Drawing the court close top line
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Court middle line model matrix
    glm::mat4 courtMiddleModelMatrix = glm::translate(iMat, glm::vec3(0.0f, -0.5f, 0.0f)) * glm::scale(iMat, glm::vec3(2.0f, 0.0f, 50.0f));
    courtMiddleModelMatrix = worldMatrix * courtMiddleModelMatrix;
    setWorldMatrix(shadowShaderProgram, courtMiddleModelMatrix);

    // Drawing the court middle line
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawNetShadow(glm::mat4 worldMatrix, int netGridVao, int cubeVao, int shadowShaderProgram)
{
    glUseProgram(shadowShaderProgram);
    glBindVertexArray(cubeVao);
    // Net right post model matrix
    glm::mat4 netRightModelMatrix = glm::translate(iMat, glm::vec3(20.0f, 3.0f, 0.0f)) * glm::scale(iMat, glm::vec3(1.0f, 8.5f, 1.0f));
    netRightModelMatrix = worldMatrix * netRightModelMatrix;
    setWorldMatrix(shadowShaderProgram, netRightModelMatrix);

    // Drawing the net right post
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Net left post model matrix
    glm::mat4 netLeftModelMatrix = glm::translate(iMat, glm::vec3(-20.0f, 3.0f, 0.0f)) * glm::scale(iMat, glm::vec3(1.0f, 8.5f, 1.0f));
    netLeftModelMatrix = worldMatrix * netLeftModelMatrix;
    setWorldMatrix(shadowShaderProgram, netLeftModelMatrix);

    // Drawing the net left post
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Net top model matrix
    glm::mat4 netTopModelMatrix = glm::translate(iMat, glm::vec3(0.0f, 6.0f, 0.0f)) * glm::scale(iMat, glm::vec3(40.0f, 1.0f, 0.2f));
    netTopModelMatrix = worldMatrix * netTopModelMatrix;
    setWorldMatrix(shadowShaderProgram, netTopModelMatrix);

    // Drawing the net top
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Net bottom model matrix
    glm::mat4 netBottomModelMatrix = glm::translate(iMat, glm::vec3(0.0f, 1.5f, 0.0f)) * glm::scale(iMat, glm::vec3(40.0f, 0.5f, 0.2f));
    netBottomModelMatrix = worldMatrix * netBottomModelMatrix;
    setWorldMatrix(shadowShaderProgram, netBottomModelMatrix);

    // Drawing the net bottom
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Net middle model matrix
    glm::mat4 netMiddleModelMatrix = glm::translate(iMat, glm::vec3(0.0f, 3.5f, 0.0f)) * glm::scale(iMat, glm::vec3(1.0f, 4.5f, 0.2f));
    netMiddleModelMatrix = worldMatrix * netMiddleModelMatrix;
    setWorldMatrix(shadowShaderProgram, netMiddleModelMatrix);

    // Drawing the net middle
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Net grid model matrix
    glm::mat4 netGridModelMatrix = glm::translate(iMat, glm::vec3(0.0f, 3.5f, 0.0f)) * glm::scale(iMat, glm::vec3(20.0f, 2.0f, 1.0f));
    netGridModelMatrix = worldMatrix * netGridModelMatrix;
    setWorldMatrix(shadowShaderProgram, netGridModelMatrix);

    // Drawing the net grid
    glBindVertexArray(netGridVao);
    glDrawArrays(GL_LINES, 0, 260);
}

void drawStadiumShadow(glm::mat4 worldMatrix, int cubeVao, int shadowShaderProgram)
{
    glBindVertexArray(cubeVao);

    // Far wall model matrix
    glm::mat4 farWallModelMatrix = glm::translate(iMat, glm::vec3(0.0f, 5.0f, -65.0f)) * glm::scale(iMat, glm::vec3(160.0f, 13.0f, 10.0f));
    farWallModelMatrix = worldMatrix * farWallModelMatrix;
    setWorldMatrix(shadowShaderProgram, farWallModelMatrix);

    // Drawing the far wall
    setUniqueColor(shadowShaderProgram, 0.039f, 0.378f, 0.039f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Near wall model matrix
    glm::mat4 nearWallModelMatrix = glm::translate(iMat, glm::vec3(0.0f, 5.0f, 65.0f)) * glm::scale(iMat, glm::vec3(160.0f, 13.0f, 10.0f));
    nearWallModelMatrix = worldMatrix * nearWallModelMatrix;
    setWorldMatrix(shadowShaderProgram, nearWallModelMatrix);

    // Drawing the near wall
    setUniqueColor(shadowShaderProgram, 0.039f, 0.378f, 0.039f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Right stand 1 model matrix
    glm::mat4 rStand1ModelMatrix = glm::translate(iMat, glm::vec3(55.0f, 2.0f, 0.0f)) * glm::scale(iMat, glm::vec3(10.0f, 7.0f, 120.0f));
    rStand1ModelMatrix = worldMatrix * rStand1ModelMatrix;
    setWorldMatrix(shadowShaderProgram, rStand1ModelMatrix);

    // Drawing the right stand 1
    setUniqueColor(shadowShaderProgram, 0.8f, 0.8f, 0.8f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Right stand 2 model matrix
    glm::mat4 rStand2ModelMatrix = glm::translate(iMat, glm::vec3(65.0f, 5.0f, 0.0f)) * glm::scale(iMat, glm::vec3(10.0f, 13.0f, 120.0f));
    rStand2ModelMatrix = worldMatrix * rStand2ModelMatrix;
    setWorldMatrix(shadowShaderProgram, rStand2ModelMatrix);

    // Drawing the right stand 2
    setUniqueColor(shadowShaderProgram, 0.8f, 0.8f, 0.8f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Right stand 3 model matrix
    glm::mat4 rStand3ModelMatrix = glm::translate(iMat, glm::vec3(75.0f, 8.0f, 0.0f)) * glm::scale(iMat, glm::vec3(10.0f, 19.0f, 120.0f));
    rStand3ModelMatrix = worldMatrix * rStand3ModelMatrix;
    setWorldMatrix(shadowShaderProgram, rStand3ModelMatrix);

    // Drawing the right stand 3
    setUniqueColor(shadowShaderProgram, 0.8f, 0.8f, 0.8f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Left stand 1 model matrix
    glm::mat4 lStand1ModelMatrix = glm::translate(iMat, glm::vec3(-55.0f, 2.0f, 0.0f)) * glm::scale(iMat, glm::vec3(10.0f, 7.0f, 120.0f));
    lStand1ModelMatrix = worldMatrix * lStand1ModelMatrix;
    setWorldMatrix(shadowShaderProgram, lStand1ModelMatrix);

    // Drawing the left stand 1
    setUniqueColor(shadowShaderProgram, 0.8f, 0.8f, 0.8f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Left stand 2 model matrix
    glm::mat4 lStand2ModelMatrix = glm::translate(iMat, glm::vec3(-65.0f, 5.0f, 0.0f)) * glm::scale(iMat, glm::vec3(10.0f, 13.0f, 120.0f));
    lStand2ModelMatrix = worldMatrix * lStand2ModelMatrix;
    setWorldMatrix(shadowShaderProgram, lStand2ModelMatrix);

    // Drawing the left stand 2
    setUniqueColor(shadowShaderProgram, 0.8f, 0.8f, 0.8f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Left stand 3 model matrix
    glm::mat4 lStand3ModelMatrix = glm::translate(iMat, glm::vec3(-75.0f, 8.0f, 0.0f)) * glm::scale(iMat, glm::vec3(10.0f, 19.0f, 120.0f));
    lStand3ModelMatrix = worldMatrix * lStand3ModelMatrix;
    setWorldMatrix(shadowShaderProgram, lStand3ModelMatrix);

    // Drawing the left stand 3
    setUniqueColor(shadowShaderProgram, 0.8f, 0.8f, 0.8f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Right wall model matrix
    glm::mat4 rightWallModelMatrix = glm::translate(iMat, glm::vec3(85.0f, 13.0f, 0.0f)) * glm::scale(iMat, glm::vec3(10.0f, 29.0f, 120.0f));
    rightWallModelMatrix = worldMatrix * rightWallModelMatrix;
    setWorldMatrix(shadowShaderProgram, rightWallModelMatrix);

    // Drawing the right wall
    setUniqueColor(shadowShaderProgram, 0.039f, 0.378f, 0.039f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Left wall model matrix
    glm::mat4 leftWallModelMatrix = glm::translate(iMat, glm::vec3(-85.0f, 13.0f, 0.0f)) * glm::scale(iMat, glm::vec3(10.0f, 29.0f, 120.0f));
    leftWallModelMatrix = worldMatrix * leftWallModelMatrix;
    setWorldMatrix(shadowShaderProgram, leftWallModelMatrix);

    // Drawing the left wall
    setUniqueColor(shadowShaderProgram, 0.039f, 0.378f, 0.039f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Corner 1 model matrix (near right)
    glm::mat4 corner1ModelMatrix = glm::translate(iMat, glm::vec3(85.0f, 9.0f, 65.0f)) * glm::scale(iMat, glm::vec3(10.0f, 21.0f, 10.0f));
    corner1ModelMatrix = worldMatrix * corner1ModelMatrix;
    setWorldMatrix(shadowShaderProgram, corner1ModelMatrix);

    // Drawing the corner 1
    setUniqueColor(shadowShaderProgram, 0.039f, 0.378f, 0.039f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Corner 2 model matrix (near left)
    glm::mat4 corner2ModelMatrix = glm::translate(iMat, glm::vec3(-85.0f, 9.0f, 65.0f)) * glm::scale(iMat, glm::vec3(10.0f, 21.0f, 10.0f));
    corner2ModelMatrix = worldMatrix * corner2ModelMatrix;
    setWorldMatrix(shadowShaderProgram, corner2ModelMatrix);

    // Drawing the corner 2
    setUniqueColor(shadowShaderProgram, 0.039f, 0.378f, 0.039f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Corner 3 model matrix (far right)
    glm::mat4 corner3ModelMatrix = glm::translate(iMat, glm::vec3(85.0f, 9.0f, -65.0f)) * glm::scale(iMat, glm::vec3(10.0f, 21.0f, 10.0f));
    corner3ModelMatrix = worldMatrix * corner3ModelMatrix;
    setWorldMatrix(shadowShaderProgram, corner3ModelMatrix);

    // Drawing the corner 3
    setUniqueColor(shadowShaderProgram, 0.039f, 0.378f, 0.039f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Corner 4 model matrix (far left)
    glm::mat4 corner4ModelMatrix = glm::translate(iMat, glm::vec3(-85.0f, 9.0f, -65.0f)) * glm::scale(iMat, glm::vec3(10.0f, 21.0f, 10.0f));
    corner4ModelMatrix = worldMatrix * corner4ModelMatrix;
    setWorldMatrix(shadowShaderProgram, corner4ModelMatrix);

    // Drawing the corner 4
    setUniqueColor(shadowShaderProgram, 0.039f, 0.378f, 0.039f);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawSphereShadow(glm::mat4 worldMatrix, int sphereVao, int shadowShaderProgram, std::vector<int> indices) {

    glm::mat4 sphereModelMatrix = glm::translate(iMat, glm::vec3(0.0f, 10.0f, 10.0f));
    sphereModelMatrix = worldMatrix * sphereModelMatrix;
    setWorldMatrix(shadowShaderProgram, sphereModelMatrix);

    glBindVertexArray(sphereVao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void drawScoreboardShadow(glm::mat4 worldMatrix, int cubeVao, int shadowShaderProgram) {

    glBindVertexArray(cubeVao);

    // Board model matrix
    glm::mat4 boardModelMatrix = glm::translate(iMat, glm::vec3(0.0f, 40.0f, -70.0f)) * glm::scale(iMat, glm::vec3(85.0f, 45.0f, 2.0f));
    boardModelMatrix = worldMatrix * boardModelMatrix;
    setWorldMatrix(shadowShaderProgram, boardModelMatrix);

    // Drawing the board
    setUniqueColor(shadowShaderProgram, 0.392f, 0.247f, 0.137f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Board leg 1 model matrix
    glm::mat4 boardLeg1ModelMatrix = glm::translate(iMat, glm::vec3(25.0f, 21.0f, -72.0f)) * glm::scale(iMat, glm::vec3(3.0f, 45.0f, 3.0f));
    boardLeg1ModelMatrix = worldMatrix * boardLeg1ModelMatrix;
    setWorldMatrix(shadowShaderProgram, boardLeg1ModelMatrix);

    // Drawing the board leg 1
    setUniqueColor(shadowShaderProgram, 0.392f, 0.247f, 0.137f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Board leg 2 model matrix
    glm::mat4 boardLeg2ModelMatrix = glm::translate(iMat, glm::vec3(-25.0f, 21.0f, -72.0f)) * glm::scale(iMat, glm::vec3(3.0f, 45.0f, 3.0f));
    boardLeg2ModelMatrix = worldMatrix * boardLeg2ModelMatrix;
    setWorldMatrix(shadowShaderProgram, boardLeg2ModelMatrix);

    // Drawing the board leg 2
    setUniqueColor(shadowShaderProgram, 0.392f, 0.247f, 0.137f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Screen model matrix
    glm::mat4 screenModelMatrix = glm::translate(iMat, glm::vec3(0.0f, 40.0f, -69.0f)) * glm::scale(iMat, glm::vec3(80.0f, 40.0f, 1.0f));
    screenModelMatrix = worldMatrix * screenModelMatrix;
    setWorldMatrix(shadowShaderProgram, screenModelMatrix);

    // Drawing the screen
    setUniqueColor(shadowShaderProgram, 0.0f, 0.0f, 0.2f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Colon 1 model matrix
    glm::mat4 colon1ModelMatrix = glm::translate(iMat, glm::vec3(0.0f, 40.0f, -68.5f)) * glm::scale(iMat, glm::vec3(1.0f, 1.0f, 1.0f));
    colon1ModelMatrix = worldMatrix * colon1ModelMatrix;
    setWorldMatrix(shadowShaderProgram, colon1ModelMatrix);

    // Drawing the colon 1
    setUniqueColor(shadowShaderProgram, 1.0f, 0.98f, 0.313f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Colon 2 model matrix
    glm::mat4 colon2ModelMatrix = glm::translate(iMat, glm::vec3(0.0f, 35.0f, -68.5f)) * glm::scale(iMat, glm::vec3(1.0f, 1.0f, 1.0f));
    colon2ModelMatrix = worldMatrix * colon2ModelMatrix;
    setWorldMatrix(shadowShaderProgram, colon2ModelMatrix);

    // Drawing the colon 2
    setUniqueColor(shadowShaderProgram, 1.0f, 0.98f, 0.313f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);

    // Drawing point numbers
    drawNumber(worldMatrix, cubeVao, shadowShaderProgram, glm::vec3(-20.0f, 38.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), score1[0]);
    drawNumber(worldMatrix, cubeVao, shadowShaderProgram, glm::vec3(-10.0f, 38.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), score1[1]);
    drawNumber(worldMatrix, cubeVao, shadowShaderProgram, glm::vec3(10.0f, 38.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), score2[0]);
    drawNumber(worldMatrix, cubeVao, shadowShaderProgram, glm::vec3(20.0f, 38.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), score2[1]);

    // Drawing set numbers
    drawNumber(worldMatrix, cubeVao, shadowShaderProgram, glm::vec3(-25.0f, 86.0f, 0.0f), glm::vec3(0.6f, 0.6f, 1.0f), setScore1);
    drawNumber(worldMatrix, cubeVao, shadowShaderProgram, glm::vec3(25.0f, 86.0f, 0.0f), glm::vec3(0.6f, 0.6f, 1.0f), setScore2);
}
