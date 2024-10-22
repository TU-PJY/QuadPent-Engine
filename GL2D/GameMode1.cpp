#include "GameMode1.h"
#include "Framework.h"
#include "MouseUtil.h"

void GameMode1::Start() {
	framework.RegisterController(Controller, MODE_TYPE_DEFAULT);
	framework.RegisterDestructor(Destructor);
	framework.RegisterModeName("GameMode1");
}

void GameMode1::Destructor() {

}

void GameMode1::ProcessKeyboard(int Type, int State, unsigned char NormalKey, int SpecialKey) {
	if (Type == KEY_TYPE_NORMAL && State == KEY_DOWN) {
		switch (NormalKey) {
		case 27: //ESC
			framework.Exit();
			break;
		}
	}
}

void GameMode1::ProcessMouse(int Button, int State, int X, int Y) {
}

void GameMode1::ProcessMouseWheel(int Button, int Wheel, int X, int Y) {
}



void GameMode1::KeyDown(unsigned char KEY, int X, int Y) {
	ProcessKeyboard(KEY_TYPE_NORMAL, KEY_DOWN, KEY, NULL);
}

void GameMode1::KeyUp(unsigned char KEY, int X, int Y) {
	ProcessKeyboard(KEY_TYPE_NORMAL, KEY_UP, KEY, NULL);
}

void GameMode1::SpecialKeyDown(int KEY, int X, int Y) {
	ProcessKeyboard(KEY_TYPE_SPECIAL, KEY_DOWN, NULL, KEY);
}

void GameMode1::SpecialKeyUp(int KEY, int X, int Y) {
	ProcessKeyboard(KEY_TYPE_SPECIAL, KEY_UP, NULL, KEY);
}

void GameMode1::MouseMotion(int X, int Y) {
	mouse.ConvertPosition(X, Y);
}

void GameMode1::MousePassiveMotion(int X, int Y) {
	mouse.ConvertPosition(X, Y);
}

void GameMode1::Controller() {
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(MousePassiveMotion);
	glutKeyboardFunc(KeyDown);
	glutKeyboardUpFunc(KeyUp);
	glutMouseWheelFunc(ProcessMouseWheel);
	glutMouseFunc(ProcessMouse);
	glutSpecialFunc(SpecialKeyDown);
	glutSpecialUpFunc(SpecialKeyUp);
}