#define GLEW_STATIC 1  
#include <GL/glew.h>  

#include <GLFW/glfw3.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>


struct LightTexturedColoredVertex
{
    LightTexturedColoredVertex(glm::vec3 _position, glm::vec3 _color, glm::vec3 _normal, glm::vec2 _uv)
        : position(_position), color(_color), normal(_normal), uv(_uv) {}

    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 normal;
    glm::vec2 uv;
};

// Light Textured Cube model
const LightTexturedColoredVertex cubeArray[] = {  // position, color, normal, UV
    LightTexturedColoredVertex(glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)), //left
    LightTexturedColoredVertex(glm::vec3(-0.5f,-0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)),
    LightTexturedColoredVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)),

    LightTexturedColoredVertex(glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
    LightTexturedColoredVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
    LightTexturedColoredVertex(glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)),

    LightTexturedColoredVertex(glm::vec3(0.5f, 0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)), // far 
    LightTexturedColoredVertex(glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)),
    LightTexturedColoredVertex(glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)),

    LightTexturedColoredVertex(glm::vec3(0.5f, 0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)),
    LightTexturedColoredVertex(glm::vec3(0.5f,-0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)),
    LightTexturedColoredVertex(glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)),

    LightTexturedColoredVertex(glm::vec3(0.5f,-0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f)), // bottom
    LightTexturedColoredVertex(glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
    LightTexturedColoredVertex(glm::vec3(0.5f,-0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)),

    LightTexturedColoredVertex(glm::vec3(0.5f,-0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
    LightTexturedColoredVertex(glm::vec3(-0.5f,-0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f)),
    LightTexturedColoredVertex(glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)),

    LightTexturedColoredVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)), // near
    LightTexturedColoredVertex(glm::vec3(-0.5f,-0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)),
    LightTexturedColoredVertex(glm::vec3(0.5f,-0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)),

    LightTexturedColoredVertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)),
    LightTexturedColoredVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)),
    LightTexturedColoredVertex(glm::vec3(0.5f,-0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)),

    LightTexturedColoredVertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)), // right
    LightTexturedColoredVertex(glm::vec3(0.5f,-0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
    LightTexturedColoredVertex(glm::vec3(0.5f, 0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)),

    LightTexturedColoredVertex(glm::vec3(0.5f,-0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
    LightTexturedColoredVertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
    LightTexturedColoredVertex(glm::vec3(0.5f,-0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)),

    LightTexturedColoredVertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)), // top
    LightTexturedColoredVertex(glm::vec3(0.5f, 0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
    LightTexturedColoredVertex(glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)),

    LightTexturedColoredVertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
    LightTexturedColoredVertex(glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
    LightTexturedColoredVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f))
};


// Cube array, but texture is repeated
const LightTexturedColoredVertex cubeArrayRepeat[] = {  // position, color, normal, UV (UV are changed now, for it to repeat the texture)
    LightTexturedColoredVertex(glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)), //left
    LightTexturedColoredVertex(glm::vec3(-0.5f,-0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 5.0f)),
    LightTexturedColoredVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(5.0f, 5.0f)),

    LightTexturedColoredVertex(glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
    LightTexturedColoredVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(5.0f, 5.0f)),
    LightTexturedColoredVertex(glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(5.0f, 0.0f)),

    LightTexturedColoredVertex(glm::vec3(0.5f, 0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(5.0f, 5.0f)), // far 
    LightTexturedColoredVertex(glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)),
    LightTexturedColoredVertex(glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 5.0f)),

    LightTexturedColoredVertex(glm::vec3(0.5f, 0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(5.0f, 5.0f)),
    LightTexturedColoredVertex(glm::vec3(0.5f,-0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(5.0f, 0.0f)),
    LightTexturedColoredVertex(glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)),

    LightTexturedColoredVertex(glm::vec3(0.5f,-0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(5.0f, 5.0f)), // bottom
    LightTexturedColoredVertex(glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
    LightTexturedColoredVertex(glm::vec3(0.5f,-0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(5.0f, 0.0f)),

    LightTexturedColoredVertex(glm::vec3(0.5f,-0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(5.0f, 5.0f)),
    LightTexturedColoredVertex(glm::vec3(-0.5f,-0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 5.0f)),
    LightTexturedColoredVertex(glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)),

    LightTexturedColoredVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 5.0f)), // near
    LightTexturedColoredVertex(glm::vec3(-0.5f,-0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)),
    LightTexturedColoredVertex(glm::vec3(0.5f,-0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(5.0f, 0.0f)),

    LightTexturedColoredVertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(5.0f, 5.0f)),
    LightTexturedColoredVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 5.0f)),
    LightTexturedColoredVertex(glm::vec3(0.5f,-0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(5.0f, 0.0f)),

    LightTexturedColoredVertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(5.0f, 5.0f)), // right
    LightTexturedColoredVertex(glm::vec3(0.5f,-0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
    LightTexturedColoredVertex(glm::vec3(0.5f, 0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(5.0f, 0.0f)),

    LightTexturedColoredVertex(glm::vec3(0.5f,-0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
    LightTexturedColoredVertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(5.0f, 5.0f)),
    LightTexturedColoredVertex(glm::vec3(0.5f,-0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 5.0f)),

    LightTexturedColoredVertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(5.0f, 5.0f)), // top
    LightTexturedColoredVertex(glm::vec3(0.5f, 0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(5.0f, 0.0f)),
    LightTexturedColoredVertex(glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)),

    LightTexturedColoredVertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(5.0f, 5.0f)),
    LightTexturedColoredVertex(glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
    LightTexturedColoredVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 5.0f))
};


// Racket grid array
glm::vec3 racketGridArray[] = {
    glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), // line 0 (vertical)
    glm::vec3(0.0f, 8.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),

    glm::vec3(0.33f, 0.35f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), // line 1
    glm::vec3(0.33f, 8.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),

    glm::vec3(0.66f, 0.7f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), // line 2
    glm::vec3(0.66f, 8.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),

    glm::vec3(0.99f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), // line 3
    glm::vec3(0.99f, 8.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),

    glm::vec3(1.33f, 1.4f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), // line 4
    glm::vec3(1.33f, 8.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),

    glm::vec3(1.66f, 1.75f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), // line 5
    glm::vec3(1.66f, 8.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),

    glm::vec3(-0.33f, 0.35f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), // line -1
    glm::vec3(-0.33f, 8.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),

    glm::vec3(-0.66f, 0.7f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), // line -2
    glm::vec3(-0.66f, 8.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),

    glm::vec3(-0.99f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), // line -3
    glm::vec3(-0.99f, 8.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),

    glm::vec3(-1.33f, 1.4f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), // line -4
    glm::vec3(-1.33f, 8.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),

    glm::vec3(-1.66f, 1.75f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), // line -5
    glm::vec3(-1.66f, 8.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),

    glm::vec3(-2.0f, 2.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), // line 6 (horizontal)
    glm::vec3(2.0f, 2.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),

    glm::vec3(-2.0f, 2.6f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), // line 7
    glm::vec3(2.0f, 2.6f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),

    glm::vec3(-2.0f, 3.2f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), // line 8
    glm::vec3(2.0f, 3.2f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),

    glm::vec3(-2.0f, 3.8f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), // line 9
    glm::vec3(2.0f, 3.8f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),

    glm::vec3(-2.0f, 4.4f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), // line 10
    glm::vec3(2.0f, 4.4f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),

    glm::vec3(-2.0f, 5.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), // line 11
    glm::vec3(2.0f, 5.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),

    glm::vec3(-2.0f, 5.6f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), // line 12
    glm::vec3(2.0f, 5.6f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),

    glm::vec3(-2.0f, 6.2f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), // line 13
    glm::vec3(2.0f, 6.2f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),

    glm::vec3(-2.0f, 6.8f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), // line 14
    glm::vec3(2.0f, 6.8f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),
};