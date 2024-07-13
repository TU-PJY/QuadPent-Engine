#include "Mode1.h"
#include "Framework.h"
#include "MouseUtil.h"
#include "CameraUtil.h"

MouseUtil mouse;

void Mode1::SetController() {
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(MousePassiveMotion);
	glutKeyboardFunc(KeyDown);
	glutKeyboardUpFunc(KeyUp);
	glutMouseWheelFunc(ProcessMouseWheel);
	glutMouseFunc(ProcessMouse);
	glutSpecialFunc(SpecialKeyDown);
	glutSpecialUpFunc(SpecialKeyUp);
}

std::string Mode1::GameMode1() {

	return __func__;
}

void Mode1::ProcessKeyboard(unsigned char KEY, int S_KEY, bool KeyDown, bool SpecialKey) {
	// Normal Key Down
	if (KeyDown && !SpecialKey) {
		switch (KEY) {
		case 27:
			glutDestroyWindow(1);
			break;
		}
	}
}

void Mode1::ProcessMouse(int button, int state, int x, int y) {
}

void Mode1::ProcessMouseWheel(int button, int Wheel, int x, int y) {
}

void Mode1::KeyDown(unsigned char KEY, int x, int y) {
	ProcessKeyboard(KEY, NULL, true, false);
}

void Mode1::KeyUp(unsigned char KEY, int x, int y) {
	ProcessKeyboard(KEY, NULL, false, false);
}

void Mode1::SpecialKeyUp(int KEY, int x, int y) {
	ProcessKeyboard(NULL, KEY, true, true);
}

void Mode1::SpecialKeyDown(int KEY, int x, int y) {
	ProcessKeyboard(NULL, KEY, false, true);
}

void Mode1::MouseMotion(int x, int y) {
	mouse.ConvertPosition(x, y);
}

void Mode1::MousePassiveMotion(int x, int y) {
	mouse.ConvertPosition(x, y);
}