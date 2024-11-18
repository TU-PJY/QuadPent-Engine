#include "EngineHeader.h"
#include "CameraUtil.h"
#include "CameraController.h"
#include "Scene.h"
#include "SoundUtil.h"
#include "TextUtil.h"
#include <thread>
#include <chrono>

bool UpdateActivateCommand;

// frametime values
float PrevTime, CurrentTime, DeltaTime;

// FPS indicator
TextUtil FPS_Ind;

GLvoid Framework::Framework() {
	glClearColor(BackColor.r, BackColor.g, BackColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	if (UpdateActivateCommand) {
		scene.Routine();
		scene.CompleteCommand();
		cameraCon.Update(DeltaTime);
		soundUtil.Update();

		if (SHOW_FPS)
			FPS_Ind.Render(-1.0 * ASPECT + 0.025, 0.95, 0.05, 1.0, L"FPS: %d", (int)(1.0 / DeltaTime));
	}

	CurrentTime = float(glutGet(GLUT_ELAPSED_TIME));
	
	if(FPSLimit > 0)
		std::this_thread::sleep_for(std::chrono::milliseconds(static_cast <int>(DestFPS)));

	DeltaTime = (CurrentTime - PrevTime) / 1000.0;
	scene.InputFrameTime(DeltaTime);

	PrevTime = CurrentTime;

	glutSwapBuffers();
	glutPostRedisplay();

	UpdateActivateCommand = true;
}

void main(int argc, char** argv) {
	Framework::SetupSystem(argc, argv);

	if (SHOW_FPS) {
		FPS_Ind.Init(L"Arial", FW_NORMAL);
		FPS_Ind.SetColor(0.0, 1.0, 0.0);
	}

	glutDisplayFunc(Framework::Framework);
	glutReshapeFunc(Framework::DisplayReshape);
	glutMainLoop();
}

void Framework::Exit() {
	glutDestroyWindow(1);
}