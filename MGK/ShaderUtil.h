#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "EngineHeader.h"

class ShaderUtil {
private:
	GLchar* vertex_source{}, * fragment_source{}, * compute_source{};
	GLuint vertex_shader{}, fragment_shader{}, compute_shader{};

public:
	char* LoadShaderFile(std::string file);
	void LoadVertexShader(std::string VertexShader);
	void LoadFragmentShader(std::string FragmentShader);
	void LoadComputeShader(std::string ComputeShader);
	void CreateShader(GLuint& Shader);
	void CreateComputeShader(GLuint& Shader);
	void CreateShaderLocation();
	void CreateSSBO();
};
extern ShaderUtil shaderUtil;