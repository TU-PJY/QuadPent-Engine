#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "EngineHeader.h"

class ShaderUtil {
private:
	GLchar *vertex_source{}, *fragment_source{};
	GLuint vertex_shader{}, fragment_shader{};

public:
	char* LoadShaderFile(std::string file);
	void LoadVertexShader(std::string VertexShader);
	void LoadFragmentShader(std::string FragmentShader);
	void CreateShader(GLuint& Shader);
	void CreateShaderLocation();
};
extern ShaderUtil shaderUtil;