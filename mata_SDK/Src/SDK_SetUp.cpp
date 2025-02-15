#include "SDK_Header.h"
#include "SDK_Scene.h"
#include "SDK_Camera.h"
#include "SDK_CameraController.h"
#include "SDK_Shader.h"
#include "SDK_Config.h"
#include "SDK_Resource.h"
#include "SDK_ModeResource.h"
#include <windows.h>

#include "SDK_LoadingMode.h"

int SDK::WIDTH = WINDOW_WIDTH;
int SDK::HEIGHT = WINDOW_HEIGHT;
int SDK::PREV_WIDTH, SDK::PREV_HEIGHT;

SDK::Object* Indicator;

SDK_Camera SDK::Camera;

GLfloat SDK::ASPECT;
SDK::ViewportRect SDK::RECT;
glm::vec3 SDK::ViewportColor;
const glm::mat4 SDK::IdentityMatrix = glm::mat4(1.0f);

SDK_SYSTEM_RESOURCE SDK::SYSRES;
SDK_IMAGE SDK::IMAGE;
SDK_SOUND SDK::SOUND;
SDK_GLOBAL SDK::GLOBAL;
SDK_FORMAT SDK::FORMAT;
SDK_FILE SDK::FILE;
SDK_MODE_RESOURCE SDK::MODE;

std::wstring SDK::LOCALE;
SDK::START_MODE_PTR SDK::START_MODE;

void SDKSystem::SetupSystem(int argc, char** argv) {
	glutInit(&argc, argv);
	SetupWindow();
	LoadShader();
	SetGlOption();
	InitSystem();
}


void SDKSystem::SetupWindow() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GL_MULTISAMPLE);

	glutInitWindowPosition(GetSystemMetrics(SM_CXSCREEN) / 2 - SDK::WIDTH / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - SDK::HEIGHT / 2);
	glutInitWindowSize(SDK::WIDTH, SDK::HEIGHT);
	glutCreateWindow(WINDOW_NAME);

	if (FULL_SCREEN_OPTION) {
		glutFullScreen();
		SDK::WIDTH = GetSystemMetrics(SM_CXSCREEN);
		SDK::HEIGHT = GetSystemMetrics(SM_CYSCREEN);
	}

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Unable to initialize GLEW\n\n";
		exit(EXIT_FAILURE);
	}

	const GLubyte* Version = glGetString(GL_VERSION);
	std::cout << Version << std::endl;
}

void SDKSystem::LoadShader() {
	SDK::Shader.LoadVertexShader("SDKResource//GLSL//Vertex.glsl");
	SDK::Shader.LoadFragmentShader("SDKResource//GLSL//Fragment_Image.glsl");
	SDK::Shader.CreateShader(IMAGE_SHADER);

	SDK::Shader.LoadVertexShader("SDKResource//GLSL//Vertex.glsl");
	SDK::Shader.LoadFragmentShader("SDKResource//GLSL//Fragment_Text.glsl");
	SDK::Shader.CreateShader(TEXT_SHADER);

	SDK::Shader.LoadVertexShader("SDKResource//GLSL//Vertex.glsl");
	SDK::Shader.LoadFragmentShader("SDKResource//GLSL//Fragment_Shape.glsl");
	SDK::Shader.CreateShader(SHAPE_SHADER);

	SDK::Shader.LoadComputeShader("SDKResource//GLSL//ComputeMatrix.glsl");
	SDK::Shader.CreateComputeShader(MATRIX_COMPT_SHADER);

	SDK::Shader.CreateShaderLocation();
	SDK::Shader.CreateSSBO();
}

void SDKSystem::SetGlOption() {
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void SDKSystem::InitSystem() {
	FPSLimit = FRAME_LIMITS;
	if (FPSLimit > 0)
		DestFPS = 1000.0 / (float)FPSLimit;

	SDK::Scene.Init(LoadingMode.Start);
}