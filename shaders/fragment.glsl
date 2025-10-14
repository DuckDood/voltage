#version 330 core

struct Material {
	float shininess;
	bool useLighting;
	sampler2D diffuse;
	sampler2D specular;

	vec4 specularColor;
	vec4 diffuseColor;
	
	bool useDiffuseTex;
	bool useSpecularTex;
};

layout (location = 0) out vec4 FragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 FragPos;

uniform vec3 viewPos;
uniform Material material;

vec3 lightColor = vec3(1, 1, 1);

// make uniform

struct Light {
	vec3 position;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

	float innerCutOff;
	float outerCutOff;

	int type;
};


vec3 resolvePointLight(Light pointLight, Material objectMaterial, vec3 texColor, vec3 specColor, vec3 objectNormal) {
	//vec4 texColorVec4 = texture(objectMaterial.diffuse, textureCoord);
	//vec3 texColor = vec3(texColorVec4);

	//vec4 specColorVec4 = texture(objectMaterial.specular, textureCoord);
	//vec3 specColor = vec3(specColorVec4);

	float dist = length(pointLight.position - FragPos);
	float attenuation = 1. / (pointLight.constant + pointLight.linear * dist + pointLight.quadratic * (dist * dist));

	vec3 result;

	// ambient
	vec3 ambient = pointLight.ambient * texColor;

	// diffuse
	vec3 norm = normalize(objectNormal);

	vec3 lightDir = normalize(pointLight.position - FragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = pointLight.diffuse * (diff * texColor);

	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), objectMaterial.shininess);
	vec3 specular = pointLight.specular * (spec * specColor);
	if(objectMaterial.shininess == 0) {
		specular = vec3(0,0,0);
	}
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	result = (ambient + diffuse + specular);
	return result;
}

vec3 resolveDirectionalLight(Light directionLight, Material objectMaterial, vec3 texColor, vec3 specColor, vec3 objectNormal) {
	//vec4 texColorVec4 = texture(objectMaterial.diffuse, textureCoord);
	//vec3 texColor = vec3(texColorVec4);

	//vec4 specColorVec4 = texture(objectMaterial.specular, textureCoord);
	//vec3 specColor = vec3(specColorVec4);

	vec3 result;

	// ambient
	vec3 ambient = directionLight.ambient * texColor;

	// diffuse
	vec3 norm = normalize(objectNormal);

	vec3 lightDir = normalize(directionLight.direction);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = directionLight.diffuse * (diff * texColor);

	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), objectMaterial.shininess);
	vec3 specular = directionLight.specular * (spec * specColor);
	if(objectMaterial.shininess == 0) {
		specular = vec3(0,0,0);
	}
	result = (ambient + diffuse + specular);
	return result;
}

vec3 resolveSpotlight(Light spotLight, Material objectMaterial, vec3 texColor, vec3 specColor, vec3 objectNormal) {
	//vec4 texColorVec4 = texture(objectMaterial.diffuse, textureCoord);
	//vec3 texColor = vec3(texColorVec4);

	//vec4 specColorVec4 = texture(objectMaterial.specular, textureCoord);
	//vec3 specColor = vec3(specColorVec4);

	float dist = length(spotLight.position - FragPos);
	float attenuation = 1. / (spotLight.constant + spotLight.linear * dist + spotLight.quadratic * (dist * dist));

	vec3 result;

	// ambient
	vec3 ambient = spotLight.ambient * texColor;

	// diffuse
	vec3 norm = normalize(objectNormal);

	vec3 lightDir = normalize(spotLight.position - FragPos);

	float theta = dot(lightDir, normalize(spotLight.direction));
	float epsilon = spotLight.innerCutOff - spotLight.outerCutOff;
	float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0);

	//if(theta > spotLight.innerCutOff) {

		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = spotLight.diffuse * (diff * texColor);

		// specular
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), objectMaterial.shininess);
		vec3 specular = spotLight.specular * (spec * specColor);
		if(objectMaterial.shininess == 0) {
			specular = vec3(0,0,0);
		}

		diffuse *= intensity;
		specular *= intensity;
			
		ambient *= attenuation;
		diffuse *= attenuation;
		specular *= attenuation;

		result = (ambient + diffuse + specular);
	//} else {
	//	ambient *= attenuation;
	//	result = ambient;
	//}
	return result;
}

uniform Light light[256];
uniform int lightNumber;

void main() {
	vec3 result = vec3(0.,0.,0.);

	vec4 texColorVec4;
	vec4 specColorVec4;

	if(material.useDiffuseTex) {
		texColorVec4 = texture(material.diffuse, texCoord);
		texColorVec4 *= material.diffuseColor;
	} else {
		texColorVec4 = material.diffuseColor;
	}
	vec3 texColor = vec3(texColorVec4);

	if(material.useSpecularTex) {
		specColorVec4 = texture(material.specular, texCoord);
		specColorVec4 *= material.specularColor;
	} else {
		specColorVec4 = material.specularColor;
	}
	vec3 specColor = vec3(specColorVec4);

	if(material.useLighting) {
		for(int i = 0; i < lightNumber; i++) {
			switch(light[i].type) {
				case 0:
				//result += resolveDirectionalLight(light[i], material, texCoord, normal);
				result += resolveDirectionalLight(light[i], material, texColor, specColor, normal);
				break;
				case 1:
				result += resolvePointLight(light[i], material, texColor, specColor, normal);
				break;
				case 2:
				result += resolveSpotlight(light[i], material, texColor, specColor, normal);
				break;
			}
		}
	} else {
		result = texColor;
	}
	FragColor = vec4(result, 1.0);
}
