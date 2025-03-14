#include "QuadPent_Header.h"

void QP::QuadPent_System::DisplayReshape(int w, int h) {
	glViewport(0, 0, w, h);

	WindowWidth = w;
	WindowHeight = h;

	Aspect = (float)WindowWidth / (float)WindowHeight;
	ViewportWidth = 2.0 * Aspect;
	ViewportHeight = 2.0;

	WindowRect.LeftX = -1.0 * Aspect;
	WindowRect.LeftY = -1.0;
	WindowRect.RightX = 1.0 * Aspect;
	WindowRect.RightY = 1.0;
}

void QP::QuadPent_System::SetBackColor(float R, float G, float B) {
	ViewportColor.r = R;
	ViewportColor.g = G;
	ViewportColor.b = B;
}

void QP::QuadPent_System::SetBackColor(QP::Color3& Color) {
	ViewportColor = Color;
}

void QP::QuadPent_System::SetBackColorRGB(int R, int G, int B) {
	ViewportColor.r = (1.0f / 255.0f) * (float)R;
	ViewportColor.g = (1.0f / 255.0f) * (float)G;
	ViewportColor.b = (1.0f / 255.0f) * (float)B;
}

void QP::QuadPent_System::SetFrameLimit(int FrameLimit) {
	FPSLimit = FrameLimit;
	DestFPS = 1000.0 / (float)FPSLimit;
}

void QP::QuadPent_System::HideCursor() {
	glutSetCursor(GLUT_CURSOR_NONE);
}

void QP::QuadPent_System::ShowCursor() {
	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
}

void QP::QuadPent_System::MoveCursor(int X, int Y) {
	glutWarpPointer(X, Y);
}

void QP::QuadPent_System::SwitchScreenState() {
	if (!FullscreenState) {
		glutFullScreen();
		FullscreenState = true;
	}
	else {
		glutLeaveFullScreen();
		FullscreenState = false;
	}
}

void QP::QuadPent_System::ChangeScreenSize(int ScreenWidth, int ScreenHeight, int PositionX, int PositionY) {
	WindowWidth = ScreenWidth;
	WindowHeight = ScreenHeight;
	glutReshapeWindow(WindowWidth, WindowHeight);
	glutPositionWindow(PositionX, PositionY);
	FullscreenState = false;
}

////////////////////////////////////////////// private
void QP::QuadPent_System::SwitchToMaximizedWindow() {
	LONG Style = GetWindowLong(System_HWND, GWL_STYLE);
	Style |= (WS_OVERLAPPEDWINDOW);
	SetWindowLong(System_HWND, GWL_STYLE, Style);
	SetWindowPos(
		System_HWND, NULL, 0, 0,
		GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
		SWP_NOZORDER | SWP_NOREDRAW
	);
	RedrawWindow(System_HWND, NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW);
	ShowWindow(System_HWND, SW_MAXIMIZE);
}

void QP::QuadPent_System::SwitchToSplashWindow() {
	LONG Style = GetWindowLong(System_HWND, GWL_STYLE);
	Style &= ~(WS_OVERLAPPEDWINDOW);
	SetWindowLong(System_HWND, GWL_STYLE, Style);
	SetWindowPos(
		QP::System_HWND, NULL,
		GetSystemMetrics(SM_CXSCREEN) / 2 - WindowWidth / 2,
		GetSystemMetrics(SM_CYSCREEN) / 2 - WindowHeight / 2,
		WindowWidth, WindowHeight, SWP_NOZORDER | SWP_NOREDRAW
	);
	RedrawWindow(System_HWND, NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW);
}
