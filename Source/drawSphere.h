#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <iostream>

#include "common.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#ifndef M_PI_2
    #define M_PI_2 1.57079632679489661923
#endif

using namespace glm;

// ** The code here is inspired by http://www.songho.ca/opengl/gl_sphere.html, shown to us by the TA. **

float sphereRadius;

// Function to generate vertices for the sphere
const std::vector<LightTexturedColoredVertex> generateSphereVertices(float radius, int numSlices, int numStacks) 
{
    ::sphereRadius = radius;

    std::vector<LightTexturedColoredVertex> vertices;

    float x, y, z, xy;                    
    float nx, ny, nz, lengthInv = 1.0f / radius;   
    float u, v;                                     
    glm::vec3 position, normal, color;
    glm::vec2 uv;

    float sectorStep = 2 * M_PI / numSlices;
    float stackStep = M_PI / numStacks;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= numStacks; ++i)
    {
        stackAngle = M_PI / 2 - i * stackStep;      
        xy = radius * cos(stackAngle);            
        z = radius * sin(stackAngle);              

        // add (sectorCount+1) vertices per stack
        for (int j = 0; j <= numSlices; ++j)
        {
            sectorAngle = j * sectorStep;      

            // vertex position (x, y, z)
            x = xy * cos(sectorAngle);       
            y = xy * sin(sectorAngle);   
            position = glm::vec3(x, y, z);

            // color
            color = glm::vec3(0.0f, 1.0f, 0.0f);

            // normalized vertex normal (nx, ny, nz)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            normal = glm::vec3(nx, ny, nz);

            // vertex tex coord (u, v) range between [0, 1]
            u = (float)j / numSlices;
            v = (float)i / numStacks;
            uv = glm::vec2(u, v);

            LightTexturedColoredVertex vertex(position, color, normal, uv);
            vertices.push_back(vertex);
        }
    }
    return vertices;
}

const std::vector<int> generateSphereIndices(int numSlices, int numStacks) {
    std::vector<int> indices;
    int k1, k2;
    for (int i = 0; i < numStacks; ++i)
    {
        k1 = i * (numSlices + 1);   
        k2 = k1 + numSlices + 1;   

        for (int j = 0; j < numSlices; ++j, ++k1, ++k2)
        {
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (numStacks - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
    return indices;
}

//glm::vec3 spherePosition = glm::vec3(8.5f, 22.0f, 30.0f);

float sphereRotationAngle = 1;
float sphereRotationIncrement = 1;

int sphereRandomNumberRange = 5;

void startPoint() {
    if (isP1sTurnToServe) {
        sphereVelocity = glm::vec3(0.25, 0, 1);
        spherePosition = glm::vec3(8.5f, 12.0f, 30.0f);
    }
    else {
        sphereVelocity = glm::vec3(-0.25, 0, -1);
        spherePosition = glm::vec3(-7.0f, 12.0f, -30.0f);
    }
    sphereAcceleration = glm::vec3(0, -0.028528f, 0);
    sphereInitialYVelocity = 0.4f;

    shouldRotateSphere = true;
    isHittingNet = false;
    sphereBounceAfterHittingNetCount = 0;
}

void resetTennisBallPosition() {
    if (isP1sTurnToServe) {
        spherePosition = glm::vec3(8.5f, 12.0f, 30.0f);
    }
    else {
        spherePosition = glm::vec3(-7.0f, 12.0f, -30.0f);
    }
    sphereAcceleration = glm::vec3(0);
    sphereVelocity = glm::vec3(0);
    sphereInitialYVelocity = 0.0f;

    shouldRotateSphere = true;
    isHittingNet = false;
    sphereBounceAfterHittingNetCount = 0;
}

bool didHitRacketX(vec3 racketPosition1, vec3 racketPosition2) {
    float racketWidth = 1.25f;
    if (spherePosition.z < 0.0f) {
        return (spherePosition.x >= racketPosition1.x - racketWidth
            && spherePosition.x <= racketPosition1.x + racketWidth);
    }
    return (spherePosition.x >= racketPosition2.x - racketWidth
                && spherePosition.x <= racketPosition2.x + racketWidth);
}

bool didHitRacketZ(vec3 racketPosition1, vec3 racketPosition2) {
    return spherePosition.z == racketPosition1.z
        || spherePosition.z == (racketPosition2.z - sphereRadius);
}

bool didHitCourt() {
    return spherePosition.y < sphereRadius;
}

bool didCrossNet() {
    return abs(spherePosition.z) < 0.1f;
}

bool didHitNet() {
    return abs(spherePosition.z) < (2 * sphereRadius) && spherePosition.y < 5.0f;
}

bool isOffCourt() {
    return abs(spherePosition.z) > 40.0f;
}

void updateSphereVelocity() {
    sphereVelocity.y += sphereAcceleration.y;
}

void updateSphereWhenHitByRacket() {
    int sphereRandomNumber = rand() % sphereRandomNumberRange;
    if (sphereRandomNumber < 3) {
        sphereVelocity = vec3(-sphereVelocity.x, sphereInitialYVelocity, -sphereVelocity.z);
    }
    else if (sphereRandomNumber  == 3) {
        sphereVelocity = vec3(0.01f * ((rand() % 24) - 12), sphereInitialYVelocity, -sphereVelocity.z);
    }
    else if (sphereRandomNumber == 4) {
        sphereVelocity = vec3(0.01f * ((rand() % 24) - 12), sphereInitialYVelocity, -sphereVelocity.z);
        //sphereVelocity = vec3(0.01f * ((rand() % 50) - 25), 0.0f, -sphereVelocity.z);
    }
    sphereRotationIncrement = -sphereRotationIncrement;
}

void updateSpherePosition(vec3 racketPosition1, vec3 racketPosition2) {

    updateSphereVelocity();
    
    if (didHitRacketZ(racketPosition1, racketPosition2) && didHitRacketX(racketPosition1, racketPosition2)) {
        updateSphereWhenHitByRacket();
    }

    if (isOffCourt()) {
        //score() based on ball z pos
        bool didP1Score = spherePosition.z > 0.0f;
        score(didP1Score, !didP1Score);
        resetTennisBallPosition();
    }

    if (didHitNet()) {
        sphereVelocity = vec3(0, sphereVelocity.y, 0);
        isHittingNet = true;
    }

    if (didHitCourt()) {
        if (!isHittingNet) {
            sphereVelocity.y = 1.0f;
        }
        else {
            if (sphereBounceAfterHittingNetCount > 8) {
                //score()based on ball z pos (opposite of offcourt)
                bool didP1Score = spherePosition.z < 0.0f;
                score(didP1Score, !didP1Score);
                resetTennisBallPosition();
            }
            else {
                sphereVelocity.y = -0.75f * sphereVelocity.y;
                sphereBounceAfterHittingNetCount++;
            }
        }
    }

    spherePosition += sphereVelocity;

}

mat4 getSphereRotation() {
    if (shouldRotateSphere && sphereInitialYVelocity != 0.0f) {
        sphereRotationAngle = (5 * sphereRotationIncrement) + sphereRotationAngle;
        return rotate(mat4(1.0f), radians(sphereRotationAngle), vec3(1, 0, 0));
    }
    return mat4(1.0f);
}

void drawSphere(glm::mat4 worldMatrix, int sphereVao, int sceneShaderProgram, std::vector<int> indices, GLuint tennisBallTextureID, glm::vec3 racketPosition1, glm::vec3 racketPosition2)
{
    updateSpherePosition(racketPosition1, racketPosition2);

    glm::mat4 sphereModelMatrix = glm::translate(iMat, spherePosition);
    sphereModelMatrix = worldMatrix * sphereModelMatrix * getSphereRotation();
    setWorldMatrix(sceneShaderProgram, sphereModelMatrix);

    glBindVertexArray(sphereVao);
    setMaterial(sceneShaderProgram, 0.4f, 0.8f, 0.2f, 10.0f, toggleShadows);
    setTexture(sceneShaderProgram, tennisBallTextureID, 0, toggleTexture);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}