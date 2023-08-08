#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec2 aUV;

uniform vec3 uniqueColor = vec3(1.0f);
uniform mat4 worldMatrix;
uniform mat4 viewMatrix = mat4(1.0);
uniform mat4 projectionMatrix = mat4(1.0);
uniform mat4 light_proj_view_matrix;
uniform mat4 spotlight_proj_view_matrix;

out vec3 vertexColor;
out vec2 vertexUV;
out vec3 fragment_normal;
out vec3 fragment_position;
out vec4 fragment_position_light_space;
out vec4 fragment_position_spotlight_space;

void main()
{
   vertexColor = uniqueColor * aColor;
   vertexUV = aUV;

   fragment_normal = mat3(worldMatrix) * aNormal;
   fragment_position = vec3(worldMatrix * vec4(aPos, 1.0));
   fragment_position_light_space = light_proj_view_matrix * vec4(fragment_position, 1.0);
   fragment_position_spotlight_space = spotlight_proj_view_matrix * vec4(fragment_position, 1.0);

   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;
   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
