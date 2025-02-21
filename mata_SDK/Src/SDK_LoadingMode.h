#pragma once
#include "SDK_Scene.h"
#include "SDK_Mouse.h"

#include "SDK_LoadingScreen.h"

class SDK_LoadingMode {
public:
	std::string ModeName{ "LoadingMode" };
	int         ModeType{ MODE_TYPE_DEFAULT };

	static void Start() {
		SetUp();
		SDK::System.SetBackColorRGB(31, 31, 31);
		SDK::Scene.AddObject(new SDK_LoadingScreen, "loading_screen", LAYER1);
	}

	static void Destructor() {

	}

	/////////////////////////////////////////////////////////////
#pragma region FoldRegion 
	std::vector<SDK::Object*> InputObject{};
	inline static SDK_LoadingMode* M_Inst;

	SDK_LoadingMode() { M_Inst = this; }

	static void SetUp() {
		M_Inst->InputObject.clear();
		SDK::Scene.RegisterInputObjectList(M_Inst->InputObject);
		SDK::Scene.RegisterDestructor(Destructor);
		SDK::Scene.RegisterController(Controller, M_Inst->ModeType);
		SDK::Scene.RegisterModeName(M_Inst->ModeName);
		SDK::Scene.RegisterModePtr(M_Inst->Start);
	}

	static void ProcessKeyEvent(SDK::KeyEvent& Event) {
		for (auto const& Object : M_Inst->InputObject)
			if (Object)  Object->InputKey(Event);

	}
	static void KeyDown(unsigned char KEY, int X, int Y) {
		if (ENABLE_DEV_EXIT && KEY == NK_ESCAPE)
			SDK::System.Exit();

		SDK::KeyEvent Event{ NORMAL_KEY_DOWN, KEY, NULL };
		ProcessKeyEvent(Event);
	}

	static void KeyUp(unsigned char KEY, int X, int Y) {
		SDK::KeyEvent Event{ NORMAL_KEY_UP, KEY, NULL };
		ProcessKeyEvent(Event);
	}

	static void SpecialKeyDown(int KEY, int X, int Y) {
		SDK::KeyEvent Event{ SPECIAL_KEY_DOWN, NULL, KEY };
		ProcessKeyEvent(Event);
	}

	static void SpecialKeyUp(int KEY, int X, int Y) {
		SDK::KeyEvent Event{ SPECIAL_KEY_UP, NULL, KEY };
		ProcessKeyEvent(Event);
	}

	static void MouseMotion(int X, int Y) {
		SDK::Mouse.ConvertPosition(X, Y);
	}

	static void MousePassiveMotion(int X, int Y) {
		SDK::Mouse.ConvertPosition(X, Y);
	}

	static void MouseWheel(int Button, int Wheel, int X, int Y) {
		int WheelEvent{};

		if (Wheel > 0)
			WheelEvent = WHEEL_UP;
		else if (Wheel < 0)
			WheelEvent = WHEEL_DOWN;

		for (auto const& Object : M_Inst->InputObject)
			if (Object)  Object->InputScroll(WheelEvent);
	}

	static void MouseButton(int Button, int State, int X, int Y) {
		int ButtonEvent{ BUTTON_NONE };

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

		if (ButtonEvent == BUTTON_NONE)
			return;

		for (auto const& Object : M_Inst->InputObject)
			if (Object)  Object->InputMouse(ButtonEvent);
	}

	static LRESULT CALLBACK ExtendedMouseButton(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam, UINT_PTR SubClassID, DWORD_PTR RefData) {
		int ProcEvent{};
		int ButtonEvent{ BUTTON_NONE };

		if (Message == WM_XBUTTONDOWN) {
			ProcEvent = GET_XBUTTON_WPARAM(wParam);

			if (ProcEvent == XBUTTON1)
				ButtonEvent = BACKWARD_BUTTON_DOWN;
			else if (ProcEvent == XBUTTON2)
				ButtonEvent = FORWARD_BUTTON_DOWN;
		}

		else if (Message == WM_XBUTTONUP) {
			ProcEvent = GET_XBUTTON_WPARAM(wParam);

			if (ProcEvent == XBUTTON1)
				ButtonEvent = BACKWARD_BUTTON_UP;
			else if (ProcEvent == XBUTTON2)
				ButtonEvent = FORWARD_BUTTON_UP;
		}

		if (ButtonEvent == BUTTON_NONE)
			return DefSubclassProc(Hwnd, Message, wParam, lParam);

		for (auto const& Object : M_Inst->InputObject)
			if (Object)  Object->InputMouse(ButtonEvent);

		return DefSubclassProc(Hwnd, Message, wParam, lParam);
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
		SetWindowSubclass(SDK::SystemHWND, ExtendedMouseButton, 1, 0);
		SDK::LastControllerProc = ExtendedMouseButton;
	}
#pragma endregion
};
extern SDK_LoadingMode LoadingMode;