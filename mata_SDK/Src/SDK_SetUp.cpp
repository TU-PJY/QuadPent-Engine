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

int MSDK::WIDTH = WINDOW_WIDTH;
int MSDK::HEIGHT = WINDOW_HEIGHT;
int MSDK::PREV_WIDTH, MSDK::PREV_HEIGHT;

MSDK::Object* Indicator;

SDK_Camera MSDK::Camera;

float MSDK::ASPECT;
MSDK::ViewportRect MSDK::WindowRect;
glm::vec3 MSDK::ViewportColor;
const glm::mat4 MSDK::IdentityMatrix = glm::mat4(1.0f);

MSDK::SDK_SYSTEM_RESOURCE MSDK::SYSRES;
MSDK::SDK_IMAGE_RESOURCE MSDK::IMAGE;
MSDK::SDK_SOUND_RESOURCE MSDK::SOUND;
MSDK::SDK_SOUND_CHANNEL_RESOURCE MSDK::CHANNEL;
MSDK::SDK_GLOBAL_RESOURCE MSDK::GLOBAL;
MSDK::SDK_FORMAT_RESOURCE MSDK::FORMAT;
MSDK::SDK_FILE_RESOURCE MSDK::FILE;
MSDK::SDK_FONT_NAME_RESOURCE MSDK::FONTNAME;
MSDK::SDK_MODE_RESOURCE MSDK::MODE;

std::wstring MSDK::LOCALE;
MSDK::START_MODE_PTR MSDK::START_MODE;

void MSDK::SDKSystem::SetupSystem(int argc, char** argv) {
	glutInit(&argc, argv);
	SetupWindow();
	LoadShader();
	SetGlOption();
	InitSystem();
}

void MSDK::SDKSystem::SetupWindow() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GL_MULTISAMPLE);

	glutInitWindowPosition(GetSystemMetrics(SM_CXSCREEN) / 2 - MSDK::WIDTH / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - MSDK::HEIGHT / 2);
	glutInitWindowSize(MSDK::WIDTH, MSDK::HEIGHT);
	glutCreateWindow(WINDOW_NAME);

	if (FULL_SCREEN_OPTION) {
		glutFullScreen();
		MSDK::WIDTH = GetSystemMetrics(SM_CXSCREEN);
		MSDK::HEIGHT = GetSystemMetrics(SM_CYSCREEN);
	}

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Unable to initialize GLEW\n\n";
		exit(EXIT_FAILURE);
	}

	const GLubyte* Version = glGetString(GL_VERSION);
	std::cout << Version << std::endl;

	if (DISABLE_ALT_EVENT) {
		RegisterHotKey(NULL, 1, MOD_ALT, VK_MENU);
		RegisterHotKey(NULL, 2, MOD_ALT | MOD_NOREPEAT, VK_MENU);
	}
}

void MSDK::SDKSystem::LoadShader() {
	MSDK::Shader.LoadVertexShader("SDKResource//GLSL//Vertex.glsl");
	MSDK::Shader.LoadFragmentShader("SDKResource//GLSL//Fragment_Image.glsl");
	MSDK::Shader.CreateShader(IMAGE_SHADER);

	MSDK::Shader.LoadVertexShader("SDKResource//GLSL//Vertex.glsl");
	MSDK::Shader.LoadFragmentShader("SDKResource//GLSL//Fragment_Text.glsl");
	MSDK::Shader.CreateShader(TEXT_SHADER);

	MSDK::Shader.LoadVertexShader("SDKResource//GLSL//Vertex.glsl");
	MSDK::Shader.LoadFragmentShader("SDKResource//GLSL//Fragment_Shape.glsl");
	MSDK::Shader.CreateShader(SHAPE_SHADER);

	MSDK::Shader.LoadComputeShader("SDKResource//GLSL//ComputeMatrix.glsl");
	MSDK::Shader.CreateComputeShader(MATRIX_COMPT_SHADER);

	MSDK::Shader.CreateShaderLocation();
	MSDK::Shader.CreateSSBO();
}

void MSDK::SDKSystem::SetGlOption() {
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void MSDK::SDKSystem::InitSystem() {
	FPSLimit = FRAME_LIMITS;
	if (FPSLimit > 0)
		DestFPS = 1000.0 / (float)FPSLimit;

	MSDK::Scene.Init(LoadingMode.Start);
}