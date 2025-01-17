#pragma once
#include "Scene.h"
#include "MouseUtil.h"
#include "LoadingScreen.h"

class Loading_Mode {
public:
	std::string ModeName{ "LoadingMode" };
	int         ModeType{ MODE_TYPE_DEFAULT };
	bool        UseObjectPtr{ false };

	std::vector<std::string> InputObjectTag
	{
		"loading_screen",
	};

	std::vector<GameObject*> InputObject{};

	/////////////////////////////////////////////////////////////

	static void Start() {
		System.SetBackColorRGB(31, 31, 31);
		scene.AddObject(new LoadingScreen, "loading_screen", LAYER1);
		SetUp();
	}

	static void Destructor() {
		ClearObjectPtr();
	}

	/////////////////////////////////////////////////////////////
	// Fold here
#pragma region FoldRegion 
	static Loading_Mode* M_Inst;

	Loading_Mode() {
		M_Inst = this;
	}

	static void SetUp() {
		scene.RegisterModeName(M_Inst->ModeName);
		scene.RegisterDestructor(Destructor);
		scene.RegisterController(Controller, M_Inst->ModeType);
	}

	static void AddObjectPtr(std::string ObjectTag) {
		if (!M_Inst->UseObjectPtr)  return;
		auto Object = scene.Find(ObjectTag);
		if (Object)  M_Inst->InputObject.emplace_back(Object);
	}

	static void ClearObjectPtr() {
		if (!M_Inst->UseObjectPtr)  return;
		M_Inst->InputObject.clear();
	}

	static void ProcessKeyEvent(KeyEvent& Event) {
		if (!M_Inst->UseObjectPtr) {
			for (auto const& Object : M_Inst->InputObjectTag)
				scene.InputKey(Object, Event);
		}
		else {
			for (auto const& Object : M_Inst->InputObject) {
				if (Object)  Object->InputKey(Event);
			}
		}
	}

	static void KeyDown(unsigned char KEY, int X, int Y) {
		KeyEvent Event{ NORMAL_KEY_DOWN, KEY, NULL };
		ProcessKeyEvent(Event);
	}

	static void KeyUp(unsigned char KEY, int X, int Y) {
		KeyEvent Event{ NORMAL_KEY_UP, KEY, NULL };
		ProcessKeyEvent(Event);
	}

	static void SpecialKeyDown(int KEY, int X, int Y) {
		KeyEvent Event{ SPECIAL_KEY_DOWN, NULL, KEY };
		ProcessKeyEvent(Event);
	}

	static void SpecialKeyUp(int KEY, int X, int Y) {
		KeyEvent Event{ SPECIAL_KEY_UP, NULL, KEY };
		ProcessKeyEvent(Event);
	}

	static void MouseMotion(int X, int Y) {
		mouse.ConvertPosition(X, Y);
	}

	static void MousePassiveMotion(int X, int Y) {
		mouse.ConvertPosition(X, Y);
	}

	static void MouseWheel(int Button, int Wheel, int X, int Y) {
		int WheelEvent = -1;

		if (Wheel > 0)
			WheelEvent = WHEEL_UP;
		else if (Wheel < 0)
			WheelEvent = WHEEL_DOWN;

		if (!M_Inst->UseObjectPtr) {
			for (auto const& Object : M_Inst->InputObjectTag)
				scene.InputScroll(Object, WheelEvent);
		}
		else {
			for (auto const& Object : M_Inst->InputObject)
				if (Object)  Object->InputScroll(WheelEvent);
		}
	}

	static void MouseButton(int Button, int State, int X, int Y) {
		int ButtonEvent = -1;

		switch (State) {
		case GLUT_DOWN:
			switch (Button) {
			case GLUT_LEFT_BUTTON:
				ButtonEvent = LEFT_BUTTON_DOWN;   break;
			case GLUT_RIGHT_BUTTON:
				ButtonEvent = RIGHT_BUTTON_DOWN;  break;
			case GLUT_MIDDLE_BUTTON:
				ButtonEvent = MIDDLE_BUTTON_DOWN; break;
			}
			break;

		case GLUT_UP:
			switch (Button) {
			case GLUT_LEFT_BUTTON:
				ButtonEvent = LEFT_BUTTON_UP;   break;
			case GLUT_RIGHT_BUTTON:
				ButtonEvent = RIGHT_BUTTON_UP;  break;
			case GLUT_MIDDLE_BUTTON:
				ButtonEvent = MIDDLE_BUTTON_UP; break;
			}
			break;
		}

		if (!M_Inst->UseObjectPtr) {
			for (auto const& Object : M_Inst->InputObjectTag)
				scene.InputMouse(Object, ButtonEvent);
		}
		else {
			for (auto const& Object : M_Inst->InputObject)
				if (Object)  Object->InputMouse(ButtonEvent);
		}
	}

	static void Controller() {
		glutMotionFunc(MouseMotion);
		glutPassiveMotionFunc(MousePassiveMotion);
		glutKeyboardFunc(KeyDown);
		glutKeyboardUpFunc(KeyUp);
		glutMouseWheelFunc(MouseWheel);
		glutMouseFunc(MouseButton);
		glutSpecialFunc(SpecialKeyDown);
		glutSpecialUpFunc(SpecialKeyUp);
	}
#pragma endregion
};
extern Loading_Mode LoadingMode;