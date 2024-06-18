#pragma once
#include "GLHeader.h"
#include <iostream>
#include <stdio.h>
#include <stdarg.h>

GLuint ImageShader;
GLuint TextShader;

class Shader {
private:
	GLchar *vertex_source{}, *fragment_source{};
	GLuint vertex_shader{}, fragment_shader{};

public:
	char* LoadBuffer(const char* file) {
		FILE* fptr; long length; char* buf;

		fptr = fopen(file, "rb");
		if (!fptr)
			return NULL;

		fseek(fptr, 0, SEEK_END);
		length = ftell(fptr);
		buf = (char*)malloc(length + 1);

		fseek(fptr, 0, SEEK_SET);
		fread(buf, length, 1, fptr);
		fclose(fptr);

		buf[length] = 0;

		return buf;
	}

	void LoadVertexShader(const char* VertexShader) {
		vertex_source = LoadBuffer(VertexShader);
		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, (const GLchar**)&vertex_source, 0);
		glCompileShader(vertex_shader);
		GLint ResultMatrix;
		GLchar errorLog[512];

		glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &ResultMatrix);
		if (!ResultMatrix) {
			glGetShaderInfoLog(vertex_shader, 512, NULL, errorLog);
			std::cout << "ERROR: vertex shader err\n" << errorLog << std::endl;

			return;
		}
	}

	void LoadFragmentShader(const char* FragmentShader) {
		fragment_source = LoadBuffer(FragmentShader);
		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, (const GLchar**)&fragment_source, 0);
		glCompileShader(fragment_shader);
		GLint ResultMatrix;
		GLchar errorLog[512];
		glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &ResultMatrix);

		if (!ResultMatrix) {
			glGetShaderInfoLog(fragment_shader, 512, NULL, errorLog);
			std::cout << "ERROR: fragment shader err\n" << errorLog << std::endl;

			return;
		}
	}

	void CreateShader(GLuint& ID) {
		ID = glCreateProgram();
		glAttachShader(ID, vertex_shader);
		glAttachShader(ID, fragment_shader);

		glLinkProgram(ID);
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		glUseProgram(ID);
	}
};
extern Shader shader;