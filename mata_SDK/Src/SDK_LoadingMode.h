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

	static LRESULT CALLBACK MouseController(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam, UINT_PTR SubClassID, DWORD_PTR RefData) {
		int ProcEvent{};
		int WheelDelta{};
		int MouseEvent{ EVENT_NONE };

		POINT CursorPosition;
		if (GetCursorPos(&CursorPosition)) {
			ScreenToClient(Hwnd, &CursorPosition);
			SDK::Mouse.ConvertPosition(CursorPosition.x, CursorPosition.y);
		}

		switch (Message) {
		case WM_LBUTTONDOWN:
			MouseEvent = LEFT_BUTTON_DOWN; break;
		case WM_RBUTTONDOWN:
			MouseEvent = RIGHT_BUTTON_DOWN; break;
		case WM_MBUTTONDOWN:
			MouseEvent = MIDDLE_BUTTON_DOWN; break;
		case WM_LBUTTONUP:
			MouseEvent = LEFT_BUTTON_UP; break;
		case WM_RBUTTONUP:
			MouseEvent = RIGHT_BUTTON_UP; break;
		case WM_MBUTTONUP:
			MouseEvent = MIDDLE_BUTTON_UP; break;

		case WM_XBUTTONDOWN:
			ProcEvent = GET_XBUTTON_WPARAM(wParam);
			if (ProcEvent == XBUTTON1)
				MouseEvent = BACKWARD_BUTTON_DOWN;
			else if (ProcEvent == XBUTTON2)
				MouseEvent = FORWARD_BUTTON_DOWN;
			break;

		case WM_XBUTTONUP:
			ProcEvent = GET_XBUTTON_WPARAM(wParam);
			if (ProcEvent == XBUTTON1)
				MouseEvent = BACKWARD_BUTTON_UP;
			else if (ProcEvent == XBUTTON2)
				MouseEvent = FORWARD_BUTTON_UP;
			break;

		case WM_MOUSEWHEEL:
			WheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			if (WheelDelta > 0)
				MouseEvent = WHEEL_UP;
			else if (WheelDelta < 0)
				MouseEvent = WHEEL_DOWN;
			break;
		}

		if (MouseEvent == EVENT_NONE)
			return DefSubclassProc(Hwnd, Message, wParam, lParam);

		for (auto const& Object : M_Inst->InputObject)
			if (Object)  Object->InputMouse(MouseEvent);

		return DefSubclassProc(Hwnd, Message, wParam, lParam);
	}

	static void Controller() {
		glutKeyboardFunc(KeyDown);
		glutKeyboardUpFunc(KeyUp);
		glutSpecialFunc(SpecialKeyDown);
		glutSpecialUpFunc(SpecialKeyUp);
		SetWindowSubclass(SDK::SystemHWND, MouseController, 1, 0);
		SDK::CurrentMouseController = MouseController;
	}
#pragma endregion
};
extern SDK_LoadingMode LoadingMode;