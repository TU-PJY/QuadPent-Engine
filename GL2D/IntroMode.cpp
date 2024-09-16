#include "IntroMode.h"
#include "Framework.h"
#include "MouseUtil.h"
#include "IntroScreen.h"

void Intro_Mode::IntroMode() {
	framework.AddObject(new IntroScreen, "intro_screen", Layer::L1);
	framework.SetController(Controller, ModeType::Default);
	framework.InputModeName(__func__);
}

void Intro_Mode::ProcessKeyboard(KeyType Type, KeyState State, unsigned char NormalKey, int SpecialKey) {
	if (auto intro_screen = framework.Find("intro_screen"); intro_screen)
		intro_screen->InputKey(Type, State, NormalKey, SpecialKey);
}

void Intro_Mode::ProcessMouse(int Button, int State, int X, int Y) {
}

void Intro_Mode::ProcessMouseWheel(int Button, int Wheel, int X, int Y) {
}



void Intro_Mode::KeyDown(unsigned char KEY, int X, int Y) {
	ProcessKeyboard(KeyType::Normal, KeyState::Down, KEY, NULL);
}

void Intro_Mode::KeyUp(unsigned char KEY, int X, int Y) {
	ProcessKeyboard(KeyType::Normal, KeyState::Up, KEY, NULL);
}

void Intro_Mode::SpecialKeyDown(int KEY, int X, int Y) {
	ProcessKeyboard(KeyType::Special, KeyState::Down, NULL, KEY);
}

void Intro_Mode::SpecialKeyUp(int KEY, int X, int Y) {
	ProcessKeyboard(KeyType::Special, KeyState::Up, NULL, KEY);
}

void Intro_Mode::MouseMotion(int X, int Y) {
	mouse.ConvertPosition(X, Y);
}

void Intro_Mode::MousePassiveMotion(int X, int Y) {
	mouse.ConvertPosition(X, Y);
}

void Intro_Mode::Controller() {
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(MousePassiveMotion);
	glutKeyboardFunc(KeyDown);
	glutKeyboardUpFunc(KeyUp);
	glutMouseWheelFunc(ProcessMouseWheel);
	glutMouseFunc(ProcessMouse);
	glutSpecialFunc(SpecialKeyDown);
	glutSpecialUpFunc(SpecialKeyUp);
}
