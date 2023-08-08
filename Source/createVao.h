#define GLEW_STATIC 1  
#include <GL/glew.h>  

#include <GLFW/glfw3.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>


// Creating a VAO definition, as all objects use the same (position, color) structure
int createVertexArrayObject(const glm::vec3* vertexArray, int arraySize)
{
    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);


    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, arraySize, vertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0,    // attribute 0 matches aPos in Vertex Shader
        3,                      // size
        GL_FLOAT,               // type
        GL_FALSE,               // normalized?
        2 * sizeof(glm::vec3),  // stride - each vertex contain 2 vec3 (position, color)
        (void*)0                // array buffer offset
    );
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1,        // attribute 1 matches aColor in Vertex Shader
        3,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(glm::vec3),
        (void*)sizeof(glm::vec3)    // color is offseted by a vec3 (comes after position)
    );
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // VAO already stored the state we just defined, safe to unbind buffer
    glBindVertexArray(0); // Unbind to not modify the VAO

    return vertexArrayObject;
}

// Creating a VAO definition, as all objects use the same (position, color, normal, UV) structure
int createLightTexturedVertexArrayObject(const LightTexturedColoredVertex* vertexArray, int arraySize)
{
    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);


    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, arraySize, vertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0,    // attribute 0 matches aPos in Vertex Shader
        3,                      // size
        GL_FLOAT,               // type
        GL_FALSE,               // normalized?
        sizeof(LightTexturedColoredVertex),  // stride - each vertex contain 2 vec3 (position, color)
        (void*)0                // array buffer offset
    );
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1,        // attribute 1 matches aColor in Vertex Shader
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(LightTexturedColoredVertex),
        (void*)sizeof(glm::vec3)    // color is offseted by a vec3 (comes after position)
    );
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2,        // attribute 2 matches aNormal in Vertex Shader
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(LightTexturedColoredVertex),
        (void*)(2 * sizeof(glm::vec3))    // color is offseted by 2 vec3 (comes after position and color)
    );
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3,        // attribute 3 matches aUV in Vertex Shader
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(LightTexturedColoredVertex),
        (void*)(3 * sizeof(glm::vec3))    // color is offseted by a vec3 (comes after position)
    );
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // VAO already stored the state we just defined, safe to unbind buffer
    glBindVertexArray(0); // Unbind to not modify the VAO

    return vertexArrayObject;
}

int createSphereVertexArrayObject(
    const LightTexturedColoredVertex* vertexArray,
    int arraySize,
    const int* indicesArray,
    int indicesCount
) {
    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, arraySize, vertexArray, GL_STATIC_DRAW);

    // Upload Indices Buffer to the GPU
    GLuint elementBufferObject;
    glGenBuffers(1, &elementBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(int), indicesArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0,    // attribute 0 matches aPos in Vertex Shader
        3,                      // size
        GL_FLOAT,               // type
        GL_FALSE,               // normalized?
        sizeof(LightTexturedColoredVertex),  // stride - each vertex contain 2 vec3 (position, color)
        (void*)0                // array buffer offset
    );
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1,        // attribute 1 matches aColor in Vertex Shader
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(LightTexturedColoredVertex),
        (void*)sizeof(glm::vec3)    // color is offseted by a vec3 (comes after position)
    );
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2,        // attribute 2 matches aNormal in Vertex Shader
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(LightTexturedColoredVertex),
        (void*)(2 * sizeof(glm::vec3))    // color is offseted by 2 vec3 (comes after position and color)
    );
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3,        // attribute 3 matches aUV in Vertex Shader
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(LightTexturedColoredVertex),
        (void*)(3 * sizeof(glm::vec3))    // color is offseted by a vec3 (comes after position)
    );
    glEnableVertexAttribArray(3);
    // Unbind the VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return vertexArrayObject;
}