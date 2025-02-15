#include "SDKHeader.h"
#include "CameraController.h"
#include "FPSInd.h"
#include "Scene.h"
#include "FrustumUtil.h"
#include <thread>
#include <chrono>

SDKSystem SDK::System;
SDKSystem* SDKSystem::S_Inst;

GLvoid SDKSystem::Main() {
	glClearColor(SDK::ViewportColor.r, SDK::ViewportColor.g, SDK::ViewportColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	if (S_Inst->UpdateActivateCommand) {
		scene.Update();
		SDK::CameraControl.Update(S_Inst->DeltaTime);
		frustum.Update();
		scene.Render();
		scene.CompleteCommand();
		soundUtil.Update();
	}

	if (SHOW_FPS && Indicator)
		Indicator->RenderIndicator();

	if (S_Inst->FPSLimit > 0) {
		std::chrono::duration<double, std::milli> Duration(S_Inst->DestFPS);
		std::this_thread::sleep_for(Duration);
	}

	S_Inst->CurrentTime = float(glutGet(GLUT_ELAPSED_TIME));
	S_Inst->DeltaTime = (S_Inst->CurrentTime - S_Inst->PrevTime) / 1000.0;
	scene.InputFrameTime(S_Inst->DeltaTime);

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
		SDK::LOCALE = LocaleName;
		std::wcout << L"Windows System Locale: " << SDK::LOCALE << std::endl;
	}
		
	SDK::System.SetupSystem(argc, argv);
	glutDisplayFunc(SDK::System.Main);
	glutReshapeFunc(SDK::System.DisplayReshape);
	glutMainLoop();
}

void SDKSystem::Exit() {
	glutDestroyWindow(1);
}

GLfloat ASP(GLfloat Value) {
	return Value * SDK::ASPECT;
}