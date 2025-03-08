#include "SDK_Header.h"
#include "SDK_Scene.h"
#include "SDK_Camera.h"
#include "SDK_CameraController.h"
#include "SDK_Shader.h"
#include "SDK_Config.h"
#include "SDK_Resource.h"
#include "SDK_ModeResource.h"

#include "SDK_LoadingMode.h"

float SDK::Aspect;
float SDK::ViewportWidth, SDK::ViewportHeight;
int SDK::WindowWidth = WINDOW_WIDTH;
int SDK::WindowHeight = WINDOW_HEIGHT;
int SDK::PrevWindowWidth, SDK::PrevWindowHeight;

SDK::SDK_Camera SDK::Camera;

SDK::ViewportRect SDK::WindowRect;

SDK::SDK_SYSTEM_RESOURCE SDK::SYSRES;
SDK::SDK_IMAGE_RESOURCE SDK::IMAGE;
SDK::SDK_SOUND_RESOURCE SDK::SOUND;
SDK::SDK_SOUND_CHANNEL_RESOURCE SDK::CHANNEL;
SDK::SDK_GLOBAL_RESOURCE SDK::GLOBAL;
SDK::SDK_FORMAT_RESOURCE SDK::FORMAT;
SDK::SDK_FILE_RESOURCE SDK::FILE;
SDK::SDK_FONT_RESOURCE SDK::FONT;
SDK::SDK_MODE_RESOURCE SDK::MODE;

SDK::START_MODE_PTR SDK::START_MODE;

HWND SDK::System_HWND;
bool SDK::System_ClippingState;
std::wstring SDK::SYSTEM_LOCALE;
std::string SDK::USER_DOCUMENT_PATH = std::filesystem::path(std::getenv("USERPROFILE")).string();

int InitializedMajorVersion, InitializedMinorVersion;

void SDK::SDK_System::SetupSystem(int argc, char** argv) {
	glutInit(&argc, argv);
	SetupWindow();
	LoadShader();
	SetGlOption();
	InitSystem();
}

void SDK::SDK_System::SetupWindow() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GL_MULTISAMPLE);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitWindowPosition(GetSystemMetrics(SM_CXSCREEN) / 2 - SDK::WindowWidth / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - SDK::WindowHeight / 2);
	glutInitWindowSize(SDK::WindowWidth, SDK::WindowHeight);
	glutCreateWindow(WINDOW_NAME);

	const unsigned char* Version = glGetString(GL_VERSION);
	std::cout << Version << std::endl;

	glGetIntegerv(GL_MAJOR_VERSION, &InitializedMajorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &InitializedMinorVersion);
	std::cout << "Initialized OpenGL Version: " << InitializedMajorVersion << "." << InitializedMinorVersion << std::endl;

	if (InitializedMajorVersion < 4 || (InitializedMajorVersion == 4 && InitializedMinorVersion < 3)) {
		std::wstring Str = L"The OpenGL support version of your graphics card is too low. Must support at least OpenGL 4.3 Version.";
		int Result = MessageBox(NULL, Str.c_str(), L"mata_SDK Error", MB_OK | MB_ICONINFORMATION);
		if (Result == IDOK)
			SDK::System.Exit();
	}

	if (FULL_SCREEN_OPTION) {
		glutFullScreen();
		SDK::WindowWidth = GetSystemMetrics(SM_CXSCREEN);
		SDK::WindowHeight = GetSystemMetrics(SM_CYSCREEN);
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

	SDK::System_HWND = FindWindowA(nullptr, WINDOW_NAME);

	std::wstring WINDOW_ICON_PATH = std::wstring(WINDOW_TITLE_BAR_ICON_FILE_PATH);
	if (!WINDOW_ICON_PATH.empty() && SDK::System_HWND) {
		HICON Icon[1]{};
		if (ExtractIconEx(WINDOW_TITLE_BAR_ICON_FILE_PATH, 0, &Icon[0], NULL, 1) > 0) {
			PostMessage(SDK::System_HWND, WM_SETICON, ICON_SMALL, (LPARAM)Icon[0]);
			PostMessage(SDK::System_HWND, WM_SETICON, ICON_BIG, (LPARAM)Icon[0]);
		}
	}
}

void SDK::SDK_System::LoadShader() {
	std::string FolderName = "SDKResource//GLSL//" + std::to_string(InitializedMajorVersion) + "." + std::to_string(InitializedMinorVersion) + "//";

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