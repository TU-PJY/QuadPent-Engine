#include "MATA_HEADER.h"
#include "Framework.h"
#include "CameraUtil.h"
#include "ImageUtil.h"
#include "DataUtil.h"
#include "FontLoaderUtil.h"
#include "RenderModeUtil.h"
#include "ShaderUtil.h"
#include "SoundUtil.h"
#include <iostream>
#include <windows.h>

int WIDTH = WindowWidth;
int HEIGHT = WindowHeight;
int PREV_WIDTH, PREV_HEIGHT;

GLfloat ASPECT;
Rect rect;
glm::vec3 BackColor;

std::string GPU_Vendor;

ShaderUtil shaderUtil;
Camera camera;
RenderModeUtil renderMode;
ImageUtil imageUtil;
SoundUtil soundUtil;
DataUtil dataUtil;
FontLoaderUtil fontloaderUtil;
Framework fw;

// frametime values
float PrevTime, CurrentTime, DeltaTime;

#include "Mode1.h"

GLvoid DisplayReshape(int w, int h) {
	glViewport(0, 0, w, h);
	WIDTH = w;
	HEIGHT = h;
}

GLvoid GLMain() {
	glClearColor(BackColor.r, BackColor.g, BackColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	fw.Routine();
	soundUtil.Update();

	CurrentTime = float(glutGet(GLUT_ELAPSED_TIME));
	DeltaTime = (CurrentTime - PrevTime) / 1000.0;

	fw.SetFrameTime(DeltaTime);

	PrevTime = CurrentTime;

	glutSwapBuffers();
	glutPostRedisplay();
}

void main(int argc, char** argv) {
	HWND hWnd = GetConsoleWindow();

	if (!ShowConsole)
		ShowWindow(hWnd, SW_HIDE);
	else
		ShowWindow(hWnd, SW_SHOWNORMAL);

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

	if (PrintGPU_VendorInfoOpt) {
		const GLubyte* GPU_VendorInfo = glGetString(GL_VENDOR);
		if (GPU_VendorInfo) {
			GPU_Vendor = reinterpret_cast<const char*>(GPU_VendorInfo);
			std::cout << "GPU Vendor: " << GPU_Vendor << "\n\n";
		}
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

	SetBackColor(0.3, 0.3, 0.3);

	imageUtil.Init();
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