#include "EngineHeader.h"
#include "Scene.h"
#include "CameraUtil.h"
#include "CameraController.h"
#include "ImageUtil.h"
#include "ShaderUtil.h"
#include "SoundUtil.h"
#include "MouseUtil.h"
#include "Config.h"
#include "GameResource.h"
#include <windows.h>

#include "LoadingMode.h"


int WIDTH = WINDOW_WIDTH;
int HEIGHT = WINDOW_HEIGHT;
int PREV_WIDTH, PREV_HEIGHT;

float DestFPS;
int FPSLimit;
bool FullscreenState;

Camera camera;
GameObject* CameraControl;

GLfloat ASPECT;
ViewportRect WindowRect;
glm::vec3 BackColor;

ShaderUtil shaderUtil;
ImageUtil imageUtil;
RandomUtil randomUtil;
MoveUtil moveUtil;
EX_Util EX;
MathUtil mathUtil;
ComputeUtil computeUtil;
FontUtil fontUtil;
StringUtil stringUtil;
ColorClipping colorClip;
AlphaClipping alphaClip;

SystemResource SysRes;
SpriteResource Sprite;
AudioResource Audio;
DataResource Data;
GlobalValue Global;

#ifdef USE_SOUND_SYSTEM
SoundUtil soundUtil;
#endif

MouseUtil mouse;
Scene scene;

void Framework::SetupSystem(int argc, char** argv) {
	HWND hWnd = GetConsoleWindow();

	switch (SHOW_CONSOLE) {
	case true:
		ShowWindow(hWnd, SW_SHOWNORMAL);
		break;

	case false:
		ShowWindow(hWnd, SW_HIDE);
		break;
	}

	glutInit(&argc, argv);
	SetupWindow();
	LoadShader();
	SetGlOption();
	InitSystem();
}


void Framework::SetupWindow() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GL_MULTISAMPLE);

	glutInitWindowPosition(GetSystemMetrics(SM_CXSCREEN) / 2 - WIDTH / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - HEIGHT / 2);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow(WINDOW_NAME);

	if (FULL_SCREEN_OPTION) {
		glutFullScreen();
		WIDTH = GetSystemMetrics(SM_CXSCREEN);
		HEIGHT = GetSystemMetrics(SM_CYSCREEN);
	}

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Unable to initialize GLEW\n\n";
		exit(EXIT_FAILURE);
	}

	const GLubyte* Version = glGetString(GL_VERSION);
	std::cout << Version << std::endl;
}

void Framework::LoadShader() {
	shaderUtil.LoadVertexShader("MGKResource//GLSL//Vertex.glsl");
	shaderUtil.LoadFragmentShader("MGKResource//GLSL//Fragment_Image.glsl");
	shaderUtil.CreateShader(IMAGE_SHADER);

	shaderUtil.LoadVertexShader("MGKResource//GLSL//Vertex.glsl");
	shaderUtil.LoadFragmentShader("MGKResource//GLSL//Fragment_Text.glsl");
	shaderUtil.CreateShader(TEXT_SHADER);

	shaderUtil.LoadVertexShader("MGKResource//GLSL//Vertex.glsl");
	shaderUtil.LoadFragmentShader("MGKResource//GLSL//Fragment_Shape.glsl");
	shaderUtil.CreateShader(SHAPE_SHADER);

	shaderUtil.LoadComputeShader("MGKResource//GLSL//ComputeMatrix.glsl");
	shaderUtil.CreateComputeShader(MATRIX_COMPT_SHADER);

	shaderUtil.CreateShaderLocation();
	shaderUtil.CreateSSBO();
}

void Framework::SetGlOption() {
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Framework::InitSystem() {
	FPSLimit = FRAME_LIMITS;
	if (FPSLimit > 0)
		DestFPS = 1000.0 / (float)FPSLimit;

	scene.AddObject(new CameraController, "MGK_OBJECT_CAMERA_CONTROLLER", LAYER1, OBJECT_TYPE_STATIC);
	CameraControl = scene.Find("MGK_OBJECT_CAMERA_CONTROLLER");

	scene.Init(LoadingMode.Start);
}