#define GLEW_STATIC 1  
#include <GL/glew.h>  

#include <GLFW/glfw3.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

#include "common.h"

// Small function to find a random float, with a minimum and maximum
float random_float(float min, float max)
{
    return ((float)rand() / RAND_MAX) * (max - min) + min;
}

// Key callback function
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Increment model up
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    {
        upArmScale *= glm::vec3(1.2f);
    }

    // Increment model down
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        upArmScale *= glm::vec3(0.8f);
    }


    // Camera Positions
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
    {
        // Player Racket (2)
        if (m == 0)
        {
            // Changing world back to default
            worldXAngle = 0.0f;
            worldYAngle = 0.0f;
            useRadialCamera = false;
            useCamera1 = true;

            // Angles to look at center of racket 2
            cameraLookAtCenter = glm::vec3(0.0f, 6.0f, 0.0f);

            // Camera position to look at racket 2
            cameraPosition1 = (glm::vec3(0.4f, 0.4f, 0.4f) * racketPosition2) + glm::vec3(-3.0f, 18.0f, 25.0f);

            whichRacket = 2;
            m++;
        }
        // Spectator in stands camera
        else if (m == 1)
        {
            // Changing world back to default
            worldXAngle = 0.0f;
            worldYAngle = 0.0f;

            // Angles to look at center of racket 2
            cameraLookAtCenter = glm::vec3(0.0f, 1.0f, 0.0f);

            // Camera position to look at racket 2
            cameraPosition1 = glm::vec3(36.0f, 16.0f, -2.0f);

            useRadialCamera = false;
            useCamera1 = true;
            whichRacket = 2;
            m++;
        }
        // High view camera
        else if (m == 2) {
            // Changing world back to default
            worldXAngle = 0.0f;
            worldYAngle = 0.0f;
            useRadialCamera = false;
            useCamera1 = false;

            // Re-setting angles to default
            theta = glm::radians(90.0f);
            phi = glm::radians(-35.0f);
            cameraLookAt = glm::vec3(cosf(phi) * cosf(theta), sinf(phi), -cosf(phi) * sinf(theta));

            // Re-setting camera zoom to default
            cameraPosition = glm::vec3(0.0f, 50.0f, 70.0f);
            m = -1;
        }
        // Drone cameras
        else if (m == -1) {

            // Changing world back to default
            worldXAngle = 0.0f;
            worldYAngle = 0.0f;

            useCamera1 = false;
            useRadialCamera = true;
            m = 0;
        }
    }
    

    // Start game
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS && canStartPoint)
    {
        startPoint();
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        if (fall == -1) {
            fall = 0;
        }
        else {
            fall = -1;
        }
        
    }


    // Toggle shadows
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
        if (toggleShadows == 1) {
            toggleShadows = 0;
        }
        else {
            toggleShadows = 1;
        }
    }
    // Toggle textures
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        if (toggleTexture == 1) {
            toggleTexture = 0;
        }
        else {
            toggleTexture = 1;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        GLuint useDefaultLightLocation = glGetUniformLocation(sceneShaderProgram, "useDefaultLight");
        if (toggleDefaultLight == true) {
            glUniform1i(useDefaultLightLocation, false);
            toggleDefaultLight = false;
        }
        else {
            glUniform1i(useDefaultLightLocation, true);
            toggleDefaultLight = true;
        }
    }

    // Toggle default light
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        GLuint useSpotlightLocation = glGetUniformLocation(sceneShaderProgram, "useSpotlight");
        if (toggleSpotlight == true) {
            glUniform1i(useSpotlightLocation, false);
            toggleSpotlight = false;
        }
        else {
            glUniform1i(useSpotlightLocation, true);
            toggleSpotlight = true;
        }

    }
    // Toggle spotlight
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        GLuint useRadialLightLocation = glGetUniformLocation(sceneShaderProgram, "useRadialLight");
        if (toggleRadialLight == true) {
            glUniform1i(useRadialLightLocation, false);
            toggleRadialLight = false;
        }
        else {
            glUniform1i(useRadialLightLocation, true);
            toggleRadialLight = true;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
    {
        // Move model to the left
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            upArmPosition.x -= 1.0f;
        }

        // Move model to the right
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            upArmPosition.x += 1.0f;
        }

        // Move model down
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            upArmPosition.y -= 1.0f;
        }

        // Move model up
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            upArmPosition.y += 1.0f;
        }

        // Move model forward (away from camera)
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            upArmPosition.z -= 1.0f;
        }

        // Move model backward (toward the camera)
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            upArmPosition.z += 1.0f;
        }
    }

    else
    {

        // Rotate model up
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            upArmXAngle1 -= 5.0f;
            upArmXAngle2 -= 5.0f;
        }

        // Rotate model down
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            upArmXAngle1 += 5.0f;
            upArmXAngle2 += 5.0f;
        }

        

        //if (whichRacket == 1) 
        //{
        //    // Rotate model to the left
        //    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        //    {
        //        upArmYAngle[whichRacket] += 5.0f;
        //    }

        //    // Rotate model to the right
        //    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        //    {
        //        upArmYAngle[whichRacket] -= 5.0f;
        //    }
        //} 
        //if (whichRacket == 2)
        //{
        //    // Rotate model to the left
        //    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        //    {
        //        upArmYAngle[whichRacket] += 5.0f;
        //    }

        //    // Rotate model to the right
        //    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        //    {
        //        upArmYAngle[whichRacket] -= 5.0f;
        //    }
        //}
     }
}

// Callback function for window resize events
void windowResizeCallback(GLFWwindow* window, int width, int height) 
{
    // Update the viewport with the new window size
    glViewport(0, 0, width, height);

    // Calculate the new aspect ratio
    float aspectRatio = static_cast<float>(width) / static_cast<float>(height);

    // Update the projection matrix with the new aspect ratio
    glm::mat4 newProjectionMatrix = glm::perspective(70.0f, aspectRatio, 0.01f, 400.0f);
    setProjectionMatrix(sceneShaderProgram, newProjectionMatrix);
    setProjectionMatrix(shadowShaderProgram, newProjectionMatrix);
}