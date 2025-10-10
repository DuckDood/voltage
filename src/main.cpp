#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "stb_image.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#define INIT_SCR_WIDTH 800
#define INIT_SCR_HEIGHT 600

void onWinResize(GLFWwindow* window, int width, int height) {
	std::cout << width << " " << height << std::endl;
	glViewport(0,0,width,height);
}

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
	/*void operator+=(float3 b) {
		*this = *this + b;
	}*/
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
	}
	return value;
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

class Transform {
	public: 
	float pitch = 0, yaw = 0, roll = 0;
	glm::vec3 position = glm::vec3(0,0,0);
	glm::vec3 scale = glm::vec3(1,1,1);

	glm::mat4 transformMatrix = glm::mat4(1.f);
	glm::mat4 invTransformMatrix = glm::mat4(1.f);
	glm::mat4 rotationMatrix = glm::mat4(1.f);
	glm::mat4 translationMatrix = glm::mat4(1.f);
	glm::mat4 scaleMatrix = glm::mat4(1.f);

	Transform* parent = NULL;

	void UpdateRotation() {
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
};

class Camera : public Transform {
	public:
	float fov;
	Camera() {
		fov = glm::radians(45.f);
	}
};

struct Material {
	GLuint diffuseTex;
	GLuint specularTex;
	float shininess;
};

class Model : public Transform {
	public:
	unsigned int VAO;
	unsigned int VBO;
	bool usable = false;
	int vertCount;
	Material mat;

		Model(std::vector<float> obj, Material material) {
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);

			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, obj.size() * sizeof(float), obj.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5*sizeof(float)));
			glEnableVertexAttribArray(2);
			vertCount = obj.size()/8;

			this->mat = material;
			usable = true;
			
		}
		Model(Model *m) {
			this->VAO = m->VAO;
			this->VBO = m->VBO;
			this->vertCount = m->vertCount;
			this->mat = m->mat;
			usable = true;
		}
	
		void Unload() {
			glBindVertexArray(0);
			glDeleteBuffers(1, &VBO);
			usable = false;
			glDeleteVertexArrays(1, &VAO);
		}
};

void getMouseDelta(double mouseX, double mouseY, double *mouseReturnX, double *mouseReturnY) {
	static double lastX = 0;
	static double lastY = 0;

	*mouseReturnX = mouseX - lastX;
	*mouseReturnY = mouseY - lastY;

	lastX = mouseX;
	lastY = mouseY;
}

struct Light {
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;

	float cutOff;

	int type;
};

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
		shader.setUniformFloat(("light["+std::to_string(i)+"].cutOff").c_str(), lights.at(i)->cutOff);
	}
}

void RenderModel(Model model, Shader shader) {
		model.UpdateRotation();

		glUseProgram(shader.programID);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, model.mat.diffuseTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, model.mat.specularTex);

		glUniformMatrix4fv(glGetUniformLocation(shader.programID, "modelMat"), 1, GL_FALSE, glm::value_ptr(model.transformMatrix));
		shader.setUniformFloat("material.shininess", model.mat.shininess);

		shader.setUniformInt("material.diffuse", 0);
		shader.setUniformInt("material.specular", 1);


		glBindVertexArray(model.VAO);
		glDrawArrays(GL_TRIANGLES, 0, model.vertCount);
}

// https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection
// i couldve figured it out but its easier to copy just for easy comparison logic
class boundingBox {
	public:
	float minX;
	float maxX;

	float minY;
	float maxY;

	float minZ;
	float maxZ;
  
	glm::vec3 position = glm::vec3(0.,0.,0.);
	glm::vec3 scale = glm::vec3(1.,1.,1.);

	void Update() {
		minX = position.x - scale.x;
		maxX = position.x + scale.x;

		minY = position.y - scale.y;
		maxY = position.y + scale.y;

		minZ = position.z - scale.z;
		maxZ = position.z + scale.z;
	}
};
bool intersect(boundingBox a, boundingBox b) {
	return a.minX <= b.maxX && a.maxX >= b.minX && a.minY <= b.maxY && a.maxY >= b.minY && a.minZ <= b.maxZ && a.maxZ >= b.minZ;
}

float square[] = {
	-1, -1, 0,
	1, -1, 0,
	-1, 1, 0,

	1, -1, 0,
	1, 1, 0,
	-1, 1, 0
};

void key_call(GLFWwindow *win, int key, int scancode, int action, int mods) {
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		*(bool*)glfwGetWindowUserPointer(win) = !*(bool*)glfwGetWindowUserPointer(win);
	} 
}

int main() {
	if(glfwInit() == GL_FALSE) {
		std::cerr << "Failed to intialise GLFW\n";
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow *window = glfwCreateWindow(INIT_SCR_WIDTH, INIT_SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if(window == NULL) {
		std::cerr << "Failed to create glfw window" << std::endl;
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	GLenum err = glewInit();
	if(err != GLEW_OK) {
		std::cerr << "Failed to init GLEW: " << glewGetErrorString(err) << std::endl;
		glfwTerminate();
		return 1;
	}
	//glfwSwapInterval(0);
	glViewport(0,0,INIT_SCR_WIDTH,INIT_SCR_HEIGHT);
	glfwSetFramebufferSizeCallback(window, onWinResize);



IMGUI_CHECKVERSION();
ImGui::CreateContext();
ImGuiIO& io = ImGui::GetIO();
io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
ImVec4 modPosIm(0,0,0,1);
ImVec4 modScaleIm(1,1,1,1);
ImVec4 modRotIm(0,0,0,1);

ImVec4 lightDiffColIm(1,1,1,1);
ImVec4 lightAmbColIm(1,1,1,1);
ImVec4 lightSpecColIm(1,1,1,1);

// Setup Platform/Renderer backends
ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
ImGui_ImplOpenGL3_Init();
bool capture = true;
glfwSetWindowUserPointer(window, &capture);
glfwSetKeyCallback(window, key_call);





	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	int width, height, channelCount;
	unsigned char *data = stbi_load("resources/textures/bricks.jpg", &width, &height, &channelCount, 0);
	unsigned int texture;
	glGenTextures(1, &texture);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	data = stbi_load("resources/textures/bricks_contrast.jpg", &width, &height, &channelCount, 0);
	unsigned int texture2;
	glGenTextures(1, &texture2);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	Material material;
	material.diffuseTex = texture;
	material.specularTex = texture2;
	material.shininess = 32;
	Model model(load3dCache("cache/cube.qucache"), material);
	Model model2(model);
	std::cout << "Parsing Done!\n";
	model.position.y = -0.5;
	model2.position.z = -1.5;

	Camera cam;

	glm::mat4 perspective = glm::perspective(cam.fov, (float)INIT_SCR_WIDTH/(float)INIT_SCR_HEIGHT, 0.1f, 100.f);

	Shader program("shaders/vertex.glsl", "shaders/fragment.glsl");
	Shader screen("shaders/screenVert.glsl", "shaders/screenFrag.glsl");

	
	std::vector<Light*> lights;
	
	Light sun;
	sun.type = 1;
	sun.ambient = glm::vec3(0.2,0.2,0.2);
	sun.diffuse = glm::vec3(1.5, 1.5 ,1.5);
	sun.specular = glm::vec3(1.f, 1.f, 1.f);
	sun.position = glm::vec3(0.f, 1.f, 0.3f);
	sun.direction = glm::vec3(0.f, 1.f, 0.0f);
	sun.constant = 1.f;
	sun.linear = 0.09f;
	sun.quadratic = 0.032f;
	sun.cutOff = glm::cos(glm::radians(12.5f));
	
	lights.push_back(&sun);

	double mouseX;
	double mouseY;

	int winX;
	int winY;

	boundingBox bound;
	boundingBox bound2;
	model.scale = glm::vec3(0.8, 2, 0.7);
	while(!glfwWindowShouldClose(window)) {


// (Your code calls glfwPollEvents())
// ...
// Start the Dear ImGui frame
ImGui_ImplOpenGL3_NewFrame();
ImGui_ImplGlfw_NewFrame();
ImGui::NewFrame();



	if(capture) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	} else {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	ImGui::Begin("Model");
	ImGui::Text("Position");
	ImGui::DragFloat("x##Pos", &modPosIm.x, 0.01);
	ImGui::DragFloat("y##Pos", &modPosIm.y, 0.01);
	ImGui::DragFloat("z##Pos", &modPosIm.z, 0.01);
	model.position.x = modPosIm.x;
	model.position.y = modPosIm.y;
	model.position.z = modPosIm.z;

	ImGui::Text("Scale");
	ImGui::DragFloat("x##Scale", &modScaleIm.x, 0.01);
	ImGui::DragFloat("y##Scale", &modScaleIm.y, 0.01);
	ImGui::DragFloat("z##Scale", &modScaleIm.z, 0.01);
	model.scale.x = modScaleIm.x;
	model.scale.y = modScaleIm.y;
	model.scale.z = modScaleIm.z;

	ImGui::Text("Rotation");
	ImGui::DragFloat("x##Rotation", &modRotIm.x, 0.003);
	ImGui::DragFloat("y##Rotation", &modRotIm.y, 0.003);
	ImGui::DragFloat("z##Rotation", &modRotIm.z, 0.003);
	model.yaw = modRotIm.x;
	model.pitch = modRotIm.y;
	model.roll = modRotIm.z;
	ImGui::End();

	ImGui::Begin("Light");
	ImGui::ColorPicker3("diffuse", (float*)&lightDiffColIm);
	sun.diffuse.x = lightDiffColIm.x;
	sun.diffuse.y = lightDiffColIm.y;
	sun.diffuse.z = lightDiffColIm.z;

	ImGui::ColorPicker3("specular", (float*)&lightSpecColIm);
	sun.specular.x = lightSpecColIm.x;
	sun.specular.y = lightSpecColIm.y;
	sun.specular.z = lightSpecColIm.z;

	ImGui::ColorPicker3("ambient", (float*)&lightAmbColIm);
	sun.ambient.x = lightAmbColIm.x;
	sun.ambient.y = lightAmbColIm.y;
	sun.ambient.z = lightAmbColIm.z;

	ImGui::End();


		glfwGetCursorPos(window, &mouseX, &mouseY);
		getMouseDelta(mouseX, mouseY, &mouseX, &mouseY);
		glfwGetWindowSize(window, &winX, &winY);

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			cam.position.z += cos(cam.yaw)*0.03*cos(cam.pitch);
			cam.position.x += sin(cam.yaw)*0.03*cos(cam.pitch);

			cam.position.y -= sin(cam.pitch)*0.03;
		}
		if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			cam.position.z -= cos(cam.yaw)*0.03*cos(cam.pitch);
			cam.position.x -= sin(cam.yaw)*0.03*cos(cam.pitch);

			cam.position.y += sin(cam.pitch)*0.03;
		}
		if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			cam.position.z += sin(cam.yaw)*0.02;
			cam.position.x -= cos(cam.yaw)*0.02;
		}
		if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			cam.position.z -= sin(cam.yaw)*0.02;
			cam.position.x += cos(cam.yaw)*0.02;
		}

		if(glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS) {
			cam.fov += glm::radians(1.);
		}
		if(glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS) {
			cam.fov -= glm::radians(1.);
		}

		if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			model.position.y+=0.01;
		}
		if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			model.position.y-=0.01;
		}
		if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			model.position.z+=0.01;
		}
		if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			model.position.z-=0.01;
		}

		if(capture) {
			cam.yaw -= mouseX/300;
			cam.pitch -= mouseY/300;
		}
		setSceneLights(program, lights);

		cam.UpdateRotation();

		bound.position = model.position;
		bound.scale = model.scale;

		bound2.position = model2.position;
		bound2.scale = model2.scale;

		bound.Update();
		bound2.Update();

		if(intersect(bound, bound2)) {
			std::cout << "int" << std::endl;
		} else {
			std::cout << "sep" << std::endl;
		}

		perspective = glm::perspective(cam.fov, (float)winX/(float)winY, 0.1f, 100.f);

		//model.UpdateRotation();

		glUseProgram(program.programID);
		//glUniformMatrix4fv(glGetUniformLocation(program.programID, "modelMat"), 1, GL_FALSE, glm::value_ptr(model.transformMatrix));
		glUniformMatrix4fv(glGetUniformLocation(program.programID, "viewMat"), 1, GL_FALSE, glm::value_ptr(cam.invTransformMatrix));
		glUniformMatrix4fv(glGetUniformLocation(program.programID, "perspMat"), 1, GL_FALSE, glm::value_ptr(perspective));
		glUniform3fv(glGetUniformLocation(program.programID, "viewPos"), 1, glm::value_ptr(cam.position));

		/*program.setUniformFloat("material.shininess", model.mat.shininess);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, model.mat.diffuseTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, model.mat.specularTex);
		program.setUniformInt("material.diffuse", 0);
		program.setUniformInt("material.specular", 1);



		glBindVertexArray(model.VAO);

		glDrawArrays(GL_TRIANGLES, 0, model.vertCount);*/
		RenderModel(model, program);
		RenderModel(model2, program);

		
		//glBindVertexArray(light.VAO);
		//glUniformMatrix4fv(glGetUniformLocation(program.programID, "modelMat"), 1, GL_FALSE, glm::value_ptr(light.transformMatrix));
		//glDrawArrays(GL_TRIANGLES, 0, light.vertCount);


// Rendering
// (Your code clears your framebuffer, renders your other stuff etc.)
ImGui::Render();
ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
// (Your code calls glfwSwapBuffers() etc.)






		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
