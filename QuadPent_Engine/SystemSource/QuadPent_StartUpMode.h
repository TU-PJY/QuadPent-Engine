#pragma once
#include "QuadPent_Scene.h"
#include "QuadPent_Mouse.h"

#include "QuadPent_StartUp.h"

class QuadPent_StartUpMode {
public:
	std::string ModeName{ "QuadPent_StartUpMode" };
	int         ModeType{ MODE_TYPE_DEFAULT };

	static void Start() {
		SetUp();
		QP::System.SetBackColor(0.0, 0.0, 0.0);
		QP::Object* LoadingScr = QP::Scene.AddSystemObject(new QuadPent_StartUp);
		QP::Scene.AddInputObject(LoadingScr);
	}

	static void Destructor() {

	}

	/////////////////////////////////////////////////////////////
#pragma region FoldRegion 
	std::vector<QP::Object*> InputObject{};
	inline static QuadPent_StartUpMode* M_Inst;

	QuadPent_StartUpMode() { M_Inst = this; }

	static void SetUp() {
		M_Inst->InputObject.clear();
		QP::Scene.RegisterInputObjectList(M_Inst->InputObject);
		QP::Scene.RegisterDestructor(Destructor);
		QP::Scene.RegisterController(ModeController, M_Inst->ModeType);
		QP::Scene.RegisterModeName(M_Inst->ModeName);
		QP::Scene.RegisterModePtr(M_Inst->Start);
	}

	static LRESULT CALLBACK ModeController(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam, UINT_PTR SubClassID, DWORD_PTR RefData) {
		int ProcEvent{};
		int WheelDelta{};
		int MouseEvent{ EVENT_NONE };

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

		case WM_KEYDOWN: case WM_KEYUP:
			if (ENABLE_DEV_EXIT && wParam == VK_ESCAPE)
				QP::System.Exit();

			QP::KeyEvent Event{};
			Event.Type = Message;
			Event.Key = wParam;

			for (auto const& Object : M_Inst->InputObject)
				if (Object)  Object->InputKey(Event);

			return DefSubclassProc(Hwnd, Message, wParam, lParam);
			break;
		}

		if (MouseEvent != EVENT_NONE) {
			for (auto const& Object : M_Inst->InputObject)
				if (Object)  Object->InputMouse(MouseEvent);
		}

		return DefSubclassProc(Hwnd, Message, wParam, lParam);
	}
#pragma endregion
};