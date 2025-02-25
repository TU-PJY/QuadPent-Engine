#pragma once
#include "SDK_Header.h"

namespace SDK {
	class SDK_Shader {
	private:
		char* VertexShaderSource{}, *FragmentShaderSource{}, *ComputeShaderSource{};
		unsigned int  VertexShaderBuffer{}, FragmentShaderBuffer{}, CommputeShaderBuffer{};

	public:
		char* LoadShaderFile(std::string file);
		void LoadVertexShader(std::string VertexShader);
		void LoadFragmentShader(std::string FragmentShader);
		void LoadComputeShader(std::string ComputeShader);
		void CreateShader(SDK::ShaderResource& Shader);
		void CreateComputeShader(SDK::ShaderResource& Shader);
		void CreateShaderLocation();
		void CreateSSBO();
	};

	extern SDK_Shader Shader;
}