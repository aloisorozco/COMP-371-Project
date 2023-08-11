#version 330 core

uniform vec3 view_position;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normals;
layout (location = 2) in vec2 aUv;


uniform vec3 uniqueColor = vec3(1.0f);
uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 light_proj_view_matrix;

out vec3 fragment_normal;
out vec3 fragment_position;
out vec4 fragment_position_light_space;
out vec2 uvs;
out vec3 vertexColor;

void main()
{
    vertexColor = uniqueColor;
    fragment_normal = mat3(worldMatrix) * normals;
    fragment_position = vec3(worldMatrix * vec4(position, 1.0));
    fragment_position_light_space = light_proj_view_matrix * vec4(fragment_position, 1.0);
    uvs = aUv;
    gl_Position = projectionMatrix * viewMatrix * worldMatrix * vec4(position, 1.0);
}
