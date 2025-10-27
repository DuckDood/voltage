#version 330 core

//!voltinclude include/material.glsl

layout (location = 0) out vec4 FragColor;


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


vec4 resolvePointLight(Light pointLight, Material objectMaterial, vec4 texColor, vec4 specColor, vec3 objectNormal, vec3 viewPos, vec3 fragPos) {
	//vec4 texColorVec4 = texture(objectMaterial.diffuse, textureCoord);
	//vec3 texColor = vec3(texColorVec4);

	//vec4 specColorVec4 = texture(objectMaterial.specular, textureCoord);
	//vec3 specColor = vec3(specColorVec4);

	float dist = length(pointLight.position - fragPos);
	float attenuation = 1. / (pointLight.constant + pointLight.linear * dist + pointLight.quadratic * (dist * dist));

	vec4 result;

	// ambient
	vec4 ambient = vec4(pointLight.ambient, 1.) * texColor;

	// diffuse
	vec3 norm = normalize(objectNormal);

	vec3 lightDir = normalize(pointLight.position - fragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec4 diffuse = vec4(pointLight.diffuse, 0.) * (diff * texColor);

	// specular
	vec3 viewDir = normalize(viewPos - fragPos);
	//vec3 reflectDir = reflect(-lightDir, norm);
	vec3 halfDir = normalize(lightDir + viewDir);

	float spec = pow(max(dot(norm, halfDir), 0.0), objectMaterial.shininess);

	vec4 specular = vec4(pointLight.specular, 1.) * (spec * specColor);
	if(objectMaterial.shininess == 0) {
		specular = vec4(0,0,0,0);
	}
	ambient.x *= attenuation;
	ambient.y *= attenuation;
	ambient.z *= attenuation;

	diffuse.x *= attenuation;
	diffuse.y *= attenuation;
	diffuse.z *= attenuation;

	specular.x *= attenuation;
	specular.y *= attenuation;
	specular.z *= attenuation;

	result = (ambient + diffuse + specular);
	return result;
}

vec4 resolveDirectionalLight(Light directionLight, Material objectMaterial, vec4 texColor, vec4 specColor, vec3 objectNormal, vec3 viewPos, vec3 fragPos) {
	//vec4 texColorVec4 = texture(objectMaterial.diffuse, textureCoord);
	//vec3 texColor = vec3(texColorVec4);

	//vec4 specColorVec4 = texture(objectMaterial.specular, textureCoord);
	//vec3 specColor = vec3(specColorVec4);

	vec4 result;

	// ambient
	vec4 ambient = vec4(directionLight.ambient, 1.) * texColor;

	// diffuse
	vec3 norm = normalize(objectNormal);

	vec3 lightDir = normalize(directionLight.direction);

	float diff = max(dot(norm, lightDir), 0.0);
	vec4 diffuse = vec4(directionLight.diffuse, 1.) * (diff * texColor);

	// specular
	vec3 viewDir = normalize(viewPos - fragPos);
	//vec3 reflectDir = reflect(-lightDir, norm);
	vec3 halfDir = normalize(lightDir + viewDir);

	float spec = pow(max(dot(norm, halfDir), 0.0), objectMaterial.shininess);
	vec4 specular = vec4(directionLight.specular, 1.) * (spec * specColor);
	if(objectMaterial.shininess == 0) {
		specular = vec4(0,0,0,0);
	}
	result = (ambient + diffuse + specular);
	return result;
}

vec4 resolveSpotlight(Light spotLight, Material objectMaterial, vec4 texColor, vec4 specColor, vec3 objectNormal, vec3 viewPos, vec3 fragPos) {
	//vec4 texColorVec4 = texture(objectMaterial.diffuse, textureCoord);
	//vec3 texColor = vec3(texColorVec4);

	//vec4 specColorVec4 = texture(objectMaterial.specular, textureCoord);
	//vec3 specColor = vec3(specColorVec4);

	float dist = length(spotLight.position - fragPos);
	float attenuation = 1. / (spotLight.constant + spotLight.linear * dist + spotLight.quadratic * (dist * dist));

	vec4 result;

	// ambient
	vec4 ambient = vec4(spotLight.ambient, 1.) * texColor;

	// diffuse
	vec3 norm = normalize(objectNormal);

	vec3 lightDir = normalize(spotLight.position - fragPos);

	float theta = dot(lightDir, normalize(spotLight.direction));
	float epsilon = spotLight.innerCutOff - spotLight.outerCutOff;
	float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0);

	//if(theta > spotLight.innerCutOff) {

		float diff = max(dot(norm, lightDir), 0.0);
		vec4 diffuse = vec4(spotLight.diffuse, 0.) * (diff * texColor);

		// specular
		vec3 viewDir = normalize(viewPos - fragPos);
		//vec3 reflectDir = reflect(-lightDir, norm);
		vec3 halfDir = normalize(lightDir + viewDir);
    	
		float spec = pow(max(dot(norm, halfDir), 0.0), objectMaterial.shininess);
		vec4 specular = vec4(spotLight.specular, 1.) * (spec * specColor);
		if(objectMaterial.shininess == 0) {
			specular = vec4(0,0,0,0);
		}

		diffuse *= intensity;
		specular *= intensity;
			
		ambient.x *= attenuation;
		ambient.y *= attenuation;
		ambient.z *= attenuation;

		diffuse.x *= attenuation;
		diffuse.y *= attenuation;
		diffuse.z *= attenuation;

		specular.x *= attenuation;
		specular.y *= attenuation;
		specular.z *= attenuation;


		result = (ambient + diffuse + specular);
	//} else {
	//	ambient *= attenuation;
	//	result = ambient;
	//}
	return result;
}

in vec2 texCoord;
in vec3 normal;
in vec3 FragPos;

in mat3 TBN;

uniform vec3 viewPos;
uniform Material material;

uniform Light light[64];
uniform int lightNumber;

void main() {
	vec4 result = vec4(0.,0.,0.,0.);

	vec4 texColorVec4 = vec4(1.,1.,1.,1.);
	vec4 specColorVec4 = vec4(1.,1.,1.,1.);

	if(material.useDiffuseTex) {
		texColorVec4 *= texture(material.diffuse, texCoord);
	}
	texColorVec4 *= material.diffuseColor;

	vec4 texColor = texColorVec4;


	if(material.useLighting) {
		if(material.useSpecularTex) {
			specColorVec4 *= texture(material.specular, texCoord);
		}
		specColorVec4 *= material.specularColor;
		vec4 specColor = specColorVec4;

		vec3 usedNormal = normal;
		if(material.useNormalMap) {
			usedNormal = vec3(texture(material.normal, texCoord));

			usedNormal = usedNormal*2. - 1.;

			usedNormal = TBN*usedNormal;

			usedNormal = normalize(usedNormal);
		}

		for(int i = 0; i < lightNumber; i++) {
			switch(light[i].type) {
				case 0:
				result += resolveDirectionalLight(light[i], material, texColor, specColor, usedNormal, viewPos, FragPos);
				break;
				case 1:
				result += resolvePointLight(light[i], material, texColor, specColor, usedNormal, viewPos, FragPos);
				break;
				case 2:
				result += resolveSpotlight(light[i], material, texColor, specColor, usedNormal, viewPos, FragPos);
				break;
			}
		}
	} else {
		result = texColor;
	}
	if(result.a <= 0.) discard;
	FragColor = result;
}
