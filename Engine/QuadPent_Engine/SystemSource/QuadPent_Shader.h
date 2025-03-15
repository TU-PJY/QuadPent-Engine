#pragma once
#include "QuadPent_Header.h"

namespace QP {
	class QuadPent_Shader {
	private:
		char* VertexShaderSource{}, *FragmentShaderSource{}, *ComputeShaderSource{};
		unsigned int  VertexShaderBuffer{}, FragmentShaderBuffer{}, CommputeShaderBuffer{};

	public:
		char* LoadShaderFile(std::string file);
		void LoadVertexShader(std::string VertexShader);
		void LoadFragmentShader(std::string FragmentShader);
		void CreateShader(QP::ShaderResource& Shader);
		void CreateShaderLocation();
	};

	extern QuadPent_Shader Shader;
}