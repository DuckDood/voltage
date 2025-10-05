#include <GL/glew.h>
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
};
