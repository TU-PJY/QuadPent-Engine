#include "EngineHeader.h"

GLvoid Framework::DisplayReshape(int w, int h) {
	glViewport(0, 0, w, h);
	WIDTH = w;
	HEIGHT = h;
}

void Framework::SetBackColor(GLfloat R, GLfloat G, GLfloat B) {
	BackColor.r = R;
	BackColor.g = G;
	BackColor.b = B;
}

void Framework::SetBackColorRGB(int R, int G, int B) {
	BackColor.r = (1.0f / 255.0f) * (GLfloat)R;
	BackColor.g = (1.0f / 255.0f) * (GLfloat)G;
	BackColor.b = (1.0f / 255.0f) * (GLfloat)B;
}

void Framework::SetFrameLimit(int FrameLimit) {
	FPSLimit = FrameLimit;
	DestFPS = 1000.0 / (float)FPSLimit;
}

void Framework::HideCursor() {
	glutSetCursor(GLUT_CURSOR_NONE);
}

void Framework::ShowCursor() {
	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
}

void Framework::SwitchScreenState() {
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
