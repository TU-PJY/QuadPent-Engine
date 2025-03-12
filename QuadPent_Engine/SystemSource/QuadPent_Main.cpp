#include "QuadPent_Header.h"
#include "QuadPent_CameraController.h"
#include "QuadPent_FPSIndicator.h"
#include "QuadPent_Scene.h"
#include "QuadPent_Frustum.h"
#include "QuadPent_Mouse.h"

QP::SDK_System QP::System;
QP::SDK_System* QP::SDK_System::S_Inst;

void QP::SDK_System::Main() {
	glClearColor(S_Inst->ViewportColor.r, S_Inst->ViewportColor.g, S_Inst->ViewportColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	POINT CursorPosition;
	if (GetCursorPos(&CursorPosition)) {
		ScreenToClient(QP::System_HWND, &CursorPosition);
		QP::Mouse.ConvertPosition(CursorPosition.x, CursorPosition.y);
	}
	
	if (S_Inst->UpdateActivateCommand) {
		QP::Scene.Update();
		QP::CameraControl.Update(S_Inst->DeltaTime);
		QP::Frustum.Update();
		QP::SoundTool.Update();
		QP::Scene.Render();
		QP::Frustum.Render();
		QP::Scene.CompleteCommand();
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
	std::locale::global(std::locale(""));
	std::wcout.imbue(std::locale());
	std::cout.imbue(std::locale());

	if (SHOW_CONSOLE) {
		if (AllocConsole()) {
			FILE* FP{};

			freopen_s(&FP, "CONOUT$", "w", stdout);
			freopen_s(&FP, "CONIN$", "r", stdin);
			freopen_s(&FP, "CONERR$", "w", stderr);

			std::cout << "Console initialized successfully." << std::endl;
		}
	}

	wchar_t LocaleName[LOCALE_NAME_MAX_LENGTH];
	if (GetUserDefaultLocaleName(LocaleName, LOCALE_NAME_MAX_LENGTH)) {
		QP::SYSTEM_LOCALE = LocaleName;
		std::wcout << L"Windows System Locale: " << QP::SYSTEM_LOCALE << std::endl;
	}
		
	QP::System.SetupSystem(argc, argv);
	glutDisplayFunc(QP::System.Main);
	glutReshapeFunc(QP::System.DisplayReshape);
	glutMainLoop();
}

void QP::SDK_System::Exit() {
	glutDestroyWindow(1);
}

float QP::ASP(float Value) {
	return Value * QP::Aspect;
}