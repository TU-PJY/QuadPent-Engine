#include "ShaderUtil.h"
#include <iostream>
#include <stdio.h>
#include <stdarg.h>
#include <fstream>

GLuint IMAGE_SHADER;
GLuint TEXT_SHADER;
GLuint MATRIX_COMPT_SHADER;

GLuint SSBO_MATRIX_INPUT, SSBO_MATRIX_OUTPUT;

ShaderLocation IMAGE_ALPHA_LOCATION, IMAGE_COLOR_LOCATION, IMAGE_MODEL_LOCATION;
ShaderLocation BLUR_STRENGTH_LOCATION, BOOL_BLUR_LOCATION, TEXEL_SIZE_LOCATION;
ShaderLocation TEXT_ALPHA_LOCATION, TEXT_COLOR_LOCATION, TEXT_MODEL_LOCATION;
ShaderLocation IMGAE_PROJECTION_LOCARION, IMAGE_VIEW_LOCATION, IMAGE_VIEW_POSITION_LOCATION;
ShaderLocation TEXT_PROJECTION_LOCATION, TEXT_VIEW_LOCATION, TEXT_VIEW_POSITION_LOCATION;

char* ShaderUtil::LoadShaderFile(std::string FileName) {
	std::ifstream ShaderFile(FileName, std::ios::in | std::ios::binary | std::ios::ate);
	if (!ShaderFile.is_open()) {
		std::cout << "Can not open shader file: " << FileName << std::endl;
		exit(EXIT_FAILURE);
	}

	std::streamsize Length = ShaderFile.tellg();
	ShaderFile.seekg(0, std::ios::beg);

	char* Buffer = new char[Length + 1];

	if (!ShaderFile.read(Buffer, Length)) {
		delete[] Buffer;
		ShaderFile.close();
		std::cout << "Can not read shader file: " << FileName << std::endl;
		exit(EXIT_FAILURE);
	}

	Buffer[Length] = '\0';

	ShaderFile.close();

	return Buffer;
}

void ShaderUtil::LoadVertexShader(std::string VertexShader) {
	vertex_source = LoadShaderFile(VertexShader);
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, (const GLchar**)&vertex_source, 0);
	glCompileShader(vertex_shader);

	GLint Result{};
	GLchar ErrorLog[512]{};

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &Result);

	if (!Result) {
		glGetShaderInfoLog(vertex_shader, 512, NULL, ErrorLog);
		std::cout << "ERROR: vertex shader error\n" << ErrorLog << std::endl;

		return;
	}
}

void ShaderUtil::LoadFragmentShader(std::string FragmentShader) {
	fragment_source = LoadShaderFile(FragmentShader);
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, (const GLchar**)&fragment_source, 0);
	glCompileShader(fragment_shader);

	GLint Result{};
	GLchar ErrorLog[512]{};

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &Result);

	if (!Result) {
		glGetShaderInfoLog(fragment_shader, 512, NULL, ErrorLog);
		std::cout << "ERROR: fragment shader error\n" << ErrorLog << std::endl;

		return;
	}
}

void ShaderUtil::LoadComputeShader(std::string ComputeShader) {
	compute_source = LoadShaderFile(ComputeShader);
	compute_shader = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(compute_shader, 1, (const GLchar**)&compute_source, 0);
	glCompileShader(compute_shader);

	GLint Result{};
	GLchar ErrorLog[512]{};

	glGetShaderiv(compute_shader, GL_COMPILE_STATUS, &Result);

	if (!Result) {
		glGetShaderInfoLog(compute_shader, 512, NULL, ErrorLog);
		std::cout << "ERROR: compute shader error\n" << ErrorLog << std::endl;

		return;
	}
}

void ShaderUtil::CreateShader(GLuint& Shader) {
	Shader = glCreateProgram();
	glAttachShader(Shader, vertex_shader);
	glAttachShader(Shader, fragment_shader);
	glLinkProgram(Shader);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

void ShaderUtil::CreateComputeShader(GLuint& Shader) {
	Shader = glCreateProgram();
	glAttachShader(Shader, compute_shader);
	glLinkProgram(Shader);
	glDeleteShader(compute_shader);
}

void ShaderUtil::CreateShaderLocation() {
	// Image Shader
	IMAGE_ALPHA_LOCATION         = glGetUniformLocation(IMAGE_SHADER, "transparency");
	IMAGE_COLOR_LOCATION         = glGetUniformLocation(IMAGE_SHADER, "objectColor");
	BLUR_STRENGTH_LOCATION       = glGetUniformLocation(IMAGE_SHADER, "Radius");
	BOOL_BLUR_LOCATION           = glGetUniformLocation(IMAGE_SHADER, "UseBlur");
	TEXEL_SIZE_LOCATION          = glGetUniformLocation(IMAGE_SHADER, "TexelSize");
	IMAGE_MODEL_LOCATION         = glGetUniformLocation(IMAGE_SHADER, "model");

	// Text Shader
	TEXT_ALPHA_LOCATION          = glGetUniformLocation(TEXT_SHADER, "transparency");
	TEXT_COLOR_LOCATION          = glGetUniformLocation(TEXT_SHADER, "objectColor");
	TEXT_MODEL_LOCATION          = glGetUniformLocation(TEXT_SHADER, "model");

	// Image Camera
	IMGAE_PROJECTION_LOCARION    = glGetUniformLocation(IMAGE_SHADER, "projection");
	IMAGE_VIEW_LOCATION          = glGetUniformLocation(IMAGE_SHADER, "view");
	IMAGE_VIEW_POSITION_LOCATION = glGetUniformLocation(IMAGE_SHADER, "viewPos");
	
	// Text Camera
	TEXT_PROJECTION_LOCATION     = glGetUniformLocation(TEXT_SHADER, "projection");
	TEXT_VIEW_LOCATION           = glGetUniformLocation(TEXT_SHADER, "view");
	TEXT_VIEW_POSITION_LOCATION  = glGetUniformLocation(TEXT_SHADER, "viewPos");
}

void ShaderUtil::CreateSSBO() {
	glGenBuffers(1, &SSBO_MATRIX_INPUT);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO_MATRIX_INPUT);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4) * 5, nullptr, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO_MATRIX_INPUT);

	glGenBuffers(1, &SSBO_MATRIX_OUTPUT);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO_MATRIX_OUTPUT);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, SSBO_MATRIX_OUTPUT);
}