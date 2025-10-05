#version 330 core

struct Material {
	float shininess;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

layout (location = 0) out vec4 FragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 FragPos;

uniform sampler2D tex;
uniform vec3 viewPos;

vec3 lightColor = vec3(1., 1., 1.);

// make uniform
vec3 lightPos = vec3(1.2f, 1.0f, 2.0f);


void main() {
	float ambientStrength = 0.1;
	float specStrength = 0.5;


	vec4 texColor = texture(tex, texCoord);
	//vec4 texColor = vec4(1.0f, 0.5f, 0.31f, 1.0);

	vec3 objColor = vec3(texColor.x, texColor.y, texColor.z);

	// ambient
	vec3 ambient = ambientStrength * lightColor;

	// diffuse
	vec3 norm = normal;
	vec3 lightDir = normalize(lightPos - FragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * objColor;
	FragColor = vec4(result, 1.0);
}
