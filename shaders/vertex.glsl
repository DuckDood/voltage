#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normals;
layout (location = 3) in vec3 tangent;

//!voltinclude include/material.glsl

uniform Material material;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 perspMat;

out vec2 texCoord;
out vec3 normal;
out vec3 FragPos;

out mat3 TBN;


void main()
{
    gl_Position = perspMat*viewMat*modelMat*vec4(position, 1.0);
	texCoord = texCoords;
	normal = mat3(transpose(inverse(modelMat))) * normals;
	FragPos = vec3(modelMat * vec4(position, 1.0));

	normal = normalize(normal);
	
	if(material.useNormalMap) {
		vec3 T = normalize(vec3(modelMat * vec4(tangent, 0.0)));

		vec3 binormal = normalize(cross(T, normal));

		TBN = mat3(T, binormal, normal);
	}
}
