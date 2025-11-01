#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "voltage.h"

class float2{
	public:
	float2 operator+(float2 b) {
		return float2(this->x + b.x, this->y + b.y);
	}
	float2 operator-(float2 b) {
		return float2(this->x - b.x, this->y - b.y);
	}
		#pragma omp parallel for
	float2 operator*(float2 b) {
		return float2(this->x * b.x, this->y * b.y);
	}
	float2 operator*(float b) {
		return float2(this->x * b, this->y * b);
	}
	float2 operator/(float b) {
		return float2(this->x / b, this->y / b);
	}
	float x;
	float y;
	float2(float x, float y) {
		this->x = x;
		this->y = y;
	}
	float2() {
	}
};
class float3{
	public:
	float x = 0;
	float y = 0;
	float z = 0;
	operator float2() const {
		return float2(this->x, this->y);
	}
	float3 operator*(float b) {
		return float3(this->x * b, this->y * b, this->z * b);
	}
	float3 operator*(float3 b) {
		return float3(this->x * b.x, this->y * b.y, this->z * b.z);
	}
	float3 operator/(float b) {
		return float3(this->x / b, this->y / b, this->z / b);
	}
	float3 operator+(float3 b) {
		return float3(this->x + b.x, this->y + b.y, this->z + b.z);
	}
	float3 operator-(float3 b) {
		return float3(this->x - b.x, this->y - b.y, this->z - b.z);
	}
	float3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	float3() {
	}
};


std::vector<float> LoadObjFile(std::string obj) {
	std::vector<float3> allPoints;
	std::vector<float3> triPoints;
	std::vector<float2> allTexCoords;
	std::vector<float2> texCoords;
	std::vector<float3> allNormals;
	std::vector<float3> normals;
	std::vector<float3> tangents;

	std::stringstream lines(obj);
	std::string line;
	int a=0;

	while(std::getline(lines, line, '\n')) {
		if(line.length() < 2) continue;
		if(line.substr(0, 2) == "v ") {
			std::vector<float> axes;
			std::stringstream words(line);
			std::string word;
			int i = 2;
			while(std::getline(words, word, ' ')) {
				if(i > 2) {
					axes.push_back(std::stof(word));
				}
				i++;
			}
			allPoints.push_back({axes[0], axes[1], axes[2]});
		} else if(line.substr(0,3) == "vt ") {

			std::vector<std::string> axes;
			std::stringstream words(line);
			std::string word;
			int i = 2;
			while(std::getline(words, word, ' ')) {
				if(i > 2) {
					axes.push_back(word);
				}
				i++;
			}
			float ax1 = std::stof(axes[0]);
			float ax2 = std::stof(axes[1]);
			float2 t = float2(ax1, 1-ax2);
			allTexCoords.push_back(t);
			
		}else if(line.substr(0,3) == "vn ") {

			std::vector<std::string> axes;
			std::stringstream words(line);
			std::string word;
			int i = 2;
			while(std::getline(words, word, ' ')) {
				if(i > 2) {
					axes.push_back(word);
				}
				i++;
			}
			float3 t = float3(std::stof(axes[0]), std::stof(axes[1]), std::stof(axes[2]));
			allNormals.push_back(t);
			
		}else

		if(line.substr(0, 2) == "f ") {
			std::stringstream words(line);
			std::string word;
			std::vector<std::string> faceIndexGroups;
			int c = 2;
			while(std::getline(words, word, ' ')) {
				if(c > 2) {
					faceIndexGroups.push_back(word);
				}
				c++;
			}

			for(int i = 0; i<faceIndexGroups.size(); i++) {
				std::vector<int> indexGroup;
				std::stringstream thisFaceGroup(faceIndexGroups[i]);
				while(std::getline(thisFaceGroup, word, '/')) {
					if(word.empty()) indexGroup.push_back(0); else
					indexGroup.push_back(std::stoi(word));
				}
				int pointIndex = indexGroup[0]-1;
				int texIndex = indexGroup.size() > 1? indexGroup.at(1)-1 : -1;
				int normIndex = indexGroup.size() > 2? indexGroup.at(2)-1 : -1;
				if(i>=3) triPoints.push_back(triPoints[triPoints.size()-(3 * i - 6)]);
				if(i>=3) triPoints.push_back(triPoints[triPoints.size()-(2)]);

				if(i>=3) texCoords.push_back(texCoords[texCoords.size()-(3 * i - 6)]);
				if(i>=3) texCoords.push_back(texCoords[texCoords.size()-(2)]);

				if(i>=3) normals.push_back(normals[normals.size()-(3 * i - 6)]);
				if(i>=3) normals.push_back(normals[normals.size()-(2)]);

				triPoints.push_back(allPoints[pointIndex]);
				texCoords.push_back(texIndex >= 0? allTexCoords[texIndex] : float2(0,0));
				normals.push_back(normIndex >= 0? allNormals[normIndex] : float3(0,0,0));
			}

		}
	} 

	// tangents calculations cause im NOT messing with the stuff up there
	// i kinda get how it works up there but i just used the sebastian lague logic ok
	// https://www.youtube.com/watch?v=yyJ-hdISgnw
	for(int i = 0; i < triPoints.size(); i+=3) {
		float3 pos1 = triPoints.at(i);
		float3 pos2 = triPoints.at(i+1);
		float3 pos3 = triPoints.at(i+2);

		float2 uv1 = texCoords.at(i);
		float2 uv2 = texCoords.at(i+1);
		float2 uv3 = texCoords.at(i+2);

		float3 edge1 = pos2 - pos1;
		float3 edge2 = pos3 - pos1;
		float2 uvDelta1 = uv2-uv1;
		float2 uvDelta2 = uv3-uv1;

		float f = 1.f / (uvDelta1.x * uvDelta2.y - uvDelta2.x * uvDelta1.y);

		float3 tangent;
		tangent.x = f * (uvDelta2.y * edge1.x - uvDelta1.y * edge2.x);
		tangent.y = f * (uvDelta2.y * edge1.y - uvDelta1.y * edge2.y);
		tangent.z = f * (uvDelta2.y * edge1.z - uvDelta1.y * edge2.z);

		tangents.push_back(tangent);
		tangents.push_back(tangent);
		tangents.push_back(tangent);
	}

	std::vector<float> value;
	for(int i = 0; i < triPoints.size(); i++) {
		value.push_back(triPoints.at(i).x);
		value.push_back(triPoints.at(i).y);
		value.push_back(triPoints.at(i).z);

		value.push_back(texCoords.at(i).x);
		value.push_back(texCoords.at(i).y);

		value.push_back(normals.at(i).x);
		value.push_back(normals.at(i).y);
		value.push_back(normals.at(i).z);

		value.push_back(tangents.at(i).x);
		value.push_back(tangents.at(i).y);
		value.push_back(tangents.at(i).z);
	}
	return value;
}

std::vector<float> LoadObjByName(std::string path) {
	std::ifstream file(path);
	std::string fileStr = "";
	for(std::string line; std::getline(file, line); fileStr += line + "\n");
	return LoadObjFile(fileStr);
}

std::vector<float> load3dCache(std::string path) {
	std::ifstream cacheFile(path, std::ios::binary);
	if(!cacheFile.good()) {
		std::cerr << "Cache file does not exist.\n";
	}
	cacheFile.seekg(0,std::ios::end);
	int size = cacheFile.tellg();
	cacheFile.seekg(0,std::ios::beg);

	std::vector<float> value(size/sizeof(float));

	cacheFile.read((char*)value.data(), size);

	return value;
}

// https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection
// i couldve figured it out but its easier to copy just for easy comparison logic
void boundingBox::Update() {
	minX = position.x - scale.x;
	maxX = position.x + scale.x;

	minY = position.y - scale.y;
	maxY = position.y + scale.y;

	minZ = position.z - scale.z;
	maxZ = position.z + scale.z;
}

bool intersect(boundingBox a, boundingBox b) {
	return a.minX <= b.maxX && a.maxX >= b.minX && a.minY <= b.maxY && a.maxY >= b.minY && a.minZ <= b.maxZ && a.maxZ >= b.minZ;
}

bool Hitbox::isOverlapped() {
	for(Hitbox *currentBox : *hitboxes) {
		if(this->id == currentBox->id) continue;
		if(intersect(*this, *currentBox)) {
			return true;
		}
	}
	return false;
}



Framebuffer::Framebuffer(int inWidth, int inHeight, std::vector<GLenum> bufferTypes) {
		this->width = inWidth;
		this->height = inHeight;
		this->bufferTypes = bufferTypes;
		this->colorBuffers.resize(this->bufferTypes.size());
		glGenFramebuffers(1, &this->framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);

		glGenTextures(this->colorBuffers.capacity(), this->colorBuffers.data());
		for(int i = 0; i < this->colorBuffers.size(); i++) {
			GLint currentBuffer = this->colorBuffers.at(i);
			glBindTexture(GL_TEXTURE_2D, currentBuffer);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glFramebufferTexture2D(GL_FRAMEBUFFER, bufferTypes.at(i), GL_TEXTURE_2D, currentBuffer, 0);
		}

		glGenRenderbuffers(1, &this->renderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, this->renderbuffer);
		
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->width, this->height);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->renderbuffer);

		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "Framebuffer failed to initialise" << std::endl;
		}
		glDrawBuffers(this->bufferTypes.size(), this->bufferTypes.data());

		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Framebuffer::Start(int inWidth, int inHeight) {
	if(inWidth != this->width || inHeight != this->height) {
		this->width = inWidth;
		this->height = inHeight;

		glDeleteFramebuffers(1, &this->framebuffer);

		glGenFramebuffers(1, &this->framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);

		glDeleteTextures(this->colorBuffers.capacity(), this->colorBuffers.data());
		glGenTextures(this->colorBuffers.capacity(), this->colorBuffers.data());

		/*glBindTexture(GL_TEXTURE_2D, this->colorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->colorBuffer, 0);*/
		for(int i = 0; i < this->colorBuffers.size(); i++) {
			GLint currentBuffer = this->colorBuffers.at(i);
			glBindTexture(GL_TEXTURE_2D, currentBuffer);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glFramebufferTexture2D(GL_FRAMEBUFFER, bufferTypes.at(i), GL_TEXTURE_2D, currentBuffer, 0);
		}
		glDrawBuffers(this->bufferTypes.size(), this->bufferTypes.data());

		glDeleteRenderbuffers(1, &this->renderbuffer);
		glGenRenderbuffers(1, &this->renderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, this->renderbuffer);
		
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->width, this->height);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->renderbuffer);

		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "Framebuffer failed to resize" << std::endl;
		}

	}
	glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
}

void Framebuffer::End() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

#if HITBOX_VIEW
HitboxRenderer::HitboxRenderer() {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 8 * 3 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	unsigned int indices[] = {
		0,1,
		0,3,
		1,2,
		2,3,

		4,5,
		4,7,
		5,6,
		6,7,

		0,4,
		1,5,
		2,6,
		3,7,

	};
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}
void HitboxRenderer::Update() {
	float vertices[] = {
		hit->minX, hit->minY, hit->minZ, // bottom left back
		hit->minX, hit->maxY, hit->minZ, // top left back
		hit->maxX, hit->maxY, hit->minZ, // top right back
		hit->maxX, hit->minY, hit->minZ, // bottom right back

		hit->minX, hit->minY, hit->maxZ, // bottom left front
		hit->minX, hit->maxY, hit->maxZ, // top left front
		hit->maxX, hit->maxY, hit->maxZ, // top right front
		hit->maxX, hit->minY, hit->maxZ, // bottom right front
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), NULL, GL_DYNAMIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
}
void HitboxRenderer::Update(Hitbox* next) {
	hit = next;
	float vertices[] = {
		hit->minX, hit->minY, hit->minZ, // bottom left back
		hit->minX, hit->maxY, hit->minZ, // top left back
		hit->maxX, hit->maxY, hit->minZ, // top right back
		hit->maxX, hit->minY, hit->minZ, // bottom right back

		hit->minX, hit->minY, hit->maxZ, // bottom left front
		hit->minX, hit->maxY, hit->maxZ, // top left front
		hit->maxX, hit->maxY, hit->maxZ, // top right front
		hit->maxX, hit->minY, hit->maxZ, // bottom right front
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), NULL, GL_DYNAMIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
}
#endif

void Transform::UpdateRotation() {
	transformMatrix = glm::mat4(1.f);
	translationMatrix = glm::mat4(1.f);
	rotationMatrix = glm::mat4(1.f);
	scaleMatrix = glm::mat4(1.f);

	translationMatrix = glm::translate(translationMatrix, position);
	rotationMatrix = glm::rotate(rotationMatrix, yaw, glm::vec3(0.f, 1.f, 0.f));
	rotationMatrix = glm::rotate(rotationMatrix, pitch, glm::vec3(1.f, 0.f, 0.f));
	rotationMatrix = glm::rotate(rotationMatrix, roll, glm::vec3(0.f, 0.f, 1.f));
	scaleMatrix = glm::scale(scaleMatrix, scale);

	transformMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	if(parent != NULL) {
		transformMatrix = parent->transformMatrix * transformMatrix;
	}
	invTransformMatrix = glm::inverse(transformMatrix);
}

Camera::Camera() {
	fov = glm::radians(45.f);
}

Model::Model(std::vector<float> obj, Material material) {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, obj.size() * sizeof(float), obj.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(5*sizeof(float)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8*sizeof(float)));
	glEnableVertexAttribArray(3);
	vertCount = obj.size()/11;

	this->mat = material;
	usable = true;
	
}
Model::Model(Model *m) {
	this->VAO = m->VAO;
	this->VBO = m->VBO;
	this->vertCount = m->vertCount;
	this->mat = m->mat;
	usable = true;
}
	
void Model::Unload() {
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
	usable = false;
	glDeleteVertexArrays(1, &VAO);
}

void Object::UpdateHitbox() {
	this->hitbox.scale = this->scale;
	this->hitbox.position = this->position;
	this->hitbox.Update();
}

void setSceneLights(Shader shader, std::vector<Light*> lights) {
	shader.setUniformInt("lightNumber", lights.size());
	for(int i = 0; i<lights.size(); i++) {
		shader.setUniformFloat(("light["+std::to_string(i)+"].constant").c_str(), lights.at(i)->constant);
		shader.setUniformFloat(("light["+std::to_string(i)+"].linear").c_str(), lights.at(i)->linear);
		shader.setUniformFloat(("light["+std::to_string(i)+"].quadratic").c_str(), lights.at(i)->quadratic);
		shader.setUniformInt(("light["+std::to_string(i)+"].type").c_str(), lights.at(i)->type);
		shader.setUniformVec3(("light["+std::to_string(i)+"].ambient").c_str(), lights.at(i)->ambient);
		shader.setUniformVec3(("light["+std::to_string(i)+"].diffuse").c_str(), lights.at(i)->diffuse);
		shader.setUniformVec3(("light["+std::to_string(i)+"].specular").c_str(), lights.at(i)->specular);
		shader.setUniformVec3(("light["+std::to_string(i)+"].position").c_str(), lights.at(i)->position);
		shader.setUniformVec3(("light["+std::to_string(i)+"].direction").c_str(), lights.at(i)->direction);
		shader.setUniformFloat(("light["+std::to_string(i)+"].innerCutOff").c_str(), lights.at(i)->innerCutOff);
		shader.setUniformFloat(("light["+std::to_string(i)+"].outerCutOff").c_str(), lights.at(i)->outerCutOff);
	}
}

void RenderModel(Model model, Shader shader) {
		model.UpdateRotation();
		switch(model.cullType) {
			case 0:
				glDisable(GL_CULL_FACE);
				break;
			case 1:
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
				break;
			case 2:
				glEnable(GL_CULL_FACE);
				glCullFace(GL_FRONT);
				break;
		}

		glUseProgram(shader.programID);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, model.mat.diffuseTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, model.mat.specularTex);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, model.mat.normal);

		glUniformMatrix4fv(glGetUniformLocation(shader.programID, "modelMat"), 1, GL_FALSE, glm::value_ptr(model.transformMatrix));
		shader.setUniformFloat("material.shininess", model.mat.shininess);

		shader.setUniformInt("material.useLighting", model.mat.useLighting);

		shader.setUniformInt("material.useDiffuseTex", model.mat.useDiffuseTex);
		shader.setUniformInt("material.useSpecularTex", model.mat.useSpecularTex);
		shader.setUniformInt("material.useNormalMap", model.mat.useNormalMap);

		shader.setUniformVec4("material.diffuseColor", model.mat.diffuseColor);
		shader.setUniformVec4("material.specularColor", model.mat.specularColor);

		shader.setUniformInt("material.diffuse", 0);
		shader.setUniformInt("material.specular", 1);
		shader.setUniformInt("material.normal", 2);


		glBindVertexArray(model.VAO);
		glDrawArrays(GL_TRIANGLES, 0, model.vertCount);
}

