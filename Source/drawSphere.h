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

// ** The code here is inspired by http://www.songho.ca/opengl/gl_sphere.html, shown to us by the TA. **

// Function to generate vertices for the sphere
const std::vector<LightTexturedColoredVertex> generateSphereVertices(float radius, int numSlices, int numStacks) 
{
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


void drawSphere(glm::mat4 worldMatrix, int sphereVao, int sceneShaderProgram, std::vector<int> indices, GLuint tennisBallTextureID) 
{

    glm::mat4 sphereModelMatrix = glm::translate(iMat, glm::vec3(4.6f, 22.0f, 6.0f));
    sphereModelMatrix = worldMatrix * sphereModelMatrix;
    setWorldMatrix(sceneShaderProgram, sphereModelMatrix);

    glBindVertexArray(sphereVao);
    setMaterial(sceneShaderProgram, 0.4f, 0.8f, 0.2f, 10.0f, toggleShadows);
    setTexture(sceneShaderProgram, tennisBallTextureID, 0, toggleTexture);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}