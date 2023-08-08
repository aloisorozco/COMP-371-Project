#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 transform_in_light_space;
uniform mat4 worldMatrix;

void main()
{
    mat4 scale_bias_matrix = mat4(vec4(0.5, 0.0, 0.0, 0.0),
                                    vec4(0.0, 0.5, 0.0, 0.0),
                                    vec4(0.0, 0.0, 0.5, 0.0),
                                    vec4(0.5, 0.5, 0.5, 1.0));

    mat4 light_space = transform_in_light_space * worldMatrix;
    gl_Position = 
                    //scale_bias_matrix * // bias the depth map coordinates
                    light_space * vec4(position, 1.0);
}
