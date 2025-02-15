#include "SDKHeader.h"

GLvoid SDKSystem::DisplayReshape(int w, int h) {
	glViewport(0, 0, w, h);
	SDK::WIDTH = w;
	SDK::HEIGHT = h;
}

void SDKSystem::SetBackColor(GLfloat R, GLfloat G, GLfloat B) {
	SDK::ViewportColor.r = R;
	SDK::ViewportColor.g = G;
	SDK::ViewportColor.b = B;
}

void SDKSystem::SetBackColorRGB(int R, int G, int B) {
	SDK::ViewportColor.r = (1.0f / 255.0f) * (GLfloat)R;
	SDK::ViewportColor.g = (1.0f / 255.0f) * (GLfloat)G;
	SDK::ViewportColor.b = (1.0f / 255.0f) * (GLfloat)B;
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

void SDKSystem::MoveCursor(int X, int Y) {
	glutWarpPointer(X, Y);
}

void SDKSystem::SwitchScreenState() {
	if (!FullscreenState) {
		glutFullScreen();
		SDK::WIDTH = GetSystemMetrics(SM_CXSCREEN);
		SDK::HEIGHT = GetSystemMetrics(SM_CYSCREEN);
		FullscreenState = true;
	}
	else {
		RECT DisplayArea{};

		if (SystemParametersInfo(SPI_GETWORKAREA, 0, &DisplayArea, 0)) {
			int DisplayWidth = DisplayArea.right - DisplayArea.left;
			int DisplayHeight = DisplayArea.bottom - DisplayArea.top;
			SDK::WIDTH = DisplayWidth;
			SDK::HEIGHT = DisplayHeight;
			glutReshapeWindow(SDK::WIDTH, SDK::HEIGHT);
			glutPositionWindow(0, 0);
			FullscreenState = false;
		}
	}
}

void SDKSystem::ChangeScreenSize(int ScreenWidth, int ScreenHeight) {
	SDK::WIDTH = ScreenWidth;
	SDK::HEIGHT = ScreenHeight;
	glutReshapeWindow(SDK::WIDTH, SDK::HEIGHT);
	glutPositionWindow(0, 0);
	FullscreenState = false;
}
