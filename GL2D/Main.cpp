#include "MATA_HEADER.h"
#include "Framework.h"
#include "CameraUtil.h"
#include "ImageUtil.h"
#include "DataUtil.h"
#include "FontLoaderUtil.h"
#include "RandomUtil.h"
#include "RenderModeUtil.h"
#include "ShaderUtil.h"
#include "SoundUtil.h"
#include "TextUtil.h"
#include "TimerUtil.h"
#include <ctime>
#include <iostream>
#include <map>
#include <windows.h>

int WIDTH = 1200;
int HEIGHT = 800;

GLfloat ASPECT;
Rect rect;
glm::vec3 BackColor;
std::string VENDOR;

ShaderUtil shaderUtil;
CameraUtil camera;
CamaraControlUtil camUtil;
RenderModeUtil renderMode;
ImageUtil imageUtil;
TextUtilUnicode textUnicode;
SoundUtil soundUtil;
DataUtil dataUtil;
FontLoaderUtil fontloaderUtil;
Framework fw;

clock_t StartTime, EndTime;
float DeltaTime;

#include "Mode1.h"

GLvoid DisplayReshape(int w, int h) {
	glViewport(0, 0, w, h);
	WIDTH = w;
	HEIGHT = h;
}

GLvoid GLMain() {
	StartTime = clock();

	glClearColor(BackColor.r, BackColor.g, BackColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	fw.Routine();
	soundUtil.Update();

	glutSwapBuffers();
	glutPostRedisplay();

	EndTime = clock();
	DeltaTime = float(EndTime - StartTime) / 1000;
	fw.SetFrameTime(DeltaTime);
}

void main(int argc, char** argv) {
	if (!ShowConsole) {
		HWND hWnd = GetConsoleWindow();
		ShowWindow(hWnd, SW_HIDE);
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GL_MULTISAMPLE);

	glutInitWindowPosition(GetSystemMetrics(SM_CXSCREEN) / 2 - WIDTH / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - HEIGHT / 2);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow(WindowName);

	if (StartWithFullScreen) {
		glutFullScreen();
		WIDTH = GetSystemMetrics(SM_CXSCREEN);
		HEIGHT = GetSystemMetrics(SM_CYSCREEN);
	}

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Unable to initialize GLEW\n\n";
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n\n";

	const GLubyte* VendorInfo = glGetString(GL_VENDOR);
	if (VendorInfo) {
		VENDOR = reinterpret_cast<const char*>(VendorInfo);
		if (PrintVendorInfoOpt)
			std::cout << "GPU Vendor: " << VENDOR << "\n\n";
	}

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	shaderUtil.LoadVertexShader("MATA_ENGINE_RES//GLSL//Vertex.glsl");
	shaderUtil.LoadFragmentShader("MATA_ENGINE_RES//GLSL//Fragment_Image.glsl");
	shaderUtil.CreateShader(ImageShader);

	shaderUtil.LoadFragmentShader("MATA_ENGINE_RES//GLSL//Fragment_Text.glsl");
	shaderUtil.CreateShader(TextShader);

	SetBackColor(1.0, 1.0, 1.0);

	imageUtil.Init();
	imageUtil.LoadImageFromList();
	soundUtil.Init();
	camera.Init();

	fw.Init(Mode1::GameMode1, Mode1::SetController);

	glutDisplayFunc(GLMain);
	glutReshapeFunc(DisplayReshape);
	glutMainLoop();
}

void SetBackColor(GLfloat R, GLfloat G, GLfloat B) {
	BackColor.r = R;
	BackColor.g = G;
	BackColor.b = B;
}