#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 viewMat;
uniform mat4 perspMat;

void main()
{
    gl_Position = perspMat*viewMat*vec4(position, 1.0);
}
