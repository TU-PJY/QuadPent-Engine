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

void Mode1::ProcessKeyboard(KeyType Type, KeyState State, unsigned char NormalKey, int SpecialKey) {
	if (Type == KeyType::NormalKey && State == KeyState::Down) {
		switch (NormalKey) {
		case 27: //ESC
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
	ProcessKeyboard(KeyType::NormalKey, KeyState::Down, KEY, NULL);
}

void Mode1::KeyUp(unsigned char KEY, int x, int y) {
	ProcessKeyboard(KeyType::NormalKey, KeyState::Up, KEY, NULL);
}

void Mode1::SpecialKeyUp(int KEY, int x, int y) {
	ProcessKeyboard(KeyType::SpecialKey, KeyState::Down, NULL, KEY);
}

void Mode1::SpecialKeyDown(int KEY, int x, int y) {
	ProcessKeyboard(KeyType::SpecialKey, KeyState::Up, NULL, KEY);
}

void Mode1::MouseMotion(int x, int y) {
	mouse.ConvertPosition(x, y);
}

void Mode1::MousePassiveMotion(int x, int y) {
	mouse.ConvertPosition(x, y);
}