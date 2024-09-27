#include "IntroMode.h"
#include "Framework.h"
#include "MouseUtil.h"
#include "IntroScreen.h"

void IntroMode::Start() {
	framework.AddObject(new IntroScreen, "intro_screen", Layer::L1);
	framework.RegisterController(Controller, ModeType::Default);
	framework.RegisterDestructor(Destructor);
	framework.RegisterModeName("IntroMode");
}

void IntroMode::Destructor() {
	soundUtil.Release(IntroSound);
	imageUtil.Release(ImageEngineLogo);
	imageUtil.Release(ImageFMODLogo);

	SetBackColor(0.5, 0.5, 0.5);
}

void IntroMode::ProcessKeyboard(KeyType Type, KeyState State, unsigned char NormalKey, int SpecialKey) {
	if (auto intro_screen = framework.Find("intro_screen"); intro_screen)
		intro_screen->InputKey(Type, State, NormalKey, SpecialKey);
}

void IntroMode::ProcessMouse(int Button, int State, int X, int Y) {
}

void IntroMode::ProcessMouseWheel(int Button, int Wheel, int X, int Y) {
}



void IntroMode::KeyDown(unsigned char KEY, int X, int Y) {
	ProcessKeyboard(KeyType::Normal, KeyState::Down, KEY, NULL);
}

void IntroMode::KeyUp(unsigned char KEY, int X, int Y) {
	ProcessKeyboard(KeyType::Normal, KeyState::Up, KEY, NULL);
}

void IntroMode::SpecialKeyDown(int KEY, int X, int Y) {
	ProcessKeyboard(KeyType::Special, KeyState::Down, NULL, KEY);
}

void IntroMode::SpecialKeyUp(int KEY, int X, int Y) {
	ProcessKeyboard(KeyType::Special, KeyState::Up, NULL, KEY);
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
