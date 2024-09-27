#include "LoadingMode.h"
#include "Framework.h"
#include "MouseUtil.h"
#include "LoadingScreen.h"

void LoadingMode::Start() {
	framework.AddObject(new LoadingScreen, "loading_screen", Layer::L1);
	framework.RegisterController(Controller, ModeType::Default);
	framework.RegisterDestructor(Destructor);
	framework.RegisterModeName("LoadingMode");
}

void LoadingMode::Destructor() {}

void LoadingMode::ProcessKeyboard(KeyType Type, KeyState State, unsigned char NormalKey, int SpecialKey) {
	if (Type == KeyType::Normal && State == KeyState::Down) {
		switch (NormalKey) {
		case 27: //ESC
			framework.Exit();
			break;
		}
	}

}

void LoadingMode::ProcessMouse(int Button, int State, int X, int Y) {
}

void LoadingMode::ProcessMouseWheel(int Button, int Wheel, int X, int Y) {
}



void LoadingMode::KeyDown(unsigned char KEY, int X, int Y) {
	ProcessKeyboard(KeyType::Normal, KeyState::Down, KEY, NULL);
}

void LoadingMode::KeyUp(unsigned char KEY, int X, int Y) {
	ProcessKeyboard(KeyType::Normal, KeyState::Up, KEY, NULL);
}

void LoadingMode::SpecialKeyDown(int KEY, int X, int Y) {
	ProcessKeyboard(KeyType::Special, KeyState::Down, NULL, KEY);
}

void LoadingMode::SpecialKeyUp(int KEY, int X, int Y) {
	ProcessKeyboard(KeyType::Special, KeyState::Up, NULL, KEY);
}

void LoadingMode::MouseMotion(int X, int Y) {
	mouse.ConvertPosition(X, Y);
}

void LoadingMode::MousePassiveMotion(int X, int Y) {
	mouse.ConvertPosition(X, Y);
}

void LoadingMode::Controller() {
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(MousePassiveMotion);
	glutKeyboardFunc(KeyDown);
	glutKeyboardUpFunc(KeyUp);
	glutMouseWheelFunc(ProcessMouseWheel);
	glutMouseFunc(ProcessMouse);
	glutSpecialFunc(SpecialKeyDown);
	glutSpecialUpFunc(SpecialKeyUp);
}
