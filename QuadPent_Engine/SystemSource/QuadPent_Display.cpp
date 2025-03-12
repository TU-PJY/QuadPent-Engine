#include "QuadPent_Header.h"

void QP::SDK_System::DisplayReshape(int w, int h) {
	glViewport(0, 0, w, h);
	QP::WindowWidth = w;
	QP::WindowHeight = h;
}

void QP::SDK_System::SetBackColor(float R, float G, float B) {
	ViewportColor.r = R;
	ViewportColor.g = G;
	ViewportColor.b = B;
}

void QP::SDK_System::SetBackColor(QP::Color3& Color) {
	ViewportColor = Color;
}

void QP::SDK_System::SetBackColorRGB(int R, int G, int B) {
	ViewportColor.r = (1.0f / 255.0f) * (float)R;
	ViewportColor.g = (1.0f / 255.0f) * (float)G;
	ViewportColor.b = (1.0f / 255.0f) * (float)B;
}

void QP::SDK_System::SetFrameLimit(int FrameLimit) {
	FPSLimit = FrameLimit;
	DestFPS = 1000.0 / (float)FPSLimit;
}

void QP::SDK_System::HideCursor() {
	glutSetCursor(GLUT_CURSOR_NONE);
}

void QP::SDK_System::ShowCursor() {
	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
}

void QP::SDK_System::MoveCursor(int X, int Y) {
	glutWarpPointer(X, Y);
}

void QP::SDK_System::SwitchScreenState() {
	if (!FullscreenState) {
		glutFullScreen();
		QP::WindowWidth = GetSystemMetrics(SM_CXSCREEN);
		QP::WindowHeight = GetSystemMetrics(SM_CYSCREEN);
		FullscreenState = true;
	}
	else {
		RECT DisplayArea{};

		if (SystemParametersInfo(SPI_GETWORKAREA, 0, &DisplayArea, 0)) {
			int DisplayWidth = DisplayArea.right - DisplayArea.left;
			int DisplayHeight = DisplayArea.bottom - DisplayArea.top;
			QP::WindowWidth = DisplayWidth;
			QP::WindowHeight = DisplayHeight;
			glutReshapeWindow(QP::WindowWidth, QP::WindowHeight);
			glutPositionWindow(0, 0);
			FullscreenState = false;
		}
	}
}

void QP::SDK_System::ChangeScreenSize(int ScreenWidth, int ScreenHeight) {
	QP::WindowWidth = ScreenWidth;
	QP::WindowHeight = ScreenHeight;
	glutReshapeWindow(QP::WindowWidth, QP::WindowHeight);
	glutPositionWindow(0, 0);
	FullscreenState = false;
}
