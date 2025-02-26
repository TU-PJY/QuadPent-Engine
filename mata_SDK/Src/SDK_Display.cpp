#include "SDK_Header.h"

void SDK::SDK_System::DisplayReshape(int w, int h) {
	glViewport(0, 0, w, h);
	SDK::WindowWidth = w;
	SDK::WindowHeight = h;
}

void SDK::SDK_System::SetBackColor(float R, float G, float B) {
	ViewportColor.r = R;
	ViewportColor.g = G;
	ViewportColor.b = B;
}

void SDK::SDK_System::SetBackColor(SDK::Color3& Color) {
	ViewportColor = Color;
}

void SDK::SDK_System::SetBackColorRGB(int R, int G, int B) {
	ViewportColor.r = (1.0f / 255.0f) * (float)R;
	ViewportColor.g = (1.0f / 255.0f) * (float)G;
	ViewportColor.b = (1.0f / 255.0f) * (float)B;
}

void SDK::SDK_System::SetFrameLimit(int FrameLimit) {
	FPSLimit = FrameLimit;
	DestFPS = 1000.0 / (float)FPSLimit;
}

void SDK::SDK_System::HideCursor() {
	glutSetCursor(GLUT_CURSOR_NONE);
}

void SDK::SDK_System::ShowCursor() {
	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
}

void SDK::SDK_System::MoveCursor(int X, int Y) {
	glutWarpPointer(X, Y);
}

void SDK::SDK_System::SwitchScreenState() {
	if (!FullscreenState) {
		glutFullScreen();
		SDK::WindowWidth = GetSystemMetrics(SM_CXSCREEN);
		SDK::WindowHeight = GetSystemMetrics(SM_CYSCREEN);
		FullscreenState = true;
	}
	else {
		RECT DisplayArea{};

		if (SystemParametersInfo(SPI_GETWORKAREA, 0, &DisplayArea, 0)) {
			int DisplayWidth = DisplayArea.right - DisplayArea.left;
			int DisplayHeight = DisplayArea.bottom - DisplayArea.top;
			SDK::WindowWidth = DisplayWidth;
			SDK::WindowHeight = DisplayHeight;
			glutReshapeWindow(SDK::WindowWidth, SDK::WindowHeight);
			glutPositionWindow(0, 0);
			FullscreenState = false;
		}
	}
}

void SDK::SDK_System::ChangeScreenSize(int ScreenWidth, int ScreenHeight) {
	SDK::WindowWidth = ScreenWidth;
	SDK::WindowHeight = ScreenHeight;
	glutReshapeWindow(SDK::WindowWidth, SDK::WindowHeight);
	glutPositionWindow(0, 0);
	FullscreenState = false;
}
