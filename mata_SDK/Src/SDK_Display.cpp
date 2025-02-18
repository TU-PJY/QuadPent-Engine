#include "SDK_Header.h"

void MSDK::SDKSystem::DisplayReshape(int w, int h) {
	glViewport(0, 0, w, h);
	MSDK::WIDTH = w;
	MSDK::HEIGHT = h;
}

void MSDK::SDKSystem::SetBackColor(float R, float G, float B) {
	MSDK::ViewportColor.r = R;
	MSDK::ViewportColor.g = G;
	MSDK::ViewportColor.b = B;
}

void MSDK::SDKSystem::SetBackColorRGB(int R, int G, int B) {
	MSDK::ViewportColor.r = (1.0f / 255.0f) * (float)R;
	MSDK::ViewportColor.g = (1.0f / 255.0f) * (float)G;
	MSDK::ViewportColor.b = (1.0f / 255.0f) * (float)B;
}

void MSDK::SDKSystem::SetFrameLimit(int FrameLimit) {
	FPSLimit = FrameLimit;
	DestFPS = 1000.0 / (float)FPSLimit;
}

void MSDK::SDKSystem::HideCursor() {
	glutSetCursor(GLUT_CURSOR_NONE);
}

void MSDK::SDKSystem::ShowCursor() {
	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
}

void MSDK::SDKSystem::MoveCursor(int X, int Y) {
	glutWarpPointer(X, Y);
}

void MSDK::SDKSystem::SwitchScreenState() {
	if (!FullscreenState) {
		glutFullScreen();
		MSDK::WIDTH = GetSystemMetrics(SM_CXSCREEN);
		MSDK::HEIGHT = GetSystemMetrics(SM_CYSCREEN);
		FullscreenState = true;
	}
	else {
		RECT DisplayArea{};

		if (SystemParametersInfo(SPI_GETWORKAREA, 0, &DisplayArea, 0)) {
			int DisplayWidth = DisplayArea.right - DisplayArea.left;
			int DisplayHeight = DisplayArea.bottom - DisplayArea.top;
			MSDK::WIDTH = DisplayWidth;
			MSDK::HEIGHT = DisplayHeight;
			glutReshapeWindow(MSDK::WIDTH, MSDK::HEIGHT);
			glutPositionWindow(0, 0);
			FullscreenState = false;
		}
	}
}

void MSDK::SDKSystem::ChangeScreenSize(int ScreenWidth, int ScreenHeight) {
	MSDK::WIDTH = ScreenWidth;
	MSDK::HEIGHT = ScreenHeight;
	glutReshapeWindow(MSDK::WIDTH, MSDK::HEIGHT);
	glutPositionWindow(0, 0);
	FullscreenState = false;
}
