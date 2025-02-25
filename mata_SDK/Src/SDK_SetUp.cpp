#include "SDK_Header.h"
#include "SDK_Scene.h"
#include "SDK_Camera.h"
#include "SDK_CameraController.h"
#include "SDK_Shader.h"
#include "SDK_Config.h"
#include "SDK_Resource.h"
#include "SDK_ModeResource.h"

#include "SDK_LoadingMode.h"

int SDK::WIDTH = WINDOW_WIDTH;
int SDK::HEIGHT = WINDOW_HEIGHT;
int SDK::PREV_WIDTH, SDK::PREV_HEIGHT;

SDK::SDK_Camera SDK::Camera;

float SDK::ASPECT;
SDK::ViewportRect SDK::WindowRect;
glm::vec3 SDK::ViewportColor;
const glm::mat4 SDK::IdentityMatrix = glm::mat4(1.0f);

SDK::SDK_SYSTEM_RESOURCE SDK::SYSRES;
SDK::SDK_IMAGE_RESOURCE SDK::IMAGE;
SDK::SDK_SOUND_RESOURCE SDK::SOUND;
SDK::SDK_SOUND_CHANNEL_RESOURCE SDK::CHANNEL;
SDK::SDK_GLOBAL_RESOURCE SDK::GLOBAL;
SDK::SDK_FORMAT_RESOURCE SDK::FORMAT;
SDK::SDK_FILE_RESOURCE SDK::FILE;
SDK::SDK_FONT_NAME_RESOURCE SDK::FONTNAME;
SDK::SDK_MODE_RESOURCE SDK::MODE;

std::wstring SDK::LOCALE;
SDK::START_MODE_PTR SDK::START_MODE;

HWND SDK::SystemHWND;

bool SDK::ClippingState;

int MajorVersion, MinorVersion;

void SDK::SDK_System::SetupSystem(int argc, char** argv) {
	glutInit(&argc, argv);
	SetupWindow();
	LoadShader();
	SetGlOption();
	InitSystem();
}

void SDK::SDK_System::SetupWindow() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GL_MULTISAMPLE);

	glutInitContextVersion(4, 3); // OpenGL 4.3
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitWindowPosition(GetSystemMetrics(SM_CXSCREEN) / 2 - SDK::WIDTH / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - SDK::HEIGHT / 2);
	glutInitWindowSize(SDK::WIDTH, SDK::HEIGHT);
	glutCreateWindow(WINDOW_NAME);

	const unsigned char* Version = glGetString(GL_VERSION);
	std::cout << Version << std::endl;

	glGetIntegerv(GL_MAJOR_VERSION, &MajorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &MinorVersion);
	std::cout << "OpenGL Version: " << MajorVersion << "." << MinorVersion << std::endl;

	if (MajorVersion < 4 || (MajorVersion == 4 && MinorVersion < 3)) {
		int Result = MessageBox(NULL, L"The OpenGL support version of your graphics card is too low. Must support at least OpenGL 4.3 version.", L"mata_SDK Error", MB_OK | MB_ICONINFORMATION);
		if (Result == IDOK)
			SDK::System.Exit();
	}

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

	if (DISABLE_ALT_EVENT) {
		RegisterHotKey(NULL, 1, MOD_ALT, VK_MENU);
		RegisterHotKey(NULL, 2, MOD_ALT | MOD_NOREPEAT, VK_MENU);
	}

	SDK::SystemHWND = FindWindowA(nullptr, WINDOW_NAME);
}

void SDK::SDK_System::LoadShader() {
	std::string FolderName = "SDKResource//GLSL//" + std::to_string(MajorVersion) + "." + std::to_string(MinorVersion) + "//";

	SDK::Shader.LoadVertexShader(std::string(FolderName + "Vertex.glsl"));
	SDK::Shader.LoadFragmentShader(std::string(FolderName + "Fragment_Image.glsl"));
	SDK::Shader.CreateShader(IMAGE_SHADER);

	SDK::Shader.LoadVertexShader(std::string(FolderName + "Vertex.glsl"));
	SDK::Shader.LoadFragmentShader(std::string(FolderName + "Fragment_Text.glsl"));
	SDK::Shader.CreateShader(TEXT_SHADER);

	SDK::Shader.LoadVertexShader(std::string(FolderName + "Vertex.glsl"));
	SDK::Shader.LoadFragmentShader(std::string(FolderName + "Fragment_Shape.glsl"));
	SDK::Shader.CreateShader(SHAPE_SHADER);

	SDK::Shader.LoadComputeShader(std::string(FolderName + "ComputeMatrix.glsl"));
	SDK::Shader.CreateComputeShader(MATRIX_COMPT_SHADER);

	SDK::Shader.CreateShaderLocation();
	SDK::Shader.CreateSSBO();
}

void SDK::SDK_System::SetGlOption() {
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void SDK::SDK_System::InitSystem() {
	FPSLimit = FRAME_LIMITS;
	if (FPSLimit > 0)
		DestFPS = 1000.0 / (float)FPSLimit;

	SDK::Scene.Init(LoadingMode.Start);
}