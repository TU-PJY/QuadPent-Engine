#include "LoadingMode.h"
#include "Framework.h"
#include "MouseUtil.h"
#include "LoadingScreen.h"

void LoadingMode::Start() {
	framework.AddObject(new LoadingScreen, "loading_screen", LAYER_1);
	framework.RegisterController(Controller, MODE_TYPE_DEFAULT);
	framework.RegisterDestructor(Destructor);
	framework.RegisterModeName("LoadingMode");
}

void LoadingMode::Destructor() {}

void LoadingMode::ProcessKeyboard(int Type, int State, unsigned char NormalKey, int SpecialKey) {
	if (auto loading_screen = framework.Find("loading_screen"); loading_screen)
		loading_screen->InputKey(Type, State, NormalKey, SpecialKey);
}

void LoadingMode::ProcessMouse(int Button, int State, int X, int Y) {
}

void LoadingMode::ProcessMouseWheel(int Button, int Wheel, int X, int Y) {
}



void LoadingMode::KeyDown(unsigned char KEY, int X, int Y) {
	ProcessKeyboard(KEY_TYPE_NORMAL, KEY_DOWN, KEY, NULL);
}

void LoadingMode::KeyUp(unsigned char KEY, int X, int Y) {
	ProcessKeyboard(KEY_TYPE_NORMAL, KEY_UP, KEY, NULL);
}

void LoadingMode::SpecialKeyDown(int KEY, int X, int Y) {
	ProcessKeyboard(KEY_TYPE_SPECIAL, KEY_DOWN, NULL, KEY);
}

void LoadingMode::SpecialKeyUp(int KEY, int X, int Y) {
	ProcessKeyboard(KEY_TYPE_SPECIAL, KEY_UP, NULL, KEY);
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
