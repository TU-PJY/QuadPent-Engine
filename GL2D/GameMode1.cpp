#include "GameMode1.h"
#include "Framework.h"
#include "MouseUtil.h"

void GameMode1::Start() {

	framework.RegisterController(Controller, ModeType::Default);
	framework.RegisterDestructor(Destructor);
	framework.RegisterModeName("GameMode1");
}

void GameMode1::Destructor() {

}

void GameMode1::ProcessKeyboard(KeyType Type, KeyState State, unsigned char NormalKey, int SpecialKey) {
	if (Type == KeyType::Normal && State == KeyState::Down) {
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
	ProcessKeyboard(KeyType::Normal, KeyState::Down, KEY, NULL);
}

void GameMode1::KeyUp(unsigned char KEY, int X, int Y) {
	ProcessKeyboard(KeyType::Normal, KeyState::Up, KEY, NULL);
}

void GameMode1::SpecialKeyDown(int KEY, int X, int Y) {
	ProcessKeyboard(KeyType::Special, KeyState::Down, NULL, KEY);
}

void GameMode1::SpecialKeyUp(int KEY, int X, int Y) {
	ProcessKeyboard(KeyType::Special, KeyState::Up, NULL, KEY);
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