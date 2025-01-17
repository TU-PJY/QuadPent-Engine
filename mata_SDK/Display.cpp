#include "SDKHeader.h"

GLvoid SDKSystem::DisplayReshape(int w, int h) {
	glViewport(0, 0, w, h);
	WIDTH = w;
	HEIGHT = h;
}

void SDKSystem::SetBackColor(GLfloat R, GLfloat G, GLfloat B) {
	BackColor.r = R;
	BackColor.g = G;
	BackColor.b = B;
}

void SDKSystem::SetBackColorRGB(int R, int G, int B) {
	BackColor.r = (1.0f / 255.0f) * (GLfloat)R;
	BackColor.g = (1.0f / 255.0f) * (GLfloat)G;
	BackColor.b = (1.0f / 255.0f) * (GLfloat)B;
}

void SDKSystem::SetFrameLimit(int FrameLimit) {
	FPSLimit = FrameLimit;
	DestFPS = 1000.0 / (float)FPSLimit;
}

void SDKSystem::HideCursor() {
	glutSetCursor(GLUT_CURSOR_NONE);
}

void SDKSystem::ShowCursor() {
	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
}

void SDKSystem::SwitchScreenState() {
	switch (FullscreenState) {
	case false:
		glutFullScreen();
		WIDTH = GetSystemMetrics(SM_CXSCREEN);
		HEIGHT = GetSystemMetrics(SM_CYSCREEN);
		FullscreenState = true;
		break;

	case true:
		WIDTH = WINDOW_WIDTH;
		HEIGHT = WINDOW_HEIGHT;
		glutReshapeWindow(WIDTH, HEIGHT);
		glutPositionWindow(0, 0);
		FullscreenState = false;
		break;
	}
}

void SDKSystem::ChangeScreenSize(int ScreenWidth, int ScreenHeight) {
	WIDTH = ScreenWidth;
	HEIGHT = ScreenHeight;
	glutReshapeWindow(WIDTH, HEIGHT);
	glutPositionWindow(0, 0);
	FullscreenState = false;
}
