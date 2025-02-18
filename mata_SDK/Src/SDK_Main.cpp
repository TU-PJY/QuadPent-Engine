#include "SDK_Header.h"
#include "SDK_CameraController.h"
#include "SDK_FPSIndicator.h"
#include "SDK_Scene.h"
#include "SDK_Frustum.h"
#include <thread>
#include <chrono>

MSDK::SDKSystem MSDK::System;
MSDK::SDKSystem* MSDK::SDKSystem::S_Inst;

void MSDK::SDKSystem::Main() {
	glClearColor(MSDK::ViewportColor.r, MSDK::ViewportColor.g, MSDK::ViewportColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	if (S_Inst->UpdateActivateCommand) {
		MSDK::Scene.Update();
		MSDK::CameraControl.Update(S_Inst->DeltaTime);
		MSDK::Frustum.Update();
		MSDK::Scene.Render();
		MSDK::Scene.CompleteCommand();
		MSDK::SoundTool.Update();
	}

	if (SHOW_FPS && Indicator)
		Indicator->RenderIndicator();

	if (S_Inst->FPSLimit > 0) {
		std::chrono::duration<double, std::milli> Duration(S_Inst->DestFPS);
		std::this_thread::sleep_for(Duration);
	}

	S_Inst->CurrentTime = float(glutGet(GLUT_ELAPSED_TIME));
	S_Inst->DeltaTime = (S_Inst->CurrentTime - S_Inst->PrevTime) / 1000.0;
	MSDK::Scene.InputFrameTime(S_Inst->DeltaTime);

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
		MSDK::LOCALE = LocaleName;
		std::wcout << L"Windows System Locale: " << MSDK::LOCALE << std::endl;
	}
		
	MSDK::System.SetupSystem(argc, argv);
	glutDisplayFunc(MSDK::System.Main);
	glutReshapeFunc(MSDK::System.DisplayReshape);
	glutMainLoop();
}

void MSDK::SDKSystem::Exit() {
	glutDestroyWindow(1);
}

float MSDK::ASP(float Value) {
	return Value * MSDK::ASPECT;
}