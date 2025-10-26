struct Material {
	float shininess;
	bool useLighting;
	sampler2D diffuse;
	sampler2D specular;
	sampler2D normal;

	vec4 specularColor;
	vec4 diffuseColor;
	
	bool useDiffuseTex;
	bool useSpecularTex;
	bool useNormalMap;
};
