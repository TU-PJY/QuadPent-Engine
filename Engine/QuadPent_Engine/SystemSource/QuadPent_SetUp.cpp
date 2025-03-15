#include "../resource.h"
#include "QuadPent_Header.h"
#include "QuadPent_Scene.h"
#include "QuadPent_Shader.h"
#include "../WorkSpace/Resource/QuadPent_Resource.h"
#include "../WorkSpace/Resource/QuadPent_ModeResource.h"

#include "QuadPent_StartUpMode.h"

float QP::Aspect;
float QP::ViewportWidth, QP::ViewportHeight;
int QP::WindowWidth, QP::WindowHeight;

QP::ViewportRect QP::WindowRect;
QP::QuadPent_Camera QP::Camera;

QP::QuadPent_SYSTEM_RESOURCE QP::SYSRES;
ASSET::QuadPent_MODE_RESOURCE ASSET::MODE;
ASSET::QuadPent_IMAGE_RESOURCE ASSET::IMAGE;
ASSET::QuadPent_SOUND_RESOURCE ASSET::SOUND;
ASSET::QuadPent_SOUND_CHANNEL_RESOURCE ASSET::CHANNEL;
ASSET::QuadPent_GLOBAL_RESOURCE ASSET::GLOBAL;
ASSET::QuadPent_FORMAT_RESOURCE ASSET::FORMAT;
ASSET::QuadPent_FILE_RESOURCE ASSET::FILE;
ASSET::QuadPent_FONT_RESOURCE ASSET::FONT;

QP::START_MODE_PTR QP::START_MODE;
QuadPent_StartUpMode StartUpMode;

HWND QP::System_HWND;
HINSTANCE QP::System_INSTANCE;

bool QP::System_ClippingState;

std::wstring QP::SYSTEM_LOCALE;
std::string QP::USER_DOCUMENT_PATH = std::filesystem::path(std::getenv("USERPROFILE")).string();
HICON QP::SYSTEM_TITLE_ICON;

int InitializedMajorVersion, InitializedMinorVersion;

void QP::QuadPent_System::SetupSystem(int argc, char** argv) {
	glutInit(&argc, argv);
	SetupWindow();
	LoadShader();
	SetGlOption();
	InitSystem();
}

void QP::QuadPent_System::SetupWindow() {
	if (ENABLE_DEV_MODE) {
		WindowWidth = DEV_SCREEN_WIDTH;
		WindowHeight = DEV_SCREEN_HEIGHT;
	}
	else {
		if (ENABLE_START_UP_SCREEN && ENABLE_SPLASH_SCREEN) {
			float Scale = (float)GetDpiForSystem() / 96.0f;
			WindowWidth = (int)(SPLASH_SCREEN_WIDTH * Scale);
			WindowHeight = (int)(SPLASH_SCREEN_HEIGHT * Scale);
		}
		else {
			WindowWidth = GetSystemMetrics(SM_CXSCREEN);
			WindowHeight = GetSystemMetrics(SM_CYSCREEN);
		}
	}

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GL_MULTISAMPLE);
	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	glutInitWindowSize(WindowWidth, WindowHeight);
	glutInitWindowPosition(
		GetSystemMetrics(SM_CXSCREEN) / 2 - WindowWidth / 2, 
		GetSystemMetrics(SM_CYSCREEN) / 2 - WindowHeight / 2
	);
	glutCreateWindow(WINDOW_NAME);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Unable to initialize GLEW\n\n";
		exit(EXIT_FAILURE);
	}

	// OpenGL version check
	const unsigned char* Version = glGetString(GL_VERSION);
	std::cout << Version << std::endl;

	glGetIntegerv(GL_MAJOR_VERSION, &InitializedMajorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &InitializedMinorVersion);
	std::cout << "Initialized OpenGL Version: " << InitializedMajorVersion << "." << InitializedMinorVersion << std::endl;

	if (InitializedMajorVersion < 4 || (InitializedMajorVersion == 4 && InitializedMinorVersion < 3)) {
		if (QP::SYSTEM_LOCALE == L"ko-KR")
			MessageBoxOut(L"QuadPent 실행 오류", L"그래픽카드가 지원하는 OpenGL 버전이 너무 낮습니다. 최소 OpenGL 4.3 버전 이상을 지원해야 합니다.");
		else
			MessageBoxOut(L"QuadPent execution error", L"The OpenGL support version of your graphics card is too low. Must support at least OpenGL 4.3 Version.");
	}

	System_HWND = FindWindowA(nullptr, WINDOW_NAME);
	System_INSTANCE = (HINSTANCE)GetWindowLongPtr(System_HWND, GWLP_HINSTANCE);

	SYSTEM_TITLE_ICON = LoadIcon(System_INSTANCE, MAKEINTRESOURCE(WINDOW_ICON));
	if (SYSTEM_TITLE_ICON) {
		SendMessage(System_HWND, WM_SETICON, ICON_SMALL, (LPARAM)SYSTEM_TITLE_ICON);
		SendMessage(System_HWND, WM_SETICON, ICON_BIG, (LPARAM)SYSTEM_TITLE_ICON);
	}

	if (!ENABLE_DEV_MODE) {
		if (ENABLE_START_UP_SCREEN && ENABLE_SPLASH_SCREEN)
			System.SwitchToSplashWindow();
		else {
			System.SwitchToMaximizedWindow();
			if (ENABLE_FULL_SCREEN)
				SwitchScreenState();
		}
	}

	if (DISABLE_ALT_EVENT) {
		RegisterHotKey(NULL, 1, MOD_ALT, VK_MENU);
		RegisterHotKey(NULL, 2, MOD_ALT | MOD_NOREPEAT, VK_MENU);
	}
}

void QP::QuadPent_System::LoadShader() {
	std::string FolderName = "SystemComponent//GLSL//" + std::to_string(InitializedMajorVersion) + "." + std::to_string(InitializedMinorVersion) + "//";

	Shader.LoadVertexShader(std::string(FolderName + "Vertex.glsl"));
	Shader.LoadFragmentShader(std::string(FolderName + "Fragment_Image.glsl"));
	Shader.CreateShader(IMAGE_SHADER);

	Shader.LoadVertexShader(std::string(FolderName + "Vertex.glsl"));
	Shader.LoadFragmentShader(std::string(FolderName + "Fragment_Text.glsl"));
	Shader.CreateShader(TEXT_SHADER);

	Shader.LoadVertexShader(std::string(FolderName + "Vertex.glsl"));
	Shader.LoadFragmentShader(std::string(FolderName + "Fragment_Shape.glsl"));
	Shader.CreateShader(SHAPE_SHADER);

	Shader.CreateShaderLocation();
}

void QP::QuadPent_System::SetGlOption() {
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void QP::QuadPent_System::InitSystem() {
	FPSLimit = FRAMERATE_LIMIT;
	if (FPSLimit > 0)
		DestFPS = 1000.0 / (float)FPSLimit;

	Scene.Init(StartUpMode.Start);
}