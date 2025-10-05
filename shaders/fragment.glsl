#version 330 core
layout (location = 0) out vec4 FragColor;

in vec2 texCoord;

void main() {
	//FragColor = vec4(0.2, 0.6, 0.7, 1); 
	FragColor = vec4(texCoord, 0., 1.); 
}
