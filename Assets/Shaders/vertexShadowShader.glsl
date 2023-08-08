#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 worldMatrix;
uniform mat4 lightProjection;
uniform mat4 lightView;

void main()
{
    mat4 transform_in_light_space = lightProjection * lightView * worldMatrix;
    gl_Position = transform_in_light_space * vec4(aPos, 1.0);
}
