#include "IntroMode.h"
#include "Framework.h"
#include "MouseUtil.h"
#include "IntroScreen.h"

void IntroMode::Start() {
	framework.AddObject(new IntroScreen, "intro_screen", LAYER_1);
	framework.RegisterController(Controller, MODE_TYPE_DEFAULT);
	framework.RegisterDestructor(Destructor);
	framework.RegisterModeName("IntroMode");
}

void IntroMode::Destructor() {
	soundUtil.Release(IntroSound);
	imageUtil.Release(ImageEngineLogo);
	imageUtil.Release(ImageFMODLogo);

	SetBackColor(0.5, 0.5, 0.5);
}

void IntroMode::ProcessKeyboard(int Type, int State, unsigned char NormalKey, int SpecialKey) {
	if (auto intro_screen = framework.Find("intro_screen"); intro_screen)
		intro_screen->InputKey(Type, State, NormalKey, SpecialKey);
}

void IntroMode::ProcessMouse(int Button, int State, int X, int Y) {
}

void IntroMode::ProcessMouseWheel(int Button, int Wheel, int X, int Y) {
}



void IntroMode::KeyDown(unsigned char KEY, int X, int Y) {
	ProcessKeyboard(KEY_TYPE_NORMAL, KEY_DOWN, KEY, NULL);
}

void IntroMode::KeyUp(unsigned char KEY, int X, int Y) {
	ProcessKeyboard(KEY_TYPE_NORMAL, KEY_UP, KEY, NULL);
}

void IntroMode::SpecialKeyDown(int KEY, int X, int Y) {
	ProcessKeyboard(KEY_TYPE_SPECIAL, KEY_DOWN, NULL, KEY);
}

void IntroMode::SpecialKeyUp(int KEY, int X, int Y) {
	ProcessKeyboard(KEY_TYPE_SPECIAL, KEY_DOWN, NULL, KEY);
}

void IntroMode::MouseMotion(int X, int Y) {
	mouse.ConvertPosition(X, Y);
}

void IntroMode::MousePassiveMotion(int X, int Y) {
	mouse.ConvertPosition(X, Y);
}

void IntroMode::Controller() {
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(MousePassiveMotion);
	glutKeyboardFunc(KeyDown);
	glutKeyboardUpFunc(KeyUp);
	glutMouseWheelFunc(ProcessMouseWheel);
	glutMouseFunc(ProcessMouse);
	glutSpecialFunc(SpecialKeyDown);
	glutSpecialUpFunc(SpecialKeyUp);
}
