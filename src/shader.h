#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <iostream>

class Shader {
	public:
	unsigned int programID;
	Shader(std::string vertPath, std::string fragPath) {
		unsigned int vertex, fragment;
		GLint success;
		GLchar info[512];

		std::ifstream vert(vertPath);
		std::string vertStr = "";
		for(std::string line; std::getline(vert, line); vertStr+=line + "\n");

		std::ifstream frag(fragPath);
		std::string fragStr = "";
		for(std::string line; std::getline(frag, line);fragStr+=line + "\n");

		const char* vertCStr = vertStr.c_str();
		const char* fragCStr = fragStr.c_str();


		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertCStr, NULL);
		glCompileShader(vertex);
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if(!success) {
			glGetShaderInfoLog(vertex, 512, NULL, info);
			std::cout << "Vertex shader compile error:\n" << info;
		}


		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragCStr, NULL);
		glCompileShader(fragment);
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if(!success) {
			glGetShaderInfoLog(fragment, 512, NULL, info);
			std::cout << "Fragment shader compile error:\n" << info;
		}

		programID = glCreateProgram();
		glAttachShader(programID, vertex);
		glAttachShader(programID, fragment);
		glLinkProgram(programID);

		glGetProgramiv(programID, GL_LINK_STATUS, &success);
		if(!success) {
			glGetProgramInfoLog(programID, 512, NULL, info);
			std::cout << "Shader Program Link error: \n" << info;
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
	void setUniformVec3(const char* uniformName, glm::vec3 value) {
		glUniform3fv(glGetUniformLocation(this->programID, uniformName), 1, glm::value_ptr(value));
	}
	void setUniformVec3Floats(const char* uniformName, float x, float y, float z) {
		glUniform3fv(glGetUniformLocation(this->programID, uniformName), 1, glm::value_ptr(glm::vec3(x,y,z)));
	}

	void setUniformVec4(const char* uniformName, glm::vec4 value) {
		glUniform4fv(glGetUniformLocation(this->programID, uniformName), 1, glm::value_ptr(value));
	}
	void setUniformVec4Floats(const char* uniformName, float x, float y, float z, float w) {
		glUniform4fv(glGetUniformLocation(this->programID, uniformName), 1, glm::value_ptr(glm::vec4(x,y,z, w)));
	}

	void setUniformInt(const char* uniformName, int x) {
		glUniform1i(glGetUniformLocation(this->programID, uniformName), x);
	}
	void setUniformFloat(const char* uniformName, float x) {
		glUniform1f(glGetUniformLocation(this->programID, uniformName), x);
	}
};
