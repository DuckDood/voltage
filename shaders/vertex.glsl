#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normals;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 perspMat;

out vec2 texCoord;
out vec3 normal;
out vec3 FragPos;

void main()
{
    gl_Position = perspMat*viewMat*modelMat*vec4(position, 1.0);
	texCoord = texCoords;
	normal = mat3(transpose(inverse(modelMat))) * normals;
	FragPos = vec3(modelMat * vec4(position, 1.0));
}
