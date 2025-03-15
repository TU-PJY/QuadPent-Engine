#include "QuadPent_Header.h"
#include "QuadPent_CameraController.h"
#include "QuadPent_FPSIndicator.h"
#include "QuadPent_Scene.h"
#include "QuadPent_Frustum.h"
#include "QuadPent_Mouse.h"
#include "../resource.h"

QP::QuadPent_System QP::System;
QP::QuadPent_System* QP::QuadPent_System::S_Inst;

void QP::QuadPent_System::Main() {
	glClearColor(S_Inst->ViewportColor.r, S_Inst->ViewportColor.g, S_Inst->ViewportColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	POINT CursorPosition;
	if (GetCursorPos(&CursorPosition)) {
		ScreenToClient(QP::System_HWND, &CursorPosition);
		Mouse.ConvertPosition(CursorPosition.x, CursorPosition.y);
	}
	
	if (S_Inst->UpdateActivateCommand) {
		Scene.Update();
		CameraControl.Update();
		Frustum.Update();
		SoundTool.Update();
		Scene.Render();
		Frustum.Render();
		Scene.CompleteCommand();
	}

	if (S_Inst->FPSLimit > 0) {
		std::chrono::duration<double, std::milli> Duration(S_Inst->DestFPS);
		std::this_thread::sleep_for(Duration);
	}

	S_Inst->CurrentTime = float(glutGet(GLUT_ELAPSED_TIME));
	S_Inst->DeltaTime = (S_Inst->CurrentTime - S_Inst->PrevTime) / 1000.0;
	QP::Scene.InputFrameTime(S_Inst->DeltaTime);

	S_Inst->PrevTime = S_Inst->CurrentTime;

	glutSwapBuffers();
	glutPostRedisplay();

	S_Inst->UpdateActivateCommand = true;
}

void main(int argc, char** argv) {
	SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);

	wchar_t LocaleName[LOCALE_NAME_MAX_LENGTH];
	if (GetUserDefaultLocaleName(LocaleName, LOCALE_NAME_MAX_LENGTH)) {
		QP::SYSTEM_LOCALE = LocaleName;
		std::wcout << L"Windows System Locale: " << QP::SYSTEM_LOCALE << std::endl;
	}

	if (FindWindowA(nullptr, WINDOW_NAME)) {
		if (QP::SYSTEM_LOCALE == L"ko-KR") 
			QP::System.MessageBoxOut(L"QuadPent 실행 오류", L"이미 하나 이상의 앱이 실행 중입니다.");
		else 
			QP::System.MessageBoxOut(L"QuadPent execution error", L"One or more apps are already running.");
		
		return;
	}

	std::locale::global(std::locale(""));
	std::wcout.imbue(std::locale());
	std::cout.imbue(std::locale());

	if (ENABLE_DEV_MODE && ENABLE_DEBUG_CONSOLE) {
		if (AllocConsole()) {
			FILE* FP{};

			freopen_s(&FP, "CONOUT$", "w", stdout);
			freopen_s(&FP, "CONIN$", "r", stdin);
			freopen_s(&FP, "CONERR$", "w", stderr);

			std::cout << "Console initialized successfully." << std::endl;
		}
	}
	
	QP::System.SetupSystem(argc, argv);
	glutDisplayFunc(QP::System.Main);
	glutReshapeFunc(QP::System.DisplayReshape);
	glutMainLoop();
}

void QP::QuadPent_System::Exit() {
	glutDestroyWindow(1);
}

float QP::ASP(float Value) {
	return Value * Aspect;
}

void QP::QuadPent_System::MessageBoxOut(std::wstring Caption, std::wstring Message) {
	PlaySoundW(TEXT("SystemComponent\\Sound\\sound-alert.wav"), NULL, SND_FILENAME | SND_ASYNC);
	HICON Icon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_DEFAULT_ICON));

	MSGBOXPARAMS MsgBox = { sizeof(MSGBOXPARAMS) };
	MsgBox.hwndOwner = NULL;
	MsgBox.hInstance = GetModuleHandle(NULL);
	MsgBox.lpszCaption = Caption.c_str();
	MsgBox.lpszText = Message.c_str();
	MsgBox.dwStyle = MB_OK | MB_USERICON;
	MsgBox.lpszIcon = MAKEINTRESOURCE(IDI_DEFAULT_ICON);

	int Result = MessageBoxIndirect(&MsgBox);
	if (Result == IDOK || Result == IDCLOSE)
		Exit();
}